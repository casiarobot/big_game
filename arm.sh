#source ~/.bashrc
#source /home/catkin_ws/devel/setup.bash
#roslaunch object_locate demo_object.launch
sudo chmod 666 /dev/ttyUSB0
rosrun arm_control arm_control_node
