#ifndef _PNC3DRENDERI_H_
#define _PNC3DRENDERI_H_

#include "pnC3dObject.h"
#include "pnC3dTrajectory.h"

//интерфейс класса для отрисовки 3д объекта
class pnC3dRenderI{
public:	
	//необходимая начальная инициализация объекта
	virtual void InitRender() const = 0;
	//функция отрисовки 3д объекта
	virtual void Render3dObject(const pnC3dObject&) const = 0;
	//функция отрисовки линии в трехмерном пространстве
	//параметры: траектория, толщина, rgb-цвет
	virtual void Render3dTrajectory(const pnC3dTrajectory&,
								float,const float[]) const = 0;
};

#endif //_PNC3DRENDERI_H_
