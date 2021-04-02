include_directories(./include)
add_library(PointsToDump SHARED  
    src/PTWriter.cpp      
    src/PTReader.cpp
    src/FS_PTWriter.cpp
    src/FS_PTReader.cpp     
)
set_target_properties(PointsToDump PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti -g"
)
target_link_libraries(PointsToDump PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(PointsToDump PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
include(GNUInstallDirs)
install(TARGETS PointsToDump LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY
./include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})