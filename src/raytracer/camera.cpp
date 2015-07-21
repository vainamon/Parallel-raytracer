#include "camera.h"

const double PI=3.1415926535897932385;

vector3 camera::transform(vector3& _vec)
{
	//float m[16];
	vector3 EyeVcr(m_pntEye.GetX(),m_pntEye.GetY(),m_pntEye.GetZ());

//	m[0]=m_vcrU.GetX();m[4]=m_vcrU.GetY();m[8]=m_vcrU.GetZ();m[12]=-EyeVcr.Dot(m_vcrU);
//	m[1]=m_vcrV.GetX();m[5]=m_vcrV.GetY();m[9]=m_vcrV.GetZ();m[13]=-EyeVcr.Dot(m_vcrV);
//	m[2]=m_vcrN.GetX();m[6]=m_vcrN.GetY();m[10]=m_vcrN.GetZ();m[14]=-EyeVcr.Dot(m_vcrN);
//	m[3]=0;m[7]=0;m[11]=0;m[15]=1.0;

	vector3 result;
	float x,y,z;
	x = _vec.GetX()*m_vcrU.GetX()+_vec.GetY()*m_vcrU.GetY()
			+_vec.GetZ()*m_vcrU.GetZ()+-EyeVcr.Dot(m_vcrU);
	y = _vec.GetX()*m_vcrV.GetX()+_vec.GetY()*m_vcrV.GetY()
			+_vec.GetZ()*m_vcrV.GetZ()+-EyeVcr.Dot(m_vcrV);
	z = _vec.GetX()*m_vcrN.GetX()+_vec.GetY()*m_vcrN.GetY()
			+_vec.GetZ()*m_vcrN.GetZ()+-EyeVcr.Dot(m_vcrN);
	result.Set(x,y,z);
	return result;
}

void camera::set(point3& _eye,point3& _look,vector3& _up)
{
	this->m_pntEye.Set(_eye);
	this->m_vcrN.SetDiff(_eye,_look);
	this->m_vcrU.Set(_up.Cross(m_vcrN));

	m_vcrN.Normalize();m_vcrU.Normalize();

	m_vcrV.Set(m_vcrN.Cross(m_vcrU));
}

void camera::setShape(double _vAng,double _asp,double _nr,double _fr)
{
	this->m_dViewAngle=_vAng;
	this->m_dAspect=_asp;
	this->m_dNearDist=_nr;
	this->m_dFarDist=_fr;
}

void camera::slide(float _du,float _dv,float _dn)
{
	float x=m_pntEye.GetX()+_du*m_vcrU.GetX()+_dv*m_vcrV.GetX()+_dn*m_vcrN.GetX();
	float y=m_pntEye.GetY()+_du*m_vcrU.GetY()+_dv*m_vcrV.GetY()+_dn*m_vcrN.GetY();
	float z=m_pntEye.GetZ()+_du*m_vcrU.GetZ()+_dv*m_vcrV.GetZ()+_dn*m_vcrN.GetZ();

	this->m_pntEye.Set(x,y,z);
}

void camera::roll(float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);

	vector3 tmp=m_vcrU;

	m_vcrU.Set(cs*tmp.GetX()+sn*m_vcrV.GetX(),cs*tmp.GetY()+sn*m_vcrV.GetY(),cs*tmp.GetZ()+sn*m_vcrV.GetZ());
	m_vcrV.Set(-sn*tmp.GetX()+cs*m_vcrV.GetX(),-sn*tmp.GetY()+cs*m_vcrV.GetY(),-sn*tmp.GetZ()+cs*m_vcrV.GetZ());
}

void camera::pitch(float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);

	vector3 tmp=m_vcrN;

	m_vcrN.Set(cs*tmp.GetX()+sn*m_vcrV.GetX(),cs*tmp.GetY()+sn*m_vcrV.GetY(),cs*tmp.GetZ()+sn*m_vcrV.GetZ());
	m_vcrV.Set(-sn*tmp.GetX()+cs*m_vcrV.GetX(),-sn*tmp.GetY()+cs*m_vcrV.GetY(),-sn*tmp.GetZ()+cs*m_vcrV.GetZ());
}

