#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <vector>

#include "window.hpp"
#include "instance.hpp"

namespace kali {
  class Application {
  public:
    int run();
  private:
    bool initWindow();
    void mainLoop();
    void cleanup();
    bool createInstance();
  private:
    int width{800};
    int height{600};
    Window window;

    VkApplicationInfo appInfo{};
    Instance instance;
  };
}
#endif // APPLICATION_HPP
