if(NOT EXISTS $ENV{GOPATH})
  set(GOPATH $ENV{GOPATH})  
elseif(EXISTS ${CMAKE_GO_PATH})
  set(GOPATH ${CMAKE_GO_PATH})
else()
  set(GOPATH "${CMAKE_CURRENT_BINARY_DIR}/go")
endif()

file(MAKE_DIRECTORY ${GOPATH})

function(ExternalGoProject_Add TARG)
  add_custom_target(${TARG} env GOPATH=${GOPATH} ${CMAKE_Go_COMPILER} get ${ARGN})
endfunction(ExternalGoProject_Add)

function(add_go_executable NAME GO_SOURCES) 
  add_custom_command(OUTPUT ${OUTPUT_DIR}/.timestamp 
    COMMAND env GOPATH=${GOPATH} ${CMAKE_Go_COMPILER} build
    -o "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}"
    ${CMAKE_GO_FLAGS} ${GO_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
  
  add_custom_target(${NAME} ALL DEPENDS ${OUTPUT_DIR}/.timestamp ${ARGN})
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${NAME} DESTINATION bin)
endfunction(add_go_executable)


function(ADD_GO_LIBRARY NAME BUILD_TYPE)
  if(BUILD_TYPE STREQUAL "STATIC")
    set(BUILD_MODE -buildmode=c-archive)
    set(LIB_NAME "lib${NAME}.a")
  else()
    set(BUILD_MODE -buildmode=c-shared)
    if(APPLE)
      set(LIB_NAME "lib${NAME}.dylib")
    else()
      set(LIB_NAME "lib${NAME}.so")
    endif()
  endif()

  file(GLOB GO_SOURCE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.go")
  add_custom_command(OUTPUT ${OUTPUT_DIR}/.timestamp
    COMMAND env GOPATH=${GOPATH} ${CMAKE_Go_COMPILER} build ${BUILD_MODE}
    -o "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}"
    ${CMAKE_GO_FLAGS} ${GO_SOURCE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

  add_custom_target(${NAME} ALL DEPENDS ${OUTPUT_DIR}/.timestamp ${ARGN})

  if(NOT BUILD_TYPE STREQUAL "STATIC")
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME} DESTINATION bin)
  endif()
endfunction(ADD_GO_LIBRARY)
