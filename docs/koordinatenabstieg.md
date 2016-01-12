# Einführung

v

# Beispiele

v

# Implementation

### Klassenübersicht

#### Instanzvariablen

| Variablenname         | Beschreibung                                    |
| --------------------- | ----------------------------------------------- |
| `Funktion& f`         | zu optimierendes Funktionsobjekt                |
| `point p`             | derzeitiger Punkt                               |
| `point q`             | letzter Punkt                                   |
| `double eps`          | Epsilon (Kovergenzkriterium)                    |
| `double golden_ratio` | Goldener Schnitt (Minimierungsalgorithmus)      |
| `bool is_done`        | Zustandsvariable                                |
| `bool x_last`         | Zustandvariable: wurde x als letztes optimiert? |
| `size_t iter_c`       | Iterationszähler                                |

#### Methoden

| Methode                                             | Beschreibung                                                                   |
| --------------------------------------------------- | ------------------------------------------------------------------------------ |
| `- double minimize_with_x_constant(double x) const` | Minimiert die Funktion mit einem konstanten Wert für x                         |
| `- double minimize_with_x_constant(double y) const` | Minimiert die Funktion mit einem konstanten Wert für y                         |
| `- void do_halfstep()`                              | Führt einen Optimierungsschritt durch (optimiert eine Variable)                |
| `+ point current_point() const`                     | returniert p                                                                   |
| `+ point last_point() const`                        | returniert q                                                                   |
| `+ void step()`                                     | führt einen Optimierungsschritt durch                                          |
| `+ bool done()`                                     | returniert Zustandsvariable                                                    |
| `+ size_t iteration_count()`                        | returniert den Wert des Iterationenzählers                                     |
| `+ void step()`                                     | führt Optimierungsschritt aus, sortiert Punkte und setzt ggf. Zustandsvariable |
| `+ optimize()`                                      | führt die komplette Optimierung durch                                          |

### Algorithmus selbst

Die Implementierung umfasst eine Optimiererklasse `coordesc_optimizer`, der bei
der Instanziierung diverse Parameter übergeben werden müssen:

* Eine von der Klasse `Funktion` abgeleitete Funktionsrepräsentation, die die
  Methode `value(double, double)` bzw. `operator()(double, double)` implementiert
  haben muss
* 1 Startpunkt `p1`, der ruhig zufällig gewählt werden kann
* Ein Double-Wert `eps`, der den zu verwendenden Epsilon-Wert angibt [optional,
  defaultmäßig .00001]

Alle optionalen Werte können nachträglich mit dem entsprechenden Getter und
Setter abgefragt und geändert werden. Die Setter für die Verhaltensfaktoren
`alpha`, `beta`, `gamma` und `delta` prüfen die Sinnhaftigkeit der übergebenen
Werte und werfen eine Exception des Typs `invalid_value` mit einer
Fehlermeldung sollten diese nicht stimmen.

Ein Minimalbeispiel zum Rauskopieren:

```c++
coordesc_optimizer cdo(fn, {-1, -5}, .0000001);
cdo.optimize();
point min = cdo.current_point();

std::cout << "The minimum is at " << min.format() << "!\nNeeded "
          << cdo.iteration_count() << " iterations.\n";
```

Vorhergehendes Beispiel instanziiert ein Optimiererobjekt `cdo`, das die
Funktion `fn` optimieren soll, mit dem Startpunkt (-1, -5)
. Das Epsilon für die Abbruchbedingung wurde als .0005 gewählt. Danach wird
die `optimize`-Methode aufgerufen, die die Optimierung bis zum Erreichen des
Konvergenzkriteriums durchführt. Anschließend kann der gefundene Punkt mittels
`current_point()` abgerufen werden. Die `point`-Klasse liegt der
Implementierung bei und umfasst die wichtigsten Vektor-Operationen sowie einen
extrem coolen Initialisiererlisten-Konstruktor, der die kurze Anschreibweise im
Konstruktoraufruf ermöglicht. Weiters wichtig sind die `format`- und
`raw`-Methoden, die den Inhalt des Punktes jeweils in einen String gießen.
`format()` ist zur schönen Ausgabe gedacht und `raw()` als Zwischenmedium zur
Weitergabe des Punktes an externe Programme (wie etwa gnuplot).

Um den Algorithmus möglichst flexibel benutzen zu können, kann auch die Methode
`step` verwendet werden, um den Algorithmus schrittweise verfolgen zu können.
Auskunft über den Zustand gibt die `done` Methode. Folgendes Beispiel optimiert
eine Funktion wie das vorige, gibt bei jedem Schritt jedoch auch den
derzeitigen Simplex aus.

```c++
coordesc_optimizer cdo(fn, {-1, -5}, .0005);

while(!nmo.done()) {
    std::cout << "\nIteration #" << cdo.iteration_count() << '\n';
    point p = cdo.current_point();
    std::cout << "P = " << p.format() << '\n';
    cdo.step();
}
```

### Testprogramm

Das Testprogramm benötigt POSIX-Pipes (i.e. ist nur auf Linux und ggf. auch OS
X lauffähig) und gnuplot zum darstellen des Optimierungsverlaufes. Folgende
Funktionen sind vorimplementiert:

* Himmelblau-Funktion (Befehl: `himmelblau`)
* Rosenbrocks Bananen-Funktion (Befehl: `banana`)
* Matyas-Funktion (Befehl: `matyas`)
* Dreihöcker-Kamelfunktion (Befehl: `camel`)
* Booth-Funktion (Befehl: `booth`)
* Beale-Funktion (Befehl: `beale`)
* Beispielfunktion 1, `3*x**2 + y**2 - 3*x*y - 3*x` (Befehl: `example1`)
* Beispielfunktion 2, `y**4 + 2*x**2 - 3*x*y + 1` (Befehl: `example2`)
* Beispielfunktion 3, `3*x**2 + y + y**2` (Befehl: `example3`)
* Beispielfunktion 4, `5*x**2 - 6*x*y + 5*y**2 - 0.0259` (Befehl: `example4`)

Ein Aufruf sieht folgendermaßen aus: `./coordesc [FUNKTIONSNAME]`. Per
default schläft das Programm zwischen Iterationsschritten, um das betrachten
der Ausgabegraphen zu ermöglichen. Ist dies nicht genug, kann via
`./coordesc [FUNKTIONSNAME] manual` erwirkt werden, dass auf eine beliebige
Usereingabe gewartet wird.

Weiters ist eine kleine Informationsfunktion eingebaut. Mittels `./nelder_mead
info [FUNKTIONSNAME]` kann diese aufgerufen werden und liefert dann
beispielsweise solche Ausgaben:

```
$ ./nelder_mead info himmelblau

H I M M E L B L A U  function
The Himmelblau function is commonly used for benchmarking optimization
algorithms. It is defined as
                    2          2         2     2
        f(x, y) = (x  + y - 11)  + (x + y  - 7)

Its minima are at positions
    f(3.2, 2.0) = 0.0
    f(-2.805118, 3.131312) = 0.0
    f(-3.779310, -3.283186) = 0.0
    f(3.584428, -1.848126) = 0.0
```
Es werden zumindest die Funktionsdefinition sowie die Minima ausgegeben.

Werden dem Programm keine Argumente übergeben, beschwert es sich
dementsprechend und zeigt eine kleine Hilfe mit den unterstützten
Funktionalitäten an. Überschüssige Argumente werden ignoriert.
