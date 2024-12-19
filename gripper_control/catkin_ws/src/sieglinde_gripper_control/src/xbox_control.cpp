#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Int32.h>
#include <unistd.h>

ros::Publisher pub_set_position1;
ros::Publisher pub_set_position2;

bool enable_button = 0;

int axis_gripper_open = 5;
int axis_gripper_close = 4;
int axis_dpad_lr = 6;

const int GRIPPER_OPEN = 6900;
const int GRIPPER_CLOSE = 2000;

const int AXIS_5_LIMIT_MIN = -100000;
const int AXIS_5_LIMIT_MAX = 100000;

const double MAX_SPEED = 10.0;
const int STEP_SPEED = 30;

const double TRIGGER_DEADBAND = 0.15;
const double SPEED_DEADBAND = 0.5;
const double DPAD_DEADBAND = 0.2;

// Initialize triggers to neutral state
float raw_close = 1.0;
float raw_open = 1.0;
float current_close_trigger = 0.0;
float current_open_trigger = 0.0;

int current_position = 0;
double commanded_position = 0;
double commanded_position2 = 0;

bool position_initialized = false;

int current_position2 = 0;
bool position2_initialized = false;
float current_dpad_lr = 0.0;

void move_gripper_angle(int angle) {
    std_msgs::Int32 msg;
    msg.data = angle;
    pub_set_position1.publish(msg);
}

void move_gripper_stepwise(int position) {
    std_msgs::Int32 msg;
    msg.data = position;
    pub_set_position2.publish(msg);
}

void positionCallback(const std_msgs::Int32::ConstPtr& msg) {
    current_position = msg->data;
    
    if (!position_initialized) {
        commanded_position = current_position;
        position_initialized = true;
    }
}

void position2Callback(const std_msgs::Int32::ConstPtr& msg) {
    current_position2 = msg->data;
    
    if (!position2_initialized) {
        commanded_position2 = current_position2;
        position2_initialized = true;
    }
}


void joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
    // Check if both triggers are pressed simultaneously
    if (msg->axes[axis_gripper_open] == 0.0 && msg->axes[axis_gripper_close] == 0.0) {
        ROS_INFO("Press both triggers to initialize");
        raw_open = 1.0;
        raw_close = 1.0;
    } else if (msg->axes[axis_gripper_open] != 0.0 && msg->axes[axis_gripper_close] != 0.0) {
        ROS_INFO("Only one trigger can be used at a time");
             raw_close = msg->axes[axis_gripper_close];
        raw_open = msg->axes[axis_gripper_open];
        enable_button = msg->buttons[6];
    }
    
    // Convert raw trigger values to usable range
    current_close_trigger = (1.0 - raw_close) / 2.0;
    current_open_trigger = (1.0 - raw_open) / 2.0;
    current_dpad_lr = msg->axes[axis_dpad_lr];
    
    ROS_INFO("Raw - Close: %.2f, Open: %.2f", raw_close, raw_open);
    ROS_INFO("Current - Close: %.2f, Open: %.2f", current_close_trigger, current_open_trigger);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "sieglinde_gripper_control");
    ros::NodeHandle nh;

    pub_set_position1 = nh.advertise<std_msgs::Int32>("set_position1", 1);
    pub_set_position2 = nh.advertise<std_msgs::Int32>("set_position2", 1);
    ros::Subscriber joy_sub = nh.subscribe("joy", 10, joyCallback);
    ros::Subscriber pos_sub = nh.subscribe("get_position1", 10, positionCallback);
    ros::Subscriber pos2_sub = nh.subscribe("get_position2", 10, position2Callback);

    ROS_INFO("Gripper control node initialized");

    ros::Rate init_rate(10);
    while ((!position_initialized || !position2_initialized) && ros::ok()) {
        ros::spinOnce();
        init_rate.sleep();
    }

    ROS_INFO("Gripper positions initialized: Commanded position1: %d, Commanded position2: %d",
             current_position, current_position2);

    ros::Rate loop_rate(400);
    while (ros::ok()) {
        ros::spinOnce();

        double speed = 0.0;
        
        // Only calculate speeds if the enable button is pressed
        if (enable_button) {
            // Open gripper
            if (current_open_trigger > TRIGGER_DEADBAND) {
                speed = MAX_SPEED * current_open_trigger;
            }
            // Close gripper
            else if (current_close_trigger > TRIGGER_DEADBAND) {
                speed = -MAX_SPEED * current_close_trigger;
            }
        }

        // Only move if speed is above minimum threshold
        if (std::abs(speed) > SPEED_DEADBAND && position_initialized) {
            commanded_position += speed;
            commanded_position = std::round(commanded_position);
            commanded_position = std::max(static_cast<double>(GRIPPER_CLOSE), 
                                       std::min(commanded_position, static_cast<double>(GRIPPER_OPEN)));
            
            static int last_published_position = commanded_position;
            if (static_cast<int>(commanded_position) != last_published_position) {
                move_gripper_angle(static_cast<int>(commanded_position));
                last_published_position = static_cast<int>(commanded_position);
                ROS_INFO("Moving to position1: %.1f (speed: %.1f)", commanded_position, speed);
            }
        }

        // Handle set_position2 with dpad
        if (current_dpad_lr > 0.5 && enable_button) {
            current_position2 += STEP_SPEED;
            move_gripper_stepwise(current_position2);
        } 
        if (current_dpad_lr < -0.5 && enable_button) {
            current_position2 -= STEP_SPEED;
            move_gripper_stepwise(current_position2);
        }

        loop_rate.sleep();
    }

    return 0;
}