/***********************************************
* @headerfile ManagedCommandBufferArray.h
* @date 26 / 09 / 2026
* @author Roomain
************************************************/
#include <array>
#include <vulkan/vulkan.hpp>
#include "notCopiable.h"
#include "Log.h"

template<size_t Size>
class ManagedCommandBufferArray
{
	friend class QueueManager;
private:
	VkCommandPool m_commandPool = VK_NULL_HANDLE;		/*!< command pool*/
	VkDevice m_logicalDevice = VK_NULL_HANDLE;			/*!< logical device*/
	std::array<VkCommandBuffer, Size> m_cmdBufArray;	/*!< managed command buffer*/

	explicit ManagedCommandBufferArray(VkDevice a_device, VkCommandPool a_cmdPool, const VkCommandBufferLevel a_level) : m_commandPool{ a_cmdPool }, m_logicalDevice{ a_device }
	{
		auto allocation = initAllocCommandBufferInfo(m_commandPool, a_level, Size);
		VK_CHECK_LOG(vkAllocateCommandBuffers(m_logicalDevice, &allocation, m_cmdBufArray.data()))
	}

public:
	ManagedCommandBufferArray() = delete;
	NOT_COPIABLE(ManagedCommandBufferArray);
	explicit ManagedCommandBufferArray(ManagedCommandBufferArray&& a_other)noexcept : m_commandPool{ a_other.m_commandPool },
		m_logicalDevice{ a_other.m_logicalDevice }, m_cmdBufArray{ a_other.m_cmdBufArray }
	{
	}

	virtual ~ManagedCommandBufferArray()
	{
		vkFreeCommandBuffers(m_logicalDevice, m_commandPool, Size, m_cmdBufArray.data());
	}
	constexpr [[nodiscard]] VkCommandBuffer operator [] (const uint32_t a_index) { return m_cmdBufArray[a_index]; }
	constexpr [[nodiscard]] const VkCommandBuffer operator [] (const uint32_t a_index)const { return m_cmdBufArray[a_index]; }
	constexpr [[nodiscard]] size_t size()const noexcept { return Size; }
	using const_iterator = std::array<VkCommandBuffer, Size>::const_iterator;
	[[nodiscard]] const_iterator cbegin() const { return m_cmdBufArray.cbegin(); }
	[[nodiscard]] const_iterator cend() const { return m_cmdBufArray.cend(); }
};