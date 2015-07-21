/*
 * Raytracer.cpp
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#include "Raytracer.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "memory.h"
#include "omp.h"

namespace Raytracer {

double Raytracer::EPSILON = 0.001;

Raytracer::Raytracer() :  m_Width(1)
						, m_Height(1)
						, m_nTraceDepth(4)
{
	// TODO Auto-generated constructor stub
	m_Scene = new Scene();

	m_Scene->initScene();
	m_Camera = new camera();

	point3 orig(0,-100,-200);
	point3 target(0,-100,0);
	vector3 up(0,1,0);

	m_Camera->set(orig,target,up);
	m_Camera->slide(0,0,100);
	m_Camera->yaw(90);
	srand(time(NULL));

}

Raytracer::~Raytracer() {
	// TODO Auto-generated destructor stub
	delete m_Camera;
	delete m_Scene;
}

void Raytracer::setBuffer(unsigned char* a_buf, int a_Width, int a_Height)
{
	m_buffer = a_buf;
	m_Width = a_Width;
	m_Height = a_Height;
}

void Raytracer::initRender()
{
	m_P1.Set(4,3,-5);
	m_P2.Set(-4,3,-5);
	m_P3.Set(4,-3,-5);
	m_P4.Set(-4,-3,-5);

	m_Origin.Set(0, 0, 0);

	m_Origin = m_Camera->transform(m_Origin);

	m_P1 = m_Camera->transform(m_P1);
	m_P2 = m_Camera->transform(m_P2);
	m_P3 = m_Camera->transform(m_P3);
	m_P4 = m_Camera->transform(m_P4);

	m_DX = (m_P2 - m_P1)*(1/float(m_Width));
	m_DY = (m_P3 - m_P1)*(1/float(m_Height));

	m_SR[0] = (float)m_Scene->getGridSize()
				/ m_Scene->getExtends().getSize().GetX();
	m_SR[1] = (float)m_Scene->getGridSize()
				/ m_Scene->getExtends().getSize().GetY();
	m_SR[2] = (float)m_Scene->getGridSize()
				/ m_Scene->getExtends().getSize().GetZ();

	m_CW = m_Scene->getExtends().getSize()
				* (1.0f / m_Scene->getGridSize());
}

void Raytracer::_PerturbRaysOnSphere(std::vector<point3>& _vec,
		vector3 _center,float _radius,int _num)
{
	int i = 0;
	float u,theta;
	float x,y,z;

	for(i=0;i<_num;i++){
		u = (float)rand()/RAND_MAX;
		u = 2*u-1;
		theta = 2*3.141592*(float)rand()/RAND_MAX;

		x = _radius*sqrtf(1-u*u)*cosf(theta) + _center.GetX();
		y = _radius*sqrtf(1-u*u)*sinf(theta) + _center.GetY();
		z = _radius*u + _center.GetZ();
		_vec.push_back(point3(x,y,z));
	}
}

float Raytracer::_calcShade(Primitive* a_Light, vector3 a_ip)
{

	float retval;
	vector3 a_Dir;

	int num = 16;

	retval = float(num);

	int i = 0;

	std::vector<point3> _points;

	Primitive* prim = 0;

	if (a_Light->getType() == Primitive::SPHERE)
	{
		if(((Sphere*)a_Light)->getLightType() == Sphere::VOL){
			_PerturbRaysOnSphere(_points,((Sphere*)a_Light)->getCentre()
					,((Sphere*)a_Light)->getRadius(),num);

			for(i = 0; i<_points.size();i++){
				a_Dir = vector3( _points[i].GetX(),
						_points[i].GetY(), _points[i].GetZ() ) - a_ip;

				float tdist = a_Dir.Length();

				a_Dir.Normalize();

				vector3 tmpPi = a_ip + a_Dir * EPSILON;

				Ray r = Ray( tmpPi, a_Dir );

				if(_findNearest(r,tdist,prim)){
					if (!prim->isLight())
						retval -= 1.0f;
				}
			}
		}else{
			a_Dir = ((Sphere*)a_Light)->getCentre() - a_ip;
			float tdist = a_Dir.Length();

			a_Dir.Normalize();

			vector3 tmpPi = a_ip + a_Dir * EPSILON;

			Ray r = Ray( tmpPi, a_Dir );

			if(_findNearest(r,tdist,prim))
				if(!prim->isLight())
					retval = 0;

		}
	}
	return retval/num;
}

unsigned int mod[] = { 0, 1, 2, 0, 1 };

int Raytracer::_findNearest(Ray& a_Ray, float& a_Dist, Primitive*& a_Prim)
{
	int retval = MISS;

	vector3 raydir, curpos;

	AABB e = m_Scene->getExtends();

	curpos = a_Ray.getOrigin();
	raydir = a_Ray.getDirection();

	// setup 3DDDA (double check reusability of primary ray data)

	vector3 cb, tmax, tdelta, cell;

	cell = (curpos - e.getPos()) * m_SR;

	int stepX, outX, X = (int)cell.GetX();
	int stepY, outY, Y = (int)cell.GetY();
	int stepZ, outZ, Z = (int)cell.GetZ();

	int GRIDSIZE = m_Scene->getGridSize();
	int GRIDSHFT = m_Scene->getGridShift();

	if ((X < 0) || (X >= GRIDSIZE) || (Y < 0) || (Y >= GRIDSIZE) || (Z < 0) || (Z >= GRIDSIZE)) return 0;
	if (raydir.GetX() > 0)
	{
		stepX = 1, outX = GRIDSIZE;
		cb[0] = e.getPos().GetX() + (X + 1) * m_CW.GetX();
	}
	else
	{
		stepX = -1, outX = -1;
		cb[0] = e.getPos().GetX() + X * m_CW.GetX();
	}
	if (raydir.GetY() > 0.0f)
	{
		stepY = 1, outY = GRIDSIZE;
		cb[1] = e.getPos().GetY() + (Y + 1) * m_CW.GetY();
	}
	else
	{
		stepY = -1, outY = -1;
		cb[1] = e.getPos().GetY() + Y * m_CW.GetY();
	}
	if (raydir.GetZ() > 0.0f)
	{
		stepZ = 1, outZ = GRIDSIZE;
		cb[2] = e.getPos().GetZ() + (Z + 1) * m_CW.GetZ();
	}
	else
	{
		stepZ = -1, outZ = -1;
		cb[2] = e.getPos().GetZ() + Z * m_CW.GetZ();
	}
	float rxr, ryr, rzr;
	if (raydir.GetX() != 0)
	{
		rxr = 1.0f / raydir.GetX();
		tmax[0] = (cb.GetX() - curpos.GetX()) * rxr;
		tdelta[0] = m_CW.GetX() * stepX * rxr;
	}
	else tmax[0] = 1000000;
	if (raydir.GetY() != 0)
	{
		ryr = 1.0f / raydir.GetY();
		tmax[1] = (cb.GetY() - curpos.GetY()) * ryr;
		tdelta[1] = m_CW.GetY() * stepY * ryr;
	}
	else tmax[1] = 1000000;
	if (raydir.GetZ() != 0)
	{
		rzr = 1.0f / raydir.GetZ();
		tmax[2] = (cb.GetZ() - curpos.GetZ()) * rzr;
		tdelta[2] = m_CW.GetZ() * stepZ * rzr;
	}
	else tmax[2] = 1000000;
	// start stepping
	ObjectList* list = 0;
	ObjectList** grid = m_Scene->getGrid();
	a_Prim = 0;
	// trace primary ray
	while (1)
	{
		list = grid[X + (Y << GRIDSHFT) + (Z << (GRIDSHFT * 2))];
		while (list)
		{
			Primitive* pr = list->getPrimitive();
			int result;
			result = pr->intersect( a_Ray, a_Dist );
			if (result)
			{
				retval = result;
				a_Prim = pr;
				goto testloop;
			}
			list = list->getNext();
		}
		if (tmax[0] < tmax[1])
		{
			if (tmax[0] < tmax[2])
			{
				X = X + stepX;
				if (X == outX) return MISS;
				tmax[0] += tdelta[0];
			}
			else
			{
				Z = Z + stepZ;
				if (Z == outZ) return MISS;
				tmax[2] += tdelta[2];
			}
		}
		else
		{
			if (tmax[1] < tmax[2])
			{
				Y = Y + stepY;
				if (Y == outY) return MISS;
				tmax[1] += tdelta[1];
			}
			else
			{
				Z = Z + stepZ;
				if (Z == outZ) return MISS;
				tmax[2] += tdelta[2];
			}
		}
	}
testloop:
	while (1)
	{
		list = grid[X + (Y << GRIDSHFT) + (Z << (GRIDSHFT * 2))];
		while (list)
		{
			Primitive* pr = list->getPrimitive();
			int result;
			result = pr->intersect( a_Ray, a_Dist );
			if (result)
			{
				a_Prim = pr;
				retval = result;
			}
			list = list->getNext();
		}
		if (tmax[0] < tmax[1])
		{
			if (tmax[0] < tmax[2])
			{
				if (a_Dist < tmax[0]) break;
				X = X + stepX;
				if (X == outX) break;
				tmax[0] += tdelta[0];
			}
			else
			{
				if (a_Dist < tmax[2]) break;
				Z = Z + stepZ;
				if (Z == outZ) break;
				tmax[2] += tdelta[2];
			}
		}
		else
		{
			if (tmax[1] < tmax[2])
			{
				if (a_Dist < tmax[1]) break;
				Y = Y + stepY;
				if (Y == outY) break;
				tmax[1] += tdelta[1];
			}
			else
			{
				if (a_Dist < tmax[2]) break;
				Z = Z + stepZ;
				if (Z == outZ) break;
				tmax[2] += tdelta[2];
			}
		}
	}
	return retval;
}

Primitive* Raytracer::_Raytrace(Ray& a_Ray, Color& a_Acc
			, int a_Depth, float a_RIndex, float& a_Dist )
{
	if (a_Depth > m_nTraceDepth)
		return 0;

	/// испускаем первичный луч

	a_Dist = 1000000.0f;

	Primitive* prim = 0;

	int result;

	/// находим ближайшее пересечение
	result = _findNearest( a_Ray, a_Dist, prim );

	if (!result) return 0;

	/// точка пересечения
	vector3 pi = a_Ray.getOrigin() + a_Ray.getDirection() * a_Dist;
	vector3 N = prim->getNormal( pi );

	for ( int l = 0; l < m_Scene->getLightsNum(); l++ )
	{
		Primitive* light = m_Scene->getLight( l );

/**********/
		/// расчет тени
		float shade = _calcShade(light,pi);

		a_Acc += prim->getMaterial()->m_cAmbient*prim->getMaterial()->m_Ambient;
