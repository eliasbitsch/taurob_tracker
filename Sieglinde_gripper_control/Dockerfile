FROM ros:noetic

# LABEL about the custom image
LABEL maintainer="ma0180@technikum-wien.at"
LABEL version="1.0"
LABEL description="Dynamixel ROS_Control"

# Update and install necessary packages
RUN apt update -y
RUN apt install -y git ros-noetic-dynamixel-sdk \
    ros-noetic-dynamixel-workbench* ros-noetic-ros-controllers \
    ros-noetic-ros-control* ros-noetic-dynamixel* \
    ros-noetic-joy 

# Create workspace directory
RUN mkdir -p /usr/src/dynamixel/catkin_ws/src

# Set the working directory for the Docker image build process
WORKDIR /usr/src/dynamixel/catkin_ws

# Copy the contents of the local catkin_ws directory to the container's catkin_ws/src
COPY ./catkin_ws/src /usr/src/dynamixel/catkin_ws/src

# Build the workspace
RUN /bin/bash -c '. /opt/ros/noetic/setup.bash; cd /usr/src/dynamixel/catkin_ws; catkin_make'

# Set default entrypoint to source the workspace and start a bash shell
CMD ["/bin/bash", "-c", ". /opt/ros/noetic/setup.bash; . /usr/src/dynamixel/catkin_ws/devel/setup.bash; exec bash"]
