#include <iostream>

#include "mmpld.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments" << std::endl;
        return -1;
    }

    std::string filename(argv[1]);

    mmpld::mmpld file(filename);

    auto frameCount = file.GetFrameCount();

    std::cout << "FrameCount " << frameCount << std::endl;

    for (unsigned int fidx = 0; fidx < frameCount; ++fidx) {
        auto frame = file.ReadFrame(fidx);

        auto pl_count = frame.data.size();

        std::cout << "ParticleListCount " << pl_count << std::endl;

        for (size_t plidx = 0; plidx < pl_count; ++plidx) {
            auto const& entry = frame.data[plidx];

            std::cout << "ParticleCount " << entry.list_header.particle_count << std::endl;
            std::cout << "VertexType " << static_cast<unsigned char>(entry.list_header.vert_type) << std::endl;
            std::cout << "ColorType " << static_cast<unsigned char>(entry.list_header.col_type) << std::endl;
        }
    }

    return 0;
}