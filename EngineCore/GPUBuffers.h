#pragma once
/***********************************************
* @headerfile GPUBuffers.h
* @date 16 / 09 / 2026
* @author Roomain
************************************************/
#include "EngineBuffer.h"

class ENGINECORE_EXPORT EngineIndexBuffer : public EngineBuffer
{
	friend class EngineDevice;
private:
	explicit EngineIndexBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
	void bind(VkCommandBuffer& a_cmdBuffer);
};

class ENGINECORE_EXPORT EngineVertexBuffer : public EngineBuffer
{
	friend class EngineDevice;
private:
	explicit EngineVertexBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
	void bind(VkCommandBuffer& a_cmdBuffer, uint32_t a_bind);
};

class ENGINECORE_EXPORT EngineUniformBuffer : public EngineBuffer
{
	friend class EngineDevice;
private:
	explicit EngineUniformBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
};

class ENGINECORE_EXPORT EngineStorageBuffer : public EngineBuffer
{
	friend class EngineDevice;
private:
	explicit EngineStorageBuffer(const DeviceContext& a_ctxt);
public:
	void allocate(const size_t& a_size);
};