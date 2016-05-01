# CMake generated Testfile for 
# Source directory: /Users/Tom/Downloads/opencv-3.1.0/modules/imgcodecs
# Build directory: /Users/Tom/Documents/university/software-project/cmake_build/modules/imgcodecs
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_perf_imgcodecs "/Users/Tom/Documents/university/software-project/cmake_build/bin/opencv_perf_imgcodecs" "--gtest_output=xml:opencv_perf_imgcodecs.xml")
set_tests_properties(opencv_perf_imgcodecs PROPERTIES  LABELS "Main;opencv_imgcodecs;Performance" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmake_build/test-reports/performance")
add_test(opencv_sanity_imgcodecs "/Users/Tom/Documents/university/software-project/cmake_build/bin/opencv_perf_imgcodecs" "--gtest_output=xml:opencv_perf_imgcodecs.xml" "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
set_tests_properties(opencv_sanity_imgcodecs PROPERTIES  LABELS "Main;opencv_imgcodecs;Sanity" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmake_build/test-reports/sanity")
