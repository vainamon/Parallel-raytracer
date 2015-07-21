#ifndef _POINT3_H_
#define _POINT3_H_

//Класс точка в трехмерном пространстве

class point3{
public:
	point3():m_fX(0),m_fY(0),m_fZ(0){};

	point3(float _x,float _y,float _z)
		:m_fX(_x),m_fY(_y),m_fZ(_z){};

	point3(const point3& _pnt)
	{
		m_fX = _pnt.GetX();
		m_fY = _pnt.GetY();
		m_fZ = _pnt.GetZ();
	};

	void Set(float _x,float _y,float _z)
	{
		m_fX=_x;
		m_fY=_y;
		m_fZ=_z;
	}

	void Set(const point3& _pnt)
	{
		m_fX = _pnt.GetX();
		m_fY = _pnt.GetY();
		m_fZ = _pnt.GetZ();
	}

	float GetX() const {return m_fX;}
	float GetY() const {return m_fY;}
	float GetZ() const {return m_fZ;}
private:
	float m_fX,m_fY,m_fZ;
};

#endif //#ifndef _POINT3_H_
