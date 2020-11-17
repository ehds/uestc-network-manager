cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(uestc_login VERSION 0.0.1 LANGUAGES C CXX)

find_package(OpenSSL)

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

add_executable(${PROJECT_NAME} login.cpp)
target_sources(${PROJECT_NAME}
  PRIVATE
  "util/crypto.cpp"
)
target_link_libraries(${PROJECT_NAME} z ${OPENSSL_LIBRARIES} )

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