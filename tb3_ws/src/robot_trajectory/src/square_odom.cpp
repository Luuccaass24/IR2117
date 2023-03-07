#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <cmath>
#include <iostream>

using namespace std::chrono_literals;
double x0;
double y0;
double xf;
double yf;
double angle0;
double anglef;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
    if (msg->pose.pose.position.x==0 && msg->pose.pose.position.y==0){
        x0 = msg->pose.pose.position.x;
        y0 = msg->pose.pose.position.y;
    }
    xf = msg->pose.pose.position.x;
    yf = msg->pose.pose.position.y;
    angle0 = atan2(y0,x0);
    anglef = atan2(yf,xf);
    
    std::cout << "Pos X: " << xf << std::endl;
    std::cout << "Pos Y: " << yf << std::endl;
    std::cout << "Orientation: " << angle0 << std::endl;
    std::cout << "Distance x: "<< xf-x0 << std::endl;
    std::cout << "Distance y: "<< yf-y0 << std::endl;
    std::cout << "Angular distance: "<<anglef-angle0<<std::endl;
}
  
int main(int argc, char * argv[]){
  
  
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("subscriber");
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);

  rclcpp::spin(node);
  
  auto node1 = rclcpp::Node::make_shared("publisher");
  auto publisher = node1->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
  node1->declare_parameter("square_length", 1.0);
  node1->declare_parameter("linear_speed", 0.1);
  node1->declare_parameter("angular_speed",M_PI_2);
  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(10ms);

  double linear_speed = node->get_parameter("linear_speed").get_parameter_value().get<double>();
  double angular_speed = node->get_parameter("angular_speed").get_parameter_value().get<double>();
  double square_length = node->get_parameter("square_length").get_parameter_value().get<double>();
  for (int j=0;j<4;j++){
      
    int i=0,n=square_length/(0.01 * linear_speed);
    while (rclcpp::ok() && (i<n)){
        message.linear.x = linear_speed;
        message.angular.z = 0.0;
        publisher->publish(message);
        rclcpp::spin_some(node1);
        loop_rate.sleep();
        i++;
    }
    i=0, n=M_PI_2/(0.01 * angular_speed);
    while (rclcpp::ok() && (i<n)){
        message.linear.x = 0.0;
        message.angular.z = angular_speed;
        publisher->publish(message);
        rclcpp::spin_some(node1);
        loop_rate.sleep();
        i++;
    }
  }
  rclcpp::shutdown();
  return 0;
}

