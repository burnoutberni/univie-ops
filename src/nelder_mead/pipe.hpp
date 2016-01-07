/******************************************************************
 * Minimal-Wrapper um `gnuplot`.                                  *
 * Autor: Sonja Biedermann, a1402891 OPS WS15 7/4                 *
 *                                                                *
 * Kommuniziert mit gnuplot über Pipes. Erlaubt das Einfüttern    *
 * von Voreinstellungen (setzen von logarithmischen Skalen o.ä.)  *
 * und das Plotten von zuvorgehenden Eingaben.                    *
 ******************************************************************/

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
