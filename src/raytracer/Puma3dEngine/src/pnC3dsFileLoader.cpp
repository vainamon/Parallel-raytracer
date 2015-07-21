#include "pnC3dsFileLoader.h"

pnC3dsFileLoader::pnC3dsFileLoader():m_ulPC(0),m_ulFileSize(0)
{
}

pnC3dsFileLoader::~pnC3dsFileLoader()
{
}

void pnC3dsFileLoader::_ParseFile(std::string _fName)
{
	this->m_fileInputFile.open(_fName.c_str(),std::ios_base::binary);
	_FileSize();
	if(!m_fileInputFile.is_open())
		throw;

	_ParseChunks();
	_EofParsing();
}

void pnC3dsFileLoader::_FileSize()
{
	m_fileInputFile.seekg(std::ios_base::end);
	m_ulFileSize = m_fileInputFile.tellg();
	m_fileInputFile.seekg(std::ios_base::beg);
}

void pnC3dsFileLoader::_MovePC(unsigned long _bytes)
{
	m_ulPC+=_bytes;
	m_fileInputFile.seekg(m_ulPC);
}

void pnC3dsFileLoader::_ParseChunks()
{
	if(!m_fileInputFile.is_open())
		return;
	if(m_fileInputFile.eof())
		return;

	pnS3dsChunk CurrentChunk;
	char* buf = (char*)&CurrentChunk;

	while(_ReadBlock((char*)&CurrentChunk,sizeof(pnS3dsChunk))){

		CurrentChunk.m_ulChunkSize;
		switch(CurrentChunk.m_usChunkIdentifer){
			case CHUNK_MAIN:
				break;
			case CHUNK_OBJMESH:
				break;
			case CHUNK_OBJBLOCK:
				_LoadObjBlock();
				break;
			case CHUNK_TRIMESH:
				break;
			case CHUNK_VERTLIST:
				_LoadVertList();
				break;
			case CHUNK_FACELIST:
				_LoadFaceList();
				break;
			case CHUNK_FACEMAT:
				_LoadFaceMat();
				break;
			case CHUNK_MAPLIST:
				_LoadMapList();
				break;
			case CHUNK_TRMATRIX:
				_SkipChunk(CurrentChunk);
				//_LoadLocalAxisInfo();
				break;
			case CHUNK_MATERIAL:
				_ParseMaterialChunk(CurrentChunk);
				break;
			default:
				_SkipChunk(CurrentChunk);
				break;
		};
	}
}

void pnC3dsFileLoader::_EofParsing()
{
	_EofLoading();
}

void pnC3dsFileLoader::_ParseMaterialChunk(pnS3dsChunk& _curch)
{
	if(!m_fileInputFile.is_open())
		return;
	if(m_fileInputFile.eof())
		return;

	pnS3dsChunk CurrentChunk;
	unsigned long BytesRead = 0;
	pnSMaterial Material;

	while(BytesRead<_curch.m_ulChunkSize){
		if(!_ReadBlock((char*)&CurrentChunk,sizeof(pnS3dsChunk)))
			return;
		switch(CurrentChunk.m_usChunkIdentifer){
			case CHUNK_MATNAME:
				Material.m_strName = _ReadASCIIZ();
				break;
			case CHUNK_AMBIENT:
				_ReadColor(CurrentChunk,Material.m_fAmbient[0]
							,Material.m_fAmbient[1]
							,Material.m_fAmbient[2]);
				Material.m_fAmbient[3] = 1;
				break;
			case CHUNK_DIFFUSE:
				_ReadColor(CurrentChunk,Material.m_fDiffuse[0]
							,Material.m_fDiffuse[1]
							,Material.m_fDiffuse[2]);
				Material.m_fDiffuse[3] = 1;
				break;
			case CHUNK_SPECULAR:
				_ReadColor(CurrentChunk,Material.m_fSpecular[0]
							,Material.m_fSpecular[1]
							,Material.m_fSpecular[2]);
				Material.m_fSpecular[3] = 1;
				break;
			case CHUNK_SHININESS:
				float Shininess;
				_ReadPercentValue(CurrentChunk,Shininess);
				Material.m_fShininess = (float)Shininess/100.0;
				break;
			case CHUNK_TEXTURE:
				break;
			case CHUNK_MAPFILE:
				Material.m_strTextureFile = _ReadASCIIZ();
				break;
			default:
				_SkipChunk(CurrentChunk);
				break;
		};
		BytesRead+=CurrentChunk.m_ulChunkSize+sizeof(pnS3dsChunk);
	}
	_NewMaterial(Material);
}

