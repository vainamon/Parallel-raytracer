#ifndef PN3DTRAJECTORY_H_
#define PN3DTRAJECTORY_H_

#include "pnCPoint3.h"
#include <vector>

//����� - ���������� � ���������� ������������
//����� �����
class pnC3dTrajectory
{
public:
	pnC3dTrajectory();
	virtual ~pnC3dTrajectory();
	
	//�������� ����� � ����������, � �����
	void AddPointBack(const pnCPoint3& _pnt){m_vecPoints.push_back(_pnt);};
	
	//������� ����� �����, ������������ ����������
	int GetPointsNum() const {return m_vecPoints.size();};
	//������� ����� �� �� ������
	pnCPoint3 operator [] (int _i) const{return m_vecPoints[_i];};
	
	//������� ������ ����� ����������
	void Clear(){m_vecPoints.clear();};
private:
	//����� ����������
	std::vector<pnCPoint3> m_vecPoints;
};

#endif /*PN3DTRAJECTORY_H_*/
