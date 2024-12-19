#!/usr/bin/env python

import os
import rospy
import rospkg
from qt_gui.plugin import Plugin
from python_qt_binding import loadUi
from python_qt_binding.QtWidgets import QWidget
from dynamic_reconfigure.server import Server

from std_msgs.msg import Int32  # Adjust message type as needed

class GripperControlPlugin(Plugin):
    def __init__(self, context):
        super(GripperControlPlugin, self).__init__(context)
        self.setObjectName('GripperControlPlugin')

        # Create QWidget
        self._widget = QWidget()
        
        # Get path to UI file
        rp = rospkg.RosPack()
        ui_file = os.path.join(rp.get_path('rqt_gripper_control'), 'resource', 'GripperControl.ui')
        loadUi(ui_file, self._widget)
        
        # Add widget to the user interface
        context.add_widget(self._widget)

        # Initialize ROS components
        self.position_subscriber = rospy.Subscriber('/get_position1', Int32, self.position_callback)
        self.position_publisher = rospy.Publisher('/set_position', Int32, queue_size=10)
        
        # Setup dynamic reconfigure server
        self.server = Server(GripperConfig, self.config_callback)

        # Connect slider to callback
        self._widget.position_slider.valueChanged.connect(self.slider_changed)
        
        # Initialize position value
        self.current_position = 0

    def position_callback(self, msg):
        """Callback for receiving position updates"""
        self.current_position = msg.data
        # Update slider and spinbox without triggering their callbacks
        self._widget.position_slider.blockSignals(True)
        self._widget.position_spinbox.blockSignals(True)
        
        self._widget.position_slider.setValue(self.current_position)
        self._widget.position_spinbox.setValue(self.current_position)
        
        self._widget.position_slider.blockSignals(False)
        self._widget.position_spinbox.blockSignals(False)

    def config_callback(self, config, level):
        """Callback for dynamic reconfigure updates"""
        # Update the GUI elements with the new configuration
        self._widget.position_slider.blockSignals(True)
        self._widget.position_spinbox.blockSignals(True)
        
        self._widget.position_slider.setValue(config.gripper_position)
        self._widget.position_spinbox.setValue(config.gripper_position)
        
        self._widget.position_slider.blockSignals(False)
        self._widget.position_spinbox.blockSignals(False)
        
        return config

    def slider_changed(self, value):
        """Callback for slider value changes"""
        # Publish the new position
        msg = Int32()
        msg.value = value
        self.position_publisher.publish(msg)

    def shutdown_plugin(self):
        """Clean up subscribers and publishers"""
        self.position_subscriber.unregister()
        self.position_publisher.unregister()