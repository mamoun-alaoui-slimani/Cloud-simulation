#pragma once
#include <vector>
#include <array>

template <typename type> class Grid3D {
protected:
    // Attributes shared by Sky and PotentialField
    std::array<int, 3> cellCount;
    std::array<double, 3> lengths;
    double cellSize;
    std::vector<std::vector<std::vector<type>>> cells;

    /**
     * @brief Initialises the grid storage of a subclass.
     *
     * Protected so subclasses can use it while a bare Grid3D cannot be
     * instantiated on its own.
     *
     * @param Nx number of cells along x
     * @param Ny number of cells along y
     * @param Nz number of cells along z
     * @param lambda edge length of a cell
     */

    Grid3D(int Nx, int Ny, int Nz, double lambda)
        : cellCount({Nx, Ny, Nz}),
          lengths({lambda * (Nx - 1), lambda * (Ny - 1), lambda * (Nz - 1)}), cellSize(lambda) {
        /* Nested so that cells[i][j][k] has i along x, j along y, k along z:
           the innermost vector is the one indexed last. Building it from the
           inside out avoids an explicit loop. */
        std::vector<type> alongZ(cellCount[2]);
        std::vector<std::vector<type>> planeYZ(cellCount[1], alongZ);
        cells = std::vector<std::vector<std::vector<type>>>(cellCount[0], planeYZ);
    }

    Grid3D() {
        std::vector<type> vec1D(0);
        std::vector<std::vector<type>> vec2D(1, vec1D);
        std::vector<std::vector<std::vector<type>>> vec3D(1, vec2D);
        cells = vec3D;
    }
};