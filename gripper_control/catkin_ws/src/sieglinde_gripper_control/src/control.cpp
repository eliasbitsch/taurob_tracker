#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/JointState.h"
#include "dynamixel_sdk/dynamixel_sdk.h"

using namespace dynamixel;

// Control table address
#define ADDR_TORQUE_ENABLE    64
#define ADDR_GOAL_POSITION    116
#define ADDR_PRESENT_POSITION 132
#define Operation_Mode        11
#define ADDR_current_limit    38
#define ADDR_goal_current     102
#define ADDR_goal_position    116

// Protocol version
#define PROTOCOL_VERSION      2.0 // Default Protocol version of DYNAMIXEL X series

// Default setting
#define DXL1_ID               1
#define DXL2_ID               2
#define BAUDRATE              57600
#define DEVICE_NAME           "/dev/ttyUSB0" // Use "$ ls /dev/ttyUSB*" to find the correct port
#define current_mode          0
#define position_current_mode 5
#define M_PI                  3.14159265358979323846

PortHandler* portHandler;
PacketHandler* packetHandler;

uint8_t dxl_error = 0;
int dxl_comm_result = COMM_TX_FAIL;

// Enable torque for both motors
void enable() {
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_TORQUE_ENABLE, 1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to enable torque for Dynamixel ID %d", DXL1_ID);
    }

    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_TORQUE_ENABLE, 1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to enable torque for Dynamixel ID %d", DXL2_ID);
    }
}

// Set operation mode for both motors
void setOperationMode() {
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL1_ID, Operation_Mode, position_current_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to set operation mode for Dynamixel ID %d", DXL1_ID);
    }

    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL2_ID, Operation_Mode, position_current_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to set operation mode for Dynamixel ID %d", DXL2_ID);
    }
}

// Initialize connection to Dynamixel motors
void init() {
    if (!portHandler->openPort()) {
        ROS_ERROR("Failed to open the port!");
    }

    if (!portHandler->setBaudRate(BAUDRATE)) {
        ROS_ERROR("Failed to set the baudrate!");
    }
}

// Publish positions for Motor 1 and Motor 2
void publish_position(ros::Publisher* motor1_pub, ros::Publisher* motor2_pub) {
    int32_t position1 = 0;
    int32_t position2 = 0;

    dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL1_ID, ADDR_PRESENT_POSITION, (uint32_t*)&position1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to read position for Dynamixel ID %d", DXL1_ID);
    }

    dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL2_ID, ADDR_PRESENT_POSITION, (uint32_t*)&position2, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        ROS_ERROR("Failed to read position for Dynamixel ID %d", DXL2_ID);
    }

    // Publish to /get_position1
    std_msgs::Int32 motor1_msg;
    motor1_msg.data = position1;
    motor1_pub->publish(motor1_msg);

    // Publish to /get_position2
    std_msgs::Int32 motor2_msg;
    motor2_msg.data = position2;
    motor2_pub->publish(motor2_msg);
}

// Set position for Motor 1
void setPosition1(const std_msgs::Int32::ConstPtr& msg) {
    int position = msg->data;

    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL1_ID, ADDR_GOAL_POSITION, position, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS) {
        ROS_INFO("setPosition1 : [ID:1] [Position:%d]", position);
    } else {
        ROS_ERROR("Failed to set position for motor 1.");
    }
}

// Set position for Motor 2
void setPosition2(const std_msgs::Int32::ConstPtr& msg) {
    int position = msg->data;

    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL2_ID, ADDR_GOAL_POSITION, position, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS) {
        ROS_INFO("setPosition2 : [ID:2] [Position:%d]", position);
    } else {
        ROS_ERROR("Failed to set position for motor 2.");
    }
}

// Set current for Motor 1
void setCurrent1(const std_msgs::Int32::ConstPtr& msg) {
    int current = msg->data;

    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL1_ID, ADDR_goal_current, current, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS) {
        ROS_INFO("setCurrent1 : [ID:1] [Current:%d]", current);
    } else {
        ROS_ERROR("Failed to set current for motor 1.");
    }
}

// Set current for Motor 2
void setCurrent2(const std_msgs::Int32::ConstPtr& msg) {
    int current = msg->data;

    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL2_ID, ADDR_goal_current, current, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS) {
        ROS_INFO("setCurrent2 : [ID:2] [Current:%d]", current);
    } else {
        ROS_ERROR("Failed to set current for motor 2.");
    }
}

int main(int argc, char** argv) {
    portHandler = PortHandler::getPortHandler(DEVICE_NAME);
    packetHandler = PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    // Initialize motors
    init();
    setOperationMode();
    enable();

    // ROS setup
    ros::init(argc, argv, "dynamixel_control");
    ros::NodeHandle nh;

    // Publishers for motor positions
    ros::Publisher motor1_pos_pub = nh.advertise<std_msgs::Int32>("/get_position1", 10);
    ros::Publisher motor2_pos_pub = nh.advertise<std_msgs::Int32>("/get_position2", 10);

    // Subscribers for setting motor positions and currents
    ros::Subscriber set_position_sub1 = nh.subscribe("/set_position1", 10, setPosition1);
    ros::Subscriber set_position_sub2 = nh.subscribe("/set_position2", 10, setPosition2);
    ros::Subscriber set_current_sub1 = nh.subscribe("/set_current1", 10, setCurrent1);
    ros::Subscriber set_current_sub2 = nh.subscribe("/set_current2", 10, setCurrent2);

    ros::Rate loop_rate(10);  // 10 Hz

    while (ros::ok()) {
        publish_position(&motor1_pos_pub, &motor2_pos_pub);  // Publish motor positions
        ros::spinOnce();  // Handle callbacks
        loop_rate.sleep();  // Sleep for the remainder of the cycle
    }

    // Disable torque when done
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_TORQUE_ENABLE, 0, &dxl_error);
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_TORQUE_ENABLE, 0, &dxl_error);

    portHandler->closePort();
    return 0;
}
