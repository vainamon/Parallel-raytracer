/*
 * primitives.h
 *
 *  Created on: 17.05.2009
 *      Author: vainamon
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include "Ray.h"
#include "vector3.h"
#include "vertex.h"

namespace Raytracer {

typedef vector3 Color;


/// используется для определения пересечения луча с поверхностью
enum{
	HIT = 1,		/// есть пересечение
	MISS = 0,
	INPRIM = -1		/// пересечение с внутренней стороны поверхности
};

/// материал
struct sMaterial {
	/// имя материала
	std::string m_strName;

	/// составляющие цвета материала
	Color m_cAmbient;
	float m_Ambient;
	float m_Diffuse;
	float m_specLevel;
	int m_Glossines;

	/// для материалов, загруженных из 3DS файла
	Color m_cDiffuse;
	Color m_cSpecular;

	float m_Reflection;
	float m_Refraction;
};

/// класс axis aligned bounding box
class AABB
{
public:
	AABB() : m_Pos( vector3( 0, 0, 0 ) ), m_Size( vector3( 0, 0, 0 ) ) {};
	AABB( vector3 a_Pos, vector3 a_Size ) : m_Pos( a_Pos )
											,m_Size( a_Size ) {};

	vector3& getPos() { return m_Pos; }
	vector3& getSize() { return m_Size; }

	bool intersect( AABB& b2 )
	{
		vector3 v1 = b2.getPos(), v2 = b2.getPos() + b2.getSize();
		vector3 v3 = m_Pos, v4 = m_Pos + m_Size;

		return ((v4.GetX() >= v1.GetX()) && (v3.GetX() <= v2.GetX()) && // x-axis overlap
				(v4.GetY() >= v1.GetY()) && (v3.GetY() <= v2.GetY()) && // y-axis overlap
				(v4.GetZ() >= v1.GetZ()) && (v3.GetZ() <= v2.GetZ()));  // z-axis overlap
	}
	bool contains( vector3& a_Pos )
	{
		vector3 v1 = m_Pos, v2 = m_Pos + m_Size;
		return ((a_Pos.GetX() >= v1.GetX()) && (a_Pos.GetX() <= v2.GetX()) &&
				(a_Pos.GetY() >= v1.GetY()) && (a_Pos.GetY() <= v2.GetY()) &&
				(a_Pos.GetZ() >= v1.GetZ()) && (a_Pos.GetZ() <= v2.GetZ()));
	}
	float w() { return m_Size.GetX(); }
	float h() { return m_Size.GetY(); }
	float d() { return m_Size.GetZ(); }
	float x() { return m_Pos.GetX(); }
	float y() { return m_Pos.GetY(); }
	float z() { return m_Pos.GetZ(); }

private:
	vector3 m_Pos, m_Size;
};

/// базовый класс для геометрических примитивов
class Primitive {
public:

	enum
	{
		SPHERE = 1,
		TRIANGLE,
		BOX
	};

	Primitive() : m_Name( "" ), m_Light( false ) {};
	virtual ~Primitive() {};

	virtual int getType() = 0;
	virtual int intersect( Ray& a_Ray, float& a_Dist ) = 0;
	virtual vector3 getNormal( vector3& a_Pos ) = 0;

	virtual void calculateRange( float& a_Pos1, float& a_Pos2, int a_Axis ) = 0;

    virtual bool intersectBox( AABB& a_Box ) = 0;

    virtual AABB getAABB() = 0;

    sMaterial* getMaterial() const
    {
        return m_Material;
    }

    void setMaterial(sMaterial* m_Material)
    {
        this->m_Material = m_Material;
    }

    std::string getName() const
    {
        return m_Name;
    }

    void setName(std::string m_Name)
    {
        this->m_Name = m_Name;
    }

    void setLight(bool m_Light)
    {
        this->m_Light = m_Light;
    }

    bool isLight() { return m_Light; }

protected:
	sMaterial* m_Material;
	std::string m_Name;
	bool m_Light;
};

/// сфера
class Sphere : public Primitive {

public:
	enum
	{
		SPOT = 1,
		VOL
	};

	int getType() { return SPHERE; }

	Sphere( vector3 a_Centre, float a_Radius ) :
		m_Centre( a_Centre ), m_Radius( a_Radius ), m_lightType (0) {};

	int intersect( Ray& a_Ray, float& a_Dist );

	vector3 getCentre() const
    {
        return m_Centre;
    }

    float getRadius() const
    {
        return m_Radius;
    }

    vector3 getNormal( vector3& a_Pos ) { return (a_Pos - m_Centre)/m_Radius; }

    void calculateRange( float& a_Pos1, float& a_Pos2, int a_Axis );

    bool intersectBox( AABB& a_Box );

    AABB getAABB();

    void setLightType(int a_t) {m_lightType = a_t;};
    int getLightType() {return m_lightType;};
private:
	vector3 m_Centre;
	float m_Radius;
	int m_lightType;
};

/// треугольник
class Triangle : public Primitive
{
public:
	int getType() { return TRIANGLE; };

	Triangle(vertex*, vertex*, vertex*);
	~Triangle();

	vector3& getNormal() { return m_N; }

	int intersect( Ray& a_Ray, float& a_Dist );

	vector3 getNormal( vector3& a_Pos );

	void calculateRange( float& a_Pos1, float& a_Pos2, int a_Axis );

	vertex* getVertex(unsigned short a_Indx)
	{
		return a_Indx>2 ? 0 : m_Vertex[a_Indx];
	}

	bool intersectBox( AABB& a_Box );

    AABB getAABB();

	bool planeBoxOverlap( vector3&, vector3&, vector3&);

private:
	vector3 m_N;
	vertex* m_Vertex[3];
	float m_U, m_V;
	float nu, nv, nd;
	int k;
	float bnu, bnv;
	float cnu, cnv;
};

class Box : public Primitive
{
public:
	int getType() { return BOX; }
	Box();
	Box( AABB& a_Box );
	int intersect( Ray& a_Ray, float& a_Dist );
	bool intersectBox( AABB& a_Box ) { return m_Box.intersect( a_Box ); }
	void calculateRange( float& a_Pos1, float& a_Pos2, int a_Axis ) {};
	vector3 getNormal( vector3& );
	bool contains( vector3 a_Pos ) { return m_Box.contains( a_Pos ); }
	vector3 getPos() { return m_Box.getPos(); }
	vector3 getSize() { return m_Box.getSize(); }

	AABB getAABB() { return m_Box; }
protected:
	AABB m_Box;
};

}
#endif /* PRIMITIVES_H_ */
