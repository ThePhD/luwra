/* Luwra
 * Minimal-overhead Lua wrapper for C++
 *
 * Copyright (C) 2016, Ole Krüger <ole@vprsm.de>
 */

#ifndef LUWRA_INTERNAL_H_
#define LUWRA_INTERNAL_H_

#include "common.hpp"
#include "internal/common.hpp"
#include "internal/types.hpp"
#include "internal/indexsequence.hpp"
#include "internal/typelist.hpp"
#include "internal/callableinfo.hpp"

LUWRA_NS_BEGIN

namespace internal {
	template <typename T>
	struct MemberInfo {
		static_assert(sizeof(T) == -1, "Template parameter to MemberInfo is not a member type");
	};

	// Information about methods
	template <typename Klass, typename Ret, typename... Args>
	struct MemberInfo<Ret (Klass::*)(Args...)>:
		CallableInfo<Ret (Klass::*)(Args...)>
	{
		using MemberOf = Klass;
	};

	template <typename Klass, typename Ret, typename... Args>
	struct MemberInfo<Ret (Klass::*)(Args...) const>:
		MemberInfo<Ret (Klass::*)(Args...)> {};

	template <typename Klass, typename Ret, typename... Args>
	struct MemberInfo<Ret (Klass::*)(Args...) volatile>:
		MemberInfo<Ret (Klass::*)(Args...)> {};

	template <typename Klass, typename Ret, typename... Args>
	struct MemberInfo<Ret (Klass::*)(Args...) const volatile>:
		MemberInfo<Ret (Klass::*)(Args...)> {};

	// Information about fields
	template <typename Klass, typename Field>
	struct MemberInfo<Field Klass::*> {
		using MemberOf = Klass;
		using FieldType = Field;
	};

	// Check whether
	template <
		template <typename, typename> class,
		typename,
		typename
	>
	struct IsPrefixOf:
		BoolConstant<false> {};

	template <
		template <typename, typename> class Compare,
		typename... Rest
	>
	struct IsPrefixOf<Compare, TypeList<>, TypeList<Rest...>>:
		BoolConstant<true> {};

	template <
		template <typename, typename> class Compare,
		typename Prefix,
		typename Base,
		typename... BaseTail
	>
	struct IsPrefixOf<Compare, TypeList<Prefix>, TypeList<Base, BaseTail...>>:
		Compare<Prefix, Base> {};

	template <
		template <typename, typename> class Compare,
		typename Prefix,
		typename... PrefixTail,
		typename Base,
		typename... BaseTail
	>
	struct IsPrefixOf<Compare, TypeList<Prefix, PrefixTail...>, TypeList<Base, BaseTail...>>:
		BoolConstant<
			Compare<Prefix, Base>::value &&
			IsPrefixOf<Compare, TypeList<PrefixTail...>, TypeList<BaseTail...>>::value
		> {};

	template <
		template <typename, typename> class,
		typename,
		typename
	>
	struct Match:
		BoolConstant<false> {};

	template <
		template <typename, typename> class Compare
	>
	struct Match<Compare, TypeList<>, TypeList<>>:
		BoolConstant<true> {};

	template <
		template <typename, typename> class Compare,
		typename Left,
		typename Right
	>
	struct Match<Compare, TypeList<Left>, TypeList<Right>>:
		Compare<Left, Right> {};

	template <
		template <typename, typename> class Compare,
		typename Left,
		typename... LeftTail,
		typename Right,
		typename... RightTail
	>
	struct Match<Compare, TypeList<Left, LeftTail...>, TypeList<Right, RightTail...>>:
		BoolConstant<
			Compare<Left, Right>::value &&
			Match<Compare, TypeList<LeftTail...>, TypeList<RightTail...>>::value
		> {};
}

LUWRA_NS_END

#endif
