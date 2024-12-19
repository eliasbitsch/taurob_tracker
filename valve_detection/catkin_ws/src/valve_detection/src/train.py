#!/usr/bin/env python3
import os

# Define the YOLOv8 training command as a string
command = (
    "yolo task=detect mode=train "                    # Task is 'detect', mode is 'train'
    "model=yolov8s.pt "                               # Use the YOLOv8 small model (yolov8s.pt)
    "data=/root/catkin_ws/src/valve_detection/dataset/valve_detection/data.yaml "  # Path to the dataset YAML file
    "epochs=50 "                                     # Number of training epochs (50)
    "imgsz=640 "                                     # Image size to resize images for training (640x640)
    "batch=16 "                                      # Batch size for training
    "workers=4 "                                     # Number of data loading workers (for parallel processing)
    "lr0=0.01 "                                      # Initial learning rate
    "lrf=0.1 "                                       # Learning rate final scaling factor
    "optimizer=adam"                                 # Use Adam optimizer for training
)

# Print the command to verify it before execution
print("Running YOLOv8 training with the following command:")
print(command)

# Run the YOLOv8 training command using os.system()
# This will execute the command in the terminal from within the Python script
os.system(command)
