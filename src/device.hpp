#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "utils.hpp"

namespace kali {

class Device {
public:
  Device();
  ~Device();
  bool init(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices,
            const std::vector<const char *> validationLayers,
            const std::vector<const char *> deviceExtensions);
  VkDevice getHandle() { return device; }
  VkQueue getGraphicsQueue() { return graphicsQueue; }
  VkQueue getPresentQueue() { return presentQueue; }
  void cleanup();

private:
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
};

} // namespace kali

#endif /* DEVICE_HPP */
