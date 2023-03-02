#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <cmath>
#include <iostream>

using namespace std::chrono_literals;
double x;
double y;
double angle;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;
    angle = msg->pose.pose.orientation.w;
    std::cout << "Pos X: " << x << std::endl;
    std::cout << "Pos Y: " << y << std::endl;
    std::cout << "Orientation: " << angle << std::endl;
}
  
int main(int argc, char * argv[]){
  
  
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("subscriber");
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

