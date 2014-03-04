#include "pch.h"
#define READ_ONLY 1
#define WRITE_ONLY 2
#define READ_WRITE 3

template <typename Container, typename ValueType, int nPropType>
class property
{
public:
	property()
	{
		m_cObject = NULL;
		Set = NULL;
		Get = NULL;
	}
	//-- This to set a pointer to the class that contain the
	//   property --
	void setContainer(Container* cObject)
	{
		m_cObject = cObject;

	}
	//-- Set the set member function that will change the value --
	void setter(void (Container::*pSet)(ValueType value))
	{
		if ((nPropType == WRITE_ONLY) || (nPropType == READ_WRITE))
			Set = pSet;
		else
			Set = NULL;
	}
	//-- Set the get member function that will retrieve the value --
	void getter(ValueType(Container::*pGet)())
	{
		if ((nPropType == READ_ONLY) || (nPropType == READ_WRITE))
			Get = pGet;
		else
			Get = NULL;
	}
	//-- Overload the '=' sign to set the value using the set
	//   member --
	ValueType operator =(const ValueType& value)
	{
		assert(m_cObject != NULL);
		assert(Set != NULL);
		(m_cObject->*Set)(value);
		return value;
	}
	//-- To make possible to cast the property class to the
	//   internal type --
	operator ValueType()
	{
		assert(m_cObject != NULL);
		assert(Get != NULL);
		return (m_cObject->*Get)();
	}
private:
	Container* m_cObject;  //-- Pointer to the module that
	//   contains the property --
	void (Container::*Set)(ValueType value);
	//-- Pointer to set member function --
	ValueType(Container::*Get)();
	//-- Pointer to get member function --
};