bool pnC3dsFileLoader::_ReadBlock(char* _buf,long _size)
{
	m_ulPC=m_fileInputFile.tellg();
	if((!m_fileInputFile.read(_buf,_size))&&(m_fileInputFile.eof()))
		return false;
	m_ulPC+=_size;
	m_fileInputFile.seekg(m_ulPC);
	return true;
}

void pnC3dsFileLoader::_ReadColor(pnS3dsChunk& _curch,float& _r,float& _g
														, float& _b)
{
	if(!m_fileInputFile.is_open())
		return;
	if(m_fileInputFile.eof())
		return;

	pnS3dsChunk CurrentChunk;
	unsigned long BytesRead = 0;

	unsigned char RGBB[3];
	float RGBF[3];

	while(BytesRead<_curch.m_ulChunkSize){
		if(!_ReadBlock((char*)&CurrentChunk,sizeof(pnS3dsChunk)))
			return;
		switch(CurrentChunk.m_usChunkIdentifer){
			case CHUNK_RGBF:
				//читаем составляющие цвета
				if (!_ReadBlock((char*)&RGBF,sizeof(RGBF)))
					return;

				_r = RGBF[0];
				_g = RGBF[1];
				_b = RGBF[2];
				break;
			case CHUNK_RGBB:
				//читаем составляющие цвета
				if (!_ReadBlock((char*)&RGBB,sizeof(RGBB)))
					return;

				_r = 1.0-(255.0-RGBB[0])/255.0;
				_g = 1.0-(255.0-RGBB[1])/255.0;
				_b = 1.0-(255.0-RGBB[2])/255.0;
				break;
			default:
				_SkipChunk(CurrentChunk);
				break;
		};
		BytesRead+=CurrentChunk.m_ulChunkSize+sizeof(pnS3dsChunk);
	}
}

void pnC3dsFileLoader::_ReadPercentValue(pnS3dsChunk& _curch, float& _percentage)
{
	if(!m_fileInputFile.is_open())
		return;
	if(m_fileInputFile.eof())
		return;

	pnS3dsChunk CurrentChunk;
	unsigned long BytesRead = 0;

	unsigned short PercentageB = 0;
	float PercentageF = 0;

	while(BytesRead<_curch.m_ulChunkSize){
		if(!_ReadBlock((char*)&CurrentChunk,sizeof(pnS3dsChunk)))
			return;
		switch(CurrentChunk.m_usChunkIdentifer){
			case CHUNK_PERCENTW:
				if (!_ReadBlock((char*)&PercentageB,sizeof(PercentageB)))
					return;
				_percentage = PercentageB;
				break;
			case CHUNK_PERCENTF:
				if (!_ReadBlock((char*)&PercentageF,sizeof(PercentageF)))
					return;
				_percentage = PercentageF;
				break;
			default:
				_SkipChunk(CurrentChunk);
				break;
		};
		BytesRead+=CurrentChunk.m_ulChunkSize+sizeof(pnS3dsChunk);
	}
}

void pnC3dsFileLoader::_SkipChunk(pnS3dsChunk& _chunk)
{
	m_ulPC+=_chunk.m_ulChunkSize-sizeof(pnS3dsChunk);
	m_fileInputFile.seekg(m_ulPC);
}

