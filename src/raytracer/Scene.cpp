/*
 * Scene.cpp
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#include "Scene.h"
#include <cstring>
#include <iostream>
#include "Puma3dEngine/src/pnC3dModel.h"

namespace Raytracer {

Scene::Scene() : m_nGridSize(128),m_nGridShift(7)
{
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
	int i = 0;
	for (i=0;i<m_Primitives.size();i++)
		delete m_Primitives[i];
	m_Primitives.clear();

	for (i=0;i<m_Vertices.size();i++)
		delete m_Vertices[i];
	m_Vertices.clear();

	for (i=0;i<m_Lights.size();i++)
		delete m_Lights[i];
	m_Lights.clear();

	for (i=0;i<m_Materials.size();i++)
		delete m_Materials[i];
	m_Materials.clear();

	for(i=0;i<m_nGridSize * m_nGridSize * m_nGridSize;i++)
	{
		if(m_Grid[i]!=0)
			delete m_Grid[i];
	}
}

void Scene::initScene()
{
	int i = 0;
	for (i=0;i<m_Primitives.size();i++)
		delete m_Primitives[i];
	m_Primitives.clear();

///////////////////////
///ground
	sMaterial* Material1 = new sMaterial();
	Material1->m_specLevel = 1.0f;
	Material1->m_Glossines = 24;
	Material1->m_cAmbient = Color( 0.4f, 0.3f, 0.3f );
	Material1->m_cDiffuse = Color( 0.4f, 0.3f, 0.3f );
	Material1->m_cSpecular = Color( 0.4f, 0.3f, 0.3f );
	Material1->m_Diffuse = 0.1f;
	Material1->m_Ambient = 0.6f;
	Material1->m_Reflection = 0.5f;
	Material1->m_Refraction = 0.0f;

	m_Materials.push_back(Material1);

	vertex* v[4];
	v[0] = new vertex( vector3(-200, -4.4, -200) );
	v[1] = new vertex( vector3(200, -4.4, -200) );
	v[2] = new vertex( vector3(-200, -4.4, 200) );
	v[3] = new vertex( vector3(200, -4.4, 200) );
	m_Vertices.push_back(v[0]);
	m_Vertices.push_back(v[1]);
	m_Vertices.push_back(v[2]);
	m_Vertices.push_back(v[3]);
	m_Primitives.push_back(new Triangle( v[0], v[3], v[1] ));
	m_Primitives.back()->setMaterial( Material1 );
	m_Primitives.back()->setName( "triangle1" );
	m_Primitives.push_back(new Triangle( v[0], v[2], v[3] ));
	m_Primitives.back()->setMaterial( Material1 );
	m_Primitives.back()->setName( "triangle2" );

//////////////////////////
	sMaterial* Material4 = new sMaterial();
	Material4->m_specLevel = 1.0f;
	Material4->m_Glossines = 24;
	Material4->m_cAmbient = Color( 0.3f, 0.5f, 0.3f );
	Material4->m_cDiffuse = Color( 0.3f, 0.5f, 0.3f );
	Material4->m_cSpecular = Color( 0.3f, 0.5f, 0.3f );
	Material4->m_Diffuse = 0.1f;
	Material4->m_Ambient = 0.6f;
	Material4->m_Reflection = 0.3f;
	Material4->m_Refraction = 0.0f;

	m_Materials.push_back(Material4);

	v[0] = new vertex( vector3(-200, 260, -200) );
	v[1] = new vertex( vector3(200, 260, -200) );
	v[2] = new vertex( vector3(-200, 260, 200) );
	v[3] = new vertex( vector3(200, 260, 200) );
	m_Vertices.push_back(v[0]);
	m_Vertices.push_back(v[1]);
	m_Vertices.push_back(v[2]);
	m_Vertices.push_back(v[3]);
	m_Primitives.push_back(new Triangle( v[0], v[3], v[1] ));
	m_Primitives.back()->setMaterial( Material4 );
	m_Primitives.back()->setName( "triangle1" );
	m_Primitives.push_back(new Triangle( v[0], v[2], v[3] ));
	m_Primitives.back()->setMaterial( Material4 );
	m_Primitives.back()->setName( "triangle2" );
///////////////////////////

	sMaterial* Material2 = new sMaterial();
	Material2->m_specLevel = 1.0f;
	Material2->m_Glossines = 24;
	Material2->m_cAmbient = Color( 0.4f, 0.4f, 0.9f );
	Material2->m_cDiffuse = Color( 0.4f, 0.4f, 0.9f );
	Material2->m_cSpecular = Color( 0.4f, 0.4f, 0.9f );
	Material2->m_Diffuse = 0.2f;
	Material2->m_Ambient = 0.3f;
	Material2->m_Reflection = 0.7f;//0.99f
	Material2->m_Refraction = 0.0f;//0.5

	m_Materials.push_back(Material2);

	v[0] = new vertex( vector3(-200, -4.4, 0) );
	v[1] = new vertex( vector3(-200, 260, 0) );
	v[2] = new vertex( vector3(0, 260, -200) );
	v[3] = new vertex( vector3(0, -4.4, -200) );
	m_Vertices.push_back(v[0]);
	m_Vertices.push_back(v[1]);
	m_Vertices.push_back(v[2]);
	m_Vertices.push_back(v[3]);
	m_Primitives.push_back(new Triangle( v[2], v[1], v[0] ));
	m_Primitives.back()->setMaterial( Material2 );
	m_Primitives.back()->setName( "triangle1" );
	m_Primitives.push_back(new Triangle( v[3], v[2], v[0] ));
	m_Primitives.back()->setMaterial( Material2 );
	m_Primitives.back()->setName( "triangle2" );

	sMaterial* Material3 = new sMaterial();
	Material3->m_specLevel = 0.5f;
	Material3->m_Glossines = 24;
	Material3->m_cAmbient = Color( 0.3f, 0.3f, 0.5f );
	Material3->m_cDiffuse = Color( 0.4f, 0.3f, 0.3f );
	Material3->m_cSpecular = Color( 0.3f, 0.4f, 0.3f );
	Material3->m_Diffuse = 0.1f;
	Material3->m_Ambient = 0.3f;
	Material3->m_Reflection = 0.2f;
	Material3->m_Refraction = 0.98f;

	m_Materials.push_back(Material3);

	m_Primitives.push_back(new Sphere( vector3( 100.0f, 110.0f, 50.0f ), 60.0f ));
	m_Primitives.back()->setName( "big sphere" );

	m_Primitives.back()->setMaterial(Material3);

/////////////////////////////////////
	// light source 1
	Sphere* l1 = new Sphere( vector3( -100.0, 180.0, 0.0 ), 10.0f );
	l1->setLightType(Sphere::SPOT);
	m_Lights.push_back(l1);
	m_Lights.back()->setLight( true );
	m_Lights.back()->setName( "light1" );

	m_Lights.back()->setMaterial(Material1);

	// light source 2
	Sphere* l2 = new Sphere( vector3( 100.0, 150.0, -50.0 ), 10.0f );
	l2->setLightType(Sphere::SPOT);
	m_Lights.push_back(l2);
	m_Lights.back()->setLight( true );
	m_Lights.back()->setName( "light2" );

	m_Lights.back()->setMaterial(Material1);
//	// light source 1
//	m_Lights.push_back(new Sphere( vector3( 0.0, 10.0, -3.0 ), 0.3f ));
//	m_Lights.back()->setLight( true );
//	m_Lights.back()->setName( "light1" );
//
//	m_Lights.back()->setMaterial(Material1);
//
//	// light source 2
//	Sphere* l2 = new Sphere( vector3( 0.0, 5.0, 10.0 ), 0.3f );
//	l2->setLightType(Sphere::SPOT);
//	m_Lights.push_back(l2);
//	m_Lights.back()->setLight( true );
//	m_Lights.back()->setName( "light2" );
//
//	m_Lights.back()->setMaterial(Material1);
}

void Scene::load3dsFile(std::string filename)
{
	pnC3dModel* model = new pnC3dModel;
	pnC3dObject* object;

	try{
		model->LoadModelFromFile(filename);
	}
	catch(...)
	{
		return;
	}
	int i = 0;
	int j = 0;
	int k = 0;

	for (i=0;i<model->getObjectsNum();i++)
	{
		object = model->getObject(i);

		std::vector<sMaterial*> Materials(object->GetMaterialsCount());

		if(object->GetMaterialsCount() != 0)
		{
			for(k = 0; k<Materials.size();k++)
			{
				pnSMaterial mat = object->GetMaterial(k);

				sMaterial* Material = new sMaterial;
				Materials[k] = Material;
				this->m_Materials.push_back(Material);

				Material->m_specLevel = 1.0f;
				Material->m_Glossines = mat.m_fShininess*128;
				Material->m_cAmbient = Color( mat.m_fAmbient[0], mat.m_fAmbient[1], mat.m_fAmbient[2] );
				Material->m_cDiffuse = Color( mat.m_fDiffuse[0], mat.m_fDiffuse[1], mat.m_fDiffuse[2] );
				Material->m_cSpecular = Color( mat.m_fSpecular[0], mat.m_fSpecular[1], mat.m_fSpecular[2] );
				Material->m_Diffuse = 0.2f;
				Material->m_Ambient = 0.4f;
				Material->m_Reflection = 0.0f;
				Material->m_Refraction = 0.0f;
			}

			int matIdx;

			int last = m_Vertices.size();

			for(j=0;j<object->GetVerticesCount();j++)
			{
				pnCPoint3 p = object->GetVertex(j);
				vertex* v = new vertex(vector3(p.GetX(),p.GetY(),p.GetZ()));
				m_Vertices.push_back(v);
			}

			for(j=0;j<object->GetFacesCount();j++){
				pnSFace face = object->GetFace(j);
				unsigned short i1 = face.m_usIndices[0];
				unsigned short i2 = face.m_usIndices[1];
				unsigned short i3 = face.m_usIndices[2];

				Triangle* triangle = new Triangle(m_Vertices[last+i1]
								,m_Vertices[last+i2],m_Vertices[last+i3]);

				m_Primitives.push_back(triangle);

				if (face.m_nMatIdx != -1)
					matIdx = face.m_nMatIdx;
				m_Primitives.back()->setMaterial(Materials[matIdx]);
			}
		}
	}
	delete model;
}

void Scene::buildGrid()
{
	// initialize regular grid
	m_Grid = new ObjectList*[m_nGridSize * m_nGridSize * m_nGridSize];
	memset( m_Grid, 0, m_nGridSize * m_nGridSize * m_nGridSize * 4 );
	vector3 p1(-250, -250, -250), p2( 250, 270, 250 );
	// calculate cell width, height and depth
	float dx = (p2.GetX() - p1.GetX()) / m_nGridSize, dx_reci = 1.0f / dx;
	float dy = (p2.GetY() - p1.GetY()) / m_nGridSize, dy_reci = 1.0f / dy;
	float dz = (p2.GetZ() - p1.GetZ()) / m_nGridSize, dz_reci = 1.0f / dz;
	m_Extends = AABB( p1, p2 - p1 );
	// store primitives in the grid cells
	int p = 0;

	for (p = 0; p < m_Primitives.size(); p++ )
	{
		AABB bound = m_Primitives[p]->getAABB();
		vector3 bv1 = bound.getPos(), bv2 = bound.getPos() + bound.getSize();
		// find out which cells could contain the primitive (based on aabb)
		int x1 = (int)((bv1.GetX() - p1.GetX()) * dx_reci)
		, x2 = (int)((bv2.GetX() - p1.GetX()) * dx_reci) + 1;

		x1 = (x1 < 0)?0:x1, x2 = (x2 > (m_nGridSize - 1))?m_nGridSize - 1:x2;

		int y1 = (int)((bv1.GetY() - p1.GetY()) * dy_reci)
		, y2 = (int)((bv2.GetY() - p1.GetY()) * dy_reci) + 1;

		y1 = (y1 < 0)?0:y1, y2 = (y2 > (m_nGridSize - 1))?m_nGridSize - 1:y2;

		int z1 = (int)((bv1.GetZ() - p1.GetZ()) * dz_reci)
		, z2 = (int)((bv2.GetZ() - p1.GetZ()) * dz_reci) + 1;

		z1 = (z1 < 0)?0:z1, z2 = (z2 > (m_nGridSize - 1))?m_nGridSize - 1:z2;
		// loop over candidate cells
		for ( int x = x1; x < x2; x++ ) for ( int y = y1; y < y2; y++ ) for ( int z = z1; z < z2; z++ )
		{
			// construct aabb for current cell
			int idx = x + y * m_nGridSize + z * m_nGridSize * m_nGridSize;

			vector3 pos( p1.GetX() + x * dx, p1.GetY() + y * dy, p1.GetZ() + z * dz );

			AABB cell( pos, vector3( dx, dy, dz ) );
			// do an accurate aabb / primitive intersection test
			if (m_Primitives[p]->intersectBox( cell ))
			{
				// object intersects cell; add to object list
				ObjectList* l = new ObjectList();
				l->setPrimitive( m_Primitives[p] );
				l->setNext( m_Grid[idx] );
				m_Grid[idx] = l;
			}
		}
	}

}

}
