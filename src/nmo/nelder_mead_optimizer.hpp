/*********************************************************************************
 * Nelder-Mead Optimierungsalgorithmus.                                          *
 * Autor: Sonja Biedermann, a1402891 (Gruppe 4)                                  *
 *                                                                               *
 * Minimiert eine gegebene Funktion f.                                           *
 * Der Kontrollfluss liegt hierbei außerhalb des Optimierers – die               *
 * aufrufende Funktion trägt die Verantwortung, die step() Methode               *
 * aufzurufen, bis ein Minimum approximiert wurde. Das ermöglicht                *
 * das leichte Einsetzen von unterschiedlichen Tools zum Visualisieren des       *
 * Optimierungsvorganges.                                                        *
 *                                                                               *
 * Als Referenzen wurden folgende Quellen verwendet:                             *
 * http://mathfaculty.fullerton.edu/mathews/n2003/neldermead/NelderMeadProof.pdf *
 * https://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method                      *
 *********************************************************************************/

#pragma once
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <sstream>

#include "Funktion.h"
#include "point.hpp"

class nelder_mead_optimizer {
private:
    Funktion& f;
    point b; // best
    point g; // good
    point w; // worst
    double eps = 0.0001;

    double alpha_ = 1;    // Alpha: Reflexionsfaktor
    double gamma_ = 2;    // Gamma: Expansionsfaktor
    double rho_   = 0.5;  // Rho: Kontrahierungsfaktor
    double sigma_ = 0.5;  // Komprimierungsfaktor

    bool is_done = false;

    point& min(point& p1, point& p2) {
        return f(p1.x, p1.y) < f(p2.x, p2.y) ? p1 : p2;
    }

    point& min(point& p1, point& p2, point& p3) {
        double y1 = f(p1.x, p1.y);
        double y2 = f(p2.x, p2.y);
        double y3 = f(p3.x, p3.y);
        return y1 < y2 ? (y1 < y3 ? p1 : p3) : (y2 < y3 ? p2 : p3);
    }

    void sort_points_by_fvalue() {
        std::swap(b, min(b, g, w));
        std::swap(g, min(g, w));
    }

    void do_step() {
        point m = (b + g) / 2; // Mittelpunkt der besten beiden Punkte
        point r = m + alpha_ * (m - w); // Reflektiere schlechtesten Punkt

        if(f(r.x, r.y) <= f(w.x, w.y)) {
            if(f(r.x, r.y) < f(b.x, b.y)) {
                point e = m + gamma_ * (m - w); // r ist bester Punkt bis jetzt. Expandiere weiter
                w = min(e, r);
                return;
            }
            w = r; // Ersetze schlechtesten Punkt durch den besseren der beiden
            return;
        }

        point c;
        if(f(r.x, r.y) < f(w.x, w.y)) { // Outside
            c = m + rho_ * (r - m);
            if(f(c.x, c.y) <= f(r.x, r.y)) {
                w = c;
                return;
            }
        } else { // Inside
            c = m + rho_ * (w - m);
            if(f(c.x, c.y < f(w.x, w.y))) {
                w = c;
                return;
            }
        }

        // Komprimiere die zwei schlechtesten Punkte in Richtung b – Simplex zieht sich zusammen
        g = b + sigma_ * (g - b);
        w = b + sigma_ * (w - b);
    }
public:
    nelder_mead_optimizer(Funktion& f, double eps, point const& p1, point const& p2, point const& p3)
        : f{ f }, b{ p1 }, g{ p2 }, w{ p3 }, eps{ eps } { sort_points_by_fvalue(); }
    nelder_mead_optimizer(Funktion& f, point p1, point p2, point p3)
        : f{ f }, b{ p1 }, g{ p2 }, w{ p3 } {}

    double& alpha() { return alpha_; }
    double& gamma() { return gamma_; }
    double& rho() { return rho_; }
    double& sigma() { return sigma_; }

    std::tuple<point, point, point> retrieve_current_simplex() {
        return std::make_tuple(b, g, w);
    }

    bool done() { return is_done; }

    void step() {
        if(is_done) { return; }
        do_step();
        sort_points_by_fvalue();

        /* Konvergenzkriterium: Differenz zwischen bestem und schlechtetesten Punkt
         * erreicht oder unterschreitet Epsilon. */
        if(std::abs(f(b.x, b.y) - f(w.x, w.y)) <= eps) { is_done = true; }
    }
};
/* vim: set ts=4 sw=4 tw=0 et :*/
