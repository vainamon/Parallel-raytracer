/*
 * Ray.h
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#ifndef RAY_H_
#define RAY_H_

#include "vector3.h"

namespace Raytracer {

class Ray {
public:
	Ray( vector3& a_Origin, vector3& a_Dir ) :
		m_Origin( a_Origin ),
		m_Direction( a_Dir ),
		m_ID(0)
	{
	}

	virtual ~Ray() {};

	vector3 getOrigin() const
    {
        return m_Origin;
    }

    void setOrigin(vector3 m_Origin)
    {
        this->m_Origin = m_Origin;
    }

    vector3 getDirection() const
    {
        return m_Direction;
    }

    void setDirection(vector3 m_Direction)
    {
        this->m_Direction = m_Direction;
    }

    void setID( int a_ID ) { m_ID = a_ID; }
	int getID() { return m_ID; }

private:
	vector3 m_Origin;
	vector3 m_Direction;
	int m_ID;
};

}

#endif /* RAY_H_ */
