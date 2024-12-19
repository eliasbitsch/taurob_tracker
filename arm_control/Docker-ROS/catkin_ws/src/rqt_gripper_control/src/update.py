#!/usr/bin/env python

import rospy
from dynamic_reconfigure.server import Server
from std_msgs.msg import Int32
from rqt_gripper_control.cfg import GripperConfig  # This will be generated from your config

class GripperController:
    def __init__(self):
        rospy.init_node('gripper_controller', anonymous=True)
        
        # Subscribe to position topic
        self.position_sub = rospy.Subscriber('/get_position1', Int32, self.position_callback)
        
        # Create dynamic reconfigure server
        self.server = Server(GripperConfig, self.config_callback)
        
        # Store current position
        self.current_position = 0

    def position_callback(self, msg):
        """Called when new position is received"""
        self.current_position = msg.data
        
        # Update dynamic reconfigure with new position
        # We use try/except because the server might not be ready
        try:
            current_config = self.server.config
            current_config['gripper_position'] = self.current_position
            self.server.update_configuration(current_config)
        except Exception as e:
            rospy.logwarn(f"Failed to update config: {e}")

    def config_callback(self, config, level):
        """Called when dynamic reconfigure values change"""
        rospy.loginfo(f"Received new position from dynamic reconfigure: {config.gripper_position}")
        return config

if __name__ == '__main__':
    try:
        controller = GripperController()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass