#include <inttypes.h>
#include <memory>
#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include <vector>

using namespace std::chrono_literals;
using turtlesim::srv::SetPen;
using turtlesim::srv::TeleportAbsolute;

using Rings = 
  olympic_interfaces::action::Rings;

using GoalHandleRings = 
  rclcpp_action::ServerGoalHandle<Rings>;
  
  rclcpp::Node::SharedPtr node = nullptr;

rclcpp_action::GoalResponse handle_goal(
  const rclcpp_action::GoalUUID & uuid, 
  std::shared_ptr<const Rings::Goal> goal)
{
  RCLCPP_INFO(rclcpp::get_logger("server"), 
    "ircle with radius: ", goal->radius);
  (void)uuid;
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}
rclcpp_action::CancelResponse handle_cancel(
  const std::shared_ptr<GoalHandleRings> goal_handle)
{
  RCLCPP_INFO(rclcpp::get_logger("server"), 
    "Got request to cancel goal");
  (void)goal_handle;
  return rclcpp_action::CancelResponse::ACCEPT;
}
void execute(const std::shared_ptr<GoalHandleRings>);

void handle_accepted(
  const std::shared_ptr<GoalHandleRings> goal_handle)
{
  std::thread{execute, goal_handle}.detach();
}
void execute(
  const std::shared_ptr<GoalHandleRings> goal_handle)
{
  RCLCPP_INFO(rclcpp::get_logger("server"), 
    "Executing goal");
  const auto goal = goal_handle->get_goal();
  auto feedback = std::make_shared<Rings::Feedback>();
  auto result = std::make_shared<Rings::Result>();
  auto & ring_number = feedback->drawing_ring;
  auto & ring_angle = feedback->ring_angle;
  
  
  float radius = goal->radius;
  auto move = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
	
  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(10ms);
  
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
        
  rclcpp::Client<TeleportAbsolute>::SharedPtr teleport =
		node->create_client<TeleportAbsolute>("/turtle1/teleport_absolute");
  rclcpp::Client<SetPen>::SharedPtr setpen =
		node->create_client<SetPen>("/turtle1/set_pen");
        
  for (int l =0;l<5;l++){
	      if (goal_handle->is_canceling()) {
	      	goal_handle->canceled(result);
	      	RCLCPP_INFO(rclcpp::get_logger("server"), 
	      	"Goal Canceled");
	      	return;
	    	}
	    	
	    	ring_number=l+1;
	    	ring_angle=0;
	    	
        	auto setpen_request = std::make_shared<SetPen::Request>();
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 1;
        	auto future_setpen = setpen->async_send_request(setpen_request);
        
        	auto teleport_request = std::make_shared<TeleportAbsolute::Request>();
        	teleport_request->x = pos[l][0];
        	teleport_request->y = pos[l][1];
        	teleport_request->theta = 0.0;
        	
        	setpen_request->r = colors[l][0];
        	setpen_request->g = colors[l][1];
        	setpen_request->b = colors[l][2];
        	setpen_request->width = 2.0;
        	setpen_request->off = 0;
        	
        	auto future_teleport = teleport->async_send_request(teleport_request);
        	future_setpen = setpen->async_send_request(setpen_request);

       
	    	int i=0,n=(2*M_PI)/(0.01 * 0.5);
		while(rclcpp::ok() && (i<n)){
			if (n/4 == i){
				ring_angle=90;
				goal_handle->publish_feedback(feedback);
			}else if (n/2 == i){
				ring_angle=180;
				goal_handle->publish_feedback(feedback);
			}else if (n/3 == i){
				ring_angle=270;
				goal_handle->publish_feedback(feedback);
			}else{
				ring_angle=360;
				goal_handle->publish_feedback(feedback);
			}
			message.linear.x = radius * 0.5;
		    	message.angular.z = 0.5;
			move->publish(message);
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

}
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("action_server");
  auto action_server = 
    rclcpp_action::create_server<Rings>(node,
      "rings",
      handle_goal,
      handle_cancel,
      handle_accepted);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

