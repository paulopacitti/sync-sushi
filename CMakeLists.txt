cmake_minimum_required (VERSION 2.8.11)
project(SyncSushi)

add_library (DrawSushi draw.c draw.h)

add_library (SyncSushi sync_sushi.c)

# adding pthreads flag and support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SyncSushi PRIVATE Threads::Threads)

target_link_libraries(SyncSushi LINK_PUBLIC m) # math library

target_include_directories(SyncSushi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(sync_sushi_exec sync_sushi.c)
target_link_libraries(sync_sushi_exec LINK_PUBLIC SyncSushi DrawSushi)

set(C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug) # debug mode