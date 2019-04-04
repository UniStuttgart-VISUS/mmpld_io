/*
 * Copyright 2019 Visualization Research Center (VISUS), University of Stuttgart
 * Alle Rechte vorbehalten
 */

#pragma once

#include <vector>

#include "list_header.h"

namespace mmpld {

struct plain_frame_header {
    float timestamp; //< only present in version 1.2
    unsigned int num_particle_lists;
};

struct frame {
    frame() : frame_id(0), timestamp(0.0f) {}
    unsigned int frame_id;
    float timestamp;
    std::vector<list_entry> data;
};

using frame_t = frame;

} // end namespace mmpld
