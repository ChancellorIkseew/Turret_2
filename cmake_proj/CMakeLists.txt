cmake_minimum_required(VERSION 3.5)
project(Turret_2)
#--------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/${OS_FOLDER})
#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Turret_2")
#-----find_cources---------------------
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "../src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "../src" "../libs/include")
#------OS_settings_1--------------------
if(WIN32)
    set(OS_FOLDER "Windows")
    set(LIB_EXT ".lib")
    set(DLL_EXT ".dll")
elseif(UNIX AND NOT APPLE)
    set(OS_FOLDER "Linux")
    set(LIB_EXT ".so")
    set(DLL_EXT ".so")
endif()
#------OS_settings_2-------------------
if(WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}../libs/MSVC_lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3${LIB_EXT}
        SDL3_image${LIB_EXT}
        angelscript64${LIB_EXT})
elseif(UNIX AND NOT APPLE)
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}../libs/GCC_so)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3
        SDL3_image
        angelscript64)
endif()
