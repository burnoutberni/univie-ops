#include <iostream>
#include "nelder_mead_optimizer.hpp"

int main() {
    struct : Funktion {
        double value(double x, double y) { return x*x + y*y; }
    } fn;

    nelder_mead_optimizer nmo(fn, {-1, -5}, {8, 8}, {3, -8}, .0000001);
    nmo.optimize();
    point min = nmo.best_point();

    std::cout << "The minimum is at " << min.format() << "!\nNeeded "
              << nmo.iteration_count() << " iterations.\n";
}
