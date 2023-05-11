#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  g_node = rclcpp::Node::make_shared("action_client");
  auto action_client = rclcpp_action::create_client<Rings>(
    g_node, "rings");

  if (!action_client->wait_for_action_server(20s)) {
    RCLCPP_ERROR(g_node->get_logger(), 
      "Action server not available after waiting");
    return 1;
  }
  

  RCLCPP_INFO(g_node->get_logger(), 
    "Sending goal");
  auto send_goal_options = 
    rclcpp_action::Client<Rings>::SendGoalOptions();
  send_goal_options.feedback_callback = feedback_callback;
  auto goal_handle_future = 
    action_client->async_send_goal(goal_msg, send_goal_options);

  auto return_code = rclcpp::spin_until_future_complete(g_node,
    goal_handle_future);
    
  if (return_code != rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_ERROR(g_node->get_logger(), 
      "send goal call failed :(");
    return 1;
  }

  action_client.reset();
  g_node.reset();
  rclcpp::shutdown();
  return 0;
}

