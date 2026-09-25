#pragma once
/***********************************************
* @headerfile StagingBuffer.h
* @date 16 / 09 / 2026
* @author Roomain
************************************************/
#include "Buffer.h"

class ENGINECORE_EXPORT StagingBuffer : public Buffer
{
	friend class EngineDevice;
private:
	explicit StagingBuffer(const DeviceContext& a_ctxt);
	void internalWrite(const void* a_data, const size_t& a_size);
	void internalRead(void* a_data, const size_t& a_offset, const size_t& a_size)const;

public:
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

	void copyTo(VkCommandBuffer& a_cmdBuffer, StagingBuffer& a_other);
};