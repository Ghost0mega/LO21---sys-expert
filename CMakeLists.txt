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

add_executable(sys_expert ${SYS_EXPERT_SOURCES})

# Include headers from src/ for all targets
target_include_directories(sys_expert PRIVATE src)

# Common warnings for GCC/Clang
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sys_expert PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Ncurses for TUI (prefer wide-character library for UTF-8)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)
if (CURSES_FOUND)
    target_include_directories(sys_expert PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(sys_expert PRIVATE ${CURSES_LIBRARIES})
else()
    message(WARNING "Curses not found; TUI may fail to link. Install ncurses and reconfigure.")
endif()
