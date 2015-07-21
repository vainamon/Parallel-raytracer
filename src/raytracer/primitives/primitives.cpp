/*
 * primitives.cpp
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#include "primitives.h"
#include <cmath>
#include <iostream>

namespace Raytracer{

const float EPSILON = 0.001;

int Sphere::intersect(Ray& a_Ray,float& a_Dist)
/// для определения пересечения со сферой
/// необходимо решить квадратное уравнение
{
 	vector3 v = a_Ray.getOrigin() - m_Centre;

	float b = -v.Dot(a_Ray.getDirection());

	float det = SQR(b) - v.Dot(v) + SQR(m_Radius);

	int retval = MISS;

	if (det > 0)
	{
		det = sqrtf(det);

		float i1 = b - det;
		float i2 = b + det;

		if (i2 > 0)
		{
			if (i1 < 0)
			{
				if (i2 < a_Dist)
				{
					a_Dist = i2;
					retval = INPRIM;
				}
			}
			else
			{
				if (i1 < a_Dist)
				{
					a_Dist = i1;
					retval = HIT;
				}
			}
		}
	}
	return retval;
}

void Sphere::calculateRange(float& a_Pos1, float& a_Pos2, int a_Axis)
{
	a_Pos1 = m_Centre[a_Axis] - m_Radius;
	a_Pos2 = m_Centre[a_Axis] + m_Radius;
}

bool Sphere::intersectBox( AABB& a_Box )
{
	float dmin = 0;
	vector3 spos = m_Centre;
	vector3 bpos = a_Box.getPos();
	vector3 bsize = a_Box.getSize();

	for ( int i = 0; i < 3; i++ )
	{
		if (spos[i] < bpos[i])
		{
			dmin = dmin + (spos[i] - bpos[i]) * (spos[i] - bpos[i]);
		}
		else if (spos[i] > (bpos[i] + bsize[i]))
		{
			dmin = dmin + (spos[i] - (bpos[i] + bsize[i]))
					* (spos[i] - (bpos[i] + bsize[i]));
		}
	}
	return (dmin <= SQR(m_Radius));
}

AABB Sphere::getAABB()
{
	vector3 size( m_Radius, m_Radius, m_Radius );
	return AABB( m_Centre - size, size * 2 );
}

unsigned int modulo[] = { 0, 1, 2, 0, 1 };

Triangle::Triangle(vertex* a_V1, vertex* a_V2, vertex* a_V3) : Primitive()
{
	m_Vertex[0] = a_V1;
	m_Vertex[1] = a_V2;
	m_Vertex[2] = a_V3;

	vector3 A = m_Vertex[0]->getPos();
	vector3 B = m_Vertex[1]->getPos();
	vector3 C = m_Vertex[2]->getPos();
	vector3 c = B - A;
	vector3 b = C - A;

	m_N = b.Cross( c );

	int u, v;
	if (fabs( m_N.GetX() ) > fabs( m_N.GetY()))
	{
		if (fabs( m_N.GetX() ) > fabs( m_N.GetZ() )) k = 0; else k = 2;
	}
	else
	{
		if (fabs( m_N.GetY() ) > fabs( m_N.GetZ() )) k = 1; else k = 2;
	}

	u = modulo[k + 1];
	v = modulo[k + 2];

	float krec = 1.0f / m_N[k];
	nu = m_N[u] * krec;
	nv = m_N[v] * krec;
	nd = m_N.Dot( A ) * krec;

	float reci = 1.0f / (b[u] * c[v] - b[v] * c[u]);
	bnu = b[u] * reci;
	bnv = -b[v] * reci;

	cnu = c[v] * reci;
	cnv = -c[u] * reci;

	m_N.Normalize();
	m_Vertex[0]->setNormal( m_N );
	m_Vertex[1]->setNormal( m_N );
	m_Vertex[2]->setNormal( m_N );
}

int Triangle::intersect(Ray& a_Ray,float& a_Dist)
{
	#define ku modulo[k + 1]
	#define kv modulo[k + 2]

	vector3 O = a_Ray.getOrigin(),
			D = a_Ray.getDirection(),
			A = m_Vertex[0]->getPos();

	const float lnd = 1.0f / (D[k] + nu * D[ku] + nv * D[kv]);
	const float t = (nd - O[k] - nu * O[ku] - nv * O[kv]) * lnd;

	if (!(a_Dist > t && t > 0)) return MISS;

	float hu = O[ku] + t * D[ku] - A[ku];
	float hv = O[kv] + t * D[kv] - A[kv];

	float beta = m_U = hv * bnu + hu * bnv;

	if (beta < 0) return MISS;

	float gamma = m_V = hu * cnu + hv * cnv;

	if (gamma < 0) return MISS;

	if ((beta + gamma) > 1) return MISS;

	a_Dist = t;
	return ((D.Dot(m_N)) > 0) ? INPRIM:HIT;
}

vector3 Triangle::getNormal(vector3& a_Pos)
{
	vector3 N1 = m_Vertex[0]->getNormal();
	vector3 N2 = m_Vertex[1]->getNormal();
	vector3 N3 = m_Vertex[2]->getNormal();
	vector3 N = N1 + m_U * (N2 - N1) + m_V * (N3 - N1);
	N.Normalize();
	N.Flip();
	return N;
}

void Triangle::calculateRange(float& a_Pos1, float& a_Pos2, int a_Axis)
{
	vector3 pos1 = m_Vertex[0]->getPos();

	a_Pos1 = pos1[a_Axis], a_Pos2 = pos1[a_Axis];

	for ( int i = 1; i < 3; i++ )
	{
		vector3 pos = m_Vertex[i]->getPos();
		if (pos[a_Axis] < a_Pos1) a_Pos1 = pos[a_Axis];
		if (pos[a_Axis] > a_Pos2) a_Pos2 = pos[a_Axis];
	}
}

bool Triangle::planeBoxOverlap(vector3& a_Normal, vector3& a_Vert, vector3& a_MaxBox)
{
	vector3 vmin, vmax;

	for( int q = 0; q < 3; q++ )
	{
		float v = a_Vert[q];
		if (a_Normal[q] > 0.0f)
		{
			vmin[q] = -a_MaxBox[q] - v;
			vmax[q] =  a_MaxBox[q] - v;
		}
		else
		{
			vmin[q] =  a_MaxBox[q] - v;
			vmax[q] = -a_MaxBox[q] - v;
		}
	}

	if (a_Normal.Dot(vmin) > 0.0f) return false;
	if (a_Normal.Dot(vmax) >= 0.0f) return true;

	return false;
}

#define FINDMINMAX( x0, x1, x2, min, max ) \
  min = max = x0; if(x1<min) min=x1; if(x1>max) max=x1; if(x2<min) min=x2; if(x2>max) max=x2;
// X-tests
#define AXISTEST_X01( a, b, fa, fb )											\
	p0 = a * v0[1] - b * v0[2], p2 = a * v2[1] - b * v2[2]; \
    if (p0 < p2) { min = p0; max = p2;} else { min = p2; max = p0; }			\
	rad = fa * a_BoxHalfsize[1] + fb * a_BoxHalfsize[2];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_X2( a, b, fa, fb )												\
	p0 = a * v0[1] - b * v0[2], p1 = a * v1[1] - b * v1[2];	\
    if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0;}			\
	rad = fa * a_BoxHalfsize[1] + fb * a_BoxHalfsize[2];				\
	if(min>rad || max<-rad) return 0;
// Y-tests
#define AXISTEST_Y02( a, b, fa, fb )											\
	p0 = -a * v0[0] + b * v0[2], p2 = -a * v2[0] + b * v2[2]; \
    if(p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; }			\
	rad = fa * a_BoxHalfsize[0] + fb * a_BoxHalfsize[2];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Y1( a, b, fa, fb )												\
	p0 = -a * v0[0] + b * v0[2], p1 = -a * v1[0] + b * v1[2]; \
    if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * a_BoxHalfsize[0] + fb * a_BoxHalfsize[2];				\
	if (min > rad || max < -rad) return 0;
// Z-tests
#define AXISTEST_Z12( a, b, fa, fb )											\
	p1 = a * v1[0] - b * v1[1], p2 = a * v2[0] - b * v2[1]; \
    if(p2 < p1) { min = p2; max = p1; } else { min = p1; max = p2; }			\
	rad = fa * a_BoxHalfsize[0] + fb * a_BoxHalfsize[1];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Z0( a, b, fa, fb )												\
	p0 = a * v0[0] - b * v0[1], p1 = a * v1[0] - b * v1[1];	\
    if(p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * a_BoxHalfsize[0] + fb * a_BoxHalfsize[1];				\
	if (min > rad || max < -rad) return 0;

bool Triangle::intersectBox(AABB& a_Box)
{
	vector3 a_BoxHalfsize = a_Box.getSize() * 0.5f;

	vector3 v0, v1, v2, normal, e0, e1, e2;
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	v0 = m_Vertex[0]->getPos() - (a_Box.getPos() + a_Box.getSize() * 0.5f);
	v1 = m_Vertex[1]->getPos() - (a_Box.getPos() + a_Box.getSize() * 0.5f);
	v2 = m_Vertex[2]->getPos() - (a_Box.getPos() + a_Box.getSize() * 0.5f);

	e0 = v1 - v0, e1 = v2 - v1, e2 = v0 - v2;
	fex = fabsf( e0[0] );
	fey = fabsf( e0[1] );
	fez = fabsf( e0[2] );
	AXISTEST_X01( e0[2], e0[1], fez, fey );
	AXISTEST_Y02( e0[2], e0[0], fez, fex );
	AXISTEST_Z12( e0[1], e0[0], fey, fex );
	fex = fabsf( e1[0] );
	fey = fabsf( e1[1] );
	fez = fabsf( e1[2] );
	AXISTEST_X01( e1[2], e1[1], fez, fey );
	AXISTEST_Y02( e1[2], e1[0], fez, fex );
	AXISTEST_Z0 ( e1[1], e1[0], fey, fex );
	fex = fabsf( e2[0] );
	fey = fabsf( e2[1] );
	fez = fabsf( e2[2] );
	AXISTEST_X2 ( e2[2], e2[1], fez, fey );
	AXISTEST_Y1 ( e2[2], e2[0], fez, fex );
	AXISTEST_Z12( e2[1], e2[0], fey, fex );
	FINDMINMAX( v0[0], v1[0], v2[0], min, max );
	if (min > a_BoxHalfsize[0] || max < -a_BoxHalfsize[0]) return false;
	FINDMINMAX( v0[1], v1[1], v2[1], min, max );
	if (min > a_BoxHalfsize[1] || max < -a_BoxHalfsize[1]) return false;
	FINDMINMAX( v0[2], v1[2], v2[2], min, max );
	if (min > a_BoxHalfsize[2] || max < -a_BoxHalfsize[2]) return false;
	normal = e0.Cross( e1 );
	if (!planeBoxOverlap( normal, v0, a_BoxHalfsize )) return false;
	return true;
}

AABB Triangle::getAABB()
{
	float minX,minY,minZ;
	float maxX,maxY,maxZ;
	calculateRange(minX,maxX,0);
	calculateRange(minY,maxY,1);
	calculateRange(minZ,maxZ,2);
	return AABB(vector3(minX,minY,minZ)
			,vector3(maxX-minX,maxY-minY,maxZ-minZ));
}

Triangle::~Triangle()
{
}


Box::Box() :
	m_Box( vector3( 0, 0, 0 ), vector3( 0, 0, 0 ) )
{
}

Box::Box( AABB& a_Box ) :
	m_Box( a_Box )
{
}

int Box::intersect( Ray& a_Ray, float& a_Dist )
{
	float dist[6];
	vector3 ip[6], d = a_Ray.getDirection(), o = a_Ray.getOrigin();
	bool retval = MISS;
	int i = 0;
	for (i = 0; i < 6; i++ ) dist[i] = -1;
	vector3 v1 = m_Box.getPos(), v2 = m_Box.getPos() + getSize();
	if (d.GetX())
	{
		float rc = 1.0f / d[0];
		dist[0] = (v1[0] - o[0]) * rc;
		dist[3] = (v2[0] - o[0]) * rc;
	}
	if (d.GetY())
	{
		float rc = 1.0f / d[1];
		dist[1] = (v1[1] - o[1]) * rc;
		dist[4] = (v2[1] - o[1]) * rc;
	}
	if (d[2])
	{
		float rc = 1.0f / d[2];
		dist[2] = (v1[2] - o[2]) * rc;
		dist[5] = (v2[2] - o[2]) * rc;
	}
	for ( i = 0; i < 6; i++ ) if (dist[i] > 0)
	{
		ip[i] = o + dist[i] * d;
		if ((ip[i][0] > (v1[0] - EPSILON)) && (ip[i][0] < (v2[0] + EPSILON)) &&
			(ip[i][1] > (v1[1] - EPSILON)) && (ip[i][1] < (v2[1] + EPSILON)) &&
			(ip[i][2] > (v1[2] - EPSILON)) && (ip[i][2] < (v2[2] + EPSILON)))
		{
			if (dist[i] < a_Dist)
			{
				a_Dist = dist[i];
				retval = HIT;
			}
		}
	}
	return retval;
}

vector3 Box::getNormal( vector3& a_Pos )
{
	float dist[6];
	dist[0] = (float)fabs( m_Box.getSize()[0] - m_Box.getPos()[0] );
	dist[1] = (float)fabs( m_Box.getSize()[0] + m_Box.getSize()[0] - m_Box.getPos()[0] );
	dist[2] = (float)fabs( m_Box.getSize()[1] - m_Box.getPos()[1] );
	dist[3] = (float)fabs( m_Box.getSize()[1] + m_Box.getSize()[1] - m_Box.getPos()[1] );
	dist[4] = (float)fabs( m_Box.getSize()[2] - m_Box.getPos()[2] );
	dist[5] = (float)fabs( m_Box.getSize()[2] + m_Box.getSize()[2] - m_Box.getPos()[2] );
	int best = 0;
	float bdist = dist[0];
	for ( int i = 1 ; i < 6; i++ ) if (dist[i] < bdist)
	{
		bdist = dist[i];
		best = i;
	}
	if (best == 0) return vector3( -1, 0, 0 );
	else if (best == 1) return vector3( 1, 0, 0 );
	else if (best == 2) return vector3( 0, -1, 0 );
	else if (best == 3)  return vector3( 0, 1, 0 );
	else if (best == 4) return vector3( 0, 0, -1 );
	else return vector3( 0, 0, 1 );
}

}
