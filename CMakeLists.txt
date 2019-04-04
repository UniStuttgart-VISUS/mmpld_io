project(mmpld_io)

add_executable(mmpld_io_test
    src/mmpld_io_test.cpp
    src/file_header.h
    src/seek_table.h
    src/frame_header.h
    src/list_header.h
    src/mmpld.h
)