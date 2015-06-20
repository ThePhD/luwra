/* Luwra
 * Minimal-overhead Lua wrapper for C++
 *
 * Copyright (C) 2015, Ole Krüger <ole@vprsm.de>
 */

#ifndef LUWRA_COMMON_H_
#define LUWRA_COMMON_H_

#include <lua.hpp>

// Check for proper Lua version
#if defined(LUA_VERSION_NUM)
	#if LUA_VERSION_NUM < 503 || LUA_VERSION >= 600
		#warning "Luwra has not been tested against your installed version of Lua"
	#end
#else
	#error "Your Lua library does not define LUA_VERSION_NUM"
#end

#define LUWRA_NS_BEGIN namespace luwra {

#define LUWRA_NS_END }

#endif