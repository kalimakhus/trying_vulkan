add_executable(vktriangle main.cpp
  application.cpp
  window.cpp
  instance.cpp
  utils.cpp
  device.cpp
  swapchain.cpp)
target_link_libraries(vktriangle glfw vulkan)
