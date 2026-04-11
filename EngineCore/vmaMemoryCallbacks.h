#pragma once
/***********************************************
* @headerfile vmaMemoryCallbacks.h
* @date 20 / 03 / 2026
* @author Roomain
************************************************/
#include "vulkan/vulkan.hpp"


/*@brief interface for debuging memory allocation*/
class IDebugVmaMemoryAllocation
{
public:
	virtual uint32_t allocationCount()const = 0;
	virtual size_t allocationSize(const uint32_t a_allocIndex)const = 0;
	virtual size_t allocationMemorysize()const = 0;
	virtual void allocateMemory(const size_t& a_size, const void* a_mem) = 0;
	virtual void freeMemory(const void* a_mem) = 0;
	virtual void reallocMemory(const size_t& a_size, const void* a_oldMem, const void* a_newMem) = 0;
};

void* vmaCpuAllocation(void* a_pUserData, size_t a_size, size_t a_alignment, VkSystemAllocationScope a_allocationScope);

void* vmaCpuReallocation(void* a_pUserData, void* a_pOriginal, size_t a_size, size_t a_alignment, VkSystemAllocationScope a_allocationScope);

void vmaCpuFree(void* a_pUserData, void* a_pMemory);