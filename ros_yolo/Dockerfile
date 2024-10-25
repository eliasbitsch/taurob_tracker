# Basisimage: ROS Noetic
FROM ros:noetic-ros-core

# Setze Umgebungsvariablen für ROS
ENV ROS_DISTRO noetic

# Aktualisiere das System und installiere Abhängigkeiten
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-catkin-tools \
    git \
    ros-noetic-rviz \
    ros-noetic-roslaunch \
    && rm -rf /var/lib/apt/lists/*

# Installiere YOLOv8 (Ultralytics)
RUN pip3 install --upgrade pip \
    && pip3 install ultralytics

# Erstelle das Arbeitsverzeichnis für den ROS-Workspace
RUN mkdir -p /catkin_ws/src
WORKDIR /catkin_ws

# Installiere ROS-Abhängigkeiten
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && \
    catkin_make"

# Setze ROS-Umgebungsvariablen in der Container-Session
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

# Setze den Arbeitsordner auf das Workspace-Verzeichnis
WORKDIR /catkin_ws/src

# Setze das Arbeitsverzeichnis auf /catkin_ws und baue den Workspace
WORKDIR /catkin_ws
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin_make"

# Öffne den Port für RViz und stelle X-Server für GUI sicher

RUN apt-get update && apt-get install -y ros-noetic-rviz

RUN pip3 install --upgrade pip \
    && pip3 install pyrealsense2

RUN apt-get update && apt-get install -y tmux nano

RUN apt-get update && apt-get install -y libusb-1.0-0

RUN sudo apt-get install -y ros-noetic-cv-bridge

RUN sudo apt-get update && apt-get install -y ros-noetic-realsense2-camera

# Create a catkin workspace
RUN mkdir -p /catkin_ws/src

# Clone the realsense-ros repository
RUN cd /catkin_ws/src && \
    git clone https://github.com/IntelRealSense/realsense-ros.git

# Set up rosdep
#RUN rosdep init
#RUN rosdep update

# Install dependencies for the realsense-ros package
# RUN cd /catkin_ws && \
#     rosdep install --from-paths src --ignore-src -r -y

# Build the workspace
#  RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && cd /catkin_ws && catkin_make"

# Source the workspace environment in every shell
RUN echo "source /catkin_ws/devel/setup.bash" >> /root/.bashrc


# Kopiere alle notwendigen Dateien ins Image (optional)
# COPY ./my_ros_package /catkin_ws/src/my_ros_package

CMD ["bash"]
