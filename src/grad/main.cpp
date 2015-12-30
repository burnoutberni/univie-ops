#include <iostream>
#include <cmath>
#include <unistd.h>
#include <stdexcept>
#include "Funktion.h"
#include "point.hpp"

int main(int argc, char** argv) {

    struct Funktion {
        
        char funktion;

        double value(double x, double y) {
            if(funktion == 'f') return pow(y, 4) + 2 * pow(x, 2) - 3 * x * y + 1;
            if(funktion == 'g') return pow((pow(x, 2) + y - 11), 2) + pow((x + pow(y, 2) - 7), 2);
        }

        double x(double x, double y) {
            if(funktion == 'f') return 4 * x - 3 * y;
            if(funktion == 'g') return 2 * (pow(y, 2) + 2 * x * y + 2 * pow(x, 3) - 21 * x - 7);
        }

        double y(double x, double y) {
            if(funktion == 'f') return 4 * pow(y, 3) - 3 * x;
            if(funktion == 'g') return 2 * (pow(y, 2) + 2 * x * y + 2 * pow(x, 3) - 13 * x - 11);
        }

    };

    Funktion f = {'f'};
    
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

        //Der Gradient & negative Gradient werden betrechnet
        point abstiegsrichtung(-gradient.x, -gradient.y);

        //Die aktuelle Abstiegsrichtung wird ausgegeben
        std::cout << "Die aktuelle Abstiegsrichtung: " << abstiegsrichtung.format() << std::endl << std::endl;

        for (int zwischenschritt = 0; (f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y)) >= (f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung)); alpha *= beta) {
            
            if(alpha == lastalpha){
                break;
                //throw std::runtime_error("Alpha kann nicht weiter veraendert werden");
            }
            
            lastalpha = alpha;
            
            std::cout << "Aktuelle Werte, Zwischenschritt No. " << ++zwischenschritt << std::endl << "alpha = " << alpha << std::endl << "Linke Seite: "
                    << f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y) << std::endl << "Rechte Seite: "
                    << f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung) << std::endl << std::endl;
            //usleep(2000000);
        }

        currentpoint = currentpoint + abstiegsrichtung * alpha;
        gradient = point(f.x(currentpoint.x, currentpoint.y), f.y(currentpoint.x, currentpoint.y));

        std::cout << "Es wurde ein passendes alpha (" << alpha << ") gefunden. Der neue Punkt: " << currentpoint.format() << std::endl << "Der Betrag des Gradienten ist " << gradient.format() << std::endl << std::endl;
        
        //usleep(3000000);

    }

    std::cout << "Vorgang abgeschlossen! Ergebnisse:" << std::endl << "Optimum: " << currentpoint.format() << "wurde gefunden im " << iteration << " Iterationsschritt"
            << std::endl << "Der Betrag des Gradienten " << gradient.format() << " ist " << gradient.betrag() << std::endl;

    return 0;

}