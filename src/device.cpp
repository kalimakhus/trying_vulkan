#include "device.hpp"
#include <set>

namespace kali {

Device::Device() {}

Device::~Device() {}

bool Device::init(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices,
                  const std::vector<const char *> validationLayers,
                  const std::vector<const char *> deviceExtensions) {

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<int> uniqueQueueFamilies = {indices.graphicsFamily,
                                       indices.presentFamily};
  float queuePriority = 1.0f;
  // we fill in as many VkDeviceQueueCreateInfo structs as the
  // uniqueQueueFamilies we have.
  for (int queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(queueCreateInfo);
  }
  VkPhysicalDeviceFeatures deviceFeatures = {};
  // Fill in the VkDeviceCreateInfo
  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount =
    static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  uint32_t validationLayersCount =
      static_cast<uint32_t>(validationLayers.size());
  if (validationLayersCount != 0) {
    createInfo.enabledLayerCount = validationLayersCount;
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  VkResult res;
  res = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
  if (res != VK_SUCCESS) {
    return false;
  }

  // Retrieve handles for device queues.
  vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
  vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);

  return true;
}

void Device::cleanup() { vkDestroyDevice(device, nullptr); }

} // namespace kali
