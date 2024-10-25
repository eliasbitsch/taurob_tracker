#!/bin/sh

XSOCK=/tmp/.X11-unix
XAUTH=/tmp/.X11-unix
SHARED_DIR=/home/fhtw_user/catkin_ws/src
HOST_DIR=$(pwd)/catkin_ws/src
IMAGE_NAME="taurob_image"

# Build the Docker image
echo -e "\e[32mBuilding Docker image: $IMAGE_NAME\e[0m"
docker build -t $IMAGE_NAME .

# Allow connections to the X server
xhost +

echo -e "\e[32mMounting folder:
    $HOST_DIR    to
    $SHARED_DIR\e[0m"

# Run the Docker container
docker run \
    -it --rm \
    --gpus all\
    --volume=$XSOCK:$XSOCK:rw \
    --volume=$XAUTH:$XAUTH:rw \
    --volume=$HOST_DIR:$SHARED_DIR:rw \
    --env="XAUTHORITY=${XAUTH}" \
    --env="DISPLAY=${DISPLAY}" \
    --privileged -v /dev/bus/usb:/dev/bus/usb \
    --device /dev/dri:/dev/dri \
    --net=host \
    --env="QT_X11_NO_MITSHM=1" \
    --env NVIDIA_DRIVER_CAPABILITIES=all \
    -e QT_SCALE_FACTOR=1.0 \
    -e GAZEBO_RESOLUTION=1920x1080 \
    --name "fhtw_ros" \
    $IMAGE_NAME bash

# Disable access to the X server
xhost -
