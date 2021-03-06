/*********************************************************************
 * Einfache Point-Implementierung.                                   *
 * Autor: Sonja Biedermann, a1402891 (Gruppe 7-4)                    *
 *                                                                   *
 * Repräsentiert eine Punkt der Form (x, y).                         *
 * Verfügt weiters über die Vektoroperationen Addition, Subtraktion, *
 * Division durch einen Skalar und Multiplikation mit einem Skalar   *
 * und einer Methode format(), die den Punkt ausgabefähig in einen   *
 * std::string formattiert.                                          *
 *********************************************************************/

#pragma once
#include <cassert>
#include <sstream>

struct point {
    double x = 0.0;
    double y = 0.0;

    point() {}
    point(double x_, double y_) : x{ x_ }, y{ y_ } {}

    // zweck: simple deklaration im funktionsaufruf als { x, y }.
    // i.e. take_a_point({ 1.8, 13.9 }) ruft den initlist-konstruktor auf,
    // um die implizite umwandlung durchführen zu können.
    point(std::initializer_list<double> li) {
        assert(li.size() == 2 && "point class only supports two dimensional points!");
        x = li.begin()[0]; y = li.begin()[1];
    }

    point operator+(point const& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    point operator-(point const& rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    point operator/(double denomiator) const {
        return {x / denomiator, y / denomiator};
    }

    point operator*(double factor) const {
        return {x * factor, y * factor};
    }

    std::string format() const {
        std::ostringstream os;
        os << "(" << x << ", " << y << ")";
        return os.str();
    }

    std::string raw() const {
        std::ostringstream os;
        os << x << ' ' << y;
        return os.str();
    }
};

point operator/(double denomiator, point const& p) {
    return p / denomiator;
}

point operator*(double factor, point const& p) {
    return p * factor;
}
/* vim: set ts=4 sw=4 tw=0 et :*/
