#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[]){
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("rings");
	
	auto move = node->create_publisher<std_msgs::msg::Twist>("/turtle1/cmd_vel",10);
	
    	rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr setpen = node->create_client<turtlesim::srv::SetPen>("set_pen");
	rclcpp::WallRate loop_rate(500ms);
	
	geometry_msgs::msg::Twist message;
	
	node->declare_parameter("radius", 1.0);
	double square_length = node->get_parameter("radius").get_parameter_value().get<double>();
	
	while (!client->wait_for_service(1s)){
        	if (!rclcpp::ok()){
            		RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
                         "Interrupted while waiting for the service.");
            		return 0;
        	}
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
                    "service not available, waiting again...");
        }
        std::vector<std::vector<int>> colors = {
        	{0, 0, 255},
        	{0, 0, 0},
        	{255, 0, 0},
        	{127, 127, 0},
        	{0, 255, 0}
        };
        
        auto pen = std::make_shared<turtlesim::srv::SetPen::Request>();
        pen->off = 0;
        pen->r = colors[i][0];
        pen->g = colors[i][0];
        pen->b = colors[i][0];
        pen->width = 2;
        
        auto dibujar = setpen->async_send_request(pen);
        if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){
        	RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
                    "Sum: %ld", result.get()->sum);
    	}else{
        	RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
                     "Failed to call service add_two_ints");
    	}
	
	while(rclcpp::ok()){
		message.linear.x = 0.2;
            	message.angular.z = 0.2;
		move->publish(message);
		rclcpp::spin_some(node);
		loop_rate.sleep();
	}
	rclcpp::shutdown();
	return 0;
}

