#include "STLReader.h"
#include <fstream>
#include <iostream>
#include <cstdint> // For uint8_t, uint32_t

namespace STLReader
{
    // Helper to read binary data
    template <typename T>
    void read_binary(std::istream &is, T &val)
    {
        is.read(reinterpret_cast<char *>(&val), sizeof(T));
    }

    // Helper to read a float from binary stream
    float read_float(std::istream &is)
    {
        float val;
        read_binary(is, val);
        return val;
    }

    // Helper to skip bytes
    void skip_bytes(std::istream &is, size_t num_bytes)
    {
        is.seekg(num_bytes, std::ios_base::cur);
    }

    MeshData ReadBinarySTL(const std::string &filepath)
    {
        MeshData meshData;
        std::ifstream file(filepath, std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error: Could not open STL file: " << filepath << std::endl;
            return meshData; // Return empty MeshData
        }

        // Skip header (80 bytes)
        skip_bytes(file, 80);

        uint32_t num_triangles;
        read_binary(file, num_triangles);

        meshData.vertices.reserve(num_triangles * 3 * 6); // 3 vertices per triangle, 6 floats per vertex (pos + normal)
        meshData.indices.reserve(num_triangles * 3);      // 3 indices per triangle

        for (uint32_t i = 0; i < num_triangles; ++i)
        {
            float normal[3];
            normal[0] = read_float(file);
            normal[1] = read_float(file);
            normal[2] = read_float(file);

            for (int j = 0; j < 3; ++j) // 3 vertices per triangle
            {
                float position[3];
                position[0] = read_float(file);
                position[1] = read_float(file);
                position[2] = read_float(file);

                // Add position
                meshData.vertices.push_back(position[0]);
                meshData.vertices.push_back(position[1]);
                meshData.vertices.push_back(position[2]);

                // Add normal
                meshData.vertices.push_back(normal[0]);
                meshData.vertices.push_back(normal[1]);
                meshData.vertices.push_back(normal[2]);

                // Add index (sequential for non-indexed drawing, or if we duplicate vertices)
                meshData.indices.push_back(meshData.indices.size()); // Each vertex gets a unique index
            }
            skip_bytes(file, 2); // Skip attribute byte count
        }

        file.close();
        std::cout << "Loaded binary STL: " << filepath << " with " << num_triangles << " triangles." << std::endl;
        return meshData;
    }
}
