#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <vulkan/vulkan.h>
namespace kali {

vector<char> readeFile(const string &fileName);
bool checkValidationLayerSupport(const vector<const char *> &validationLayers);
bool checkDeviceExtensionsSupport(VkPhysicalDevice device,
                                  const vector<const char *> &deviceExtensions);
VkResult createDebugReportCallbackExt(
    VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
    VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
void destroyDebugReportCallbackExt(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks *pAllocator);

struct QueueFamilyIndices {
  int graphicsFamily = -1;
  int presentFamily = -1;

  bool isComplete() { return graphicsFamily >= 0 && presentFamily >= 0; }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface);

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  vector<VkSurfaceFormatKHR> formats;
  vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface);
} // namespace kali
#endif // UTILS_HPP
