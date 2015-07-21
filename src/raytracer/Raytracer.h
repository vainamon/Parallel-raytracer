/*
 * Raytracer.h
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "Scene.h"
#include "camera.h"

namespace Raytracer {

class Raytracer {
public:
	static double EPSILON;
public:
	Raytracer();
	virtual ~Raytracer();

	void setBuffer( unsigned char* a_buf, int a_Width, int a_Height );

	void initRender();
	bool render(int,int);

    camera* getCamera()
    {
        return m_Camera;
    }

	Scene* getScene() { return m_Scene; }

protected:

	Primitive* _renderRay( vector3 a_dir, Color& a_Acc );

	Primitive* _Raytrace( Ray& a_Ray, Color& a_Acc
			, int a_Depth, float a_RIndex, float& a_Dist );

	void _PerturbRays( std::vector<point3>&, vector3,
								vector3,vector3,int _num = 4);

	void _PerturbRaysOnSphere( std::vector<point3>&,
								vector3,float,int _num = 4);

	float _calcShade(Primitive* a_Light, vector3 a_ip);

	int _findNearest( Ray& a_Ray, float& a_Dist, Primitive*& a_Prim );

private:

	vector3 m_DX, m_DY, m_P1, m_P2, m_P3, m_P4;

	vector3 m_Origin;

	camera* m_Camera;

	unsigned char* m_buffer;
	int m_Width, m_Height;
	Scene* m_Scene;

	int m_nTraceDepth;

	vector3 m_SR, m_CW;
	int m_CurID;
};

}

#endif /* RAYTRACER_H_ */
