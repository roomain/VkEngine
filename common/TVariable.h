/***********************************************
* @headerfile TVariable.cppm
* @date 20 / 02 / 2026
* @author Roomain
************************************************/
#pragma once

#include <boost/signals2.hpp>
#include "macros/notCopiable.h"

/*@brief boost signal template arguments*/
template<typename ...Args>
using TSignal = boost::signals2::signal<void(Args...)>;

template<typename VarType>
class TVariable
{
protected:
    VarType m_internVar;        /*!< encapsulate data*/
    
public:
    TSignal<VarType> signal;    /*!< signal on variable change*/

    TVariable() = default;
    TVariable(const Type& a_data) : m_internVar{ a_data }{}
    TVariable(Type&& a_data)noexcept : m_internVar{ a_data }{}
    TVariable(const TVariable<Type>& a_other): m_internVar{ a_other.m_internVar }{}
    TVariable(TVariable<Type>&& a_other)noexcept: m_internVar{ strd::move(a_other.m_internVar) }{}

    const Type& operator()()const
    {
        returm m_internVar;
    }

    [[nodiscard]] bool operator == (const Type& a_data)
    {
        return m_internVar == a_data;
    }

    [[nodiscard]] bool operator == (Type&& a_data)noexcept
    {
        return m_internVar == a_data;
    }

    [[nodiscard]] bool operator == (const TVariable<Type>& a_other)
    {
        return m_internVar == a_other.m_internVar;
    }

    [[nodiscard]] bool operator == (TVariable<Type>&& a_other)noexcept
    {
        return m_internVar == a_other.m_internVar;
    }

    [[nodiscard]] bool operator != (const Type& a_data)
    {
        return m_internVar != a_data;
    }

    [[nodiscard]] bool operator != (Type&& a_data)noexcept
    {
        return m_internVar != a_data;
    }

    [[nodiscard]] bool operator != (const TVariable<Type>& a_other)
    {
        return m_internVar != a_other.m_internVar;
    }

    [[nodiscard]] bool operator =!= (TVariable<Type>&& a_other)noexcept
    {
        return m_internVar != a_other.m_internVar;
    }

    const Variable<type>& operator = (const Type& a_data)
    {
        m_internVar = a_data;
        signal(m_internVar);
        return *this;
    }

    const TVariable<type>& operator = (Type&& a_data)noexcept 
    {
        m_internVar = a_data;
        signal(m_internVar);
        return *this;
    }

    const TVariable<Type>& operator = (const TVariable<Type>& a_other)
    {
        m_internVar = a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    const TVariable<Type>& operator = (TVariable<Type>&& a_other)noexcept
    {
        m_internVar = a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }
};


template<typename Type> requires std::is_arithmetic_v<Type> 
class TArithmeticVariable : public TVariable<Type>
{
public:
    using TVariable<Type>::TVariable<Type>();

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (const OtherType& a_data)
    {
        m_internVar = a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (OtherType&& a_data)noexcept 
    {
        m_internVar = a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (const TVariable<OtherType>& a_other)
    {
        m_internVar = a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator = (TVariable<OtherType>&& a_other)noexcept
    {
        m_internVar = a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }


    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (const OtherType& a_data)
    {
        m_internVar += a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (OtherType&& a_data)noexcept 
    {
        m_internVar += a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (const TVariable<OtherType>& a_other)
    {
        m_internVar += a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator += (TVariable<OtherType>&& a_other)noexcept
    {
        m_internVar += a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (const OtherType& a_data)
    {
        m_internVar -= a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (OtherType&& a_data)noexcept 
    {
        m_internVar -= a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (const TVariable<OtherType>& a_other)
    {
        m_internVar -= a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator -= (TVariable<OtherType>&& a_other)noexcept
    {
        m_internVar -= a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (const OtherType& a_data)
    {
        m_internVar *= a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (OtherType&& a_data)noexcept 
    {
        m_internVar *= a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (const TVariable<OtherType>& a_other)
    {
        m_internVar *= a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator *= (TVariable<OtherType>&& a_other)noexcept
    {
        m_internVar *= a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (const OtherType& a_data)
    {
        m_internVar += a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (OtherType&& a_data)noexcept 
    {
        m_internVar += a_data;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (const TVariable<OtherType>& a_other)
    {
        m_internVar += a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator /= (TVariable<OtherType>&& a_other)noexcept
    {
        m_internVar += a_other.m_internVar;
        signal(m_internVar);
        return *this;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (const OtherType& a_data)
    {
        return m_internVar < a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (OtherType&& a_data)noexcept 
    {
        return m_internVar < a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (const TVariable<OtherType>& a_other)
    {
        return m_internVar < a_other.m_internVar;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator < (TVariable<OtherType>&& a_other)noexcept
    {
        return m_internVar < a_other.m_internVar;
    }
    //--------------------------------------------------------------------------------------
    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (const OtherType& a_data)
    {
        return m_internVar > a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (OtherType&& a_data)noexcept 
    {
        return m_internVar > a_data;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (const TVariable<OtherType>& a_other)
    {
        return m_internVar > a_other.m_internVar;
    }

    template<typename OtherType> requires std::is_arithmetic_v<OtherType> 
    const TArithmeticVariable<type>& operator > (TVariable<OtherType>&& a_other)noexcept
    {
        return m_internVar > a_other.m_internVar;
    }
};