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

class coordesc_optimizer {
private:
    Funktion& f;
    point p; // current pint
    point q; // last point
    double eps;
    double const golden_ratio = (sqrt(5) - 1) / 2;

    bool is_done = false; // Zustand: fertig optimiert?
    bool x_last = false;  // Zustand: wurde x als letzte Variable minimiert?
    size_t iter_c = 0;

    // Goldener Schnitt
    double minimize_with_x_constant(double x) {
        // Annahme: implementierte Funktionen haben Minima im y-Bereich [-10, 10]
        double a = -10;
        double b = 10;

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
        // Annahme: implementierte Funktionen haben Minima im x-Bereich [-10, 10]
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

    void do_halfstep() {
        ++iter_c;
        q = p;
        if(x_last) { p.y = minimize_with_x_constant(p.x); }
        else { p.x = minimize_with_y_constant(p.y); }
        x_last = !x_last;
    }

public:
    coordesc_optimizer(Funktion& f, point const& p, double eps = 0.0001)
        : f( f ), p{ p }, eps{ eps } {}
    point current_point() { return p; }
    point last_point() { return q; }

    bool done() const { return is_done; }
    size_t iteration_count() const { return iter_c; }

    void step() {
        if(is_done) { return; }
        do_halfstep();
        if(std::abs(p.x - q.x) <= eps && std::abs(p.y - q.y) <= eps) { is_done = true; }
    }

    void optimize() { while(!is_done) { step(); } }
};
