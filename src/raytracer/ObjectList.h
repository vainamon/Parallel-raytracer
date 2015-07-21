/*
 * ObjectList.h
 *
 *  Created on: 11.06.2009
 *      Author: vainamon
 */

#ifndef OBJECTLIST_H_
#define OBJECTLIST_H_

namespace Raytracer{

class ObjectList
{
public:
	ObjectList() : m_Primitive( 0 ), m_Next( 0 ) {}
	~ObjectList() { delete m_Next; }

	void setPrimitive( Primitive* a_Prim ) { m_Primitive = a_Prim; }
	Primitive* getPrimitive() { return m_Primitive; }

	void setNext( ObjectList* a_Next ) { m_Next = a_Next; }
	ObjectList* getNext() { return m_Next; }
private:
	Primitive* m_Primitive;
	ObjectList* m_Next;
};

}

#endif /* OBJECTLIST_H_ */
