# CMake generated Testfile for 
# Source directory: /Users/Tom/Downloads/opencv-master/modules/highgui
# Build directory: /Users/Tom/Documents/university/software-project/modules/highgui
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_highgui "/Users/Tom/Documents/university/software-project/bin/opencv_test_highgui" "--gtest_output=xml:opencv_test_highgui.xml")
set_tests_properties(opencv_test_highgui PROPERTIES  LABELS "Main;opencv_highgui;Accuracy" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/test-reports/accuracy")
