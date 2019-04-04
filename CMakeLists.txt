project(mmpld_io)

file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")

add_library(mmpld_io INTERFACE)
target_include_directories(mmpld_io INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
install(FILES ${header_files} DESTINATION "${CMAKE_INSTALL_PREFIX}/include")


option(BUILD_TEST "Build test exe" OFF)

if (BUILD_TEST)
    add_executable(mmpld_io_test
        test/mmpld_io_test.cpp
    )
    target_link_libraries(mmpld_io_test mmpld_io)
    install(TARGETS mmpld_io_test RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()
