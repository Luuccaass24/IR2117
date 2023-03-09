#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <cmath>
#include <iostream>

using namespace std::chrono_literals;
double xo;
double yo;
double xf;
double yf;
double angleo;
double anglef;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
    if (msg->pose.pose.position.x==0 && msg->pose.pose.position.y==0){
        xo = msg->pose.pose.position.x;
        yo = msg->pose.pose.position.y;
    }
    xf = msg->pose.pose.position.x;
    yf = msg->pose.pose.position.y;
    angleo = atan2(yo,xo);
    anglef = atan2(yf,xf);
    
    std::cout << "Pos X: " << xf << std::endl;
    std::cout << "Pos Y: " << yf << std::endl;
    std::cout << "Orientation: " << angleo << std::endl;
    std::cout << "Distance x: "<< xf-xo << std::endl;
    std::cout << "Distance y: "<< yf-yo << std::endl;
    std::cout << "Angular distance: "<<anglef-angleo<<std::endl;
}
  
int main(int argc, char * argv[]){
  
  //subscriber
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("subscriber");
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);

  //publisher
  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
  node->declare_parameter("square_length", 1.0);
  node->declare_parameter("linear_speed", 0.1);
  node->declare_parameter("angular_speed",M_PI_2);
  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(10ms);

  double linear_speed = node->get_parameter("linear_speed").get_parameter_value().get<double>();
  double angular_speed = node->get_parameter("angular_speed").get_parameter_value().get<double>();
  double square_length = node->get_parameter("square_length").get_parameter_value().get<double>();
  
  for (int j=0;j<4;j++){
    int linear_iterations = square_length/(0.01 * linear_speed);
    int angular_iterations = M_PI_2/(0.01 * angular_speed);
      
    int i=0;
    while (rclcpp::ok() && (i<linear_iterations+angular_iterations)){
        if (i<linear_iterations){
            message.linear.x = linear_speed;
            message.angular.z = 0.0;
            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
            i++;
        }else{
            message.linear.x = 0.0;
            message.angular.z = angular_speed;
            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
            i++;
        }
    }
    
  }
  message.linear.x = 0.0;
  message.angular.z = 0.0;
  rclcpp::shutdown();
  return 0;
}

