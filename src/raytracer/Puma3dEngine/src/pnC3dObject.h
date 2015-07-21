#ifndef _PNC3DOBJECT_H_
#define _PNC3DOBJECT_H_

//#include "matrix/matrix.h"
#include "pnPrimitiveStructs.h"
#include "error.h"
#include <string>

//3х мерный объект
class pnC3dObject{
public:
	pnC3dObject();
	~pnC3dObject();

	//присваиваем имя объекту
	void SetObjectName(std::string _name)
	{
		m_strName=_name;
	};

	//возврат имени объекта
	std::string GetObjectName()
	{
		return m_strName;
	};

	//добавляем вершину объекту
	void AddVertex(float,float,float);
	//добавляем грань объекту
	void AddFace(unsigned short,unsigned short
					,unsigned short,unsigned short
					,unsigned short);
	//добавляем иноформацию о материале грани
	void AddFaceMat(std::string,unsigned short);
	//добавляем информацию о текстуре для вершины
	void AddMapVertex(float,float);
	//устанавливаем матрицу преобразования объекта в локальной СК и вектор переноса
	//void SetLocalTransform(math::matrix<float>,pnCVector3);
	//добавляем материал, используемый объектом
	void AddMaterial(pnSMaterial);

	//расчет нормалей в каждой вершине объекта
	void ComputeNormals();

	unsigned short GetVerticesCount() const {return m_vecVertices.size();};
	unsigned short GetUVCount() const {return m_vecTexCoords.size();};
	unsigned short GetFacesCount() const {return m_vecFaces.size();};
	unsigned short GetNormalsCount() const {return m_vecNormals.size();};
	unsigned short GetMaterialsCount() const {return m_vecMaterials.size();};

	pnCPoint3 GetVertex(unsigned short) const;
	pnSUV GetUV(unsigned short) const;
	pnSFace GetFace(unsigned short) const;
	pnCVector3 GetNormal(unsigned short) const;
	pnSMaterial GetMaterial(int) const;

	//устанавливаем матрицу преобразования объекта в глобальной СК
	//void SetGlobalTransformMx(math::matrix<float>);
	//устанавливает матрицу преобразования как единичную
	//void SetDefaultGlobalTrMx();
	//возвращает матрицу преобразования объекта
	//параметр: массив из 16 вещественных чисел
	//void GetGlobalTransformMx(float[]) const;
	//возвращает координаты центра локальной СК объекта
	pnCPoint3 GetCSCenter() const;
private:
	//имя объекта
	std::string m_strName;
	//вершины объекта
	std::vector<pnCPoint3> m_vecVertices;
	//координаты текстур для каждой вершины
	std::vector<pnSUV> m_vecTexCoords;
	//треугольники сетки объекта
	std::vector<pnSFace> m_vecFaces;
	//нормали в каждой вершине объекта
	std::vector<pnCVector3> m_vecNormals;
	//материалы объекта
	std::vector<pnSMaterial> m_vecMaterials;

	//вектор переноса локальной СК объекта
	pnCVector3 m_vcrTranslate;
	//матрица преобразования объекта относительно локальной СК
	//math::matrix<float> m_mxLocalTransform;
	//матрица преобразования объекта относительно глобальной СК
	//math::matrix<float> m_mxGlobalTransform;
};

#endif //_PNC3DOBJECT_H_
