# Specify the C++ standard
add_library(arpwatch_compiler_flags INTERFACE)
target_compile_features(arpwatch_compiler_flags INTERFACE cxx_std_17)

# Add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  arpwatch_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-O2;-g>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-O2>>"
)

# Find the libcurl package
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include)

# Add files to build here
set(src_files
  arp_checks.cpp  
  db_interface.cpp
  send_mail.cpp
  syslog_interface.cpp
)

# Add the executable
add_executable(
  arpwatch 
  arpwatch.cpp
  ${src_files} 
)

# Link the compiler flags to the executable
target_link_libraries(
  arpwatch 
  PUBLIC arpwatch_compiler_flags
  PRIVATE ${PQXX_LINK_LIBRARIES} PostgreSQL::PostgreSQL
)

# Add the binary tree to the search path for include files
target_include_directories(
  arpwatch 
  PUBLIC ${CMAKE_SOURCE_DIR}/include
)

# Install arpwatch in the bin directory
install(
  TARGETS arpwatch 
  DESTINATION "${PROJECT_BINARY_DIR}"
)
