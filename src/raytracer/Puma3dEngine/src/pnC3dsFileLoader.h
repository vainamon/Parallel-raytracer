#ifndef _PNC3DSFILELOADER_H_
#define _PNC3DSFILELOADER_H_

#include <fstream>
#include "pn3dsFileFormat.h"
#include "error.h"
#include "pnCVector3.h"
#include "pnPrimitiveStructs.h"

class pnC3dsFileLoader{
public:
	pnC3dsFileLoader();

	virtual ~pnC3dsFileLoader();
protected:
	//разбор указанного по имени файла
	void _ParseFile(std::string);

//функции, которые необходимо определить в наследующем классе
protected:
	//нашли новый объект в файле, параметр - имя объекта
	virtual void _NewObject(std::string) = 0;
	//нашли новую вершину объекта, параметры - координаты вершины
	virtual void _New3dVertex(float,float,float) = 0;
	//нашли новую грань объекта, параметры - индексы вершин + флаги + индекс грани
	virtual void _New3dFace(unsigned short,unsigned short
							,unsigned short,unsigned short
							,unsigned short) = 0;
	//нашли информацию о материале, которую использует грань
	//параметры - имя материала, индекс грани
	virtual void _NewFaceMaterial(std::string, unsigned short) = 0;
	//нашли новую вершину текстуры для объекта параметры - координаты вершины
	virtual void _NewMapVertex(float,float) = 0;
	//нашли информацию о локальной СК объекта
	//параметры - матрица поворота объекта, вектор переноса
	//virtual void _NewLocalAxisInfo(math::matrix<float>,pnCVector3) = 0;
	//нашли новый материал
	virtual void _NewMaterial(pnSMaterial) = 0;
	//конец загрузки
	virtual void _EofLoading() = 0;
private:
	//функция узнает размер текущего файла и заносит его в m_ulFileSize
	void _FileSize();
	//сдвиг указателя на указанное число байт
	void _MovePC(unsigned long);

	//разбор чанков в текущем файле
	void _ParseChunks();
	//конец разбора файла
	void _EofParsing();
	//разбор чанка материала в текущем файле
	void _ParseMaterialChunk(pnS3dsChunk&);

	//пропускаем указанный чанк
	void _SkipChunk(pnS3dsChunk&);
	//читаем в буфер указанное число байт из файла
	bool _ReadBlock(char*,long);
	//читаем ASCIIZ строку из файла
	std::string _ReadASCIIZ();
	//читаем в текущем чанке три байта цвета
	//- RGB и конвертируем их в три вещественных числа
	void _ReadColor(pnS3dsChunk&,float&,float&,float&);
	//читаем какой либо чанк, содержащий процентное соотношение чего либо
	//например CHUNK_SHININESS
	void _ReadPercentValue(pnS3dsChunk&,float&);

	//читаем блок объекта
	void _LoadObjBlock();
	//читаем список вершин объекта
	void _LoadVertList();
	//читаем список граней (треугольников) объекта
	void _LoadFaceList();
	//читаем информацию о материале, который используют грани
	void _LoadFaceMat();
	//читаем информацию о координатах текстуры для каждой вершины
	void _LoadMapList();
	//читаем информацию о локальной СК объекта
	//void _LoadLocalAxisInfo();
private:
	//указатель на текущую позицию в файле
	unsigned long m_ulPC;
	//размер файла
	unsigned long m_ulFileSize;
	//поток входного файла
	std::ifstream m_fileInputFile;
};

#endif //_PNC3DSFILELOADER_H_
