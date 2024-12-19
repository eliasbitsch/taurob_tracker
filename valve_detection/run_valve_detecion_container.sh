#!/bin/bash

# Set the image name
IMAGE_NAME="ros_noetic_yolov8_rviz_nvidia"

# Build the Docker image
echo "Building Docker image: $IMAGE_NAME"
docker build -t $IMAGE_NAME .

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Docker build failed. Exiting."
    exit 1
fi

# Setup environment variables for X11
XSOCK=/tmp/.X11-unix
XAUTH=/root/.Xauthority
SHARED_DIR=/root/catkin_ws  # General folder path
HOST_DIR=$(pwd)/catkin_ws  # Adjust this based on where your local workspace is located

echo -e "\e[32mMounting folder:
    $HOST_DIR    to
    $SHARED_DIR\e[0m"

xhost +
# Run the Docker container
docker run \
    -it --rm \
    --gpus device=0 \
    --shm-size=8g \
    --volume=$XSOCK:$XSOCK:rw \
    --volume=$XAUTH:$XAUTH:rw \
    --volume=$HOST_DIR:$SHARED_DIR:rw \
    --env="XAUTHORITY=${XAUTH}" \
    --env="DISPLAY=${DISPLAY}" \
    --privileged -v /dev:/dev \
    --device /dev/video0 --device /dev/video1 --device /dev/video2 --device /dev/video3 --device /dev/video4 --device /dev/video5 \
    --net=host \
    --env="QT_X11_NO_MITSHM=1" \
    --env NVIDIA_DRIVER_CAPABILITIES=all \
    -e QT_SCALE_FACTOR=1.0 \
    -e GAZEBO_RESOLUTION=1920x1080 \
    --name "ros_yolo" \
    $IMAGE_NAME
