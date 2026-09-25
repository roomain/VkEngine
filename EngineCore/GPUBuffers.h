#pragma once
/***********************************************
* @headerfile GPUBuffers.h
* @date 16 / 09 / 2026
* @author Roomain
************************************************/
#include "Buffer.h"

class ENGINECORE_EXPORT IndexBuffer : public Buffer
{
	friend class EngineDevice;
private:
	explicit IndexBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
	void bind(VkCommandBuffer& a_cmdBuffer);
};

class ENGINECORE_EXPORT VertexBuffer : public Buffer
{
	friend class EngineDevice;
private:
	explicit VertexBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
	void bind(VkCommandBuffer& a_cmdBuffer, uint32_t a_bind);
};


class ENGINECORE_EXPORT StorageBuffer : public Buffer
{
	friend class EngineDevice;
private:
	explicit StorageBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
};