/**********/
		/// считаем диффузную составляющую
		vector3 L = ((Sphere*)light)->getCentre() - pi;

		L.Normalize();

		vector3 N = prim->getNormal( pi );

		if (prim->getMaterial()->m_Diffuse > 0)
		{
			float dot = N.Dot(L);

			if (dot > 0)
			{
				float diff = dot * prim->getMaterial()->m_Diffuse * shade;
				/// добавляем диффузную составляющую
				a_Acc = a_Acc + (prim->getMaterial()->m_cDiffuse
						+ light->getMaterial()->m_cDiffuse)*diff;
			}
		}

/**********/
		/// specular
		if (prim->getMaterial()->m_specLevel> 0)
		{
			vector3 V = a_Ray.getDirection();
			vector3 R = L - 2.0f * L.Dot(N) * N;
			float dot = V.Dot(R);

			if (dot > 0)
			{
				float spec = powf( dot, prim->getMaterial()->m_Glossines)
						* prim->getMaterial()->m_specLevel * shade;
				a_Acc += spec * light->getMaterial()->m_cSpecular;
			}
		}
/**********/
	}
/**********/
	/// вычисляем отражения
	float refl = prim->getMaterial()->m_Reflection;

	if (refl > 0.0f)
	{
		vector3 N = prim->getNormal(pi);
		vector3 R = a_Ray.getDirection() - 2.0f *N*N.Dot(a_Ray.getDirection());
		if (a_Depth < m_nTraceDepth)
		{
			Color rcol(0,0,0);
			float dist;
			vector3 tmpPi = pi + R * 0.001;
			Ray reflRay(tmpPi, R);
			_Raytrace( reflRay, rcol, a_Depth + 1, a_RIndex, dist );
			a_Acc += refl * rcol * prim->getMaterial()->m_cAmbient;
		}
	}
