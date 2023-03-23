#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std::chrono_literals;
std::vector<float> v;

void topic_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    v = msg->ranges;
    std::cout<<"----"<<std::endl;
    for(int i=0;i<10;i++){
        std::cout<<v[i]<<std::endl;
    }
    std::cout<<"----"<<std::endl;
    for(int i=350;i<=359;i++){
        std::cout<<v[i]<<std::endl;
    }
}
  
int main(int argc, char * argv[]){
  
  //subscriber
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("subscriber");
  auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>("scan", 10, topic_callback);

  //publisher
  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(10ms);
  while (rclcpp::ok()){
    message.linear.x = 0;
    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}
