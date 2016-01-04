#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cmath>
#include <fstream>
#include <thread>
#include <chrono>

#include "Funktion.h"
#include "coordesc_optimizer.hpp"
#include "gnuplotter.hpp"

std::string read_all_about(std::string const& topic) {
    std::ifstream file("info.txt");
    if(!file) { return std::string(); }
    std::string result;
    std::string buffer;
    while(getline(file, buffer) && buffer != topic);
    while(getline(file, buffer) && buffer != "----") result += buffer + '\n';
    return result;
}

int main(int argc, char const** argv) {
    // {{{ GNUPLOT SETTINGS
    char const* settings =
        "set palette rgbformulae 33,13,10\n"
        "set border 15 front linetype -1 linewidth 1.000\n"
        "set logscale z 10\n"
        "set view map\n"
        "set isosamples 60, 60\n"
        "set contour base\n"
        "set xlabel 'x'\n"
        "set xrange [*:*] noreverse nowriteback\n"
        "set ylabel 'y'\n"
        "set zlabel ''\n"
        "set yrange [*:*] noreverse nowriteback\n"
        "set zrange [*:*] noreverse nowriteback\n"
        "set cntrparam levels 25\n"
        "set surface\n";
    // }}}

    // FUNCTION STRUCTS {{{
    struct : Funktion {
        double value(double x, double y) { return 3*x*x + y*y - 3*x*y - 3*x; }
    } example1;

    struct : Funktion {
        double value(double x, double y) { return y*y*y*y + 2*x*x - 3*x*y + 1; }
    } example2;

    struct : Funktion {
        double value(double x, double y) { return 3*x*x + y + y*y; }
    } example3;

    struct : Funktion {
        double value(double x, double y) { return 5*x*x - 6*x*y + 5*y*y - 0.0259; }
    } example4;

    struct : Funktion {
        double value(double x, double y) { return (x*x + y - 11)*(x*x + y - 11) + (x + y*y - 7)*(x + y*y -7); }
    } himmelblau;

    struct : Funktion {
        double value(double x, double y) { return 100*(y - x*x)*(y - x*x) + (x - 1)*(x - 1); }
    } banana;

    struct : Funktion {
        double value(double x, double y) { return (x + 2*y - 7)*(x + 2*y - 7) + (2*x + y - 5)*(2*x + y - 5); }
    } booth;

    struct : Funktion {
        double value(double x, double y) { return 0.26*(x*x + y*y) - 0.48*x*y; }
    } matyas;

    struct : Funktion {
        double value(double x, double y) { return (1.5 - x + x*y)*(1.5 - x + x*y) + (2.25 - x + x*y*y)*(2.25 - x + x*y*y) + (2.625 - x + x*y*y*y)*(2.625 - x + x*y*y*y); }
    } beale;

    struct : Funktion {
        double value(double x, double y) { return 2*x*x - 1.05*x*x*x*x + (x*x*x*x*x*x)/6 + x*y + y*y; }
    } camel;

    struct : Funktion {
        double value(double x, double y) { return x*x + y*y; }
    } sphere;
    // }}}

    // HASHMAP {{{
    std::unordered_map<std::string, std::pair<std::string, Funktion*>> plot_functions = {
        {"example1", std::make_pair("3*x**2 + y**2 - 3*x*y - 3*x", &example1)},
        {"example2", std::make_pair("y**4 + 2*x**2 - 3*x*y + 1", &example2)},
        {"example3", std::make_pair("3*x**2 + y + y**2", &example3)},
        {"example4", std::make_pair("5*x**2 - 6*x*y + 5*y**2 - 0.0259", &example4)},
        {"himmelblau", std::make_pair("(x**2 + y - 11)**2 + (x + y**2 - 7)**2", &himmelblau)},
        {"banana", std::make_pair("100*(y - x**2)**2 + (x - 1)**2", &banana)},
        {"matyas", std::make_pair("0.26*(x**2 + y**2) - 0.48*x*y", &matyas)},
        {"booth", std::make_pair("(x + 2*y - 7)**2 + (2*x + y - 5)**2", &booth)},
        {"sphere", std::make_pair("x**2 + y**2", &sphere)},
        {"beale", std::make_pair("(1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2", &beale)},
        {"camel", std::make_pair("2*x**2 - 1.05*x**4 + (x**6)/6 + x*y + y**2", &camel)}
    };
    // }}}

    if(argc < 2) {
        std::cerr << "No function specified.\n"
                  << "Try one of these:\n\n"
                  << "\thimmelblau – minimizes the Himmelblau function\n"
                  << "\tbanana – minimizes Rosenbrock's Banana function\n"
                  << "\tmatyas – minimizes the Matyas function\n"
                  << "\tcamel – minimizes the three-hump camel function\n"
                  << "\tbooth – minimizes Booth's function\n"
                  << "\tbeale – minimizes Beale's function\n"
                  << "\tsphere – minimizes the sphere function\n"
                  << "\texample1 – minimizes the function 3*x**2 + y**2 - 3*x*y - 3*x\n"
                  << "\texample2 – minimizes the function y**4 + 2*x**2 - 3*x*y + 1\n"
                  << "\texample3 – minimizes the function 3*x**2 + y + y**2\n"
                  << "\texample4 – minimizes the function 5*x**2 - 6*x*y + 5*y**2 - 0.0259\n"
                  << "\n"
                  << "To specify manual stepping (by user input), add `manual` after the function name.\n"
                  << "To learn more about any one of these, use `info [function]`.\n";
        return 1;
    }

    if(std::string("info").compare(argv[1]) == 0) {
        if(argc < 3) {
            std::cerr << "Please specify the function you want to know more about.\n";
            return 1;
        }
        std::string info = read_all_about(argv[2]);
        if(info.empty()) {
            std::cerr << "I know nothing about this function.\n"
                      << "Perhaps you mistyped the function name or the database file ('info.txt') is gone?\n";
            return 1;
        }
        std::cout << info;
        return 0;
    }

    bool manual = (argc > 2 && std::string("manual").compare(argv[2]) == 0);

    std::cout << "\tC O O R D I N A T E  D E S C E N T\n"
              << "\tDEMOTOOL\n"
              << "\tMinimizes one of 9 given functions.\n"
              << "\tDisplays progress via gnuplot.\n"
              << "\t***********************************\n\n";

    Funktion* user_choice;
    if(plot_functions.count(argv[1])) {
        user_choice = plot_functions[argv[1]].second;
    } else {
        std::cerr << "No such function. (\"" << argv[1] << "\")\n";
        return 1;
    }

    gnuplotter gnu(pipe("gnuplot -p 2> /dev/null"));
    gnu.push_settings(settings);
    gnu.push_settings(std::string("set title '") + argv[1] + "'\n");
    std::ostringstream oss;
    oss << "splot [-10.5:10.5] [-10.5:10.5] \\\n" << plot_functions[argv[1]].first
        << " with lines palette notitle nosurface, \\\n"
        << "'-' with lines lc rgb 'red' notitle\n";
    std::string base_cmd = oss.str();

    coordesc_optimizer cdo(*user_choice, {-0.5, -1}, 0.000001);
    int n = 0;
    while(!cdo.done()) {
        std::cout << "\n#" << n++ << '\n';
        point p = cdo.retrieve_current_point();
        std::cout << "P = " << p.format() << '\n';

        base_cmd += p.raw() + " 1\n";

        gnu.plot_command() = base_cmd + "e\n";
        gnu.replot();
        cdo.step();
        if(manual) {
            std::cin.get();
            if(std::cin.eof()) { return 0; }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}
/* vim: set ts=4 sw=4 tw=0 et :*/
