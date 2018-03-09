
(cl:in-package :asdf)

(defsystem "object_locate-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "apple_pos" :depends-on ("_package_apple_pos"))
    (:file "_package_apple_pos" :depends-on ("_package"))
  ))