/**********/
	float refr = prim->getMaterial()->m_Refraction;
	if ((refr > 0) && (a_Depth < m_nTraceDepth))
	{
		float n = a_RIndex / refr;
		vector3 N = prim->getNormal( pi ) * (float)result;
		float cosI = -N.Dot(a_Ray.getDirection());

		float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);

		if (cosT2 > 0.0f)
		{
			vector3 T = (n * a_Ray.getDirection()) + (n * cosI - sqrtf( cosT2 )) * N;

			Color rcol( 0, 0, 0 );
			float dist;

			vector3 tmpPi = pi + T * 0.001;

			Ray refrRay(tmpPi, T);
			_Raytrace( refrRay, rcol, a_Depth + 1, refr, dist );
			/// Beer's law
			Color absorbance = prim->getMaterial()->m_cAmbient * 0.01f * -dist;
			Color transparency = Color( expf( absorbance.GetX() )
										, expf( absorbance.GetY() )
										, expf( absorbance.GetZ() ) );
			a_Acc += rcol * transparency;
		}
	}
/**********/

	return prim;

}

void Raytracer::_PerturbRays( std::vector<point3>& _vec, vector3 _p,
		vector3 _vecDW ,vector3 _vecDH, int _num)
{
	float dw = _vecDW.Length()/_num;
	float dh = _vecDH.Length()/_num;

	_p = _p - _vecDH/2;
	_p = _p - _vecDW/2;

	_vecDW.Normalize();
	_vecDH.Normalize();

	float rndW,rndH;

	vector3 result;

	int i = 0, j= 0;

	for(i=0;i<_num;i++){
		for(j=0;j<_num;j++){
			rndW = dw*(0.25f + ((float)rand()/RAND_MAX)*0.5f);

			rndH = dh*(0.25f + ((float)rand()/RAND_MAX)*0.5f);

			result = _p+_vecDH*(i*dh+rndH)+_vecDW*(j*dw+rndW);

			_vec.push_back(point3(result.GetX(),result.GetY()
					,result.GetZ()));
		}
	}
}

