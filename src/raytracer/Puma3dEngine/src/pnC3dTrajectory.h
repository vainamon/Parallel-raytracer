#ifndef PN3DTRAJECTORY_H_
#define PN3DTRAJECTORY_H_

#include "pnCPoint3.h"
#include <vector>

//класс - траектория в трехмерном пространстве
//набор точек
class pnC3dTrajectory
{
public:
	pnC3dTrajectory();
	virtual ~pnC3dTrajectory();
	
	//добавить точку к траектории, в конец
	void AddPointBack(const pnCPoint3& _pnt){m_vecPoints.push_back(_pnt);};
	
	//возврат числа точек, составляющих траектроию
	int GetPointsNum() const {return m_vecPoints.size();};
	//возврат точки по ее номеру
	pnCPoint3 operator [] (int _i) const{return m_vecPoints[_i];};
	
	//очищает список точек траектории
	void Clear(){m_vecPoints.clear();};
private:
	//точки траектории
	std::vector<pnCPoint3> m_vecPoints;
};

#endif /*PN3DTRAJECTORY_H_*/
