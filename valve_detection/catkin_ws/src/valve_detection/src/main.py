#! /usr/bin/env /bin/python3
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
from ultralytics import YOLO
import time

def main():
    # Initialize the ROS node
    rospy.init_node('valve_detection_publisher', anonymous=True)

    # Create a CvBridge object to convert between OpenCV images and ROS messages
    bridge = CvBridge()

    # Create publishers for raw and detected images
    pub_raw = rospy.Publisher('camera/image_raw', Image, queue_size=1)
    pub_detected = rospy.Publisher('camera/valve_detection', Image, queue_size=1)

    # Load the YOLO model (Make sure the path to the model is correct)
    model = YOLO('/root/catkin_ws/src/valve_detection/train2/weights/best.pt')

    # Open the webcam (or RealSense camera)
    cap = cv2.VideoCapture(8)  # Change to 8 for RealSense or default camera ID (0)
    if not cap.isOpened():
        rospy.logerr("Failed to open camera!")
        return

    # Set loop rate
    rate = rospy.Rate(30)  # 30 Hz

    while not rospy.is_shutdown():
        ret, frame = cap.read()
        if not ret:
            rospy.logwarn("Empty frame captured, skipping...")
            continue

        # Publish the raw image
        try:
            raw_msg = bridge.cv2_to_imgmsg(frame, "bgr8")
            pub_raw.publish(raw_msg)
        except Exception as e:
            rospy.logerr(f"Error converting image to ROS message: {e}")
            continue

        # Run inference on the frame
        results = model(frame, conf=0.50)  # Lower confidence threshold to 50%

        # Draw bounding boxes on the frame
        img_with_boxes = results[0].plot()  # Add bounding boxes to the image

        # Publish the detected image
        try:
            detected_msg = bridge.cv2_to_imgmsg(img_with_boxes, "bgr8")
            pub_detected.publish(detected_msg)
        except Exception as e:
            rospy.logerr(f"Error converting detected image to ROS message: {e}")

        # Sleep to control the frame rate
        rate.sleep()

    # Release the camera and close any OpenCV windows when the node shuts down
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
