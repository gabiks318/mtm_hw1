cmake_minimum_required(VERSION 3.0.0)

project(hw1 VERSION 0.1.0 LANGUAGES C)

set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

add_subdirectory("${PROJECT_SOURCE_DIR}/tests")


link_directories(.)
add_executable(event_manager tests/event_manager_example_tests.c event_manager.c event.c date.c member_list.c member.c)
target_link_libraries(event_manager libpriority_queue.a)
