#!/bin/bash

# Build the Docker image
docker build -t dynamixel_new .

XSOCK=/tmp/.X11-unix
XAUTH=/tmp/.X11-unix
SHARED_DIR=/usr/src/dynamixel/catkin_ws/src
HOST_DIR=$(pwd)/catkin_ws/src


xhost +
echo -e "\e[32mMounting fodler:
    $HOST_DIR    to
    $SHARED_DIR\e[0m"


docker run \
    -it --rm \
    --volume=$XSOCK:$XSOCK:rw \
    --volume=$XAUTH:$XAUTH:rw \
    --volume=$HOST_DIR:$SHARED_DIR:rw \
    --env="XAUTHORITY=${XAUTH}" \
    --env="DISPLAY=${DISPLAY}" \
    --privileged -v /dev:/dev \
    --net=host \
    --name "dynamixel_axis_5_and_gripper" \
    dynamixel_new bash
xhost -
