// #include <ros/ros.h>
// #include <moveit/move_group_interface/move_group_interface.h>
// #include <moveit/planning_scene_interface/planning_scene_interface.h>
// #include <moveit_msgs/DisplayRobotState.h>
// #include <moveit_msgs/DisplayTrajectory.h>
// #include <moveit_msgs/AttachedCollisionObject.h>
// #include <moveit_msgs/CollisionObject.h>
// #include <moveit_visual_tools/moveit_visual_tools.h>
// #include <GLFW/glfw3.h>
// #include <iostream>

// // Xbox controller button mappings
// const int XBOX_A = 0;
// const int XBOX_B = 1;
// const int XBOX_X = 2;
// const int XBOX_Y = 3;
// const int XBOX_LB = 4;
// const int XBOX_RB = 5;
// const int XBOX_BACK = 6;
// const int XBOX_START = 7;
// const int XBOX_LEFT_STICK = 8;
// const int XBOX_RIGHT_STICK = 9;

// // Axes mappings
// const int LEFT_STICK_X = 0;
// const int LEFT_STICK_Y = 1;
// const int RIGHT_STICK_X = 2;
// const int RIGHT_STICK_Y = 3;
// const int TRIGGER_LEFT = 4;
// const int TRIGGER_RIGHT = 5;

// static void glfw_error_callback(int error, const char* description) {
//     fprintf(stderr, "GLFW Error %d: %s\n", error, description);
// }

// int main(int argc, char** argv) {
//     // Initialize ROS
//     ros::init(argc, argv, "xbox_controller_interface");
//     ros::NodeHandle nh;
//   ros::spin();  // Keeps the node alive and processes callbacks


//     // Initialize MoveIt
//     static const std::string PLANNING_GROUP = "arm";
//     moveit::planning_interface::MoveGroupInterface move_group_interface(PLANNING_GROUP);
//     moveit_msgs::RobotTrajectory trajectory;
//     std::vector<geometry_msgs::Pose> waypoints;

//     // Initialize GLFW
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit()) {
//         return 1;
//     }

//     // Create a window (required for GLFW input processing)
//     GLFWwindow* window = glfwCreateWindow(400, 300, "Robot Controller", NULL, NULL);
//     if (!window) {
//         glfwTerminate();
//         return 1;
//     }
//     glfwMakeContextCurrent(window);

//     // Movement speed settings
//     float speed = 0.001f;  // Base speed
//     float speed_multiplier = 1.0f;

//     std::cout << "Xbox controller interface started. Controls:" << std::endl;
//     std::cout << "Left stick: X/Y movement" << std::endl;
//     std::cout << "Right stick up/down: Z movement" << std::endl;
//     std::cout << "LB/RB: Adjust speed" << std::endl;
//     std::cout << "Back button: Exit" << std::endl;

//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();

//         // Check if controller is connected
//         if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
//             int count;
//             const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
//             const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

//             // Get current pose
//             geometry_msgs::PoseStamped current_pose = move_group_interface.getCurrentPose();
//             geometry_msgs::Pose target_pose = current_pose.pose;
//             bool should_move = false;

//             // Handle movement with left stick (X/Y) and right stick (Z)
//             if (fabs(axes[LEFT_STICK_X]) > 0.1) {
//                 target_pose.position.x += axes[LEFT_STICK_X] * speed * speed_multiplier;
//                 should_move = true;
//             }
//             if (fabs(axes[LEFT_STICK_Y]) > 0.1) {
//                 target_pose.position.y -= axes[LEFT_STICK_Y] * speed * speed_multiplier;
//                 should_move = true;
//             }
//             if (fabs(axes[RIGHT_STICK_Y]) > 0.1) {
//                 target_pose.position.z -= axes[RIGHT_STICK_Y] * speed * speed_multiplier;
//                 should_move = true;
//             }

//             // Speed control with bumpers
//             if (buttons[XBOX_LB]) {
//                 speed_multiplier = 0.5f;  // Slow mode
//             } else if (buttons[XBOX_RB]) {
//                 speed_multiplier = 2.0f;  // Fast mode
//             } else {
//                 speed_multiplier = 1.0f;  // Normal speed
//             }

//             // Execute movement if needed
//             if (should_move) {
//                 waypoints.clear();
//                 waypoints.push_back(target_pose);
                
//                 moveit_msgs::RobotTrajectory trajectory;
//                 const double jump_threshold = 0.0;
//                 const double eef_step = 0.01;
                
//                 double fraction = move_group_interface.computeCartesianPath(
//                     waypoints, eef_step, jump_threshold, trajectory);
                
//                 move_group_interface.asyncExecute(trajectory);
//             }

//             // Exit program with Back button
//             if (buttons[XBOX_BACK]) {
//                 glfwSetWindowShouldClose(window, GLFW_TRUE);
//             }
//         }

//         // Small delay to prevent excessive CPU usage
//         ros::Duration(0.01).sleep();
//     }

//     // Cleanup
//     ros::shutdown();
//     glfwDestroyWindow(window);
//     glfwTerminate();

//     return 0;
// }