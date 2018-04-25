#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <vulkan/vulkan.h>

#include "device.hpp"
#include "window.hpp"
#include "swapchain.hpp"

namespace kali {

class Instance {
public:
  Instance() {}
  ~Instance() {}
  bool init(const VkApplicationInfo *appInfo, Window *pWindow,
            bool enableValidation);
  VkInstance getHandle() { return instance; }
  void cleanup();

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
                uint64_t obj, size_t location, int32_t code,
                const char *layerPrefix, const char *msg, void *userData);

private:
  void setupDebugCallback();
  void pickPhysicalDevice(const std::vector<const char*> deviceExtensions);
  bool isDeviceSuitable(VkPhysicalDevice device,
                        const std::vector<const char*> deviceExtensions);

private:
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  Device device;
  SwapChain swapChain;
  VkSurfaceKHR surface;

  bool validationEnabled{false};
  VkDebugReportCallbackEXT callback;
  QueueFamilyIndices queueIndices;
};

} // namespace kali

#endif // INSTANCE_HPP
