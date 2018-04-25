#include "instance.hpp"
#include "utils.hpp"

#include <iostream>
#include <stdexcept>

namespace kali {

bool Instance::init(const VkApplicationInfo *appInfo, Window *pWindow,
                    bool enableValidation) {

  vector<const char *> validationLayers;
  vector<const char *> extensions = Window::getRequiredExtensions();

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = appInfo;
  // If validation is enabled (only in debug build)
  // add the Debug Report Extension to the required extensions list.
  // check for validation layer support.
  if (enableValidation) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");

    if (!checkValidationLayerSupport(validationLayers))
      throw std::runtime_error("Required Layers not available!");
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    return false;
  // Create a Debug Report Callback if Validation is enabled.
  if (enableValidation) {
    validationEnabled = true;
    setupDebugCallback();
  }
  // Retrieve a Surface of the active window.
  surface = pWindow->getSurface(instance);
  if (surface == VK_NULL_HANDLE) {
    cleanup();
    return false;
  }
  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  // Pick our Physical Device.
  pickPhysicalDevice(deviceExtensions);
  // Create a kali::Device that wraps a VkDevice.
  if (!device.init(physicalDevice, queueIndices, validationLayers,
                   deviceExtensions)) {
    cleanup();
    return false;
  }
  // Create a kali::SwapChain that wraps a VkSwapchainKHR
  if (!swapChain.init(pWindow, physicalDevice, surface, device.getHandle())) {
    cleanup();
    return false;
  }
  return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(
    VkDebugReportFlagsEXT /*flags*/, VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/, size_t /*location*/, int32_t /*code*/,
    const char * /*layerPrefix*/, const char *msg, void * /*userData*/) {

  std::cerr << "Validation Layer: " << msg << std::endl;
  return VK_FALSE;
}

void Instance::setupDebugCallback() {

  VkDebugReportCallbackCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags =
      VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  if (createDebugReportCallbackExt(instance, &createInfo, nullptr, &callback) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create debug callback!");
  }
}

void Instance::cleanup() {

  swapChain.cleanup(device.getHandle());
  device.cleanup();
  if (validationEnabled) {
    destroyDebugReportCallbackExt(instance, callback, nullptr);
  }
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyInstance(instance, nullptr);
}

bool Instance::isDeviceSuitable(
    VkPhysicalDevice device, const std::vector<const char *> deviceExtensions) {
  QueueFamilyIndices indices = findQueueFamilies(device, surface);
  if (indices.isComplete()) {
    queueIndices = indices;
  }

  bool extensionsSupported =
      checkDeviceExtensionsSupport(device, deviceExtensions);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }
  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

void Instance::pickPhysicalDevice(
    const std::vector<const char *> deviceExtensions) {

  uint32_t deviceCount;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("Failed to find GPUs with Vulkan support!");
  }
  std::cout << "Found " << deviceCount << " Physical Devices." << std::endl;

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (const auto& device : devices) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    std::cout << deviceProperties.deviceName << "\n";
  }
  for (const auto &device : devices) {
    if (isDeviceSuitable(device, deviceExtensions)) {
      physicalDevice = device;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }
}

} // namespace kali
