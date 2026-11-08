add_executable(turret_tests
    hitbox_test.cpp
    validator_test.cpp
    inplace_vector_test.cpp
    catch2/catch_amalgamated.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/engine/io/parser/validator.cpp
)

target_include_directories(turret_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libs/include"
)

add_test(NAME turret_tests COMMAND turret_tests)
