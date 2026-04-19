#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry

class AmrMonitor(Node):
    def __init__(self):
        super().__init__('amr_monitor')
        # 订阅里程计，获取机器人位置
        self.create_subscription(Odometry, '/odom', self.odom_callback, 10)

    def odom_callback(self, msg):
        pos = msg.pose.pose.position
        self.get_logger().info(f'AMR 当前位置: x={pos.x:.2f}, y={pos.y:.2f}')

def main():
    rclpy.init()
    rclpy.spin(AmrMonitor())
    rclpy.shutdown()

if __name__ == '__main__':
    main()