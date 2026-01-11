cmake_minimum_required(VERSION 3.5)
project(Turret_2)
#--------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
#--------------------------------------
set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#-----find_cources---------------------
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "../src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "../src" "../libs/include")
#------OS_settings---------------------
if(WIN32)
    set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/MSVC_lib")
    target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "${LIBS_DIR}/zlib.lib"
        "${LIBS_DIR}/SDL3.lib"
        "${LIBS_DIR}/SDL3_mixer.lib"
        "${LIBS_DIR}/angelscript64.lib")
elseif(UNIX AND NOT APPLE)
    set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/GCC_so")
    target_link_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "${LIBS_DIR}/libz.so.1"
        "${LIBS_DIR}/libSDL3.so.0"
        "${LIBS_DIR}/libSDL3_mixer.so.0"
        "${LIBS_DIR}/libangelscript64.so.2.37.0")
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread atomic) # Linux std::atomic
endif()

