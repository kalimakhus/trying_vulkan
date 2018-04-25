#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace kali {
  class Window {
  public:
    Window() {}
    ~Window() {}
    bool init(int width, int height, const char* title);
    VkSurfaceKHR getSurface(VkInstance instance);
    void cleanup();
    bool shouldClose();
    VkExtent2D getExtents() const { return extents; }
  public:
    static std::vector<const char*> getRequiredExtensions();
  private:
    GLFWwindow* window{nullptr};
    VkSurfaceKHR surface{VK_NULL_HANDLE};
    bool createSurface(VkInstance instance);
    VkExtent2D extents{};
  };
}
#endif // WINDOW_HPP
