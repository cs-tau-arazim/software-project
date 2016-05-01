# CMake generated Testfile for 
# Source directory: /Users/Tom/Downloads/opencv-3.1.0/modules/stitching
# Build directory: /Users/Tom/Documents/university/software-project/cmake_build/modules/stitching
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_perf_stitching "/Users/Tom/Documents/university/software-project/cmake_build/bin/opencv_perf_stitching" "--gtest_output=xml:opencv_perf_stitching.xml")
set_tests_properties(opencv_perf_stitching PROPERTIES  LABELS "Main;opencv_stitching;Performance" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmake_build/test-reports/performance")
add_test(opencv_sanity_stitching "/Users/Tom/Documents/university/software-project/cmake_build/bin/opencv_perf_stitching" "--gtest_output=xml:opencv_perf_stitching.xml" "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
set_tests_properties(opencv_sanity_stitching PROPERTIES  LABELS "Main;opencv_stitching;Sanity" WORKING_DIRECTORY "/Users/Tom/Documents/university/software-project/cmake_build/test-reports/sanity")
