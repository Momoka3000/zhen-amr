cmake_minimum_required(VERSION 3.8)
project(zhen-amr)

# 寻找基础依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# --- C++ 节点配置 ---
add_executable(obstacle_avoidance src/obstacle_avoidance.cpp)
ament_target_dependencies(obstacle_avoidance rclcpp sensor_msgs geometry_msgs)

# --- 核心：安装规则 (决定了 ros2 run 能否找到命令) ---

# 1. 安装 C++ 可执行文件到 lib/zhen-amr 目录下
install(TARGETS
  obstacle_avoidance
  DESTINATION lib/${PROJECT_NAME}
)

# 2. 安装 Python 脚本 (amr_monitor.py)
install(PROGRAMS
  scripts/amr_monitor.py
  DESTINATION lib/${PROJECT_NAME}
)

# 3. 安装资源文件夹到 share/zhen-amr 目录下
install(DIRECTORY urdf world launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()