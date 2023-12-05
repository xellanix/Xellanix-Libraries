#ifndef XELLANIX_NUMBER_HELPER_H
#define XELLANIX_NUMBER_HELPER_H
#endif // !XELLANIX_NUMBER_HELPER_H

#ifndef XELLANIX_UTILITY_H
#include "utility.h"
#endif // !XELLANIX_UTILITY_H

#include <utility>
#include <limits>

// Functions:
// ipow, ceil, ceil2, floor, floor2
// is based on
// https://github.com/elbeno/constexpr/blob/master/src/include/cx_math.h
namespace xellanix::type::helper::detail
{
	template <typename... Types>
	struct is_integral;
	template <typename T>
	struct is_integral<T> : std::is_integral<T> {};
	template <typename T, typename... Rest>
	struct is_integral<T, Rest...>
	{
		static constexpr bool value = std::is_integral_v<T> && is_integral<Rest...>::value;
	};

	template <typename... Types>
	struct is_floating_point;
	template <typename T>
	struct is_floating_point<T> : std::is_floating_point<T> {};
	template <typename T, typename... Rest>
	struct is_floating_point<T, Rest...>
	{
		static constexpr bool value = std::is_floating_point_v<T> && is_floating_point<Rest...>::value;
	};

	template <typename... Types>
	struct is_arithmetic;
	template <typename T>
	struct is_arithmetic<T> : std::is_arithmetic<T> {};
	template <typename T, typename... Rest>
	struct is_arithmetic<T, Rest...>
	{
		static constexpr bool value = std::is_arithmetic_v<T> && is_arithmetic<Rest...>::value;
	};
}

namespace xellanix::type::helper
{
	#pragma region TYPE ALIASING

	template<typename T>
	using enable_if_arithm = typename std::enable_if_t<std::is_arithmetic_v<T>, bool>;
	template<typename... T>
	using enable_if_arithms = typename std::enable_if_t<detail::is_arithmetic<T...>::value, bool>;
	template<typename T>
	using enable_if_arithm_t = typename std::enable_if_t<std::is_arithmetic_v<T>, T>;
	template<typename... T>
	using enable_if_arithms_t = std::enable_if<detail::is_arithmetic<T...>::value, typename std::common_type_t<T...>>;

	template<typename T>
	using enable_if_integral = typename std::enable_if_t<std::is_integral_v<T>, bool>;
	template<typename... T>
	using enable_if_integrals = typename std::enable_if_t<detail::is_integral<T...>::value, bool>;

	template<typename T>
	using enable_if_floating_point = typename std::enable_if_t<std::is_floating_point_v<T>, bool>;
	template<typename... T>
	using enable_if_floating_points = typename std::enable_if_t<detail::is_floating_point<T...>::value, bool>;

	template<typename Source, typename Target>
	inline constexpr bool is_same_r_v = std::is_same_v<std::remove_cv_t<Source>, Target>;
	template<typename Source, typename Target>
	using enable_if_same = typename std::enable_if_t<is_same_r_v<Source, Target>, bool>;
	template<typename Source, typename Target>
	using enable_if_same_t = typename std::enable_if_t<is_same_r_v<Source, Target>, Target>;

	template<typename T, typename... NumericOptions>
	using enable_if_any_numerics = typename std::enable_if_t<std::is_arithmetic_v<T> || std::_Is_any_of_v<std::remove_cv_t<T>, NumericOptions...>, bool>;

	template<typename T>
	using enable_if_signed = typename std::enable_if_t<std::is_signed_v<T>, bool>;

	template<typename T>
	using enable_if_unsigned = typename std::enable_if_t<std::is_unsigned_v<T>, bool>;

	template<typename T>
	using enable_if_any_signed = typename std::enable_if_t<std::is_signed_v<T> || std::is_unsigned_v<T>, bool>;

	#pragma endregion

	#pragma region NUMERIC LIMITS

	template<typename T, enable_if_arithm<T> = true>
	inline constexpr decltype(auto) nmin()
	{
		return (std::numeric_limits<T>::min)();
	}

	template<typename T, enable_if_arithm<T> = true>
	inline constexpr decltype(auto) nmax()
	{
		return (std::numeric_limits<T>::max)();
	}

	template<typename T, enable_if_arithm<T> = true>
	inline constexpr decltype(auto) limit_range()
	{
		return std::pair{ nmin<T>(), nmax<T>() };
	}

	template<typename LT, typename RT>
	inline constexpr auto is_same_limit()
	{
		const auto lhs = xellanix::type::helper::limit_range<LT>();
		const auto rhs = xellanix::type::helper::limit_range<RT>();

		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	#pragma endregion
}