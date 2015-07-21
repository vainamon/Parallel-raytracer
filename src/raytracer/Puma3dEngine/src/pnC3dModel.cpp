#include "pnC3dModel.h"

pnC3dModel::pnC3dModel() : pnC3dsFileLoader()
					, m_3dObjObject(NULL), m_3dRndrRender(NULL)
{
}

pnC3dModel::pnC3dModel(const pnC3dRenderI* _3dRnd)
										: pnC3dsFileLoader()
											, m_3dObjObject(NULL)
											, m_3dRndrRender(_3dRnd)
{
}

pnC3dModel::~pnC3dModel()
{
	int i=0;
	for(i=0;i<m_vec3dObjObjects.size();i++){
		if(m_vec3dObjObjects[i] != NULL)
			delete m_vec3dObjObjects[i];
	}
}

void pnC3dModel::_SetObject(pnC3dObject* _3dObj)
{
	m_3dObjObject = _3dObj;
}

void pnC3dModel::SetRender(const pnC3dRenderI* _render)
{
	m_3dRndrRender = _render;
}

void pnC3dModel::LoadModelFromFile(std::string _file)
{
	int i=0;

	for(i=0;i<m_vec3dObjObjects.size();i++){
		if(m_vec3dObjObjects[i] != NULL)
			delete m_vec3dObjObjects[i];
	}

	_ParseFile(_file);
}

void pnC3dModel::LoadModel()
{
}

void pnC3dModel::DestroyModel()
{
	int i=0;

	for(i=0;i<m_vec3dObjObjects.size();i++){
		if(m_vec3dObjObjects[i] != NULL)
			delete m_3dObjObject;
	}

	m_vec3dObjObjects.clear();
	m_3dObjObject = NULL;
}

void pnC3dModel::DrawModel()
{
	if(m_3dRndrRender == NULL) return;

	int i=0;

	for(i=0;i<m_vec3dObjObjects.size();i++){
		if(m_vec3dObjObjects[i] != NULL)
			m_3dRndrRender->Render3dObject(*m_vec3dObjObjects[i]);
	}
}

void pnC3dModel::_NewObject(std::string _name)
{
	m_3dObjObject = new pnC3dObject;
	m_3dObjObject->SetObjectName(_name);
	m_vec3dObjObjects.push_back(m_3dObjObject);
}

void pnC3dModel::_New3dVertex(float _x,float _y,float _z)
{
	if(m_3dObjObject != NULL)
		m_3dObjObject->AddVertex(_x,_y,_z);
}

void pnC3dModel::_New3dFace(unsigned short _a,unsigned short _b
							,unsigned short _c,unsigned short _flags
							,unsigned short _idx)
{
	if(m_3dObjObject != NULL)
		m_3dObjObject->AddFace(_a,_b,_c,_flags,_idx);
}

void pnC3dModel::_NewFaceMaterial(std::string _name, unsigned short _faceId)
{
	if(m_3dObjObject != NULL)
		m_3dObjObject->AddFaceMat(_name,_faceId);
}

void pnC3dModel::_NewMapVertex(float _u, float _v)
{
	if(m_3dObjObject != NULL)
		m_3dObjObject->AddMapVertex(_u,_v);
}

//void pnC3dModel::_NewLocalAxisInfo(math::matrix<float> _trm,pnCVector3 _trlate)
//{
//	if(m_3dObjObject != NULL)
//		m_3dObjObject->SetLocalTransform(_trm, _trlate);
//}

void pnC3dModel::_NewMaterial(pnSMaterial _material)
{
	m_vecMaterials.push_back(_material);
}

void pnC3dModel::_EofLoading()
{
	int i = 0;
	int j = 0;
	int k = 0;
	pnSMaterial CurMat;

	for(i = 0;i<m_vec3dObjObjects.size();i++){
		m_vec3dObjObjects[i]->ComputeNormals();

		//добавляем используемые материалы к объекту
		for(j = 0;j<m_vecMaterials.size();j++){
			for(k = 0;k<m_vec3dObjObjects[i]->GetMaterialsCount();k++){
				CurMat = m_vec3dObjObjects[i]->GetMaterial(k);
				if(CurMat.m_strName == m_vecMaterials[j].m_strName)
					m_vec3dObjObjects[i]->AddMaterial(m_vecMaterials[j]);
			}
		}
	}

	m_vecMaterials.clear();
}

//void pnC3dModel::SetGlobalTransformMx(math::matrix<float> _mx)
//{
//	int i=0;
//
//	for(i=0;i<m_vec3dObjObjects.size();i++){
//		if(m_vec3dObjObjects[i] != NULL)
//			m_vec3dObjObjects[i]->SetGlobalTransformMx(_mx);
//	}
//}
