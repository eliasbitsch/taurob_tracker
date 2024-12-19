#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <rqt_gripper_control/GripperConfig.h>
#include <std_msgs/Int32.h>

ros::Publisher gripper_pub1;
ros::Publisher gripper_pub2;

void callback(rqt_gripper_control::GripperConfig &config, uint32_t level) {
    // Publish position for both grippers
    std_msgs::Int32 msg1;
    msg1.data = config.gripper_position;  // Position for gripper 1
    gripper_pub1.publish(msg1);
    
    std_msgs::Int32 msg2;
    msg2.data = config.axis_5_position;  // Position for gripper 2
    gripper_pub2.publish(msg2);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "gripper_control");
    ros::NodeHandle nh;

    // Publisher for both gripper positions
    gripper_pub1 = nh.advertise<std_msgs::Int32>("set_position1", 10);
    gripper_pub2 = nh.advertise<std_msgs::Int32>("set_position2", 10);

    // Dynamic Reconfigure Server
    dynamic_reconfigure::Server<rqt_gripper_control::GripperConfig> server;
    dynamic_reconfigure::Server<rqt_gripper_control::GripperConfig>::CallbackType f;

    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);

    ros::spin();
    return 0;
}
