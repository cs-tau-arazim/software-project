# CMake generated Testfile for 
# Source directory: /Users/Tom/Downloads/opencv-3.1.0/modules/video
# Build directory: /Users/Tom/Documents/university/software-project/cmakeTry2/modules/video
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_perf_video "/Users/Tom/Documents/university/software-project/cmakeTry2/bin/opencv_perf_video" "--gtest_output=xml:opencv_perf_video.xml")
set_tests_properties(opencv_perf_video PROPERTIES  LABELS "Main;opencv_video;Performance" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmakeTry2/test-reports/performance")
add_test(opencv_sanity_video "/Users/Tom/Documents/university/software-project/cmakeTry2/bin/opencv_perf_video" "--gtest_output=xml:opencv_perf_video.xml" "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
set_tests_properties(opencv_sanity_video PROPERTIES  LABELS "Main;opencv_video;Sanity" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmakeTry2/test-reports/sanity")