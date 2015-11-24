/*******************************************************************
 * Driver-Programm für den Nelder-Mead Optimierungsalgorithmus.    *
 * Autor: Sonja Biedermann, a1402891 OPS WS15 7/4                  *
 *                                                                 *
 * Führt die Optimierung durch und stellt das Ergebnis einer jeden *
 * Iteration grafisch dar. Verwendet dazu POSIX-Pipes und Gnuplot. *
 * Verfügt weiters über ein kleines Info-Feature, dass auf Wunsch  *
 * des Benutzers Daten (wie etwa die Tiefpunkte der Funktion, die  *
 * zur Überprüfung der Ergebnisse nützlich sein können) über eine  *
 * bestimmte unterstützte Funktion anzeigen.                       *
 *******************************************************************/

#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cmath>
#include <fstream>
#include <thread>
#include <chrono>

#include "Funktion.h"
#include "nelder_mead_optimizer.hpp"

// PIPE {{{
class pipe {
private:
    FILE* handle;
public:
    pipe(std::string const& cmd) : handle{ popen(cmd.c_str(), "w") } {}
    pipe(pipe&& other) : handle{ std::move(other.handle) } { other.handle = NULL; }
    ~pipe() { fflush(handle); if(handle) { fclose(handle); } }
    pipe& flush() { fflush(handle); return *this; }
    pipe& operator<<(std::string input) {
        fputs(input.c_str(), handle);
        return *this;
    }
};
// }}}

// GNUPLOTTER {{{
class gnuplotter {
private:
    pipe pipehandle;
    std::string plot_cmd;
public:
    gnuplotter(std::string const& plot_cmd = "")
        : pipehandle{ pipe("gnuplot -p 2> /dev/null") }, plot_cmd{ plot_cmd } {}
    gnuplotter(pipe&& p) : pipehandle{ std::move(p) } {}
    gnuplotter& push_settings(std::string settings) {
        pipehandle << settings;
        pipehandle.flush();
        return *this;
    }
    std::string& plot_command() { return plot_cmd; }
    gnuplotter& replot() {
        pipehandle << plot_cmd;
        pipehandle.flush();
        return *this;
    }
};
// }}}

std::string read_all_about(std::string const& topic) {
    std::ifstream file("info.txt");
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
        "set cntrparam levels 10\n"
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
    // }}}
    // HASHMAP {{{
    std::unordered_map<std::string, std::pair<std::string, Funktion*>> plot_functions = {
        {"example1", std::make_pair("3*x**2 + y**2 - 3*x*y - 3*x", &example1)},
        {"example2", std::make_pair("y**4 + 2*x**2 - 3*x*y + 1", &example2)},
        {"example3", std::make_pair("3*x**2 + y + y**2", &example3)},
        {"himmelblau", std::make_pair("(x**2 + y - 11)**2 + (x + y**2 - 7)**2", &himmelblau)},
        {"banana", std::make_pair("100*(y - x**2)**2 + (x - 1)**2", &banana)},
        {"matyas", std::make_pair("0.26*(x**2 + y**2) - 0.48*x*y", &matyas)},
        {"booth", std::make_pair("(x + 2*y - 7)**2 + (2*x + y - 5)**2", &booth)},
        {"beale", std::make_pair("(1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2", &beale)},
        {"camel", std::make_pair("2*x**2 - 1.05*x**4 + (x**6)/6 + x*y + y**2", &camel)}
    };
    // }}}

    if(argc < 2) {
        std::cerr << "No function specified.\n"
                  << "Try one of these:\n"
                  << "\thimmelblau – minimizes the Himmelblau function\n"
                  << "\tbanana – minimizes Rosenbrock's Banana function\n"
                  << "\tmatyas – minimizes the Matyas function\n"
                  << "\tcamel – minimizes the three-hump camel function\n"
                  << "\tbooth – minimizes Booth's function\n"
                  << "\tbeale – minimizes Beale's function\n"
                  << "\texample1 – minimizes the function 3*x**2 + y**2 - 3*x*y - 3*x\n"
                  << "\texample2 – minimizes the function y**4 + 2*x**2 - 3*x*y + 1\n"
                  << "\texample3 – minimizes the function 3*x**2 + y + y**2\n";
        return 1;
    }

    if(std::string("info").compare(argv[1]) == 0) {
        if(argc < 3) {
            std::cerr << "Please specify the function you want to know more about.\n";
            return 1;
        }
        std::string info = read_all_about(argv[2]);
        std::cout << info;
        return 0;
    }

    bool manual = (argc > 2 && std::string("manual").compare(argv[2]) == 0);

    std::cout << "\tN E L D E R - M E A D\n"
              << "\tDEMOTOOL\n"
              << "\tMinimizes one of 8 given functions.\n"
              << "\tDisplays progress via gnuplot.\n"
              << "\t***********************************\n\n";

    Funktion* user_choice;
    try {
        user_choice = plot_functions.at(argv[1]).second;
    } catch(std::out_of_range const& ex) {
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

    nelder_mead_optimizer nmo(*user_choice, 0.0000005, {-1, -5}, {8, 8}, {3, -8});
    int n = 0;
    while(!nmo.done()) {
        std::cout << "#" << n++ << '\n';
        auto points = nmo.retrieve_current_simplex();
        std::cout << "B = " << std::get<0>(points).format() << '\n'
                  << "G = " << std::get<1>(points).format() << '\n'
                  << "W = " << std::get<2>(points).format() << '\n';

        oss.str("");
        oss << base_cmd;
        oss << std::get<0>(points).raw()<< " 1\n"
            << std::get<1>(points).raw() << " 1\n"
            << std::get<2>(points).raw() << " 1\n"
            << std::get<0>(points).raw() << " 1\n"
            << "e\n";

        gnu.plot_command() = oss.str();
        gnu.replot();
        nmo.step();
        if(manual) {
            std::cin.get();
            if(std::cin.eof()) { return 0; }
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(0.5));
        }
    }
}
/* vim: set ts=4 sw=4 tw=0 et :*/
