#pragma once
#include <iostream>

#include "Funktion.h"
#include "point.hpp"

class coordesc_optimizer {
private:
    Funktion& f;
    point p; // current pint
    point q; // last point
    double eps;
    double const golden_ratio = (sqrt(5) - 1) / 2;

    bool is_done = false;

    // Goldener Schnitt
    double minimize_with_x_constant(double x) {
        // Schlechte Annahmen
        double a = -1000;
        double b = 1000;

        double c = b - golden_ratio * (b - a);
        double d = a + golden_ratio * (b - a);

        while(std::abs(c - d) > 0.000001) {
            if(f(x, c) < f(x, d)) {
                b = d;
                d = c;
                c = b - golden_ratio * (b - a);
            } else {
                a = c;
                c = d;
                d = a + golden_ratio * (b - a);
            }
        }
        return (b + a) / 2;
    }

    double minimize_with_y_constant(double y) {
        // Schlechte Annahmen
        double a = -10;
        double b = 10;

        double c = b - golden_ratio * (b - a);
        double d = a + golden_ratio * (b - a);

        while(std::abs(c - d) > 0.000001) {
            if(f(c, y) < f(d, y)) {
                b = d;
                d = c;
                c = b - golden_ratio * (b - a);
            } else {
                a = c;
                c = d;
                d = a + golden_ratio * (b - a);
            }
        }
        return (b + a) / 2;
    }

    void do_step() {
        q = p;
        p.x = minimize_with_y_constant(p.y);
        p.y = minimize_with_x_constant(p.x);
    }

public:
    coordesc_optimizer(Funktion& f, point const& p, double eps = 0.0001)
        : f( f ), p{ p }, eps{ eps } {}
    point retrieve_current_point() { return p; }
    point retrieve_last_point() { return q; }

    bool done() const { return is_done; }

    void step() {
        if(is_done) { return; }
        do_step();
        if(std::abs(p.x - q.x) <= eps && std::abs(p.y - q.y) <= eps) { is_done = true; }
    }

    void optimize() { while(!is_done) { step(); } }
};
