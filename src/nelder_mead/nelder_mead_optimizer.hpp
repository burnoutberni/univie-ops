/*********************************************************************************
 * Nelder-Mead Optimierungsalgorithmus.                                          *
 * Autor: Sonja Biedermann, a1402891 (Gruppe 7, Team 4)                          *
 *                                                                               *
 * Minimiert eine gegebene Funktion f.                                           *
 * Wendet dazu den Nelder-Mead Algorithmus an, der einen sich an die Funkions-   *
 * landschaft anpassenden Simplex zum Minimum wandern lässt.                     *
 *                                                                               *
 * Als Referenzen wurden folgende Quellen verwendet:                             *
 * http://mathfaculty.fullerton.edu/mathews/n2003/neldermead/NelderMeadProof.pdf *
 * https://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method                      *
 *********************************************************************************/

#pragma once
#include <cmath>
#include <tuple>
#include <exception>

#include "Funktion.h"
#include "point.hpp"

struct invalid_value: public std::exception {
    char const* err = "invalid value";
    invalid_value(char const* e) : err{ e } {}
    char const* what() const noexcept { return err; }
};

class nelder_mead_optimizer {
private:
    Funktion& f;
    point b; // best
    point g; // good
    point w; // worst
    double eps;
    size_t iter_c = 0;

    double alpha_; // Alpha: Reflexionsfaktor, default: 1
    double gamma_; // Gamma: Expansionsfaktor, default: 2
    double beta_;  // Rho: Kontrahierungsfaktor, default: .5
    double delta_; // Komprimierungsfaktor, default: .5

    bool is_done = false;

    point& min(point& p1, point& p2) {
        return f(p1.x, p1.y) < f(p2.x, p2.y) ? p1 : p2;
    }

    point& min(point& p1, point& p2, point& p3) {
        double z1 = f(p1.x, p1.y);
        double z2 = f(p2.x, p2.y);
        double z3 = f(p3.x, p3.y);
        return z1 < z2 ? (z1 < z3 ? p1 : p3) : (z2 < z3 ? p2 : p3);
    }

    void sort_points_by_fvalue() {
        std::swap(b, min(b, g, w));
        std::swap(g, min(g, w));
    }

    void do_step() {
        point m = (b + g) / 2; // mittelpunkt
        point r = m + alpha_ * (m - w); // reflektiere schlechtesten punkt

        if(f(r.x, r.y) < f(w.x, w.y)) {
            if(f(r.x, r.y) < f(b.x, b.y)) { // r besser as b: expandiere weiter
                point e = m + gamma_ * (m - w);
                w = min(e, r);
                return;
            }
            w = r;
            return;
        }

        point c;
        if(f(r.x, r.y) < f(w.x, w.y)) { // outside: näher an r
            if(f(c.x, c.y) < f(r.x, r.y)) {
                w = c;
                return;
            }
        } else { // inside: näher an w
            c = m + beta_ * (w - m);
            if(f(c.x, c.y) < f(w.x, w.y)) {
                w = c;
                return;
            }
        }

        g = b + delta_ * (g - b);
        w = b + delta_ * (w - b);
    }

public:
    nelder_mead_optimizer(Funktion& f, point const& p1, point const& p2, point const& p3, double eps = 0.00001,
            double alpha = 1, double gamma = 2, double beta = .5, double delta = .5)
        : f( f ), b{ p1 }, g{ p2 }, w{ p3 }, eps{ eps } {
            sort_points_by_fvalue();
            set_alpha(alpha);
            set_gamma(gamma);
            set_beta(beta);
            set_delta(delta);
        }

    double alpha() const { return alpha_; }
    double gamma() const { return gamma_; }
    double beta() const  { return beta_; }
    double delta() const { return delta_; }

    size_t iteration_count() const { return iter_c; }
    point const& best_point() const { return b; }

    bool done() const { return is_done; }

    std::tuple<point, point, point> current_simplex() const {
        return std::make_tuple(b, g, w);
    }

    void set_alpha(double alpha) {
        if(!(alpha > 0)) {
            throw invalid_value("alpha value violates constraint alpha > 0");
        }
        alpha_ = alpha;
    }

    void set_gamma(double gamma) {
        if(!(gamma > 1)) {
            throw invalid_value("gamma value violates constraint gamma > 1");
        } else if(!(gamma > alpha_)) {
            throw invalid_value("gamma value violates constraint gamma > alpha");
        }
        gamma_ = gamma;
    }

    void set_beta(double beta) {
        if(!(0 < beta && beta < 1)) {
            throw invalid_value("beta value violates constraint 0 < beta < 1");
        }
        beta_ = beta;
    }

    void set_delta(double delta) {
        if(!(0 < delta && delta < 1)) {
            throw invalid_value("delta value violates constraint 0 < delta < 1");
        }
        delta_ = delta;
    }

    void step() {
        if(is_done) { return; }
        do_step();
        ++iter_c;
        sort_points_by_fvalue();

        /* Konvergenzkriterium: Differenz zwischen bestem und schlechtetestem Punkt
         * erreicht oder unterschreitet Epsilon. */
        if(std::abs(f(b.x, b.y) - f(w.x, w.y)) <= eps) { is_done = true; }
    }

    void optimize() { while(!is_done) { step(); } }
};
/* vim: set ts=4 sw=4 tw=0 et :*/
