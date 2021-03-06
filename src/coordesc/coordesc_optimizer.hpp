/****************************************************************************
 * Koordinatenabstiegs Optimierungsalgorithmus.                             *
 * Autor: Sonja Biedermann, a1402891 (Gruppe 4)                             *
 *                                                                          *
 * Minimiert eine gegebene Funktion f.                                      *
 * Zerlegt dabei das eigentliche Problem (Minimierung einer                 *
 * zweidimensionalen Funktion) in zwei Teilprobleme (Minimierung            *
 * einer eindimensionalen Funktion), indem es jeweils eine Variable         *
 * als konstant annimmt, und die andere mittels Golden Section Search       *
 * minimiert.                                                               *
 *                                                                          *
 * Als Referenzen wurde die Praktikumsbeschreibung und folgendes verwendet: *
 * https://en.wikipedia.org/wiki/Golden_section_search                      *
 ****************************************************************************/

#pragma once
#include <iostream>
#include <cmath>

#include "Funktion.h"
#include "point.hpp"

struct invalid_value: public std::exception {
    char const* err = "invalid value";
    invalid_value(char const* e) : err{ e } {}
    char const* what() const noexcept { return err; }
};

class coordesc_optimizer {
private:
    Funktion& f;
    point p; // current pint
    point q; // last point
    double eps;
    double const golden_ratio = (sqrt(5) - 1) / 2;

    // Spannt den Bereich auf, der bei der Minimierung betrachtet wird.
    double lo_x = -10;
    double hi_x = 10;
    double lo_y = -10;
    double hi_y = 10;

    bool is_done = false; // Zustand: fertig optimiert?
    bool x_last = false;  // Zustand: wurde x als letzte Variable minimiert?
    size_t iter_c = 0;

    double minimize_with_y_constant(double y) const {
        double a = lo_x;
        double b = hi_x;

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

    double minimize_with_x_constant(double x) const {
        double a = lo_y;
        double b = hi_y;

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

    void do_halfstep() {
        q = p;
        if(x_last) { p.y = minimize_with_x_constant(p.x); }
        else { p.x = minimize_with_y_constant(p.y); }
        x_last = !x_last;
    }

public:
    coordesc_optimizer(Funktion& f, point const& p, double eps = 0.0001)
        : f( f ), p{ p }, q{ p }, eps{ eps } {}
    point current_point() const { return p; }
    point last_point() const { return q; }

    bool done() const { return is_done; }
    size_t iteration_count() const { return iter_c; }

    double lower_bound_x() const { return lo_x; }
    double upper_bound_x() const { return hi_x; }
    double lower_bound_y() const { return lo_y; }
    double upper_bound_y() const { return hi_y; }

    void set_lower_bound_x(double x) {
        if(!(x < hi_x)) {
            throw invalid_value("new lower bound must satisfy lower < upper. "
                                "set upper bound first.");
        }
        lo_x = x;
    }

    void set_upper_bound_x(double x) {
        if(!(x > lo_x)) {
            throw invalid_value("new upper bound must satisfy upper > lower. "
                                "set lower bound first.");
        }
        hi_x = x;
    }

    void set_lower_bound_y(double y) {
        if(!(y < hi_y)) {
            throw invalid_value("new lower bound must satistfy lower < upper. "
                                "set upper bound first.");
        }
        hi_y = y;
    }

    void set_upper_bound_x(double y) {
        if(!(y > lo_y)) {
            throw invalid_value("new upper bound must satisfy upper > lower. "
                                "set lower bound first.");
        }
        hi_y = y;
    }

    void step() {
        if(is_done) { return; }
        ++iter_c;
        do_halfstep();
        if(std::abs(p.x - q.x) <= eps && std::abs(p.y - q.y) <= eps) { is_done = true; }
    }

    void optimize() { while(!is_done) { step(); } }
};
