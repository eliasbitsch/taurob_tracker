#!/bin/bash

set -e

# Source ROS setup scripts
if [[ -f /opt/ros/noetic/setup.bash ]]; then
    source /opt/ros/noetic/setup.bash
else
    echo "Warning: ROS setup script not found at /opt/ros/noetic/setup.bash."
fi

# Source Catkin workspace setup if it exists
CATKIN_SETUP="/root/catkin_ws/devel/setup.bash"
if [[ -f "$CATKIN_SETUP" ]]; then
    source "$CATKIN_SETUP"
else
    echo "Warning: Catkin workspace setup script not found at $CATKIN_SETUP."
fi

# Set default command to bash if none is provided
if [[ -z "$1" ]]; then
    set -- "/bin/bash" -l
fi

# Execute the command as root
exec "$@"
