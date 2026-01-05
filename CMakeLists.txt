cmake_minimum_required(VERSION 3.16)

project(sys_expert C)

# Use C11 and strict warnings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for tooling (already appears in build/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all C sources under src/
file(GLOB_RECURSE SYS_EXPERT_SOURCES CONFIGURE_DEPENDS src/*.c)
set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
set(UI_SOURCE   "${CMAKE_CURRENT_SOURCE_DIR}/src/ui.c")

# Ncurses for TUI (prefer wide-character library for UTF-8)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)

# Build a reusable library without main.c (and optionally without ui.c)
set(LIB_SOURCES ${SYS_EXPERT_SOURCES})
list(REMOVE_ITEM LIB_SOURCES "${MAIN_SOURCE}")
if (NOT CURSES_FOUND)
    list(REMOVE_ITEM LIB_SOURCES "${UI_SOURCE}")
    message(STATUS "Curses not found; building without TUI. Use -t/--text-only to run.")
endif()

add_library(sys_expert_lib STATIC ${LIB_SOURCES})
target_include_directories(sys_expert_lib PUBLIC src)

# Common warnings for GCC/Clang
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sys_expert_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (CURSES_FOUND)
    target_include_directories(sys_expert_lib PUBLIC ${CURSES_INCLUDE_DIR})
    target_link_libraries(sys_expert_lib PUBLIC ${CURSES_LIBRARIES})
    target_compile_definitions(sys_expert_lib PUBLIC HAVE_CURSES)
endif()

# Main executable links the library and contains only main.c
add_executable(sys_expert ${MAIN_SOURCE})
target_link_libraries(sys_expert PRIVATE sys_expert_lib)
target_include_directories(sys_expert PRIVATE src)

# Enable CTest and add unit tests
include(CTest)
enable_testing()

# Test executables
add_executable(test_proposition tests/test_proposition.c)
target_link_libraries(test_proposition PRIVATE sys_expert_lib)
target_include_directories(test_proposition PRIVATE src)
add_test(NAME test_proposition COMMAND test_proposition)

add_executable(test_list_proposition tests/test_list_proposition.c)
target_link_libraries(test_list_proposition PRIVATE sys_expert_lib)
target_include_directories(test_list_proposition PRIVATE src)
add_test(NAME test_list_proposition COMMAND test_list_proposition)

add_executable(test_regle tests/test_regle.c)
target_link_libraries(test_regle PRIVATE sys_expert_lib)
target_include_directories(test_regle PRIVATE src)
add_test(NAME test_regle COMMAND test_regle)

add_executable(test_bc tests/test_bc.c)
target_link_libraries(test_bc PRIVATE sys_expert_lib)
target_include_directories(test_bc PRIVATE src)
add_test(NAME test_bc COMMAND test_bc)

add_executable(test_inference tests/test_inference.c)
target_link_libraries(test_inference PRIVATE sys_expert_lib)
target_include_directories(test_inference PRIVATE src)
add_test(NAME test_inference COMMAND test_inference)
