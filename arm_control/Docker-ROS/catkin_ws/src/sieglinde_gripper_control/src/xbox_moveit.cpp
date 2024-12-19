#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/PoseStamped.h>

moveit::planning_interface::MoveGroupInterface* move_group_ptr = nullptr;
double speed_scaling = 0.05;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    if (!move_group_ptr) {
        ROS_ERROR("MoveGroup not initialized!");
        return;
    }

    try {
        const double deadzone = 0.1;
        
        // Check if any axis is being moved beyond the deadzone
        if (fabs(joy->axes[0]) > deadzone || 
            fabs(joy->axes[1]) > deadzone || 
            fabs(joy->axes[3]) > deadzone) {
            
            // Get current pose
            geometry_msgs::PoseStamped current_pose = move_group_ptr->getCurrentPose();
            std::vector<geometry_msgs::Pose> waypoints;
            geometry_msgs::Pose target = current_pose.pose;

            // Calculate movements
            target.position.x += joy->axes[1] * speed_scaling;  // Forward/Back
            target.position.y += joy->axes[0] * speed_scaling;  // Left/Right
            target.position.z += joy->axes[3] * speed_scaling;  // Up/Down

            ROS_INFO("Target position: x=%f, y=%f, z=%f", 
                    target.position.x, target.position.y, target.position.z);

            waypoints.push_back(target);
            
            // Plan path
            moveit_msgs::RobotTrajectory trajectory;
            double eef_step = 0.02;
            double jump_threshold = 0.0;
            
            double fraction = move_group_ptr->computeCartesianPath(waypoints, 
                                                                 eef_step, 
                                                                 jump_threshold, 
                                                                 trajectory);
            
            if (fraction > 0.0) {
                move_group_ptr->asyncExecute(trajectory);
                ROS_INFO("Executing movement, planning success: %f", fraction);
            }
        }

        // Speed control with bumpers
        if (joy->buttons[4]) { // LB
            speed_scaling = std::max(0.001, speed_scaling - 0.001);
            ROS_INFO("Speed: %f", speed_scaling);
        }
        if (joy->buttons[5]) { // RB
            speed_scaling = std::min(0.1, speed_scaling + 0.001);
            ROS_INFO("Speed: %f", speed_scaling);
        }
    }
    catch (const std::exception& e) {
        ROS_ERROR("Error in joy callback: %s", e.what());
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "xbox_arm_controller");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(2);
    spinner.start();

    ROS_INFO("Waiting for robot state...");
    ros::Duration(2.0).sleep();

    try {
        // Initialize MoveGroup
        move_group_ptr = new moveit::planning_interface::MoveGroupInterface("arm");
        
        // Set basic parameters
        move_group_ptr->setMaxVelocityScalingFactor(0.5);
        move_group_ptr->setMaxAccelerationScalingFactor(0.5);
        move_group_ptr->setPlanningTime(0.1);  // Set shorter planning time
        
        // Print some debug info
        ROS_INFO("Planning frame: %s", move_group_ptr->getPlanningFrame().c_str());
        ROS_INFO("End effector link: %s", move_group_ptr->getEndEffectorLink().c_str());
        
        // Subscribe to joystick
        ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, joyCallback);
        
        ROS_INFO("Xbox controller ready. Controls:");
        ROS_INFO("Left stick: X/Y movement");
        ROS_INFO("Right stick up/down: Z movement");
        ROS_INFO("LB/RB: Adjust speed (current: %f)", speed_scaling);

        // Check if everything is okay
        if (!move_group_ptr->startStateMonitor()) {
            ROS_ERROR("Could not start state monitor!");
            return 1;
        }

        // Wait for robot state
        if (!move_group_ptr->getCurrentState()) {
            ROS_ERROR("Could not get current state!");
            return 1;
        }

        ros::waitForShutdown();
        delete move_group_ptr;
    }
    catch (const std::exception& e) {
        ROS_ERROR("Exception in main: %s", e.what());
        delete move_group_ptr;
        return 1;
    }

    return 0;
}