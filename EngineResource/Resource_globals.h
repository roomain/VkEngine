#pragma once

#ifdef RESOURCES_EXPORTS
#define RESOURCES_EXPORT __declspec(dllexport)
#else
#define RESOURCES_EXPORT __declspec(dllimport)
#endif
