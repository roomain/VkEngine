/***********************************************
* @headerfile TCallOnce.cppm
* @date 20 / 02 / 2026
* @author Roomain
************************************************/
#pragma once

#include <atomic>
#include "macros/notCopiable.h"


/*@brief do action only one time in loop*/
template <typename Action>
class TCallOnce
{
    Action m_action;                    /*!< action to do*/
    std::atomic_bool m_isDone = false;  /*!< indicate if action is already done*/
public:
    TCallOnce() = delete;
    NOT_COPIABLE(TCallOnce)
    explicit TCallOnce(const Action& action) : m_action{ action } {}
    explicit TCallOnce(Action&& action)noexcept : m_action{ action } {}
    void operator ()()
    {
        if (!m_isDone)
        {
            m_action();
            m_isDone = true;
        }
    }
    [[nodiscard]] bool isExecuted()const { return m_isDone; }
};