#include "pnC3dObject.h"

pnC3dObject::pnC3dObject()
{
	m_strName="";
	m_vcrTranslate.Set(0.0,0.0,0.0);
	//SetDefaultGlobalTrMx();
}

pnC3dObject::~pnC3dObject()
{
}

void pnC3dObject::AddVertex(float _x, float _y, float _z)
{
	pnCPoint3 Vertex(_x,_y,_z);
	m_vecVertices.push_back(Vertex);
}

void pnC3dObject::AddFace(unsigned short _a,unsigned short _b
							,unsigned short _c,unsigned short _flags
							,unsigned short _idx)
{
	pnSFace Face;
	Face.m_usIndices[0]=_a;
	Face.m_usIndices[1]=_b;
	Face.m_usIndices[2]=_c;
	Face.m_nMatIdx = -1;

	if (m_vecFaces.size()>0){
		if(_idx<m_vecFaces.size())
			m_vecFaces[_idx] = Face;
		else
			m_vecFaces.push_back(Face);
	}else
		m_vecFaces.push_back(Face);
}

void pnC3dObject::AddFaceMat(std::string _name,unsigned short _idx)
{
	int i = 0;
	unsigned short MatIdx = 0;

	while((i<m_vecMaterials.size())&&(m_vecMaterials[i].m_strName != _name))
		i++;

	if(i<m_vecMaterials.size()){
		MatIdx = i;
	}else{
		pnSMaterial NewMat;
		NewMat.m_strName = _name;
		NewMat.m_strTextureFile = "";
		m_vecMaterials.push_back(NewMat);
		MatIdx = m_vecMaterials.size()-1;
	}

	if(_idx<m_vecFaces.size()){
		m_vecFaces[i].m_nMatIdx = MatIdx;
	}else{
		pnSFace Face;
		Face.m_nMatIdx = MatIdx;
		Face.m_usIndices[0] = 0;
		Face.m_usIndices[1] = 0;
		Face.m_usIndices[2] = 0;
		m_vecFaces.resize(_idx+1);
		m_vecFaces[_idx] = Face;
	}
}

void pnC3dObject::AddMapVertex(float _u, float _v)
{
	pnSUV UV;
	UV.m_fU = _u;
	UV.m_fV = _v;

	m_vecTexCoords.push_back(UV);
}

//void pnC3dObject::SetLocalTransform(math::matrix<float> _trm,pnCVector3 _trlate)
//{
//	m_mxLocalTransform = _trm;
//	m_vcrTranslate = _trlate;
//}

void pnC3dObject::AddMaterial(pnSMaterial _material)
{
	int i = 0;

	while((i<m_vecMaterials.size())&&(m_vecMaterials[i].m_strName != _material.m_strName))
		i++;

	if(i<m_vecMaterials.size())
		m_vecMaterials[i] = _material;
	else
		m_vecMaterials.push_back(_material);
}

void pnC3dObject::ComputeNormals()
{
	pnCVector3 Vector1, Vector2, Vector3;

	int i = 0;
	int j = 0;

	pnCVector3* FaceNormals = new pnCVector3[m_vecFaces.size()];

	for(i=0; i < m_vecFaces.size(); i++){
		Vector1.SetDiff(m_vecVertices[m_vecFaces[i].m_usIndices[0]]
						,m_vecVertices[m_vecFaces[i].m_usIndices[2]]);
		Vector2.SetDiff(m_vecVertices[m_vecFaces[i].m_usIndices[2]]
						,m_vecVertices[m_vecFaces[i].m_usIndices[1]]);

		Vector3.Set(Vector1.Cross(Vector2));
		Vector3.Normalize();

		FaceNormals[i] = Vector3;
	}

	pnCVector3 SumVector(0.0, 0.0, 0.0);
	int shared=0;

	for (i = 0; i < m_vecVertices.size(); i++){
		for (j = 0; j < m_vecFaces.size(); j++){
				if (m_vecFaces[j].m_usIndices[0] == i ||
					m_vecFaces[j].m_usIndices[1] == i ||
					m_vecFaces[j].m_usIndices[2] == i)
				{
					SumVector = SumVector + FaceNormals[j];
					shared++;
				}
		}
		if (shared != 0)
			SumVector = SumVector/float(-shared);
		SumVector.Normalize();
		m_vecNormals.push_back(SumVector);

		SumVector.Set(0.0, 0.0, 0.0);
		shared = 0;
	}

	delete[] FaceNormals;
}

pnCPoint3 pnC3dObject::GetVertex(unsigned short _idx) const
{
	if (_idx<m_vecVertices.size())
		return m_vecVertices[_idx];
	else
		return pnCPoint3(0.0,0.0,0.0);
}

