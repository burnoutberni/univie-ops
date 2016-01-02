#include <iostream>
#include <cmath>
#include <unistd.h>
#include <stdexcept>
#include "Funktion.h"
#include "Funktionimplemented.hpp"
#include "point.hpp"
#include "armijo.hpp"

int main(int argc, char** argv) {

    f f;
    
    //Variablen fuer die Armijo-Bedingung
    int iteration = 1;
    double alpha = 1;
    double lastalpha = 0;
    double c = 0.1;
    double beta = 0.9;
    double bedingung = 0.3;
    point currentpoint(1, 1);

    //Ausgabe der wichtigsten Variablen
    std::cout << "alpha = " << alpha << std::endl << "c = " << c << std::endl << "beta = " << beta << std::endl << "Bedingung: Betrag d. Gradienten muss kleiner als "
            << bedingung << " sein" << std::endl << std::endl;
    point gradient(f.x(currentpoint.x, currentpoint.y), f.y(currentpoint.x, currentpoint.y));



    for (; gradient.betrag() >= bedingung; ++iteration) {
        
        alpha = 1;

        //Der aktuelle Iterationsschritt wird ausgegeben
        std::cout << "Iterationsschritt No." << iteration << std::endl;

        //Der aktuelle Punkt wird ausgegeben
        std::cout << "Der aktuelle Punkt: " << currentpoint.format() << std::endl;
        
        armijo standardarm(300000);
        alpha = standardarm.armijoschleife(f, currentpoint, gradient);
        std::cout << standardarm.protokoll;

        currentpoint = currentpoint + standardarm.abstiegsrichtung * alpha;
        gradient = point(f.x(currentpoint.x, currentpoint.y), f.y(currentpoint.x, currentpoint.y));

        std::cout << "Es wurde ein passendes alpha (" << alpha << ") gefunden. Der neue Punkt: " << currentpoint.format() << std::endl << "Der Betrag des Gradienten ist " << gradient.format() << std::endl << std::endl;

    }

    std::cout << "Vorgang abgeschlossen! Ergebnisse:" << std::endl << "Optimum: " << currentpoint.format() << "wurde gefunden im " << iteration << " Iterationsschritt"
            << std::endl << "Der Betrag des Gradienten " << gradient.format() << " ist " << gradient.betrag() << std::endl;

    return 0;

}