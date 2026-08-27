#include "GaussianPeak.h"
#include "Mountain.h"
#include "Renderer.h"
#include <cmath>

GaussianPeak::GaussianPeak(double i0, double j0, double h, double sx, double sy)
    : Mountain(i0, j0, h, sx, sy) {}

double GaussianPeak::altitude(double i, double j) const {
    double diff_i = ((-(i - i0) * (i - i0)) / (2.0 * sigmaX * sigmaX));
    double diff_j = ((-(j - j0) * (j - j0)) / (2.0 * sigmaY * sigmaY));
    double H = (height * std::exp(diff_i + diff_j));
    if (H < 0.5)
        H = 0.0;
    return H;
}

void GaussianPeak::drawOn(Renderer& renderer) {
    renderer.draw(*this);
}

std::ostream& GaussianPeak::print(std::ostream& out) const {
    out << "A Gaussian peak with parameters: " << std::endl;
    return Mountain::print(out);
}