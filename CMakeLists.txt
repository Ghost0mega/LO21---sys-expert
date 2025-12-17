cmake_minimum_required(VERSION 3.16)

project(sys_expert C)

# Use C11 and strict warnings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for tooling (already appears in build/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all C sources under src/ (recursively)
file(GLOB_RECURSE SYS_EXPERT_SOURCES CONFIGURE_DEPENDS
    src/*.c
)

# Ncurses for TUI (prefer wide-character library for UTF-8)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)

# If curses is not found, exclude ui.c from sources so build succeeds
set(UI_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/ui.c")
if (NOT CURSES_FOUND)
    list(REMOVE_ITEM SYS_EXPERT_SOURCES "${UI_SOURCE}")
    message(STATUS "Curses not found; building without TUI. Use -t/--text-only to run.")
endif()

add_executable(sys_expert ${SYS_EXPERT_SOURCES})

# Include headers from src/ for all targets
target_include_directories(sys_expert PRIVATE src)

# Common warnings for GCC/Clang
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sys_expert PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (CURSES_FOUND)
    target_include_directories(sys_expert PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(sys_expert PRIVATE ${CURSES_LIBRARIES})
    target_compile_definitions(sys_expert PRIVATE HAVE_CURSES)
endif()
