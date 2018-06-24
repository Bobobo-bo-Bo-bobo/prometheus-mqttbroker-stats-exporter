cmake_minimum_required(VERSION 2.6)
project(badphish-prometheus-mqttbroker-stats-exporter)
set(PROMETHEUS_MQTTBROKER_STATS_EXPORTER_VERSION "1.0.1")
# set(CMAKE_BUILD_TYPE Debug)

include (CheckFunctionExists)
include (FindPkgConfig)

check_function_exists(memset HAVE_MEMSET)

# check for libmosquitto
find_library(LIBMOSQUITTO mosquitto)
if (LIBMOSQUITTO-NOTFOUND)
    message(FATAL_ERROR "Required library libmosquitto not found")
endif(LIBMOSQUITTO-NOTFOUND)

# check for uuid
pkg_search_module(LIBUUID REQUIRED uuid)
include_directories(SYSTEM ${LIBUUID_INCLUDE_DIRS})
link_directories(${LIBUUID_LIBRARY_DIRS})

# check for microhttpd
pkg_search_module(LIBMICROHTTPD REQUIRED libmicrohttpd)
include_directories(SYSTEM ${LIBMICROHTTPD_INCLUDE_DIRS})
link_directories(${LIBMICROHTTPD_LIBRARY_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)

find_package(Threads REQUIRED)

add_library(mqtt_connect mqtt_connect.c)
add_library(usage usage.c)
add_library(exporter exporter.c)
add_library(util util.c)

configure_file("${PROJECT_SOURCE_DIR}/prometheus-mqttbroker-stats-exporter.h.in" "${PROJECT_SOURCE_DIR}/prometheus-mqttbroker-stats-exporter.h")

add_executable(prometheus-mqttbroker-stats-exporter main.c)
target_link_libraries(prometheus-mqttbroker-stats-exporter usage)
target_link_libraries(prometheus-mqttbroker-stats-exporter mqtt_connect)
target_link_libraries(prometheus-mqttbroker-stats-exporter util)
target_link_libraries(prometheus-mqttbroker-stats-exporter exporter)
target_link_libraries(prometheus-mqttbroker-stats-exporter "-lmosquitto")
target_link_libraries(prometheus-mqttbroker-stats-exporter ${LIBUUID_LIBRARIES})
target_link_libraries(prometheus-mqttbroker-stats-exporter ${LIBMICROHTTPD_LIBRARIES})
target_link_libraries(prometheus-mqttbroker-stats-exporter ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS prometheus-mqttbroker-stats-exporter DESTINATION sbin)

