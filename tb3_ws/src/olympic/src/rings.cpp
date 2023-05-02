#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include <cmath>

using namespace std::chrono_literals;

int main(int argc, char * argv[]){
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("rings");
	
	auto move = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
	
	auto teleport = node->create_client<turtlesim::srv::TeleportAbsolute>("teleport_absolute");
	while (!teleport->wait_for_service(std::chrono::seconds(1))) {
    		if (!rclcpp::ok()) {
      			RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      			return 1;
    		}
    	RCLCPP_INFO(node->get_logger(), "Waiting for the teleport service to 	be available...");
  	}

	
    	auto setpen= node->create_client<turtlesim::srv::SetPen>("set_pen");
    	while (!setpen->wait_for_service(std::chrono::seconds(1))) {
    		if (!rclcpp::ok()) {
      			RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      			return 1;
    		}
    		RCLCPP_INFO(node->get_logger(), "Waiting for the set_pen service to be available...");
  	}
    	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist message;
	
	node->declare_parameter("radius", 1.0);
	double radius = node->get_parameter("radius").get_parameter_value().get<double>();
	
        
        std::vector<std::vector<int>> colors = {
        	{0, 0, 255},
        	{0, 0, 0},
        	{255, 0, 0},
        	{127, 127, 0},
        	{0, 255, 0}
        };
        std::vector<std::vector<double>> pos = {
        {2.25,7.75}, 
        {2.25+2*radius+0.5,7.75},
        {2.25+4*radius+1,7.75},
        {2.25+radius, 3.25},
        {2.25+2*radius,3.25}
        };
        
        for (int l =0;l<5;l++){
        
        	auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
        	request->x = pos[l][0];
        	request->y = pos[l][1];
        	request->theta = 0.0;
        	
        	auto setpen_request = std::make_shared<turtlesim::srv::SetPen::Request>();
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 1;
        	
        	auto future_teleport = teleport->async_send_request(request);
        	auto future_setpen = setpen->async_send_request(setpen_request);

       
	    	int i=0,n=2*M_PI/(0.01 * 0.2);
		while(rclcpp::ok() && (i<n)){
			message.linear.x = 0.2;
		    	message.angular.z = 0.2;
			move->publish(message);
			rclcpp::spin_some(node);
			loop_rate.sleep();
		}
		
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 1;
        	future_setpen = setpen->async_send_request(setpen_request);
	}
	rclcpp::shutdown();
	return 0;
}

