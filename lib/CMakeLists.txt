include_directories(./include)
add_library(PointsToDump SHARED  
    src/PTWriter.cpp       
)
set_target_properties(PointsToDump PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti -g"
)
target_link_libraries(PointsToDump PRIVATE nlohmann_json::nlohmann_json)
include(GNUInstallDirs)
install(TARGETS PointsToDump LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})