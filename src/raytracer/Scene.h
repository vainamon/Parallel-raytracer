/*
 * Scene.h
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "primitives/primitives.h"
#include "ObjectList.h"

namespace Raytracer {

class KdTree;

class Scene {
public:
	Scene();
	virtual ~Scene();

	void initScene();

	int getPrimitivesNum(){ return m_Primitives.size(); }

	Primitive* getPrimitive( int a_Idx ) const { return m_Primitives[a_Idx]; }

	int getLightsNum(){ return m_Lights.size(); }

	Primitive* getLight( int a_Idx ) const { return m_Lights[a_Idx]; }

	AABB& getExtends() { return m_Extends;}

	void load3dsFile(std::string);

	void buildGrid();
	ObjectList** getGrid(){return m_Grid;};

	int getGridSize(){return m_nGridSize;};
	int getGridShift(){return m_nGridShift;};
private:

	std::vector<Primitive*> m_Primitives;
	std::vector<Primitive*> m_Lights;
	std::vector<sMaterial*>  m_Materials;
	std::vector<vertex*>	m_Vertices;
	AABB m_Extends;

	ObjectList** m_Grid;

	int m_nGridSize;
	int m_nGridShift;
};

}

#endif /* SCENE_H_ */
