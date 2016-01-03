#ifndef FUNKTIONIMPLEMENTED_HPP
#define FUNKTIONIMPLEMENTED_HPP

struct f : Funktion {

    double value(double x, double y) {
        return pow(y, 4) + 2 * pow(x, 2) - 3 * x * y + 1;
    }

    double x(double x, double y) {
        return 4 * x - 3 * y;
    }

    double y(double x, double y) {
        return 4 * pow(y, 3) - 3 * x;
    }

};

struct g : Funktion {

    double value(double x, double y) {
        return pow((pow(x, 2) + y - 11), 2) + pow((x + pow(y, 2) - 7), 2);
    }

    double x(double x, double y) {
        return 2 * (pow(y, 2) + 2 * x * y + 2 * pow(x, 3) - 21 * x - 7);
    }

    double y(double x, double y) {
        return 2 * (pow(y, 2) + 2 * x * y + 2 * pow(x, 3) - 13 * x - 11);
    }

};

struct h : Funktion {

    double value(double x, double y) {
        return 3 * pow(x, 2) + pow(y, 2) - 3 * x * y - 3 * x;
    }

    double x(double x, double y) {
        return 6 * x - 3 * y - 3;
    }

    double y(double x, double y) {
        return 2 * y - 3 * x;
    }

};


#endif /* FUNKTIONIMPLEMENTED_HPP */

