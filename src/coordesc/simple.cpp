#include <iostream>
#include "coordesc_optimizer.hpp"

int main() {
    struct : Funktion {
        double value(double x, double y) { return x*x + y*y; }
    } fn;

    coordesc_optimizer cdo(fn, {-1, -5}, .0000001);
    cdo.optimize();
    point min = cdo.current_point();

    std::cout << "The minimum is at " << min.format() << "!\nNeeded "
              << cdo.iteration_count() << " iterations.\n";
}
