form launch import LaunchDescription
from launch_ros.actions import Node
import launch.actions

def generate_launch_description():
    return LaunchDescription([
        launch.actions.DeclareLaunchArguments('bagfile'),
        launch.actions.ExecuteProcess(
            cmd=['ros2', 'bag', 'play', launch.substitutions.LaunchConfiguration('bagfile')], output='screen'
        ),
        Node(
            package='obstacles',
            executable='detector'
            )
    ])
    
