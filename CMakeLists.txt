cmake_minimum_required(VERSION 3.4)
project(PointsToDump)
set(LLVM_ROOT "" CACHE PATH "/usr/lib/llvm-10")

list(APPEND CMAKE_PREFIX_PATH "${LLVM_ROOT}/share/llvm/cmake")
find_package(LLVM REQUIRED 10.0.0)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

find_package(nlohmann_json REQUIRED 3.9.1)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

include_directories(./lib/include)
add_subdirectory(lib)

include(GNUInstallDirs)
install(DIRECTORY
lib/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})