#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "utils.hpp"
#include "window.hpp"

namespace kali {

class SwapChain {
public:
  SwapChain() {}
  ~SwapChain() {}
  bool init(const Window* pWindow, VkPhysicalDevice physicalDevice,
            VkSurfaceKHR surface, VkDevice device);
  VkSwapchainKHR getHandle() { return swapChain; }
  void cleanup(VkDevice device);
private:
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR
  chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availableModes);
  VkExtent2D chooseSwapExtent(const Window* pWindow,
                              const VkSurfaceCapabilitiesKHR& capabilities);
private:
  VkSwapchainKHR swapChain;
};

} // namespace kali

#endif /* SWAPCHAIN_HPP */
