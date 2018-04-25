cmake_minimum_required(VERSION 3.5)
project(vktriangle)
link_directories(/home/tamer/vulkan/VulkanSDK/1.1.73.0/x86_64/lib)
include_directories(/home/tamer/vulkan/VulkanSDK/1.1.73.0/x86_64/include)

add_subdirectory(src)
