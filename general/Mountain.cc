#include "Mountain.h"



Mountain::Mountain(double i0, double j0, double h, double sx, double sy)
    : i0(i0), j0(j0), height(h), sigmaX(sx), sigmaY(sy)
{ }

std::ostream& Mountain::print(std::ostream& out) const {
    out << "i0: " << i0 << ", j0: " << j0 << ", height: " << height << ", sigmaX: " << sigmaX << ", sigmaY: " << sigmaY << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, Mountain const& m){
    return m.print(out);
}