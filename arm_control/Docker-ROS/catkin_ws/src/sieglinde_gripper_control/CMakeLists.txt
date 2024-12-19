cmake_minimum_required(VERSION 3.0.2)
project(sieglinde_gripper_control)

add_compile_options(-std=c++17)

# Find required packages
find_package(catkin REQUIRED
  COMPONENTS
    roscpp
    std_msgs
    moveit_commander 
    moveit_core
    moveit_fake_controller_manager
    moveit_msgs
    moveit_ros_move_group
    moveit_ros_planning
    moveit_ros_planning_interface
    moveit_ros_robot_interaction
)

# Find the Eigen3 package
find_package(Eigen3 REQUIRED)

################################################################################
# Declare ROS messages, services, and actions
################################################################################

catkin_package(
  CATKIN_DEPENDS
    roscpp
    std_msgs
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}  # Add Eigen3 include directory
)

# Link the necessary libraries to the executables
# add_executable(xbox_control_node src/xbox_control.cpp)
# add_dependencies(xbox_control_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(xbox_control_node ${catkin_LIBRARIES})

add_executable(xbox_moveit_node src/xbox_moveit.cpp)
add_dependencies(xbox_moveit_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(xbox_moveit_node ${catkin_LIBRARIES} Eigen3::Eigen)  # Link Eigen3

# add_executable(axis_node src/axis.cpp)
# add_dependencies(axis_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(axis_node ${catkin_LIBRARIES})
