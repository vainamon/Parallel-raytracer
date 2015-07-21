#include "pnCVector3.h"
#include <cmath>

void pnCVector3::Normalize()
{
	double lenSq=m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ;
	if(lenSq>0.0000001){
		float ScaleFactor=1/(float)sqrt(lenSq);
		m_fX*=ScaleFactor;
		m_fY*=ScaleFactor;
		m_fZ*=ScaleFactor;
	}
}

pnCVector3 pnCVector3::Cross(const pnCVector3& _vcr) const
{
	pnCVector3 ret(m_fY*_vcr.GetZ()-m_fZ*_vcr.GetY(),
		m_fZ*_vcr.GetX()-m_fX*_vcr.GetZ(),
		m_fX*_vcr.GetY()-m_fY*_vcr.GetX());
	return ret;
}

float pnCVector3::Dot(const pnCVector3& _vcr) const
{
	return m_fX*_vcr.GetX()+m_fY*_vcr.GetY()+m_fZ*_vcr.GetZ();
}

float pnCVector3::Length() const
{
	return sqrt(m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ);
}

pnCVector3 pnCVector3::operator +(pnCVector3& _vcr)
{
	return pnCVector3(this->m_fX+_vcr.GetX(),
						this->m_fY+_vcr.GetY(),
						this->m_fZ+_vcr.GetZ()
						);
}

pnCVector3 pnCVector3::operator -(pnCVector3& _vcr)
{
	return pnCVector3(this->m_fX-_vcr.GetX(),
						this->m_fY-_vcr.GetY(),
						this->m_fZ-_vcr.GetZ()
						);
}

pnCVector3 pnCVector3::operator /(float _scalar)
{
	if(_scalar == 0)
		throw;

	return pnCVector3(m_fX/_scalar,
						m_fY/_scalar,
						m_fZ/_scalar
						);
}

pnCVector3 pnCVector3::operator *(float _scalar)
{
	return pnCVector3(m_fX*_scalar,
						m_fY*_scalar,
						m_fZ*_scalar
						);
}
