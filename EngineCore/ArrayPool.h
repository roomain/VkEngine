#pragma once
/***********************************************
* @headerfile ArrayPool.h
* @date 09 / 03 / 2026
* @author Roomain
************************************************/
#include <optional>
#include "ScopedLink.h"

/*@brief used with thread pool:
* Contains table of object each one 
* can be used by only one thread
*/
template<typename Type, size_t Size>
class ArrayPool
{
private:
	std::array<Type, Size> m_data;
	std::array<bool, Size> m_inUsed;

public:
	ArrayPool()
	{
		// init use 
		for (auto& isUsed : m_inUsed)
			isUsed = false;
	}

	ArrayPool(std::array<Type, Size> a_data) : ArrayPool(),
		m_data{ a_data }
	{
	}

	Type& operator[] (const unsigned int a_index)
	{
		return m_data[a_index];
	}

	Type* data()
	{
		return m_data.data();
	}

	const Type* data()const
	{
		return m_data.data();
	}

	constexpr size_t size()const
	{
		return m_data.size();
	}

	std::optional<ScopedLink<Type>> next()
	{
		int index = 0;
		for (auto& isUsed : m_inUsed)
		{
			if (!isUsed)
			{
				return std::make_optional<ScopedLink<Type>>(m_data[index], isUsed)(m_data[index], isUsed);
			}
			++index;
		}
		return std::optional<ScopedLink<Type>>();
	}
};