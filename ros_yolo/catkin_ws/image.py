import rospy
import pyrealsense2 as rs
import numpy as np
from sensor_msgs.msg import Image, PointCloud2
from std_msgs.msg import Header
import sensor_msgs.point_cloud2 as pc2
import cv2
from cv_bridge import CvBridge
import tf

class RealSensePublisher:
    def __init__(self):
        rospy.init_node('realsense_publisher', anonymous=True)
        
        # Create publishers
        self.image_pub = rospy.Publisher('/camera/color/image_raw', Image, queue_size=10)
        self.pointcloud_pub = rospy.Publisher('/camera/pointcloud', PointCloud2, queue_size=10)
        
        # Create TF broadcaster
        self.tf_broadcaster = tf.TransformBroadcaster()

        # Create RealSense pipeline
        self.pipeline = rs.pipeline()
        self.config = rs.config()
        self.config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
        self.config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

        # Start the pipeline
        self.pipeline.start(self.config)

        # Set up cv_bridge
        self.bridge = CvBridge()

    def publish(self):
        while not rospy.is_shutdown():
            frames = self.pipeline.wait_for_frames()
            color_frame = frames.get_color_frame()
            depth_frame = frames.get_depth_frame()

            if not color_frame or not depth_frame:
                continue

            # Convert images to ROS format
            color_image = np.asanyarray(color_frame.get_data())
            depth_image = np.asanyarray(depth_frame.get_data())

            # Publish color image
            ros_image = self.bridge.cv2_to_imgmsg(color_image, "bgr8")
            ros_image.header = Header()
            ros_image.header.stamp = rospy.Time.now()
            ros_image.header.frame_id = "camera_color_frame"
            self.image_pub.publish(ros_image)

            # Generate point cloud
            pc = rs.pointcloud()
            points = pc.calculate(depth_frame)
            v, t = points.get_vertices(), points.get_texture_coordinates()
            pc_data = np.asanyarray(v)
            pc_data = pc_data.view(np.float32).reshape(-1, 3)

            # Create PointCloud2 message
            header = Header()
            header.stamp = rospy.Time.now()
            header.frame_id = "camera_depth_frame"

            # Convert to PointCloud2 message
            pointcloud_msg = pc2.create_cloud_xyz32(header, pc_data)
            self.pointcloud_pub.publish(pointcloud_msg)

            # Publish TF transforms
            self.tf_broadcaster.sendTransform(
                (0, 0, 0),  # Translation
                (0, 0, 0, 1),  # Rotation (identity)
                rospy.Time.now(),
                "camera_color_frame",  # Child frame
                "odom"  # Parent frame
            )

            # Optional: Show image using OpenCV
            cv2.imshow("Color Image", color_image)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    def cleanup(self):
        self.pipeline.stop()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        publisher = RealSensePublisher()
        publisher.publish()
    except rospy.ROSInterruptException:
        pass
    finally:
        publisher.cleanup()
