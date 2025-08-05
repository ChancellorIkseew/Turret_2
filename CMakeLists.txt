cmake_minimum_required(VERSION 3.13)
project(Turret_2)

# Определение ОС и настроек
if(WIN32)
    set(OS_FOLDER "Windows")
    set(LIB_EXT ".lib")
    set(DLL_EXT ".dll")
    set(ANGELSCRIPT_LIB "angelscript64")
elseif(UNIX AND NOT APPLE)
    set(OS_FOLDER "Linux")
    set(LIB_EXT ".so")
    set(DLL_EXT ".so")
    set(ANGELSCRIPT_LIB "angelscript")
endif()

# Настройка выходных директорий
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/${OS_FOLDER})

# Поиск исходных файлов
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Директории заголовков
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Платформозависимые настройки
if(WIN32)
    # Windows
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lib
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3${LIB_EXT}
        SDL3_image${LIB_EXT}
        ${ANGELSCRIPT_LIB}${LIB_EXT}
    )

    # Копирование DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/*${DLL_EXT}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    )

elseif(UNIX AND NOT APPLE)
    # Linux
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/linux
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3
        SDL3_image
        ${ANGELSCRIPT_LIB}
    )

    # Копирование .so и ресурсов
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/*${DLL_EXT}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    )
endif()

# Общие настройки
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "Turret_2"
)