Primitive* Raytracer::_renderRay( vector3 a_dir, Color& a_Acc )
{
	Box e (m_Scene->getExtends());
	Color acc( 0, 0, 0 );
	Ray r(m_Origin, a_dir);
	// advance ray to scene bounding box boundary
	if (!e.contains( m_Origin ))
	{
		float bdist = 10000.0f;
		if (e.intersect( r, bdist ))
			r.setOrigin( m_Origin + (bdist + EPSILON) * a_dir );
	}
	float dist;
	return _Raytrace( r, a_Acc, 1, 1.0f, dist );
}

bool Raytracer::render(int b_height, int e_height)
{
	vector3 lpos,dir;
	int x,y;
	int i = 0;
	int height = e_height - b_height;

	#pragma omp parallel for private(lpos,dir,x,y)
	for ( i = 0; i < height*m_Width; i++ )
	{
		y = b_height + i/m_Width;
		x = i%m_Width;

		lpos = m_P1+y*m_DY+x*m_DX;
		/// испускаем первичный луч
		Color acc(0,0,0);

		dir = lpos - m_Origin;

		dir.Normalize();

		Primitive* prim = _renderRay( dir, acc );

		int red, green, blue;

		if((true)&&(prim != 0)) {

			std::vector<point3> rayPoints;
			_PerturbRays(rayPoints,lpos,m_DX,m_DY,4);
			int i = 0;
			for(i = 0; i<rayPoints.size();i++){
				dir = vector3( rayPoints[i].GetX(),
					rayPoints[i].GetY(), rayPoints[i].GetZ() ) - m_Origin;

				dir.Normalize();

				_renderRay( dir, acc );
			}

			red = (int)(acc.GetX() * 16);
			green = (int)(acc.GetY() * 16);
			blue = (int)(acc.GetZ() * 16);
		}else{
			red = (int)(acc.GetX() * 256);
			green = (int)(acc.GetY() * 256);
			blue = (int)(acc.GetZ() * 256);
		}

		if (red > 255) red = 255;
		if (green > 255) green = 255;
		if (blue > 255) blue = 255;

		m_buffer[4*i] = blue;
		m_buffer[4*i+1] = green;
		m_buffer[4*i+2] = red;
		m_buffer[4*i+3] = 255;
	}
	return true;
}

}
