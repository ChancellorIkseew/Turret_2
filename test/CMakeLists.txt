# test/CMakeLists.txt — Catch2 amalgamated (no FetchContent needed)

add_executable(Turret_2_tests
    hitbox_test.cpp
    validator_test.cpp
    inplace_vector_test.cpp
    catch2/catch_amalgamated.cpp
    # Game sources needed by tests:
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/engine/io/parser/validator.cpp
)

target_include_directories(turret_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libs/include"
)

include(CTest)
add_test(NAME turret_tests COMMAND turret_tests)
