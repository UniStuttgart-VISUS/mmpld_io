/*
 * Copyright 2019 Visualization Research Center (VISUS), University of Stuttgart
 * Alle Rechte vorbehalten
 */

#pragma once

#include <array>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "file_header.h"
#include "frame_header.h"
#include "list_header.h"
#include "seek_table.h"


namespace mmpld {

/**
 * Class representing an MMPLD file.
 * Currently it supports only read operations.
 */
class mmpld {
public:
    /**
     * ctor
     * 
     * @param filename the full path to the MMPLD file
     * 
     * @throws logic_error if the file is ill-formed or if the seek table is corrupt
     */
    explicit mmpld(std::string const& filename) {
        file_.open(filename, std::ios::binary);

        // read file header and seek_table
        file_header_ = ReadFileHeader(file_);

        seek_table_ = ReadSeekTable(file_, file_header_.num_frames);
    }


    mmpld(mmpld const& rhs) = delete;


    mmpld& operator=(mmpld const& rhs) = delete;


    mmpld(mmpld&& rhs) = default;


    mmpld& operator=(mmpld&& rhs) = default;


    ~mmpld() { file_.close(); }


    /**
     * Returns the number of frames in the MMPLD file.
     */
    constexpr unsigned int GetFrameCount() const noexcept { return file_header_.num_frames; }


    /**
     * Returns the object space bounding box of the data.
     */
    std::array<float, 6> GetBBox() const noexcept { return file_header_.bbox; }


    /**
     * Reads the specified frame form the file
     * 
     * @param idx index of the frame to read
     * 
     * @throws out_of_range if the index is out of bounds
     * 
     * @returns the frame read from the file
     */
    frame_t ReadFrame(unsigned int const idx) {
        if (idx >= file_header_.num_frames) {
            throw std::out_of_range("Requested frame is out of range");
        }

        auto const offset = seek_table_[idx];
        auto const toread = seek_table_[idx + 1] - offset;

        file_.seekg(offset);

        std::vector<char> temp(toread);
        file_.read(temp.data(), toread);

        auto it = temp.begin();

        plain_frame_header pfh;

        /*if (IsVersion(file_header_, 1, 2)) {
            std::copy(it, it + 4, reinterpret_cast<char*>(&pfh.timestamp));
            it += 4;
        }*/

        std::copy(it, it + 4, reinterpret_cast<char*>(&pfh.num_particle_lists));
        it += 4;

        frame_t ret;
        ret.frame_id = idx;
        ret.timestamp = pfh.timestamp;
        ret.data.resize(pfh.num_particle_lists);

        for (unsigned int plidx = 0; plidx < pfh.num_particle_lists; ++plidx) {
            auto& entry = ret.data[plidx];

            entry.list_header.vert_type = static_cast<vertex_type>(*(it++));
            entry.list_header.col_type = static_cast<color_type>(*(it++));


            if (!HasRadius(entry.list_header.vert_type) && HasData(entry.list_header.vert_type)) {
                std::copy(it, it + 4, reinterpret_cast<char*>(&entry.list_header.global_radius));
                it += 4;
            }

            if (!HasColor(entry.list_header.col_type)) {
                std::copy(it, it + 4, entry.list_header.global_color.begin());
                it += 4;
            }

            if (HasIntensity(entry.list_header.col_type)) {
                std::copy(it, it + 8, reinterpret_cast<char*>(entry.list_header.intensity_range.data()));
                it += 8;
            }

            std::copy(it, it + 8, reinterpret_cast<char*>(&entry.list_header.particle_count));
            it += 8;

            if (HasData(entry.list_header.vert_type)) {

                if (IsVersion(file_header_, 1, 3)) {
                    std::copy(it, it + 24, reinterpret_cast<char*>(entry.list_header.lbox.data()));
                    it += 24;
                }

                entry.vertex_offset = 0;
                entry.vertex_stride = vertex_size[static_cast<unsigned char>(entry.list_header.vert_type)];

                entry.color_offset = entry.vertex_stride;
                entry.color_stride = color_size[static_cast<unsigned char>(entry.list_header.col_type)];

                auto const stride = entry.vertex_stride + entry.color_stride;
                auto const datasize = stride * entry.list_header.particle_count;
                entry.data.resize(datasize);
                std::copy(it, it + datasize, entry.data.begin());
                it += datasize;
            }
        }

        return ret;
    }

private:
    std::ifstream file_;
    file_header_t file_header_;
    seek_table_t seek_table_;
}; // end class mmpld

} // namespace mmpld