void camera::yaw(float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);

	vector3 tmp=m_vcrN;

	m_vcrN.Set(cs*tmp.GetX()+sn*m_vcrU.GetX(),cs*tmp.GetY()+sn*m_vcrU.GetY(),cs*tmp.GetZ()+sn*m_vcrU.GetZ());
	m_vcrU.Set(-sn*tmp.GetX()+cs*m_vcrU.GetX(),-sn*tmp.GetY()+cs*m_vcrU.GetY(),-sn*tmp.GetZ()+cs*m_vcrU.GetZ());
}

//+++++++++++++++++++++++++++++++++++++++++++//
//Вспомогательные функции формирующие соответсвенно координату x,y,z
//согласно матрице поворота вокруг оси _axis на угол _angle
float GetXCoord(const vector3& _vec,const vector3& _axis,float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);
	float V=1-cs;

	return _vec.GetX()*(SQR(_axis.GetX())*V+cs)
		+_vec.GetY()*(_axis.GetX()*_axis.GetY()*V+_axis.GetZ()*sn)
		+_vec.GetZ()*(_axis.GetX()*_axis.GetZ()*V-_axis.GetY()*sn);
}

float GetYCoord(const vector3& _vec,const vector3& _axis,float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);
	float V=1-cs;

	return _vec.GetX()*(_axis.GetX()*_axis.GetY()*V-_axis.GetZ()*sn)
		+_vec.GetY()*(SQR(_axis.GetY())*V+cs)
		+_vec.GetZ()*(_axis.GetY()*_axis.GetZ()*V+_axis.GetX()*sn);
}

float GetZCoord(const vector3& _vec,const vector3& _axis,float _angle)
{
	float cs=cos(PI/180*_angle);
	float sn=sin(PI/180*_angle);
	float V=1-cs;

	return _vec.GetX()*(_axis.GetX()*_axis.GetZ()*V+_axis.GetY()*sn)
		+_vec.GetY()*(_axis.GetY()*_axis.GetZ()*V-_axis.GetX()*sn)
		+_vec.GetZ()*(SQR(_axis.GetZ())*V+cs);
}
//+++++++++++++++++++++++++++++++++++++++++++//

void camera::rotateArndAxis(vector3 _axis,float _angle)
{
	_axis.Normalize();

	m_vcrN.Set(GetXCoord(m_vcrN,_axis,_angle),GetYCoord(m_vcrN,_axis,_angle),GetZCoord(m_vcrN,_axis,_angle));
	m_vcrU.Set(GetXCoord(m_vcrU,_axis,_angle),GetYCoord(m_vcrU,_axis,_angle),GetZCoord(m_vcrU,_axis,_angle));
	m_vcrV.Set(GetXCoord(m_vcrV,_axis,_angle),GetYCoord(m_vcrV,_axis,_angle),GetZCoord(m_vcrV,_axis,_angle));
}

void camera::sphereRotate(float _dx, float _dy, float _angle)
{
	vector3 vV(0,1,0);
	vector3 adir(_dx,_dy,0);
	adir.Normalize();

	float ang = acos((adir.GetX()*vV.GetX()+adir.GetY()*vV.GetY()+adir.GetZ()*vV.GetZ())
		/(adir.Length()*vV.Length()));

	ang=ang*(180/PI);
	if(_dx>0)
		ang=-ang;

	vector3 axs(GetXCoord(m_vcrV,m_vcrN,ang+90),GetYCoord(m_vcrV,m_vcrN,ang+90)
		,GetZCoord(m_vcrV,m_vcrN,ang+90));

	vector3 tmp(m_pntEye.GetX(),m_pntEye.GetY(),m_pntEye.GetZ());

	m_pntEye.Set(GetXCoord(tmp,axs,_angle),GetYCoord(tmp,axs,_angle),GetZCoord(tmp,axs,_angle));
	rotateArndAxis(axs,_angle);
}

void camera::PYCombRotate(float _dx, float _dy, float _angle)
{
	vector3 vV(0,1,0);
	vector3 adir(_dx,_dy,0);
	adir.Normalize();

	float ang = acos((adir.GetX()*vV.GetX()+adir.GetY()*vV.GetY()+adir.GetZ()*vV.GetZ())
		/(adir.Length()*vV.Length()));

	ang=ang*(180/PI);
	if(_dx>0)
		ang=-ang;

	vector3 axs(GetXCoord(m_vcrV,m_vcrN,ang+90),GetYCoord(m_vcrV,m_vcrN,ang+90)
		,GetZCoord(m_vcrV,m_vcrN,ang+90));

	rotateArndAxis(axs,_angle);
}
