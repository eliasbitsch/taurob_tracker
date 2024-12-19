project(valve_detection)

# Find necessary ROS packages
find_package(catkin REQUIRED COMPONENTS
    roscpp
    sensor_msgs
    image_transport
    cv_bridge
)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Declare the package
catkin_package()

# Include the necessary directories
include_directories(${catkin_INCLUDE_DIRS})