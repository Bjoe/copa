project(CodeCompletion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost 1.55 REQUIRED COMPONENTS program_options filesystem)
find_package(Clang 3.9 REQUIRED COMPONENTS
  clang
  clangFrontend
  clangDriver
  clangTooling
  clangParse
  clangSema
  clangAnalysis
  clangSerialization
  clangRewriteFrontend
  clangRewrite
  clangEdit
  clangAST
  clangASTMatchers
  clangLex
  clangBasic
  LLVM
)

#set(CLANG_STDCPP libc++.so.1.0)
#find_library(CLANG_STDCPP_LIBRARIES ${CLANG_STDCPP})
#find_path(CLANG_STDCPP_INCLUDE_DIRS string HINTS /usr/include/c++/v1)

message(STATUS "LLVM_CPPLAGS: ${LLVM_CPPLAGS}")
message(STATUS "LLVM_LDFLAGS: ${LLVM_LDFLAGS}")
message(STATUS "Found libclang: ${Clang_LIBRARIES}")
message(STATUS "Found clang headers: ${Clang_INCLUDE_DIRS}")
#message(STATUS "Found clang std ${CLANG_STDCPP}: ${CLANG_STDCPP_LIBRARIES}")
#message(STATUS "Found clang std headers: ${CLANG_STDCPP_INCLUDE_DIRS}")

include_directories(
  .
  ${Boost_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
  #${CLANG_STDCPP_INCLUDE_DIRS}
)


set(SOURCE
  main.cpp
  cxxclass.cpp
  cxxmethod.cpp
)


#add_definitions(${LLVM_CPPLAGS})# -stdlib=libc++)
add_definitions(-std=c++11)

add_executable(coco ${SOURCE})
target_link_libraries(coco ${Clang_LIBRARIES} ${Boost_LIBRARIES} ${LLVM_LDFLAGS} stdc++)# ${CLANG_STDCPP_LIBRARIES})
