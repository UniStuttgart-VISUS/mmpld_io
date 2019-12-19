/*
 * Copyright 2019 Visualization Research Center (VISUS), University of Stuttgart
 * Alle Rechte vorbehalten
 */

#pragma once

#include <array>
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace mmpld {

struct plain_file_header {
    char magic[6];
    uint16_t version;
    uint32_t num_frames;
    float bbox[6];
    float cbox[6];
};

struct version {
    version() : major_(0), minor_(0) {}
    uint8_t major_;
    uint8_t minor_;
};

using version_t = version;

struct file_header {
    file_header() : num_frames(0), bbox(), cbox() {}
    version_t version;
    uint32_t num_frames;
    std::array<float, 6> bbox;
    std::array<float, 6> cbox;
};

using file_header_t = file_header;


/**
 * Helper function to convert version format
 */
inline version_t ConvertVersion(unsigned short const version) noexcept {
    version_t ret;
    ret.major_ = version / 100;
    ret.minor_ = version % 100;
    return ret;
}


/**
 * Helper function to convert a plain_file_header to a file_header_t
 */
inline file_header_t ConvertFileHeader(plain_file_header const& pfh) {
    file_header_t ret;

    ret.version = ConvertVersion(pfh.version);

    ret.num_frames = pfh.num_frames;
    std::copy(std::begin(pfh.bbox), std::end(pfh.bbox), ret.bbox.begin());
    std::copy(std::begin(pfh.cbox), std::end(pfh.cbox), ret.cbox.begin());

    return ret;
}


/**
 * Reads the header of the given file.
 *
 * @param file the mmpld file to read
 *
 * @return the file header read from the MMPLD file
 *
 * @throws logic_error if the file is not in MMPLD format
 */
inline file_header_t ReadFileHeader(std::ifstream& file) {
    plain_file_header pfh;
    file.read(reinterpret_cast<char*>(&pfh), sizeof(pfh));

    if (std::strcmp(pfh.magic, "MMPLD") != 0) {
        throw std::logic_error("Bad MMPLD Header");
    }

    return ConvertFileHeader(pfh);
}


/**
 * Checks if the file has at least the requested version
 */
constexpr bool IsVersion(file_header_t const& fh, unsigned short const major, unsigned short const minor) noexcept {
    return (fh.version.major_ == major && fh.version.minor_ >= minor) || fh.version.major_ > major;
}

} // end namespace mmpld
