#pragma once
/***********************************************
* @headerfile ScopedLink.h
* @date 09 / 03 / 2026
* @author Roomain
************************************************/
#include <functional>
#include "notCopiable.h"

/*@brief working with array pool. Manage usage of a variable of type Type*/
template<typename Type>
class ScopedLink
{
private:
	std::reference_wrapper<Type> m_dataRef;		/*!< managed data*/
	std::reference_wrapper<bool> m_dataInUse;	/*!< managed using flag*/

public:
	ScopedLink() = delete;
	NOT_COPIABLE(ScopedLink)
	ScopedLink(ScopedLink&& a_other)noexcept : m_dataRef{ a_other.m_dataRef }, m_dataInUse{ a_other.m_dataInUse } {}
	ScopedLink(Type& a_data, bool& a_use) : m_dataRef{ a_data }, m_dataInUse{ a_use }
	{
		m_dataInUse.get() = true;
	}

	~ScopedLink()
	{
		m_dataInUse.get() = false;
	}

	Type& get()
	{
		return m_dataRef.get();
	}

	void operator = (const Type& data)
	{
		m_dataRef.get() = data;
	}

	void operator = (Type&& data)noexcept
	{
		m_dataRef.get() = data;
	}
};

