/*
 * Copyright 2019 Visualization Research Center (VISUS), University of Stuttgart
 * Alle Rechte vorbehalten
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace mmpld {

using seek_table_t = std::vector<uint64_t>;

/**
 * Reads the seek table from the given file, assuming that the file pointer is at the correct position.
 * 
 * @throws logic_error if a table value is below 60 (seek table is corrupt).
 * 
 * @returns the seek table with num_frames+1 entries. The last entry should be EOF position
 */
inline seek_table_t ReadSeekTable(std::ifstream& file, unsigned int const num_frames) {
    seek_table_t ret(num_frames + 1);

    file.read(reinterpret_cast<char*>(ret.data()), (num_frames + 1) * sizeof(seek_table_t::value_type));

    auto const check = std::all_of(ret.begin(), ret.end(), [](auto const& val) { return val >= 60; });

    // TODO Check also file size
    if (!check) {
        throw std::logic_error("Corrupt seek table");
    }

    return ret;
}

} // end namespace mmpld