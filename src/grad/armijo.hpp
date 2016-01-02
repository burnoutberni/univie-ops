/*********************************************************************
 * Implementiert die Schrittweitenbedingung mittels Armijo-Bedingung *
 * Autor: Christoph Pressler, 1407392 (Gruppe 7-4)                   *
 *********************************************************************/

#ifndef ARMIJO_HPP
#define ARMIJO_HPP

struct armijo {
    
    //Die Richtung des steilsten Abstiegs
    point abstiegsrichtung;
    //Die "Strenge" der Bedingung
    double c = 0.1;
    //Die Staerke der Aenderung von alpha jede iteration
    double beta = 0.9;
    //Die Zeit, welche zwischen den Iterationsschritten vergehen soll
    int sleep = 0;
    //Der Iterationszaehler auf welchen man im Nachhinein zugreifen kann
    int iteration = 0;
    //Ein Protokoll auf welches man im Nachhinein zugreifen kann
    std::string protokoll;
    
    armijo() {}
    armijo(int sleep_) : sleep{ sleep_ } {}
    armijo(int sleep_, double beta_, double c_) : c{ c_ }, beta{ beta_ }, sleep{ sleep_ } {}
    
    //Es werden eine Funktion und zwei Punkte uebergeben; TODO: Die Funktion ueberarbeiten, aktuell muss eine Variable vom Typ f (ein Struct welcher vom Struct Funktion abgeileitet wird) vorliegen
    double armijoschleife(f &f, point currentpoint, point gradient) {
        
        std::ostringstream os;
        //Die Abstiegsrichtung wird berechnet
        abstiegsrichtung = point(-gradient.x, -gradient.y);
        
        iteration = 0;
        
        double alpha = 1;
        double lastalpha = 0;
        
        //Die Bedingung der for-Schleife ist die Armijo-Bedingung
        for (; (f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y)) >= (f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung)); alpha *= beta) {
            
            //Sollte es zu dem Problem kommen, dass sich alpha nicht mehr aendert, wird der Vorgang abgebrochen; TODO: Besseres Errorhandling
            if(alpha == lastalpha){
                break;
                //throw std::runtime_error("Alpha kann nicht weiter veraendert werden");
            }
            
            lastalpha = alpha;
            
            //Das Protokoll wird mit hilfreichen Informationen befuellt
            os << "Aktuelle Werte, Zwischenschritt No. " << ++iteration << std::endl << "alpha = " << alpha << std::endl << "Linke Seite: "
                    << f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y) << std::endl << "Rechte Seite: "
                    << f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung) << std::endl << std::endl;
            
            //Es wird eine Pause gemacht damit der Nutzer den Output lesen kann
            usleep(sleep);
        }
        
        //Das Protokoll wird gespeichert
        protokoll = os.str();
        //Das gefundene alpha wird retouniert
        return alpha;

    }
    
};


#endif /* ARMIJO_HPP */

