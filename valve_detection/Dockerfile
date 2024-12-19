# Base Image: ROS Noetic
FROM ros:noetic-ros-base

# Install dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-catkin-tools \
    git \
    ros-noetic-rviz \
    ros-noetic-roslaunch \
    ros-noetic-cv-bridge \
    ros-noetic-realsense2-camera \
    tmux nano libusb-1.0-0 \
    && pip3 install --upgrade pip \
    && pip3 install ultralytics pyrealsense2 roboflow \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


# Install specific OpenCV version
RUN pip3 install --no-cache-dir opencv-python==4.8.0.74

# Set up workspace for ROS packages
WORKDIR /root/catkin_ws
RUN mkdir -p src
WORKDIR /root/catkin_ws/src

# Clone RealSense ROS (if needed)
# Uncomment the next line if not relying on `ros-noetic-realsense2-camera` from apt
#RUN git clone https://github.com/IntelRealSense/realsense-ros.git

# Build workspace
WORKDIR /root/catkin_ws
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin_make"

# Source ROS environment in the container
RUN echo "source /root/catkin_ws/devel/setup.bash" >> ~/.bashrc

# Set entrypoint to bash for ease of use
ENTRYPOINT ["/bin/bash"]
