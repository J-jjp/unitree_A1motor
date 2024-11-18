cmake_minimum_required(VERSION 3.1.0)
project(unitree_actuator_sdk)

# 使用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件路径
include_directories(include)
link_directories(lib)

# 查找 yaml-cpp 库
find_package(yaml-cpp REQUIRED)

# 根据系统架构选择库
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(EXTRA_LIBS libUnitreeMotorSDK_Arm64.so)
else()
    set(EXTRA_LIBS libUnitreeMotorSDK_Linux64.so)
endif()

# 示例程序 example_a1_motor
add_executable(example_a1_motor example/example_a1_motor.cpp)
# 链接 Unitree SDK 和 yaml-cpp
target_link_libraries(example_a1_motor ${EXTRA_LIBS} yaml-cpp)

# 其他示例程序（如需要，取消注释）
# add_executable(example_a1_motor_output example/example_a1_motor_output.cpp)
# target_link_libraries(example_a1_motor_output ${EXTRA_LIBS} yaml-cpp)

# add_executable(example_b1_motor example/example_b1_motor.cpp)
# target_link_libraries(example_b1_motor ${EXTRA_LIBS} yaml-cpp)

# add_executable(example_goM8010_6_motor example/example_goM8010_6_motor.cpp)
# target_link_libraries(example_goM8010_6_motor ${EXTRA_LIBS} yaml-cpp)

# add_executable(changeID example/changeID.cpp)
# target_link_libraries(changeID ${EXTRA_LIBS} yaml-cpp)

# Python绑定部分
set(LIBRARY_OUTPUT_PATH "../lib")
add_subdirectory(thirdparty/pybind11)
pybind11_add_module(unitree_actuator_sdk thirdparty/python_wrapper/wrapper.cpp)
target_link_libraries(unitree_actuator_sdk PRIVATE ${EXTRA_LIBS})
set_target_properties(unitree_actuator_sdk PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
