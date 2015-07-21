/*
 * vertex.h
 *
 *  Created on: 27.05.2009
 *      Author: vainamon
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector3.h"

class vertex
{
public:
	vertex() {};
	vertex( vector3 a_Pos) : m_Pos( a_Pos ) {};

	vector3& getNormal() { return m_Normal; }
	vector3& getPos() { return m_Pos; }
	void setPos( vector3& a_Pos ) { m_Pos = a_Pos; }
	void setNormal( vector3& a_N ) { m_Normal = a_N; }
private:
	vector3 m_Pos;
	vector3 m_Normal;
};

#endif /* VERTEX_H_ */
