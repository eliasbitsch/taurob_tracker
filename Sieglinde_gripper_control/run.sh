#!/bin/bash

# Build the Docker image
docker build -t ros_noetic_image .

# Run the Docker container
docker run -it --rm \
    --network host \
    --name ros_noetic_container \
    -v $(pwd)/workspace:/workspace \
    --device /dev/ttyUSB0 \  # Adjust this according to your device
    ros_noetic_image
