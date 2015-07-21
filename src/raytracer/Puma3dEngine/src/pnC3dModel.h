#ifndef _PNC3DMODEL_H_
#define _PNC3DMODEL_H_

#define PI 3.141592

#include "pnC3dsFileLoader.h"
#include "pnC3dObject.h"
#include "pnC3dRenderI.h"

class pnC3dModel : public pnC3dsFileLoader{
public:
	pnC3dModel();
	pnC3dModel(const pnC3dRenderI*);

	virtual ~pnC3dModel();

	//установка рендера, визуализирующего объект/объекты
	virtual void SetRender(const pnC3dRenderI*);

	//загружает модель из указанного файла
	void LoadModelFromFile(std::string);
	//переопределяется классом-потомком, для загрузки всех его объектов
	virtual void LoadModel();
	//уничтожение модели необходимо переопределить,
	//если происходит наследование от данного класса
	virtual void DestroyModel();
	//отрисовка объекта/объектов
	virtual void DrawModel();

	//устанавливаем матрицу преобразования всех объектов модели в глобальной СК
	//void SetGlobalTransformMx(math::matrix<float>);

	int getObjectsNum(){return m_vec3dObjObjects.size();};
	pnC3dObject* getObject(int _num){return m_vec3dObjObjects[_num];};

//функции, унаследованные от класса pnC3dsFileLoader
protected:
	//необходимо переопределять в наследуемом классе
	virtual void _NewObject(std::string);
	virtual void _EofLoading();
	//----------------------------------------------

	virtual void _New3dVertex(float,float,float);
	virtual void _New3dFace(unsigned short,unsigned short
							,unsigned short,unsigned short
							,unsigned short);
	virtual void _NewFaceMaterial(std::string, unsigned short);
	virtual void _NewMapVertex(float,float);
	//virtual void _NewLocalAxisInfo(math::matrix<float>,pnCVector3);
	virtual void _NewMaterial(pnSMaterial);
protected:
	//установка указателя на текущий объект при разборе
	void _SetObject(pnC3dObject*);

	//материалы модели, которые используются в объкте/объектах этой модели
	//временный массив, который может очищаться после загрузки модели
	std::vector<pnSMaterial> m_vecMaterials;
private:
	//указатель на текущий объект
	//при разборе 3ds файла
	pnC3dObject* m_3dObjObject;
	//указатель на используемый визуализатор объекта
	const pnC3dRenderI* m_3dRndrRender;

	//объекты модели
	std::vector<pnC3dObject*> m_vec3dObjObjects;
};

#endif //_PNC3DMODEL_H_
