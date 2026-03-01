#pragma once
/***********************************************
* @headerfile reflection_traits.h
* @date 26 / 02 / 2026
* @author Roomain
************************************************/
#include <type_traits>
#include <vector>
#include <array>
#include <list>

// Type traits check if type is vector
template<typename Type>
struct is_std_vector : std::false_type {};


template<typename Type, typename Alloc>
struct is_std_vector<std::vector<Type, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

//-------------------------------------------------------------------------------
// Type traits check if type is list
template<typename Type>
struct is_std_list : std::false_type {};


template<typename Type, typename Alloc>
struct is_std_list<std::list<Type, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_std_list_v = is_std_list<T>::value;
//-------------------------------------------------------------------------------
// Type traits check if type is array
template<typename Type>
struct is_std_array : std::false_type {};


template<typename Type, std::size_t Size>
struct is_std_array<std::array<Type, Size>> : std::true_type {};

template<typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;

//--------------------------------------------------------------------------------

/*@brief indicate if object is reflective*/
template<typename Type>
struct is_reflective
{
private:
	// template used for compile time evaluation
	template<typename C>
	static constexpr std::true_type reflective(decltype(&C::deserialize));

	template<typename C>
	static constexpr std::false_type reflective(...);

public:
	enum { value = reflective<Type>(0) };
};
