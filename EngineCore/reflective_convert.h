#pragma once

#ifdef BEGIN_ENUM
#undef BEGIN_ENUM
#endif

#ifdef VALUE_ENUM
#undef VALUE_ENUM
#endif

#ifdef END_ENUM
#undef END_ENUM
#endif

#ifdef BEGIN_FLAG
#undef BEGIN_FLAG
#endif

#ifdef VALUE_FLAG
#undef VALUE_FLAG
#endif

#ifdef END_FLAG
#undef END_FLAG
#endif


#define BEGIN_FLAG(Type, FlagType)
#define VALUE_FLAG(Value)
#define END_FLAG

#include "reflect_convert_from_string.h"
#include "VkEnumMacros.h"

#ifdef BEGIN_ENUM
#undef BEGIN_ENUM
#endif

#ifdef VALUE_ENUM
#undef VALUE_ENUM
#endif

#ifdef END_ENUM
#undef END_ENUM
#endif

#include "reflect_convert_to_string.h"
#include "VkEnumMacros.h"

#ifdef BEGIN_ENUM
#undef BEGIN_ENUM
#endif

#ifdef VALUE_ENUM
#undef VALUE_ENUM
#endif

#ifdef END_ENUM
#undef END_ENUM
#endif

#ifdef BEGIN_FLAG
#undef BEGIN_FLAG
#endif

#ifdef VALUE_FLAG
#undef VALUE_FLAG
#endif

#ifdef END_FLAG
#undef END_FLAG
#endif