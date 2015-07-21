#ifndef _PNPRIMITIVESTRUCTS_H_
#define _PNPRIMITIVESTRUCTS_H_

#include "pnCVector3.h"
#include "pnCPoint3.h"
#include <string>

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

#include <vector>

//��������� ������������ � �����
struct pnSFace{
	//������� ������
	unsigned short m_usIndices[3];
	//������ ���������
	int m_nMatIdx;
};

//���������� ��������
struct pnSUV{
	float m_fU;
	float m_fV;
};

//��������
struct pnSMaterial{
	//��� ���������
	std::string m_strName;
	//��� ����� � ���������
	std::string m_strTextureFile;
	//������������ ����� ���������
	float m_fAmbient[4];
	float m_fDiffuse[4];
	float m_fSpecular[4];
	//���������� ���������
	float m_fShininess;
};

#endif //_PNPRIMITIVESTRUCTS_H_
