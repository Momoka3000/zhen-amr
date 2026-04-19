#ifndef ZHEN_AMR__OBSTACLE_AVOIDANCE_HPP_
#define ZHEN_AMR__OBSTACLE_AVOIDANCE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"

namespace zhen_amr {

class ObstacleAvoidance : public rclcpp::Node {
public:
  explicit ObstacleAvoidance(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
  // 回调函数声明
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

  // 成员变量
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
  
  // 避障参数
  float safe_distance_ = 0.8;
  float linear_vel_ = 0.2;
};

} // namespace zhen_amr

#endif // ZHEN_AMR__OBSTACLE_AVOIDANCE_HPP_