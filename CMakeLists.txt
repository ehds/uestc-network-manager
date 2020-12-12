cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(uestcnetwork VERSION 0.0.1 LANGUAGES C CXX)


option(UESTC_NETWORKMANAGER_TESTS "Build unit test" ON)
if (APPLE)
    # This is a bug in CMake that causes it to prefer the system version over
    # the one in the specified ROOT folder.
    set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} /usr/local/Cellar/openssl@1.1/1.1.1g/)
    set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)
    set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)
endif()

find_package(OpenSSL REQUIRED)

# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

include_directories(
  "."
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/thirdparty/cpp-httplib"
  ${OPENSSL_INCLUDE_DIR}
)
add_library("uestc_util" "")
target_sources("uestc_util"
  PRIVATE
  "util/crypto.cpp"
  "util/config.cpp"
)
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
  "login.cpp"
)
target_link_libraries(uestc_util ${OPENSSL_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} uestc_util)

if (UESTC_NETWORKMANAGER_TESTS)
  enable_testing()
  add_subdirectory("thirdparty/googletest")
  add_subdirectory("test")
endif(UESTC_NETWORKMANAGER_TESTS)
if (UNIX)
  set(SYSTEMD_SERVICE_TYPE_DOCSTRING "The value used as the Type= in the systemd unit file.")
  set(WATCHDOG_SEC_DOCSTRING "The value used as WatchdogSec= in the systemd unit file.")
  set(WATCHDOG_SEC "30" CACHE STRING ${WATCHDOG_SEC_DOCSTRING})
  if(SYSTEMD_FOUND)
    set(SYSTEMD_SERVICE_TYPE "notify" CACHE STRING ${SYSTEMD_SERVICE_TYPE_DOCSTRING})
  else()
    set(SYSTEMD_SERVICE_TYPE "simple" CACHE STRING ${SYSTEMD_SERVICE_TYPE_DOCSTRING})
  endif()
  set(SERVICE_USER_DOCSTRING "The value used as the User= in the systemd unit file.")
  if(NOT DEFINED SERVICE_USER)
    set(SERVICE_USER "nobody" CACHE STRING ${SERVICE_USER_DOCSTRING})
  endif()
  set(SERVICE_GROUP_DOCSTRING "The value used as the Group= in the systemd unit file.")
  if(NOT DEFINED SERVICE_GROUP)
    set(SERVICE_GROUP "nobody" CACHE STRING ${SERVICE_GROUP_DOCSTRING})
  endif()

  configure_file(unit/uestcnetwork.service.cmake uestcnetwork.service)

  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
  install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/uestcnetwork.service DESTINATION lib/systemd/system     COMPONENT init)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/user.conf DESTINATION /etc/uestc_networkmanager COMPONENT configuration)
endif (UNIX)