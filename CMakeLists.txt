cmake_minimum_required(VERSION 3.10)
project(LO21_sys_expert)

set(CMAKE_C_STANDARD 99)

add_executable(sys_expert src/main.c)

# Custom target 'run'
add_custom_target(run
    COMMAND $<TARGET_FILE:sys_expert>
    DEPENDS sys_expert
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running the application..."
)
