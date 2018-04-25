#include "utils.hpp"
#include <cstring>
#include <exception>
#include <iostream>
#include <set>

namespace kali {
/**
 * FUNC: readFile
 */
vector<char> readFile(const string &fileName) {
  ifstream file(fileName, ios::ate | ios::binary);
  if (!file.is_open()) {
    throw runtime_error("Unable to open file!");
  }
  size_t fileSize = (size_t)file.tellg();
  vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}
/**
 * FUNC: checkValidationLayerSupport
 * DESC: Checks if Validation Layers passed by the user in validationLayers
 *       can be found in the set of layers available to the Instance.
 */
bool checkValidationLayerSupport(const vector<const char *> &validationLayers) {
  // retrieve number of layers available
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::cout << "Found " << layerCount << "layers\n";

  // retrieve the layer properties structs.
  vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound)
      return false;
  }
  return true;
}
/**
 *  \brief Checks for required device extinsions support
 *
 *  Provided with a list of device extensions that the application requires
 *  and checks if they were supported by the device.
 *
 *  \param device the physical device whose extensions are queried.
 *  \param deviceExtensions a vector initialized with the names of required
 *         extensions.
 *  \return true if all required extensions are supported, false otherwise.
 */
bool checkDeviceExtensionsSupport(
    VkPhysicalDevice device, const vector<const char *> &deviceExtensions) {

  uint32_t extensionsCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount,
                                       nullptr);
  vector<VkExtensionProperties> availableExtensions(extensionsCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount,
                                       availableExtensions.data());
  set<string> requiredExtensions(deviceExtensions.begin(),
                                 deviceExtensions.end());
  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

VkResult createDebugReportCallbackExt(
    VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
    VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback) {
  auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugReportCallbackEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pCallback);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void destroyDebugReportCallbackExt(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugReportCallbackEXT");
  if (func != nullptr) {
    func(instance, callback, pAllocator);
  }
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface) {

  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueCount > 0 &&
        queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    VkBool32 presentSupport = false;
    // Check Physical Device surface support (i.e. present support) for this
    // QueueFamily
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (queueFamily.queueCount > 0 && presentSupport) {
      indices.presentFamily = i;
    }
    if (indices.isComplete()) {
      break;
    }
  }
  return indices;
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}
} // namespace kali
