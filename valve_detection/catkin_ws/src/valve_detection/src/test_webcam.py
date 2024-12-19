#!/usr/bin/env python3

import cv2
from ultralytics import YOLO
import time

# Load the model
model = YOLO('src/valve_detection/train2/weights/best.pt')

# Open the webcam (usually the default camera)
cap = cv2.VideoCapture(0)  # Use 0 for the default webcam

# Check if the webcam is opened correctly
if not cap.isOpened():
    print("Error: Could not open webcam")
    exit()

while True:
    # Read a frame from the webcam
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        break

    # Convert the frame to a tensor and move it to CPU if CUDA issues occur
    frame_tensor = frame  # frame is a NumPy array, no need to use .to("cpu") directly on it
    
    # Run inference on the frame (you can specify the confidence threshold here)
    results = model(frame_tensor, conf=0.50)  # Lower the confidence threshold to 50%

    # Draw the results on the frame
    img_with_boxes = results[0].plot()  # This adds bounding boxes to the frame

    # Display the frame with detections
    cv2.imshow('Webcam Detection', img_with_boxes)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Optional: Add a small delay to control frame rate (if needed)
    time.sleep(0.1)

# Release the webcam and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()
