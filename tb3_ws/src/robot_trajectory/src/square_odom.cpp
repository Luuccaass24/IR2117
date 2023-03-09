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
double xoo;
double yoo;
double zoo;
double woo;
double sini;
double cosi;
double angleo;
double distance;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
    if (msg->pose.pose.position.x==0 && msg->pose.pose.position.y==0){
        xo = msg->pose.pose.position.x;
        yo = msg->pose.pose.position.y;
        xoo = msg->pose.pose.orientation.x;
        yoo = msg->pose.pose.orientation.y;
        zoo = msg->pose.pose.orientation.z;
        woo = msg->pose.pose.orientation.w;
        sini = 2*(woo*zoo + xoo*yoo);
        cosi = 1-2*(yoo*yoo+zoo*zoo);
        angleo = atan2(sini,cosi);
    }
    xf = msg->pose.pose.position.x;
    yf = msg->pose.pose.position.y;
    distance = std::sqrt(std::pow(xf-xo,2)+std::pow(yf-yo,2));
    double xfo =msg->pose.pose.orientation.x; 
    double yfo = msg->pose.pose.orientation.y;
    double wfo = msg->pose.pose.orientation.w;
    double zfo = msg->pose.pose.orientation.z;
    double sinf = 2*(wfo*zfo + xfo*yfo);
    double cosf = 1-2*(yfo*yfo+zfo*zfo);
    double anglef = atan2(sinf,cosf);
    
    
    std::cout << "Pos X: " << xf << std::endl;
    std::cout << "Pos Y: " << yf << std::endl;
    std::cout << "Orientation: " << angleo << std::endl;
    std::cout << "Distance: "<< std::sqrt(std::pow(xf-xo,2)+std::pow(yf-yo,2)) << std::endl;
    
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
    while (rclcpp::ok() && (i<=linear_iterations+angular_iterations)){
        if (distance<=1.0){
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
        xo=xf;
        yo=yf;
    }
    
  }
  message.linear.x = 0.0;
  message.angular.z = 0.0;
  publisher->publish(message);
  rclcpp::spin_some(node);
  rclcpp::shutdown();
  return 0;
}

