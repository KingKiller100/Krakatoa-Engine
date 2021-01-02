#pragma once

#include <HelperMacros.hpp>

#ifdef KRAKOA_OS_WINDOWS
#	if KRAKOA_DYNAMIC_LINK
#		ifdef KRAKOA_BUILD_DLL
#			define KRAKOA_API __declspec(dllexport)
#			define EXPIMP_TEMPLATE 
#		else
#			define KRAKOA_API __declspec(dllimport)
#			define EXPIMP_TEMPLATE extern
#		endif
#	else
#		define KRAKOA_API 
#		define EXPIMP_TEMPLATE 
#endif
#else
#	error SORRY BLOKE, KRAKOA ONLY SUPPORTS WINDOWS!
#endif

#define KRK_TRUE KLIB_TRUE
#define KRK_FALSE KLIB_FALSE

// Standard Placeholders
#define PL_HOLDER(x) std::placeholders::_##x

// Binding
#define KRK_BIND1(func) std::bind((&func), this, PL_HOLDER(1))
#define KRK_BIND2(func) std::bind((&func), this, PL_HOLDER(1), PL_HOLDER(2))

#ifndef KRAKOA_PROFILE
#	ifndef KRK_ENABLE_STATISTICS
#		define KRK_ENABLE_STATISTICS KRK_TRUE
#	endif
#endif

#ifndef KRK_ENABLE_ASSERT
# define KRK_ENABLE_ASSERT KRK_TRUE
#endif

