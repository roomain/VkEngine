#pragma once
/***********************************************
* @headerfile TSignal.h
* @date 16 / 09 / 2026
* @author Roomain
************************************************/
#include <boost/signals2.hpp>

/*@brief boost signal template arguments*/
template<typename ...Args>
using TSignal = boost::signals2::signal<void(Args...)>;