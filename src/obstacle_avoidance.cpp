#include <memory>
#include <vector>
#include <algorithm>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"

class ObstacleAvoidance : public rclcpp::Node {
public:
  ObstacleAvoidance() : Node("obstacle_avoidance") {
    // 使用 SensorDataQoS 以兼容 Gazebo 的雷达插件
    auto sensor_qos = rclcpp::QoS(10).best_effort().durability_volatile();

    scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/scan", 
      sensor_qos, 
      std::bind(&ObstacleAvoidance::scan_callback, this, std::placeholders::_1));

    cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    
    RCLCPP_INFO(this->get_logger(), "--- 避障节点优化版启动 ---");
    RCLCPP_INFO(this->get_logger(), "策略：探测正前方区域，转向时配合微小位移以跳出死循环");
  }

private:
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    auto drive_cmd = geometry_msgs::msg::Twist();
    
    // 1. 定义检测窗口：只检查正前方 30 度范围内的点（假设 180 个点对应 180 度）
    // 这能有效防止“余光”扫到侧面的障碍物导致一直打转
    int total_points = msg->ranges.size();
    int center_index = total_points / 2;
    int window = 15;
    
    float min_distance = 10.0;

    for (int i = center_index - window; i <= center_index + window; ++i) {
        if (i >= 0 && i < total_points) {
            float d = msg->ranges[i];
            // 忽略 0.15m 以内的点（通常是扫到了自身支架）
            if (d > 0.15 && d < min_distance) {
                min_distance = d;
            }
        }
    }

    // 2. 避障控制逻辑
    if (min_distance < 0.8) {
      // 发现障碍物：执行规避动作
      RCLCPP_WARN(this->get_logger(), "障碍物过近: [%.2f m]! 正在绕行...", min_distance);

      // 小车边转边“蹭”，绕过箱子的边缘
      drive_cmd.linear.x = 0.05; 
      drive_cmd.angular.z = 0.8;  // 加大转向速度，动作更果断
    } else {
      // 路径安全：全速前进
      drive_cmd.linear.x = 0.2;
      drive_cmd.angular.z = 0.0;
    }
    
    cmd_pub_->publish(drive_cmd);
  }

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ObstacleAvoidance>());
  rclcpp::shutdown();
  return 0;
}