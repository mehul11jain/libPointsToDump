set(LLVM_LINK_COMPONENTS core support irreader passes)
include_directories(./include)
add_library(PointsToDump SHARED  
    src/test.cpp       
)
set_target_properties(PointsToDump PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti -g"
)
target_link_libraries(PointsToDump PRIVATE nlohmann_json::nlohmann_json)
llvm_config (PointsToDump ${LLVM_LINK_COMPONENTS})
install(TARGETS PointsToDump LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})