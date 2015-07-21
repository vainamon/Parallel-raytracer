#ifndef _PNCCAMERAGL_H_
#define _PNCCAMERAGL_H_

#include <cmath>

#include "primitives/point3.h"
#include "primitives/vector3.h"

//Класс камера трехмерной сцены

class camera{
public:
	camera(){};

	//устанавливаем камеру: наблюдатель, куда направлен взгляд,
	//вектор "вверх"
	void set(point3&,point3&,vector3&);

	//накреняем камеру (параметр: угол крена)
	void roll(float);

	//изменяем тангаж камеры
	void pitch(float);

	//рыскаем камерой
	void yaw(float);

	//передвигаем камеру
	void slide(float,float,float);

	//поворот камеры вокруг произвольного вектора
	void rotateArndAxis(vector3,float);

	//сферический поворот камеры, радиус сферы
	//равен: глаз камеры - точка наблюдения
	//параметры - смещение по x,y (возможно мыши) и угол поворота
	void sphereRotate(float,float,float);

	//комбинированный поворот камеры вокруг собственных осей U,V
	//параметры - смещение по x,y (возможно мыши) и угол поворота
	void PYCombRotate(float,float,float);

	//устанавливаем объем обзора
	void setShape(double,double,double,double);


	vector3 getVcrN()const {return m_vcrN;};

	vector3 transform(vector3&);

private:
	point3 m_pntEye;
	vector3 m_vcrU,m_vcrV,m_vcrN;

	//форма отображаемого объема
	double m_dViewAngle,m_dAspect,m_dNearDist,m_dFarDist;
};

#endif //_PNCCAMERAGL_H_
