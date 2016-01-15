#include <iostream>
#include <cmath>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include "Funktion.cpp"
#include "point.hpp"
#include "gnuplot_i.hpp"


//Die Standard-Variablen
double stanc = 0.1;
double stanbeta = 0.9;
double stanbedingung = 0.3;
double stanarmijowaitingtime = 0.5;
double stanstepwaitingtime = 1;
int stanmaxiteration = 100000;
point stanstartpoint(1, 1);

int iteration = 1;
double alpha = 1;
double lastalpha = 0;
double c = stanc;
double beta = stanbeta;
double bedingung = stanbedingung;
double armijowaitingtime = stanarmijowaitingtime;
double stepwaitingtime = stanstepwaitingtime;
int maxiteration = stanmaxiteration;
point startpoint = stanstartpoint;
point currentpoint;
point abstiegsrichtung;
Gnuplot g1;

		
int main(int argc, char** argv) {
	
	//Die Funktionen werden definiert
	struct : Funktion {
		
		int choice = 1;
		
		double value(double x, double y) {
			switch(choice){
				case 1:
				{
					return pow(y, 4) + 2 * pow(x, 2) - 3 * x * y + 1;
				}
				case 2:
				{
					return 100 * pow((y-pow(x, 2)),2) + pow((1-x), 2);
				}
				case 3:
				{
					return 3 * pow(x, 2) + pow(y, 2) - 3 * x * y - 3 * x;
				}
			}
			return 0;
		}

		double x(double x, double y) {
			switch(choice){
				case 1:
				{
					return 4 * x - 3 * y;;
				}
				case 2:
				{
					return -400*x*y + 400 * pow(x,3) + 2*x -2;
				}
				case 3:
				{
					return 6 * x - 3 * y - 3;
				}
			}
			return 0;
		}

		double y(double x, double y) {
			switch(choice){
				case 1:
				{
					return 4 * pow(y, 3) - 3 * x;
				}
				case 2:
				{
					return 200*y - 200*pow(x,2);
				}
				case 3:
				{
					return 2 * y - 3 * x;
				}
			}
			return 0;
		}
		
		std::string gnuplotout(){
			switch(choice){
				case 1:
				{
					return "f(x,y) = y**4+2*x**2-3*x*y+1";
				}
				case 2:
				{
					return "f(x,y) = 100 * (y-x**2)**2 + (x-1)**2";
				}
				case 3:
				{
					return "f(x,y) = 3*x**2+y**2-3*x*y-3*x";
				}
			}
			return "";
		}
		
	} f;
    
    //Die Kontroller-Variable, die kontrolliert was getan wird
    int input;
    
    //Das Programm wird ausgeführt bis die Kontroll-Variable = 0 ist
    do{
		
		currentpoint = startpoint;
		
		//Menue-output und User-input
		std::cout << "***** Hauptmenue *****"<< std::endl
				<< "1..........Gradientenverfahren starten" << std::endl
				<< "2..........Hilfe" << std::endl
				<< "3..........Konfiguration" << std::endl
				<< "0..........Beenden" << std::endl;
				
		std::cin >> input;
		
		//Der zentrale Switch, bei welchem entschieden wird, was getan wird
		switch(input){
			
			//Das Gradientenverfahren
			case 1:
			{
				//Wenn das Gradientenverfahren schon einmal durchgefuehrt wurde, wird der iterationszaehler und gnuplot resetet
				if(iteration != 1){
					iteration = 1;
					g1.reset_all();
				}
				
				//Gnuplot wird konfiguriert
				g1 << "set contour both"
				<< "set isosamples 40"
				<< "set cntrparam levels 30"
				<< "set nokey"
				<< "set palette defined (0 'white', 1 'orange', 2 'red')";
				g1.set_pointsize(5);
				std::vector<double> x, y, z;
				
				//Wahl der Funktion
				std::cout << "Welche Testfunktion soll verwendet werden?" << std::endl
						<< "1..........y⁴+2x²-3xy+1" << std::endl
						<< "2..........(x²+y-11)²+(x+y²-7)²" << std::endl
						<< "3..........3x²y²-3xy-3x" << std::endl;
						
				std::cin >> f.choice;
				
				if(f.choice > 3 || f.choice < 1){
					std::cout << "Das ist keine gueltige Funktion" << std::endl;
					break;
				}
				
				//Die ausgewaehlte Funktion wird an Gnuplot uebergeben
				g1 << f.gnuplotout();
				g1 << "splot f(x, y)";
				
				//Ausgabe der wichtigsten Variablen
				std::cout << "Es wurde das Gradientenverfahren mit folgenden Werten gestartet:" << std::endl
						<< "alpha....." << alpha << std::endl
						<< "c........." << c << std::endl
						<< "beta......" << beta << std::endl
						<< "Bedingung: Betrag d. Gradienten muss kleiner als " << bedingung << " sein" << std::endl << std::endl;
				
				//Der erste Gradient wird initialisiert
				point gradient(f.x(currentpoint.x, currentpoint.y), f.y(currentpoint.x, currentpoint.y));

				//Die Anfangswerte werden ausgegeben
				std::cout << "Der Startpunkt ist : " << currentpoint.format() << std::endl 
		 				<< "Der Betrag des ersten Gradienten " << gradient.format() << " ist " << gradient.betrag() << std::endl << std::endl;

				for (; gradient.betrag() >= bedingung && iteration <= maxiteration; ++iteration) {
					
					alpha = 1;

					//Der aktuelle Iterationsschritt wird ausgegeben
					std::cout << "Iterationsschritt No." << iteration << std::endl;

					//Der aktuelle Punkt wird ausgegeben
					std::cout << "Der aktuelle Punkt: " << currentpoint.format() << std::endl;
					
					abstiegsrichtung = point(-gradient.x, -gradient.y);
					double armijoiteration = 0;
					
					//Die Bedingung der for-Schleife ist die Armijo-Bedingung
					for (; (f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y)) >= (f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung)); alpha *= beta) {
						
						//Sollte es zu dem Problem kommen, dass sich alpha nicht mehr aendert, wird der Vorgang abgebrochen; TODO: Besseres Errorhandling
						if(alpha == lastalpha){
							throw std::runtime_error("Alpha kann nicht weiter veraendert werden, versuchen Sie c oder den Startpunkt zu aendern");
						}
						
						lastalpha = alpha;
						
						//Es werden hilfreiche Informationen ausgegeben
						std::cout << "Aktuelle Werte, Zwischenschritt No. " << ++armijoiteration << std::endl << "alpha = " << alpha << std::endl << "Linke Seite: "
								<< f.value(currentpoint.x + alpha * abstiegsrichtung.x, currentpoint.y + alpha * abstiegsrichtung.y) << std::endl << "Rechte Seite: "
								<< f.value(currentpoint.x, currentpoint.y) + c * alpha * (gradient * abstiegsrichtung) << std::endl << std::endl;
								
						usleep(armijowaitingtime * 1000000);
								
					}
					

					currentpoint = currentpoint + abstiegsrichtung * alpha;
					gradient = point(f.x(currentpoint.x, currentpoint.y), f.y(currentpoint.x, currentpoint.y));

					std::cout << "Es wurde ein passendes alpha (" << alpha << ") gefunden. Der neue Punkt: " << currentpoint.format() << std::endl << "Der Betrag des Gradienten " << gradient.format() << " ist " << gradient.betrag() << std::endl << std::endl;
										
					x.push_back(currentpoint.x);
					y.push_back(currentpoint.y);
					z.push_back(f.value(currentpoint.x,currentpoint.y));

					try{
						
						g1.reset_plot();
						
						if(iteration >= 2){
							
							std::ostringstream os;
							os << "set arrow " << iteration << " from " << std::to_string(x[iteration-2])<< "," << std::to_string(y[iteration-2]) << "," << std::to_string(z[iteration-2]) << " to " << std::to_string(x[iteration-1]) << "," << std::to_string(y[iteration-1]) << "," << std::to_string(z[iteration-1]);
							std::string a = os.str();
							g1 << a;
							
						}
						
						g1 << "splot f(x,y)";
						g1.plot_xyz(x,y,z,"");
					}
					catch (GnuplotException ge){
						std::cout << ge.what() << std::endl;
					}
					
					usleep(stepwaitingtime * 1000000);
				}

				std::cout << "Vorgang abgeschlossen! Ergebnisse:" << std::endl << "Optimum: " << currentpoint.format() << " wurde gefunden im " << iteration << ". Iterationsschritt"
						<< std::endl << "Der Betrag des Gradienten " << gradient.format() << " ist " << gradient.betrag() << std::endl;
				break;
			}
			
			case 2:
			{
				std::cout << "Dieses Programm berechnet mit Hilfe des Gradientenverfahrens das Optimum einer der drei vorimplementierten Funktionen. Naehre Informationen zum Gradientenferfahren un der Schrittweitenbestimmung"
						<< "ittels Armijo gibt es im beiligendem PDF." << std::endl;
				break;
			}
				
			case 3:
			{

				int confinput;
				std::cout << "Was wollen Sie aendern? Druecken Sie die vorne angegebene Taste um einen Wert zu aendern oder etwas Anderes um die Konfiguration zu verlassen. Die aktuellen Werte sind: " << std::endl
						<< "1: c........................................................................................" << c << std::endl
						<< "2: beta....................................................................................." << beta << std::endl
						<< "3: Die Wartezeit zwischen den Schritten der Alpha-Ermittlung (in Sekunden).................." << armijowaitingtime << std::endl
						<< "4: Die Wartezeit wischen den Schritten wenn ein neuer Punkt gefunden wurde (in Sekunden)...." << stepwaitingtime << std::endl
						<< "5: Der Startpunkt..........................................................................." << currentpoint.format() << std::endl
						<< "6: Der Betrag des Gradienten (fuer die Abbruchbedingung)...................................." << bedingung << std::endl
						<< "7: Die maximale Anzahl an Iterationen......................................................." << maxiteration << std::endl;
						
				std::cin >> confinput;
				
				switch(confinput){
					
					case 1:
					{
						std::cout << "Bitte geben Sie das neue c ein (Um den Standardwert (" << stanc << ") zu waehlen druecken Sie die 2)" << std::endl;
						std::cin >> c;
						if(c == 2){
							c = stanc;
						}
						if(c > 1 || c < 0){
							std::cout << "Dieser Wert ist nicht erlaubt, er muss zwischen 0 und 1 liegen" << std::endl;
						}
						
						break;
					}
					
					case 2:
					{
						std::cout << "Bitte geben Sie das neue Beta ein (Um den Standardwert (" << stanbeta << ") zu waehlen druecken Sie die 2)" << std::endl;
						std::cin >> beta;
						if(beta == 2){
							beta = stanbeta;
						}
						if(beta > 1 || beta < 0){
							std::cout << "Dieser Wert ist nicht erlaubt, er muss zwischen 0 und 1 liegen" << std::endl;
						}
						break;
					}
					
					case 3:
					{
						std::cout << "Bitte geben Sie die neue Wartezeit ein (Um den Standardwert (" << stanarmijowaitingtime << ") zu waehlen geben Sie 99 ein)" << std::endl;
						std::cin >> armijowaitingtime;
						if(armijowaitingtime == 99){
							armijowaitingtime = stanarmijowaitingtime;
						}
						break;
					}
					
					case 4:
					{
						std::cout << "Bitte geben Sie die neue Wartezeit ein (Um den Standardwert (" << stanstepwaitingtime << ") zu waehlen geben Sie 99 ein)" << std::endl;
						std::cin >> stepwaitingtime;
						if(stepwaitingtime == 99){
							stepwaitingtime = stanstepwaitingtime;
						}
						break;
					}
					
					case 5:
					{
						std::cout << "Bitte geben Sie das x des neuen Startpunktes ein (Um den Standardwert (" << stanstartpoint.x << ") zu waehlen geben Sie 99 ein)" << std::endl;
						std::cin >> startpoint.x;
						if(startpoint.x == 99){
							startpoint.x = stanstartpoint.x;
						}
						std::cout << "Bitte geben Sie das y des neuen Startpunktes ein (Um den Standardwert (" << stanstartpoint.y << ") zu waehlen geben Sie 99 ein)" << std::endl;
						std::cin >> startpoint.y;
						if(startpoint.y == 99){
							startpoint.y = stanstartpoint.y;
						}
						break;
					}
					
					case 6:
					{
						std::cout << "Bitte geben Sie den neuen Betrag ein (Um den Standardwert (" << stanbedingung << ") zu waehlen geben Sie 99 ein)" << std::endl;
						std::cin >> bedingung;
						if(bedingung == 99){
							bedingung = stanbedingung;
						}
						break;
					}
					
					case 7:
					{
						std::cout << "Bitte geben Sie die neue Anzahl an maximalen Iterationen ein (Um den Standardwert (" << stanmaxiteration << ") zu waehlen geben Sie 0 ein)" << std::endl;
						std::cin >> maxiteration;
						if(maxiteration == 0){
							maxiteration = stanmaxiteration;
						}
						break;
					}
						
				}
				
				break;
			}
				
			case 0:
			{
				break;
			}
				
			default:
			{
				std::cout << "Unbekannter Befehl, fuer Hilfe druecken Sie die 2\n";
				break;
			}
			
			
		}
			
	}while(input);
	
    return 0;
}
