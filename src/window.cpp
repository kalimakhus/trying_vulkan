#include "window.hpp"

namespace kali {
std::vector<const char *> Window::getRequiredExtensions() {
  uint32_t glfwExtensionsCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionsCount);
  return extensions;
}
bool Window::init(int width, int height, const char *title) {
  if (!glfwInit())
    return false;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
    return false;
  extents.width = width;
  extents.height = height;

  return true;
}

VkSurfaceKHR Window::getSurface(VkInstance instance) {
  if (surface == VK_NULL_HANDLE) {
    if (!createSurface(instance)) {
      return VK_NULL_HANDLE;
    }
  }
  return surface;
}

bool Window::createSurface(VkInstance instance) {
  VkResult res;
  res = glfwCreateWindowSurface(instance, window, nullptr, &surface);
  if(res != VK_SUCCESS) {
    return false;
  }
  return true;
}

void Window::cleanup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::shouldClose() { return (glfwWindowShouldClose(window) != 0); }
} // namespace kali
