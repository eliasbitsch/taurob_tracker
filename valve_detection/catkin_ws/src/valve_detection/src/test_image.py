#!/usr/bin/env python3
from ultralytics import YOLO

# Load the model
model = YOLO('src/valve_detection/train2/weights/best.pt')

# Set the desired confidence threshold (e.g., 0.5 means only detections with confidence > 50% will be kept)
conf_threshold = 0.5

# Run inference on an image with the confidence threshold
results = model('/root/catkin_ws/src/download/1.JPEG', save=True, imgsz=800, conf=conf_threshold)

# Display results
results[0].show()