std::string pnC3dsFileLoader::_ReadASCIIZ()
{
	char c = ' ';
	long count = 0;
	std::string result="";

	do{
		c = m_fileInputFile.get();
		result.push_back(c);
		count++;
	}while (c!=0);
	result.erase(result.size()-1,result.size());

	m_ulPC+=count;
	m_fileInputFile.seekg(m_ulPC);

	return result;
}

void pnC3dsFileLoader::_LoadObjBlock()
{
	//читаем имя объекта
	_NewObject(_ReadASCIIZ());
}

void pnC3dsFileLoader::_LoadVertList()
{
	unsigned short VertexCount;
	float vertex[3];

	//считываем количество вершин
	if (!_ReadBlock((char*)&VertexCount,2))
		return;

    while (VertexCount-- > 0)
	{
		//читаем координаты очередной вершины
		if (!_ReadBlock((char*)vertex, sizeof(vertex)))
			return;
		_New3dVertex(vertex[0], vertex[2], -vertex[1]);
    }
}

void pnC3dsFileLoader::_LoadFaceList()
{
    unsigned short FaceCount;
    unsigned short FaceInfo[4];
	unsigned short i = 0;

	//читаем число полигонов объекта
    if (!_ReadBlock((char*)&FaceCount, sizeof(FaceCount)))
		return;
    while (FaceCount-- > 0)
	{
        if (!_ReadBlock((char*) FaceInfo, sizeof(FaceInfo)))
			return;
		_New3dFace(FaceInfo[0], FaceInfo[1], FaceInfo[2], FaceInfo[3],i);

		i++;
    }
}

void pnC3dsFileLoader::_LoadFaceMat()
{
	unsigned short FacesCount, FaceNumber;
	std::string MaterialName;

	//читаем имя материала
	MaterialName = _ReadASCIIZ();

	//читаем число граней, использующих этот материал
	if (!_ReadBlock((char *) &FacesCount, sizeof(FacesCount)))
		return;

	while (FacesCount-- > 0){
		//считываем индексы граней, использующие данный материал
		if (!_ReadBlock((char*)&FaceNumber, sizeof(FaceNumber)))
			return;
		_NewFaceMaterial(MaterialName, FaceNumber);
	}
}

void pnC3dsFileLoader::_LoadMapList()
{
	unsigned short VertexCount;
	float MapVert[2];

	//читаем число вершин
	if (!_ReadBlock((char*) &VertexCount, sizeof(VertexCount)))
		return;
	while (VertexCount-- > 0){
		//читаем координаты вершины
		if (!_ReadBlock((char*) MapVert, sizeof(MapVert)))
			return;
		_NewMapVertex(MapVert[0], MapVert[1]);
    }
}

//void pnC3dsFileLoader::_LoadLocalAxisInfo()
//{
//	float rotMatrix[9];
//	float transVector[3];
//
//	//читаем матрицу поворота
//	if (!_ReadBlock((char*) rotMatrix, sizeof(rotMatrix)))
//		return;
//	//читаем вектор переноса
//	if (!_ReadBlock((char*) transVector, sizeof(transVector)))
//		return;
//
//	math::matrix<float> Transform(3,3);
//	pnCVector3 Translate;
//
//	Transform(0,0) = rotMatrix[0]; Transform(0,1) = rotMatrix[1]; Transform(0,2) = rotMatrix[2];
//	Transform(1,0) = rotMatrix[3]; Transform(1,1) = rotMatrix[4]; Transform(1,2) = rotMatrix[5];
//	Transform(2,0) = rotMatrix[6]; Transform(2,1) = rotMatrix[7]; Transform(2,2) = rotMatrix[8];
//
//	Translate.Set(transVector[0], transVector[2], -transVector[1]);
//
//	_NewLocalAxisInfo(Transform, Translate);
//}
