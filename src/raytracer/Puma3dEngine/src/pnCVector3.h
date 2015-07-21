#ifndef _PNCVECTOR3_H_
#define _PNCVECTOR3_H_

#include "pnCPoint3.h"
#include "error.h"
//Класс вектор в трехмерном пространстве

inline float SQR(float x){ return x*x; }

class pnCVector3{
public:
	pnCVector3():m_fX(0),m_fY(0),m_fZ(0){};

	pnCVector3(float _x,float _y,float _z)
		:m_fX(_x),m_fY(_y),m_fZ(_z){};

	pnCVector3(const pnCVector3& _vcr)
		:m_fX(_vcr.GetX()),m_fY(_vcr.GetY()),m_fZ(_vcr.GetZ()){};

	void Set(float _x,float _y,float _z)
	{
		m_fX=_x;
		m_fY=_y;
		m_fZ=_z;
	}

	void Set(pnCVector3 _vcr)
	{
		m_fX=_vcr.GetX();
		m_fY=_vcr.GetY();
		m_fZ=_vcr.GetZ();
	}

	void Flip(){m_fX=-m_fX;m_fY=-m_fY;m_fZ=-m_fZ;}

	void SetDiff(pnCPoint3& _pnt1,pnCPoint3& _pnt2)
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
	pnCVector3 Cross(const pnCVector3&) const ;
	//Возврат скалярного произведения на вектор-аргумент
	float Dot(const pnCVector3&) const;
	//Возврат длины вектора
	float Length() const;

	pnCVector3 operator +(pnCVector3&);
	pnCVector3 operator -(pnCVector3&);
	pnCVector3 operator /(float);
	pnCVector3 operator *(float);
private:
	float m_fX,m_fY,m_fZ;
};

#endif //_PNCVECTOR3_H_
