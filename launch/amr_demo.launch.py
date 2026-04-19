from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 启动 C++ 避障节点
        Node(
            package='zhen_amr',
            executable='obstacle_avoidance',
            name='avoidance'
        ),
        # 启动 Python 监控节点
        Node(
            package='zhen_amr',
            executable='amr_monitor.py',
            name='monitor'
        )
    ])