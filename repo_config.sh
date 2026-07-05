#!/bin/bash
# REPO Config
## General Config
export REPO_NAME="robot_framework_ros"
export BUILD_TOOL="catkin" #cmake, ...

## Coverage Config
export LINE_COVERAGE_THRESHOLD=50
export BRANCH_COVERAGE_THRESHOLD=0 # Don't care about Branch Coverage.