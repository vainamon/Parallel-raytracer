#ifndef _PNC3DRENDERI_H_
#define _PNC3DRENDERI_H_

#include "pnC3dObject.h"
#include "pnC3dTrajectory.h"

//��������� ������ ��� ��������� 3� �������
class pnC3dRenderI{
public:	
	//����������� ��������� ������������� �������
	virtual void InitRender() const = 0;
	//������� ��������� 3� �������
	virtual void Render3dObject(const pnC3dObject&) const = 0;
	//������� ��������� ����� � ���������� ������������
	//���������: ����������, �������, rgb-����
	virtual void Render3dTrajectory(const pnC3dTrajectory&,
								float,const float[]) const = 0;
};

#endif //_PNC3DRENDERI_H_
