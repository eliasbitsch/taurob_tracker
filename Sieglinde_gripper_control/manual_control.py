import time
import rospy
from std_msgs.msg import Int32

# Initialize the ROS node
rospy.init_node('manual_gripper_control', anonymous=False)
pub_grip = rospy.Publisher('set_position2', Int32, queue_size=1)  # Gripper control
pub_rot = rospy.Publisher('set_current1', Int32, queue_size=1)    # Rotation axis control

def move_gripper(position):
    """Move Rotation Axis with position (min = -4095; max = 4095)"""
    pub_grip.publish(position)

def move_rotation(current):
    """Move Gripper with speed/current (min = -100; max = 100)"""
    pub_rot.publish(current)

def manual_control():
    print("Manual Control: Type 'exit' to quit.")
    print("Options: 1 - Control Gripper, 2 - Control Rotation Axis")
    
    while True:
        try:
            # User input for which motor to control
            motor_choice = input("Enter motor choice (1 for Gripper, 2 for Rotation): ")
            if motor_choice.lower() == 'exit':
                break

            if motor_choice not in ['1', '2']:
                print("Invalid choice. Please enter 1 or 2.")
                continue

            if motor_choice == '1':
                # Control the rotation axis
                current = input("Enter current for rotation axis (-100 to 100): ")
                if current.lower() == 'exit':
                    break
                current = int(current)

                # Validate input
                if current < -100 or current > 100:
                    print("Invalid input for current. Please enter a value between -100 and 100.")
                    continue
                
                move_rotation(current)  # Move rotation axis

            elif motor_choice == '2':
                # Control the gripper
                angle = input("Enter position for gripper (-4095 to 4095): ")
                if angle.lower() == 'exit':
                    break
                angle = int(angle)

                # Validate input
                if angle < -4095 or angle > 4095:
                    print("Invalid input for angle. Please enter a value between -4095 and 4095.")
                    continue
                
                move_gripper(angle)  # Move gripper

            # Small delay to allow motor response
            time.sleep(0.1) 

        except ValueError:
            print("Invalid input. Please enter numeric values.")

if __name__ == "__main__":
    try:
        manual_control()
    finally:
        # Ensure motors are stopped when exiting
        move_rotation(0)
        move_gripper(0)
        time.sleep(1)