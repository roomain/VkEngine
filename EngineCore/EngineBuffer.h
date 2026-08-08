#pragma once
/***********************************************
* @headerfile EngineBuffer.h
* @date 03 / 06 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "DeviceContext.h"
#include "EngineExceptions.h"
#include "VulkanBufferInitializers.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief class for using buffer with vulkan*/
class ENGINECORE_EXPORT EngineBuffer
{
	friend class EngineDevice;
private:
	DeviceContext m_devCtx;							/*!< device context*/
	VmaAllocation m_allocation = VK_NULL_HANDLE;	/*!< vma allocation*/
	VkBufferCreateInfo m_bufferCreateInfo;			/*!< buffer create information*/
	VkBuffer m_buffer = VK_NULL_HANDLE;				/*!< buffer handle*/
	VkDeviceSize m_activeSize = 0;					/*!< used size must be <= m_bufferCreateInfo.size*/

	void internalWrite(const void* a_data, const size_t& a_size);
	void internalRead(void* a_data, const size_t& a_offset, const size_t& a_size)const;
	void releaseBuffer();
	explicit EngineBuffer(const DeviceContext& a_ctxt);

public:
	EngineBuffer() = delete;
	~EngineBuffer();
	[[nodiscard]] constexpr VkBuffer buffer()const { return m_buffer; }
	[[nodiscard]] VkDeviceSize allocationSize()const { return m_bufferCreateInfo.size; }
	[[nodiscard]] VkDeviceSize bufferSize()const { return m_activeSize; }

	template<typename Type>
	void writeData(const Type* a_buffer, const size_t& a_bufferByteSize)
	{
		internalWrite(a_buffer, a_bufferByteSize * sizeof(Type));
	}

	template<typename Type>
	void readData(Type* a_buffer, const size_t& a_offset, const size_t& a_bufferByteSize)const
	{
		internalRead(a_buffer, a_offset * sizeof(Type), a_bufferByteSize * sizeof(Type));
	}

	template<typename Type>
	void readData(Type* a_buffer, const size_t& a_bufferByteSize)const
	{
		internalRead(a_buffer, 0, a_bufferByteSize * sizeof(Type));
	}
};

using EngineBufferPtr = std::shared_ptr<EngineBuffer>;

#pragma warning(pop)
