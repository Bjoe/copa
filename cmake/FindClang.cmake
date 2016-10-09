include(FindPackageHandleStandardArgs)

set(FIND_HINTS /usr/lib/llvm-${Clang_FIND_VERSION})
if(NOT Foo_FIND_QUIETLY)
  message(STATUS "Search clang in: ${FIND_HINTS}")
endif()

find_path(Clang_INCLUDE_DIR
  clang-c/Index.h
  HINTS ${FIND_HINTS}
  PATH_SUFFIXES include)

set(Clang_LIBS "")
foreach(COMPONENT ${Clang_FIND_COMPONENTS})
  find_library(Clang_LIBRARY_${COMPONENT}
    lib${COMPONENT}.a lib${COMPONENT} ${COMPONENT}
    HINTS ${FIND_HINTS}
    PATH_SUFFIXES lib
    )
  if(NOT Foo_FIND_QUIETLY)
    message(STATUS "Clang component ${COMPONENT}: ${Clang_LIBRARY_${COMPONENT}}")
  endif()
  list(APPEND Clang_LIBS ${Clang_LIBRARY_${COMPONENT}})
endforeach()

#find_package_handle_standard_args(Clang
#  REQUIRED_VARS Clang_LIBS Clang_INCLUDE_DIR
#  HANDLE_COMPONENTS
#  )

find_program(LLVM_CONFIG_EXECUTABLE llvm-config-${Clang_FIND_VERSION})

execute_process(COMMAND ${LLVM_CONFIG_EXECUTABLE} --system-libs OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
execute_process(COMMAND ${LLVM_CONFIG_EXECUTABLE} --cxxflags OUTPUT_VARIABLE LLVM_CPPLAGS OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

#if(Clang_FOUND)
  set(Clang_LIBRARIES ${Clang_LIBS})
  set(Clang_INCLUDE_DIRS ${Clang_INCLUDE_DIR})
  #set(Clang_DEFINITIONS ${})
#endif()

#mark_as_advanced(LLVM_LDFLAGS2)
