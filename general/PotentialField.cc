
#include <array>
#include <iostream>
#include <iomanip>
#include "PotentialField.h"

/**
 * @brief Builds a potential field of the given size.
 * Delegates the storage to Grid3D.
 *
 * @param Nx number of cells along x
 * @param Ny number of cells along y
 * @param Nz number of cells along z
 * @param lambda edge length of a cell
 */
PotentialField::PotentialField(int Nx, int Ny, int Nz, double lambda)
    : Grid3D<Potential>::Grid3D(Nx, Ny, Nz, lambda) {}

/**
 * @brief Initialises the field and its boundary conditions.
 * Potentials are left at (0,0) below the terrain, which is how those
 * cells are later recognised and skipped. Laplacians all start at (0,0).
 *
 * @param v velocity at the boundary
 * @param m terrain that shapes the field
 */
void PotentialField::initialize(double v, const Mountain& m) {
    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {
                if (k >= m.altitude(i, j) or i == 0 or j == 0 or i == cellCount[0] - 1 or
                    j == cellCount[1] - 1) {
                    cells[i][j][k] = Potential(Vector2D(-v * (k * cellSize) / 2,
                                                        v * (j * cellSize - (lengths[1] / 2)) / 2),
                                               Vector2D());
                } else {
                    cells[i][j][k] = Potential();
                    // already (0,0) from the default constructor; kept so an
                }
            }
        }
    }
}

/**
 * @brief Computes the Laplacian of every interior cell.
 *
 * Cells below the terrain are skipped.
 */
void PotentialField::computeLaplacians() {
    for (int i(1); i < cellCount[0] - 1; ++i) {
        for (int j(1); j < cellCount[1] - 1; ++j) {
            for (int k(1); k < cellCount[2] - 1; ++k) {
                // a zero potential marks a cell below the terrain
                if (not cells[i][j][k].isZero()) {
                    cells[i][j][k].computeLaplacian(cells[i - 1][j][k], cells[i][j - 1][k],
                                                    cells[i][j][k - 1], cells[i + 1][j][k],
                                                    cells[i][j + 1][k], cells[i][j][k + 1]);
                }
            }
        }
    }
}

/**
 * @brief Prints every potential, as: i j k P1 P2
 * i j k P1 P2
 */
void PotentialField::printPotentials() const {
    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {
                std::cout << i << " " << j << " " << k << " ";
                cells[i][j][k].printPotential();
                std::cout << std::endl;
            }
        }
    }
}

/**
 * @brief Prints every Laplacian, as: i j k L2 L3
 *  i j k L2 L3
 */
void PotentialField::printLaplacians() const {
    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {
                std::cout << i << " " << j << " " << k << " ";
                cells[i][j][k].printLaplacian();
                std::cout << " " << std::endl;
            }
        }
    }
}
/**
 * @brief Residual used to decide whether the solve has converged.
 * @return the sum of the squared norms of every Laplacian
 */
double PotentialField::residual() const {
    double squaredNorm(0.0);
    for (int i(0); i < cellCount[0]; i++) {
        for (int j(0); j < cellCount[1]; j++) {
            for (int k(0); k < cellCount[2]; k++) {
                squaredNorm += cells[i][j][k].laplacianSquaredNorm();
            }
        }
    }
    return squaredNorm;
}

/**
 * @brief Runs one relaxation step over every interior cell.
 */
void PotentialField::iterate(const double eps) {
    for (int i(1); i < cellCount[0] - 1; i++) {
        for (int j(1); j < cellCount[1] - 1; j++) {
            for (int k(1); k < cellCount[2] - 1; k++) {
                cells[i][j][k].iterate(eps);
            }
        }
    }
}

/**
 * @brief Solves Laplace's equation by relaxation, until the residual
 *
 * @param threshold residual below which the field is considered converged
 * @param maxIterations cap on the iteration count, to bound the loop
 * @param print when true, prints the field at every iteration
 */
void PotentialField::solve(double threshold, int maxIterations, bool print) {
    int iterationCount(1);
    double residual(this->residual());
    while ((threshold < residual) and (iterationCount <= maxIterations)) {
        this->iterate();
        this->computeLaplacians();
        if (print) {
            this->printPotentials(); // Si verbeuse
            this->printLaplacians();
        }
        residual = this->residual(); // recompute the residual from the updated potentials
        iterationCount++;
    }
}

/**
 * @brief Wind velocity at (i, j, k), from the surrounding potentials.
 * @param i
 * @param j
 * @param k
 *
 * @return the three velocity components vx, vy, vz
 */
std::array<double, 3> PotentialField::velocity(int i, int j, int k) const {
    std::array<double, 3> velocity({0, 0, 0}); // Array de 3 double nul

    if (i == 0 or j == 0 or k == 0 or i == cellCount[0] - 1 or j == cellCount[1] - 1 or
        k == cellCount[2] - 1)
        return velocity;

    cells[i][j][k].velocity(velocity, cells[i - 1][j][k], cells[i][j - 1][k], cells[i][j][k - 1],
                            cells[i + 1][j][k], cells[i][j + 1][k], cells[i][j][k + 1]);

    for (size_t i(0); i < velocity.size(); ++i) {
        velocity[i] *= (1 / (2 * cellSize));
    }
    return velocity;
}

/**
 * @brief Whether the potential at (i, j, k) is zero, i.e. below terrain.
 *
 * Indices outside the box count as zero rather than being indexed.
 * d'indexer hors bornes.
 */
bool PotentialField::isPotentialZero(int i, int j, int k) const {
    if (0 <= i and i < cellCount[0] and 0 <= j and j < cellCount[1] and 0 <= k and
        k < cellCount[2]) {
        return cells[i][j][k].isZero();
    }
    return true;
}

/** @brief Prints, for every cell: i j k P1 P2 vx vy vz v2
 * i j k P1 P2 vx vy vz v2
 */
void PotentialField::printVelocities() const {
    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {
                if (not cells[i][j][k].isZero()) {

                    std::cout << i << " " << j << " " << k << " ";
                    cells[i][j][k].printPotential();

                    std::array<double, 3> vit(this->velocity(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    std::cout << std::setprecision(6) << " " << vit[0] << " " << vit[1] << " "
                              << vit[2] << " " << V2 << " " << std::endl;
                }
            }
        }
    }
}

/**
 * @brief Prints every velocity on a stream.
 *
 * @param out output stream
 * @return the stream
 */
std::ostream& PotentialField::printVelocities(std::ostream& out) const {
    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {
                if (not cells[i][j][k].isZero()) {

                    out << i << " " << j << " " << k << " ";
                    cells[i][j][k].printPotential(out);

                    std::array<double, 3> vit(this->velocity(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    out << std::setprecision(6) << " " << vit[0] << " " << vit[1] << " " << vit[2]
                        << " " << V2 << " " << std::endl;
                }
            }
        }
    }
    return out;
}

/**
 * @brief Stream insertion operator.
 *
 * @param out output stream
 * @param field field to print
 * @return the stream
 */
std::ostream& operator<<(std::ostream& out, PotentialField const& field) {
    out << "The system holds a potential field: " << std::endl;
    return field.printVelocities(out);
}

/**
 * @brief The edge length of a cell.
 */
double PotentialField::getCellSize() const {
    return cellSize;
}

/**
 * @brief The number of cells along each axis.
 */
std::array<int, 3> PotentialField::getCellCount() const {
    return cellCount;
}
