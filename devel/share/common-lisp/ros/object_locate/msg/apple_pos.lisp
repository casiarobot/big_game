; Auto-generated. Do not edit!


(cl:in-package object_locate-msg)


;//! \htmlinclude apple_pos.msg.html

(cl:defclass <apple_pos> (roslisp-msg-protocol:ros-message)
  ((data
    :reader data
    :initarg :data
    :type (cl:vector std_msgs-msg:Char)
   :initform (cl:make-array 0 :element-type 'std_msgs-msg:Char :initial-element (cl:make-instance 'std_msgs-msg:Char)))
   (pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose)))
)

(cl:defclass apple_pos (<apple_pos>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <apple_pos>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'apple_pos)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name object_locate-msg:<apple_pos> is deprecated: use object_locate-msg:apple_pos instead.")))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <apple_pos>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader object_locate-msg:data-val is deprecated.  Use object_locate-msg:data instead.")
  (data m))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <apple_pos>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader object_locate-msg:pose-val is deprecated.  Use object_locate-msg:pose instead.")
  (pose m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <apple_pos>) ostream)
  "Serializes a message object of type '<apple_pos>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'data))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <apple_pos>) istream)
  "Deserializes a message object of type '<apple_pos>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'std_msgs-msg:Char))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<apple_pos>)))
  "Returns string type for a message object of type '<apple_pos>"
  "object_locate/apple_pos")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'apple_pos)))
  "Returns string type for a message object of type 'apple_pos"
  "object_locate/apple_pos")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<apple_pos>)))
  "Returns md5sum for a message object of type '<apple_pos>"
  "cb605b25d565ff4ea61c702d2fc0b3a2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'apple_pos)))
  "Returns md5sum for a message object of type 'apple_pos"
  "cb605b25d565ff4ea61c702d2fc0b3a2")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<apple_pos>)))
  "Returns full string definition for message of type '<apple_pos>"
  (cl:format cl:nil "std_msgs/Char[] data~%geometry_msgs/Pose pose~%~%================================================================================~%MSG: std_msgs/Char~%char data~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'apple_pos)))
  "Returns full string definition for message of type 'apple_pos"
  (cl:format cl:nil "std_msgs/Char[] data~%geometry_msgs/Pose pose~%~%================================================================================~%MSG: std_msgs/Char~%char data~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <apple_pos>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <apple_pos>))
  "Converts a ROS message object to a list"
  (cl:list 'apple_pos
    (cl:cons ':data (data msg))
    (cl:cons ':pose (pose msg))
))
