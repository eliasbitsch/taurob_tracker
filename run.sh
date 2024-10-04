#!/bin/sh

XSOCK=/tmp/.X11-unix
XAUTH=/tmp/.X11-unix
SHARED_DIR=/home/fhtw_user/catkin_ws/src
HOST_DIR=$(pwd)/catkin_ws/src
IMAGE_NAME="taurob_image"

# Allow connections to the X server
xhost +

echo -e "\e[32mMounting folder:
    $HOST_DIR    to
    $SHARED_DIR\e[0m"

# Run the Docker container
docker run \
    -it --rm \
    --volume=$XSOCK:$XSOCK:rw \
    --volume=$XAUTH:$XAUTH:rw \
    --volume=$HOST_DIR:$SHARED_DIR:rw \
    --env="XAUTHORITY=${XAUTH}" \
    --env="DISPLAY=${DISPLAY}" \
    --privileged -v /dev/bus/usb:/dev/bus/usb \
    --net=host \
    --name "fhtw_ros" \
    $IMAGE_NAME bash

# Disable access to the X server
xhost -

