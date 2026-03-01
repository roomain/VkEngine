#pragma once

#ifdef REFLECTION_EXPORTS
#define REFLECTION_EXPORT __declspec(dllexport)
#else
#define REFLECTION_EXPORT __declspec(dllimport)
#endif
