#!/usr/bin/env python3

import sys
import rospy
import moveit_commander
import moveit_msgs.msg
import geometry_msgs.msg



if __name__=="__main__":
    try:
        #demo example to demontrate setting stored poses in moveit srdf file
        moveit_commander.roscpp_initialize(sys.argv)
        rospy.init_node("move_group_interface_demo", anonymous=True)
        
        robot = moveit_commander.RobotCommander()
        scene = moveit_commander.PlanningSceneInterface()

        group_name = "arm"
        move_group = moveit_commander.MoveGroupCommander(group_name)

        print("============ Printing robot state")
        print(robot.get_current_state())

        print("============ setting robot to look_rear")
        move_group.set_named_target("look_rear")
        move_group.go()

        print("============ setting robot to raise_up")
        move_group.set_named_target("look_up")
        move_group.go()

    except rospy.ROSInterruptException as e:
        rospy.loginfo(e)
        exit(0)
    except KeyboardInterrupt:
        exit(0)

