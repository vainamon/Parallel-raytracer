#include "vector3.h"
#include <cmath>

void vector3::Normalize()
{
	double lenSq=m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ;
	if(lenSq>0.0000001){
		float ScaleFactor=1/(float)sqrt(lenSq);
		m_fX*=ScaleFactor;
		m_fY*=ScaleFactor;
		m_fZ*=ScaleFactor;
	}
}

vector3 vector3::Cross(const vector3& _vcr) const
{
	vector3 ret(m_fY*_vcr.GetZ()-m_fZ*_vcr.GetY(),
		m_fZ*_vcr.GetX()-m_fX*_vcr.GetZ(),
		m_fX*_vcr.GetY()-m_fY*_vcr.GetX());
	return ret;
}

float vector3::Dot(const vector3& _vcr) const
{
	return m_fX*_vcr.GetX()+m_fY*_vcr.GetY()+m_fZ*_vcr.GetZ();
}

float vector3::Length() const
{
	return sqrt(m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ);
}

vector3 vector3::operator +(vector3 _vcr)
{
	return vector3(this->m_fX+_vcr.GetX(),
						this->m_fY+_vcr.GetY(),
						this->m_fZ+_vcr.GetZ()
						);
}

void vector3::operator +=(vector3 _vcr)
{
	this->m_fX+=_vcr.GetX();
	this->m_fY+=_vcr.GetY();
	this->m_fZ+=_vcr.GetZ();
}

vector3 vector3::operator -(vector3 _vcr)
{
	return vector3(this->m_fX-_vcr.GetX(),
						this->m_fY-_vcr.GetY(),
						this->m_fZ-_vcr.GetZ()
						);
}

vector3 vector3::operator *(vector3 _vec)
{
	return vector3(m_fX*_vec.GetX(),
						m_fY*_vec.GetY(),
						m_fZ*_vec.GetZ()
						);
}

vector3 vector3::operator /(float _scalar) throw(CError)
{
	if(_scalar == 0)
		throw CError("pnCVector3 : Division by zero.");

	return vector3(m_fX/_scalar,
						m_fY/_scalar,
						m_fZ/_scalar
						);
}

vector3 vector3::operator *(float _scalar)
{
	return vector3(m_fX*_scalar,
						m_fY*_scalar,
						m_fZ*_scalar
						);
}

float&  vector3::operator [](unsigned short idx)
{
	switch(idx){
	case 0:
		return m_fX;
	case 1:
		return m_fY;
	case 2:
		return m_fZ;
	default:
		return m_fX;
	};
}
