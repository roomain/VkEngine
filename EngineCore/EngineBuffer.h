#pragma once
/***********************************************
* @headerfile EngineBuffer.h
* @date 03 / 06 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <atomic>
#include "DeviceContext.h"
#include "notCopiable.h"
#include "enginecore_globals.h"

class ENGINECORE_EXPORT EngineBuffer
{
	friend class EngineDevice;

protected:
	struct VMABuffer
	{
		VmaAllocation allocation = VK_NULL_HANDLE;	/*!< vma allocation*/
		VkBuffer buffer = VK_NULL_HANDLE;			/*!< staging buffer handle*/
	};

	void releaseBuffer(VMABuffer& a_buffer);
	void createBuffer(const VkBufferCreateInfo bufferInfo, VMABuffer& a_buffer);
	void reallocBuffer(const VkDeviceSize& a_size);

	DeviceContext m_devCtx;					/*!< device context*/
	VkDeviceSize m_bufferSize = 0;			/*!< buffer size*/
	VMABuffer m_buffer;						/*!< buffer*/

	explicit EngineBuffer(const DeviceContext& a_ctxt);

public:
	NOT_COPIABLE(EngineBuffer)
	EngineBuffer() = delete;
	virtual ~EngineBuffer();
	[[nodiscard]] constexpr VkBuffer buffer()const { return m_buffer.buffer; }
	[[nodiscard]] VkDeviceSize allocationSize()const { return m_bufferSize; }
};

