#include "application.hpp"
#include <iostream>

using namespace std;

namespace kali {
int Application::run() {
  if (!initWindow()) {
    return -1;
  }
  if (!createInstance()) {
    return -1;
  }
  mainLoop();
  cleanup();

  return 0;
}

bool Application::initWindow() {
  if (!window.init(width, height, "Vulkan")) {
    return false;
  }
  return true;
}

bool Application::createInstance() {
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
#ifdef NDEBUG
  bool enableValidation = false;
#else
  bool enableValidation = true;
#endif

  if (!instance.init(&appInfo, &window, enableValidation))
    return false;

  return true;
}

void Application::mainLoop() {
  while (!window.shouldClose()) {
    glfwPollEvents();
  }
}

void Application::cleanup() {
  window.cleanup();
  instance.cleanup();
}

} // namespace kali
