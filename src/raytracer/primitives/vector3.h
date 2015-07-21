#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "point3.h"
#include "../error.h"
//Класс вектор в трехмерном пространстве

inline double SQR(double x){ return x*x; }

class vector3{
public:
	vector3():m_fX(0),m_fY(0),m_fZ(0){};

	vector3(float _x,float _y,float _z)
		:m_fX(_x),m_fY(_y),m_fZ(_z){};

	vector3(const vector3& _vcr)
		:m_fX(_vcr.GetX()),m_fY(_vcr.GetY()),m_fZ(_vcr.GetZ()){};

	void Set(float _x,float _y,float _z)
	{
		m_fX=_x;
		m_fY=_y;
		m_fZ=_z;
	}

	void Set(vector3 _vcr)
	{
		m_fX=_vcr.GetX();
		m_fY=_vcr.GetY();
		m_fZ=_vcr.GetZ();
	}

	void Flip(){m_fX=-m_fX;m_fY=-m_fY;m_fZ=-m_fZ;}

	void SetDiff(point3& _pnt1,point3& _pnt2)
	//Устанавливаем вектор исходя из разницы между двумя точками
	{
		m_fX=_pnt1.GetX()-_pnt2.GetX();
		m_fY=_pnt1.GetY()-_pnt2.GetY();
		m_fZ=_pnt1.GetZ()-_pnt2.GetZ();
	}

	float GetX() const {return m_fX;}
	float GetY() const {return m_fY;}
	float GetZ() const {return m_fZ;}

	void Normalize();

	//Возврат векторного произведения на вектор-аргумент
	vector3 Cross(const vector3&) const ;
	//Возврат скалярного произведения на вектор-аргумент
	float Dot(const vector3&) const;
	//Возврат длины вектора
	float Length() const;

	vector3 operator +(vector3);
	void operator +=(vector3);
	vector3 operator -(vector3);
	vector3 operator *(vector3);
	vector3 operator /(float) throw(CError);
	vector3 operator *(float);
	float& operator[] (unsigned short);
private:
	float m_fX,m_fY,m_fZ;
};

inline vector3 operator *(float _num,vector3 _vec)
{
	return vector3(_vec.GetX()*_num
			,_vec.GetY()*_num
			,_vec.GetZ()*_num);
}

#endif //_VECTOR3_H_
