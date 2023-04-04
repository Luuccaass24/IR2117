#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "example_interfaces/msg/bool.hpp"

using namespace std::chrono_literals;

bool front_obstacle, left_obstacle, right_obstacle;
std::string state;


void callback_front(const example_interfaces::msg::Bool::SharedPtr msg){
	example_interfaces::msg::Bool out_msg;
    	front_obstacle = out_msg.data;
}

void callback_left(const example_interfaces::msg::Bool::SharedPtr msg){
	example_interfaces::msg::Bool out_msg;
    	left_obstacle = out_msg.data;
}

void callback_right(const example_interfaces::msg::Bool::SharedPtr msg){
	example_interfaces::msg::Bool out_msg;
    	right_obstacle = out_msg.data;
}

int main(int argc, char * argv[]){
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("avoidance");
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
	auto subs_front = node->create_subscription<example_interfaces::msg::Bool>("/front/obstacle", 10 ,callback_front);
	auto subs_left = node->create_subscription<example_interfaces::msg::Bool>("/left/obstacle", 10 ,callback_left);
	auto subs_right = node->create_subscription<example_interfaces::msg::Bool>("/right/obstacle", 10 ,callback_right);
	geometry_msgs::msg::Twist message;
	rclcpp::WallRate loop_rate(50ms);
	
	
	
	while (rclcpp::ok()){
	state = " ";
	if (front_obstacle == false && left_obstacle == false && right_obstacle == false){
		state = "front";
	}else if (front_obstacle == true && left_obstacle == false && right_obstacle==false){
		state = "random";
	}else if (front_obstacle == true && left_obstacle == true && right_obstacle == false){
		state = "right";
	}else{
		state = "left";
	}
		switch(state){
			case "front":
				message.linear.x = 0.2;
				message.angular.z = 0;
				publisher->publish(message);
				rclcpp::spin_some(node);
				loop_rate.sleep();
				break;
			case "random":
	
		}
	}
	rclcpp::shutdown();
	return 0;
}
