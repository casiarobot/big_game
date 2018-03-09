cmake_minimum_required(VERSION 2.8.3)
project(object_locate)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  message_generation
  roscpp
  std_msgs
  cv_bridge
  image_transport
  tf
  
)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)


#find_package(OpenCV REQUIRED)
#if(OPENCV_FOUND)
set(glfw3 /usr/local/lib/cmake/glfw3/)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif()

find_package(OpenCV 3 REQUIRED)
find_package(librealsense 1 REQUIRED) #librealsense package currently lacks required pkgconfig file


set(CMAKE_CXX_FLAGS "-fPIE -fPIC -std=c++11 -O2 -D_FORTIFY_SOURCE=2 -fstack-protector -Wformat -Wformat-security -Wall ${CMAKE_CXX_FLAGS}")

add_message_files(
   FILES
   apple_pos.msg
#  Extrinsics.msg
)

generate_messages(
 DEPENDENCIES 
 geometry_msgs std_msgs
)

catkin_package(
  LIBRARIES realsense
  CATKIN_DEPENDS  roscpp  std_msgs message_runtime
  image_transport
)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)
#add_library(realsense_ros_camera
#   src/camera_node.cpp
#)
link_libraries("/home/catkin_ws/src/object_locate/libs/libdarknet.so")

#add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_generate_messages_cpp)
#add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
add_executable(object_locate src/object_locate.cpp)
add_executable(key_test src/key_test.cpp)

target_link_libraries(object_locate
   realsense
   ${OPENGL_gl_LIBRARY}
   ${GLFW_LIBRARIES}
   ${catkin_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${CMAKE_THREAD_LIBS_INIT}
   m 
   pthread
   darknet
#   ${OpenMP_LIBS}
 )
target_link_libraries(key_test
#   realsense
#   ${OPENGL_gl_LIBRARY}
#   ${GLFW_LIBRARIES}
   ${catkin_LIBRARIES}
#   ${OpenCV_LIBRARIES}
   ${CMAKE_THREAD_LIBS_INIT}
#   ${OpenMP_LIBS}
 )

#if (CATKIN_ENABLE_TESTING)
# # find_package(rostest REQUIRED)

#  add_executable(realsense_ros_tests_camera test/camera_core.cpp)
#  target_link_libraries(realsense_ros_tests_camera
#    ${catkin_LIBRARIES}
#    ${GTEST_LIBRARIES}
#    -lpthread -lm
#    )
#  add_dependencies(realsense_ros_tests_camera ${PROJECT_NAME}_generate_messages_cpp)
#  add_dependencies(realsense_ros_tests_camera ${catkin_EXPORTED_TARGETS})
#endif()


install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(FILES nodelet_plugins.xml 
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

