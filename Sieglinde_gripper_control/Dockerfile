# Use the official ROS Noetic base image
FROM ros:noetic-ros-base

# Set environment variables
ENV ROS_PACKAGE_PATH=/opt/ros/noetic/share
ENV ROS_DISTRO=noetic

# Install necessary packages
RUN apt-get update && \
    apt-get install -y \
    ros-noetic-dynamixel-sdk \
    ros-noetic-dynamixel-controllers \
    ros-noetic-dynamixel-msgs \
    python3-rosdep \
    python3-catkin-tools \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Initialize rosdep
RUN rosdep init && \
    rosdep update

# Create a workspace
RUN mkdir -p /workspace/src

# Set the working directory
WORKDIR /workspace

# Source the ROS environment
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

# Entry point
CMD ["bash"]