pnSUV pnC3dObject::GetUV(unsigned short _idx) const
{
	if (_idx<m_vecTexCoords.size())
		return m_vecTexCoords[_idx];
	else{
		pnSUV UV;
		UV.m_fU = 0.0;
		UV.m_fV = 0.0;
		return UV;
	}
}

pnSFace pnC3dObject::GetFace(unsigned short _idx) const
{
	if (_idx<m_vecFaces.size())
		return m_vecFaces[_idx];
	else{
		pnSFace Face;
		Face.m_nMatIdx = -1;
		Face.m_usIndices[0] = 0;
		Face.m_usIndices[1] = 0;
		Face.m_usIndices[2] = 0;
		return Face;
	}
}

pnCVector3 pnC3dObject::GetNormal(unsigned short _idx) const
{
	if (_idx<m_vecNormals.size())
		return m_vecNormals[_idx];
	else
		return pnCVector3(0.0,0.0,0.0);
}

pnSMaterial pnC3dObject::GetMaterial(int _idx) const
{
	if (_idx<m_vecMaterials.size())
		return m_vecMaterials[_idx];
	else{
		pnSMaterial Material;
		Material.m_strName = "";
		Material.m_strTextureFile = "";
		Material.m_fAmbient[0]=0.0;Material.m_fAmbient[1]=0.0;Material.m_fAmbient[2]=0.0;
		Material.m_fDiffuse[0]=0.0;Material.m_fDiffuse[1]=0.0;Material.m_fDiffuse[2]=0.0;
		Material.m_fSpecular[0]=0.0;Material.m_fSpecular[1]=0.0;Material.m_fSpecular[2]=0.0;
		return Material;
	}
}

//void pnC3dObject::SetGlobalTransformMx(math::matrix<float> _mx)
//{
//	m_mxGlobalTransform = _mx;
//}
//
//void pnC3dObject::SetDefaultGlobalTrMx()
//{
//	int i = 0;
//	int j = 0;
//	m_mxGlobalTransform.SetSize(4,4);
//	for(i = 0;i<m_mxGlobalTransform.RowNo();i++)
//		for(j = 0;j<m_mxGlobalTransform.ColNo();j++)
//			i==j?m_mxGlobalTransform(i,j)=1:m_mxGlobalTransform(i,j)=0;
//}
//
//void pnC3dObject::GetGlobalTransformMx(float* _trMatrix) const
//{
//	try{
//		if((m_mxGlobalTransform.RowNo()<3)&&(m_mxGlobalTransform.ColNo()<3)){
//			_trMatrix[0] = 1;_trMatrix[4] = 0;_trMatrix[8] = 0;_trMatrix[12] = 0;
//			_trMatrix[1] = 0;_trMatrix[5] = 1;_trMatrix[9] = 0;_trMatrix[13] = 0;
//			_trMatrix[2] = 0;_trMatrix[6] = 0;_trMatrix[10] = 1;_trMatrix[14] = 0;
//			_trMatrix[3] = 0;_trMatrix[7] = 0;_trMatrix[11] = 0;_trMatrix[15] = 1;
//			return;
//		}
//
//		_trMatrix[0] = m_mxGlobalTransform(0,0);_trMatrix[4] = m_mxGlobalTransform(1,0);
//		_trMatrix[8] = m_mxGlobalTransform(2,0);_trMatrix[12] = m_mxGlobalTransform(3,0);
//
//		_trMatrix[1] = m_mxGlobalTransform(0,1);_trMatrix[5] = m_mxGlobalTransform(1,1);
//		_trMatrix[9] = m_mxGlobalTransform(2,1);_trMatrix[13] = m_mxGlobalTransform(3,1);
//
//		_trMatrix[2] = m_mxGlobalTransform(0,2);_trMatrix[6] = m_mxGlobalTransform(1,2);
//		_trMatrix[10] = m_mxGlobalTransform(2,2);_trMatrix[14] = m_mxGlobalTransform(3,2);
//
//		_trMatrix[3] = m_mxGlobalTransform(0,3);_trMatrix[7] = m_mxGlobalTransform(1,3);
//		_trMatrix[11] = m_mxGlobalTransform(2,3);_trMatrix[15] = m_mxGlobalTransform(3,3);
//	}
//	catch (...){
//		throw;
//	}
//}

pnCPoint3 pnC3dObject::GetCSCenter() const
{
	return pnCPoint3(m_vcrTranslate.GetX(),m_vcrTranslate.GetY(),m_vcrTranslate.GetZ());
}
