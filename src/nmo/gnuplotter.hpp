/******************************************************************
 * Minimal-Wrapper um `gnuplot`.                                  *
 * Autor: Sonja Biedermann, a1402891 OPS WS15 7/4                 *
 *                                                                *
 * Kommuniziert mit gnuplot über Pipes. Erlaubt das Einfüttern    *
 * von Voreinstellungen (setzen von logarithmischen Skalen o.ä.)  *
 * und das Plotten von zuvorgehenden Eingaben.                    *
 ******************************************************************/

#include <string>

#include "pipe.hpp"

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
