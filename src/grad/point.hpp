/*********************************************************************
 * Einfache Point-Implementierung.                                   *
 * Autor: Sonja Biedermann, a1402891 (Gruppe 7-4), leicht abgaendert *
 * von Christoph Pressler, 1407392 (Gruppe 7-4)                      *
 *                                                                   *
 * Repräsentiert eine Punkt der Form (x, y).                         *
 * Verfügt weiters über die Vektoroperationen Addition, Subtraktion, *
 * Division durch einen Skalar und Multiplikation mit einem Skalar   *
 * und einer Methode format(), die den Punkt ausgabefähig in einen   *
 * std::string formattiert.                                          *
 *********************************************************************/
#pragma once
#include <sstream>

struct point {
    double x = 0.0;
    double y = 0.0;

    point() {}
    point(double x_, double y_) : x{ x_ }, y{ y_ } {}

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
    
    double operator*(point factor) const{
        return (x * factor.x + y * factor.y);
    }

    double betrag() const{
        return sqrt(pow(x,2) * pow(y,2));
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
