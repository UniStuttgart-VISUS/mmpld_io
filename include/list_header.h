/*
 * Copyright 2019 Visualization Research Center (VISUS), University of Stuttgart
 * Alle Rechte vorbehalten
 */

#pragma once

#include <array>
#include <cinttypes>
#include <vector>

namespace mmpld {

enum class vertex_type : uint8_t {
    NONE = 0,
    FLOAT_XYZ = 1,
    FLOAT_XYZR = 2,
    SHORT_XYZ = 3,
    DOUBLE_XYZ = 4 //< only valid for version 1.3 and in combination with UINT16_RGBA or DOUBLE_I
};

static uint32_t vertex_size[] = {0, 12, 16, 6, 8};

enum class color_type : uint8_t {
    NONE = 0,
    UINT8_RGB = 1,
    UINT8_RGBA = 2,
    FLOAT_I = 3,
    FLOAT_RGB = 4,
    FLOAT_RGBA = 5,
    SHORT_RGBA = 6,
    DOUBLE_I = 7 //< only valid for version 1.3
};

static uint32_t color_size[] = {0, 3, 4, 4, 12, 16, 8, 8};

struct plain_list_header {
    uint8_t vert_type;
    uint8_t col_type;
    float global_radius;
    uint8_t global_color[4];
    float intensity_range[2];
    uint64_t particle_count;
    float lbox[6]; //< only for version 1.3 and later
};

struct list_header {
    list_header()
        : vert_type(vertex_type::NONE)
        , col_type(color_type::NONE)
        , global_radius(0.0f)
        , global_color()
        , intensity_range()
        , particle_count(0)
        , lbox() {}
    vertex_type vert_type;
    color_type col_type;
    float global_radius;
    std::array<unsigned char, 4> global_color;
    std::array<float, 2> intensity_range;
    uint64_t particle_count;
    std::array<float, 6> lbox; //< only for version 1.3 and later
};

using list_header_t = list_header;

struct list_entry {
    list_entry() : vertex_offset(0), vertex_stride(0), color_offset(0), color_stride(0) {}
    list_header_t list_header;
    unsigned int vertex_offset;
    unsigned int vertex_stride;
    unsigned int color_offset;
    unsigned int color_stride;
    std::vector<char> data;
};


/**
 * Returns whether the given vertex type contains radii in the data stream
 */
constexpr bool HasRadius(vertex_type const vt) noexcept { return vt == vertex_type::FLOAT_XYZR; }


/**
 * Returns whether the given color type allows for a color stream
 */
constexpr bool HasColor(color_type const ct) noexcept { return ct != color_type::NONE; }


/**
 * Returns whether the given color type is an intensity stream
 */
constexpr bool HasIntensity(color_type const ct) noexcept {
    return ct == color_type::FLOAT_I || ct == color_type::DOUBLE_I;
}


/**
 * Returns whether the current list entry contains any data at all
 */
constexpr bool HasData(vertex_type const vt) noexcept { return vt != vertex_type::NONE; }

} // end namespace mmpld
