#pragma once
/***********************************************
* @headerfile EngineDevice.h
* @date 13 / 03 / 2026
* @author Roomain
************************************************/
#include "DeviceContext.h"
#include "QueuesManager.h"
#include "EngineParallelWorker.h"

/*@brief Base class of device created by EngineApplication*/
class EngineDevice
{
	friend class EngineApplication;
private:
	uint32_t m_deviceIndex;		/*!< device index in instance*/
	DeviceContext m_deviceCtx;	/*!< device context: device handle + instance handle + memory */
	QueuesManager m_queuesMng;	
	// todo queue management
	
public:
	EngineDevice() = delete;

	const DeviceContext& deviceContext()const;
	template<size_t Size>
	EngineParallelWorker<Size> createParallelWorker(/*todo queue flag*/)
	{
		//
	}

};
