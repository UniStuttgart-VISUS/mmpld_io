cmake_minimum_required(VERSION 3.13...3.28)

project(mmpld_io
    LANGUAGES CXX)

# Library
file(GLOB_RECURSE header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.h")

add_library(mmpld_io INTERFACE)
add_library(mmpld_io::mmpld_io ALIAS mmpld_io)
target_sources(mmpld_io PRIVATE ${header_files})
include(GNUInstallDirs)
target_include_directories(mmpld_io INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mmpld_io>
)

# Install
install(FILES ${header_files} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mmpld_io")
install(TARGETS mmpld_io EXPORT mmpld_ioTargets)
install(EXPORT mmpld_ioTargets
    FILE mmpld_ioConfig.cmake
    NAMESPACE mmpld_io::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mmpld_io
)

# Test
option(BUILD_TEST "Build test exe" OFF)

if (BUILD_TEST)
    add_executable(mmpld_io_test
        test/mmpld_io_test.cpp
    )
    target_link_libraries(mmpld_io_test mmpld_io)
    set_property(TARGET mmpld_io_test PROPERTY CXX_STANDARD 17)
    install(TARGETS mmpld_io_test RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()
