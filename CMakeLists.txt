cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(test VERSION 0.0.1 LANGUAGES C CXX)

find_package(OpenSSL)

# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

include_directories(
	"${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/thirdparty/cpp-httplib"
  ${OPENSSL_INCLUDE_DIR}
	 "."
)

add_executable(login login.cpp)
target_sources(login
  PRIVATE
  "util/crypto.cpp"
)
target_link_libraries(login z ${OPENSSL_LIBRARIES} )
