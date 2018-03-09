;; Auto-generated. Do not edit!


(when (boundp 'object_locate::apple_pos)
  (if (not (find-package "OBJECT_LOCATE"))
    (make-package "OBJECT_LOCATE"))
  (shadow 'apple_pos (find-package "OBJECT_LOCATE")))
(unless (find-package "OBJECT_LOCATE::APPLE_POS")
  (make-package "OBJECT_LOCATE::APPLE_POS"))

(in-package "ROS")
;;//! \htmlinclude apple_pos.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass object_locate::apple_pos
  :super ros::object
  :slots (_data _pose ))

(defmethod object_locate::apple_pos
  (:init
   (&key
    ((:data __data) (let (r) (dotimes (i 0) (push (instance std_msgs::Char :init) r)) r))
    ((:pose __pose) (instance geometry_msgs::Pose :init))
    )
   (send-super :init)
   (setq _data __data)
   (setq _pose __pose)
   self)
  (:data
   (&rest __data)
   (if (keywordp (car __data))
       (send* _data __data)
     (progn
       (if __data (setq _data (car __data)))
       _data)))
  (:pose
   (&rest __pose)
   (if (keywordp (car __pose))
       (send* _pose __pose)
     (progn
       (if __pose (setq _pose (car __pose)))
       _pose)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Char[] _data
    (apply #'+ (send-all _data :serialization-length)) 4
    ;; geometry_msgs/Pose _pose
    (send _pose :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Char[] _data
     (write-long (length _data) s)
     (dolist (elem _data)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Pose _pose
       (send _pose :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Char[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (let (r) (dotimes (i n) (push (instance std_msgs::Char :init) r)) r))
     (dolist (elem- _data)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Pose _pose
     (send _pose :deserialize buf ptr-) (incf ptr- (send _pose :serialization-length))
   ;;
   self)
  )

(setf (get object_locate::apple_pos :md5sum-) "cb605b25d565ff4ea61c702d2fc0b3a2")
(setf (get object_locate::apple_pos :datatype-) "object_locate/apple_pos")
(setf (get object_locate::apple_pos :definition-)
      "std_msgs/Char[] data
geometry_msgs/Pose pose

================================================================================
MSG: std_msgs/Char
char data

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

")



(provide :object_locate/apple_pos "cb605b25d565ff4ea61c702d2fc0b3a2")


