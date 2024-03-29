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
	
	auto teleport = node->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
	while (!teleport->wait_for_service(std::chrono::seconds(1))) {
    		if (!rclcpp::ok()) {
      			RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      			return 1;
    		}
    	RCLCPP_INFO(node->get_logger(), "Waiting for the teleport service to 	be available...");
  	}

	
    	auto setpen= node->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
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
        	{0, 0, 0},
        	{0, 0, 255},
        	{255, 0, 0},
        	{255, 255, 0},
        	{0, 255, 0}
        };
        double inicio = 5.5;
        std::vector<std::vector<double>> pos = {
        {inicio,inicio + radius/2}, 
        {inicio - 2*radius *1.1,inicio + radius/2},
        {inicio+radius*2*1.1,inicio + radius/2},
        {inicio - radius*1.1, inicio - radius/2},
        {inicio+radius*1.1,inicio - radius/2}
        };
        
        for (int l =0;l<5;l++){
        	auto setpen_request = std::make_shared<turtlesim::srv::SetPen::Request>();
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 1;
        	auto future_setpen = setpen->async_send_request(setpen_request);
        
        	auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
        	request->x = pos[l][0];
        	request->y = pos[l][1];
        	request->theta = 0.0;
        	
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 0;
        	
        	auto future_teleport = teleport->async_send_request(request);
        	future_setpen = setpen->async_send_request(setpen_request);

       
	    	int i=0,n=(2*M_PI)/(0.01 * 0.5);
		while(rclcpp::ok() && (i<n)){
			message.linear.x = radius * 0.5;
		    	message.angular.z = 0.5;
			move->publish(message);
			rclcpp::spin_some(node);
			loop_rate.sleep();
			i++;
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

