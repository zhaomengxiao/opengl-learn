#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace STLReader
{
    struct MeshData
    {
        std::vector<float> vertices; // Position (3) + Normal (3)
        std::vector<unsigned int> indices;
    };

    // Function to read a binary STL file
    MeshData ReadBinarySTL(const std::string &filepath);
}
