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
VectorXd vect;
float proximo;
std::vector<float> v;


void topic_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
    v = msg->ranges;
    vect.conservativeResize(20);
    int n=10;
    std::cout<<"----"<<std::endl;
    for(int i=0;i<10;i++){
        std::cout<<v[i]<<std::endl;
        vect(i)=v[i];
    }
    std::cout<<"----"<<std::endl;
    for(int i=350;i<=359;i++){
        std::cout<<v[i]<<std::endl;
        if (i<360){
            vect(n)=v[i];
            n++;
        }
    }
    proximo = vect.minCoeff();
    std::cout<<"Min: "<<proximo<<std::endl;
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
    if (proximo <= 1.8){
        message.linear.x = 0;
        message.angular.z = 0;
        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }else{
        message.linear.x = 0.5;
        message.angular.z = 0;
        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
  }
  rclcpp::shutdown();
  return 0;
}
