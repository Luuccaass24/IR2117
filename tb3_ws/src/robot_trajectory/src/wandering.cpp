#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std::chrono_literals;
using  Eigen::VectorXd;
VectorXd vect_izq;
VectorXd vect_drch;
float proximo_izq;
float proximo_drch;
std::vector<float> v;


void topic_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    v = msg->ranges;
    vect_izq.conservativeResize(10);
    vect_drch.conservativeResize(10);
    int n=0;
    std::cout<<"----"<<std::endl;
    for(int i=0;i<10;i++){
        std::cout<<v[i]<<std::endl;
        vect_izq(i)=v[i];
    }
    std::cout<<"----"<<std::endl;
    for(int i=350;i<=359;i++){
        std::cout<<v[i]<<std::endl;
        if (i<360){
            vect_drch(n)=v[i];
            n++;
        }
    }
    proximo_izq = vect_izq.minCoeff();
    proximo_drch = vect_drch.minCoeff();
    std::cout<<"Min_izq: "<<proximo_izq<<std::endl;
    std::cout<<"Min_drch: "<<proximo_drch<<std::endl;
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
    if (proximo_drch <= 1.5 and proximo_izq <= 1.5){
        message.linear.x = 0;
        if (proximo_drch < proximo_izq){
            message.angular.z = 0.1;
        }else{
            message.angular.z = -0.1;
        }
        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }else{
        message.linear.x = 0.3;
        message.angular.z = 0;
        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
  }
  rclcpp::shutdown();
  return 0;
}
