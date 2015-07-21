#ifndef _CERROR_H_
#define _CERROR_H_

#include <string>

class CError{
//Класс для любых ошибок
public:
	CError(const std::string& _error):m_strError(_error){};
	std::string GetError(){return m_strError;};
private:
	std::string m_strError;
};

#endif //#ifndef _CERROR_H_
