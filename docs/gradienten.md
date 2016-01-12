# Die Idee und der Zweck des Gradientenverfahren

Grundsätzlich ist das Gradientenverfahren ein Optimierungsverfahren, welches das Minimum einer mehrdimensionalen Funktion errechnen. Der Gradient ```∇f(x,y)``` einer Funktion ```f(x)``` zeigt, aus später gezeigten Gründen, immer in die Richtung des steilsten Anstiegs, folglich zeigt ```-∇f(x,y)``` in die Richtung des stärksten Abstiegs. Das Gradientenverfahren macht sich dies zunutze und man "wandert" eine gewisse Dauer in diese Richtung, wie "lange" man in die errechnete Richtung "wandert" hängt unter anderem auch von der gewählten Methode ab.

# Der Ablauf des Gradientenverfahrens
* Es wird ein beliebiger Startpunkt ```P=(x,y)``` ausgewählt.
* Es wird die Ableitung der Funktion ```f(x,y)```, also ```∇f(x)``` gebildet. Sollte dieser Gradient bereits den Wert null haben sind wir bereits fertig und haben ein lokales Minimum.
* Der Punkt ```P``` wird in den Gradienten eingesetzt und der negative Wert berechnet, also ```-∇f(x_{P},y_{P})```.
* Um eine neuen Punkt ```P^([n+1])``` zu finden stellen wir nun folgende Gleichung auf: ```P^([n+1]) = P^([n]) + α[n] * s^([n])```. Nun haben wir die Richtung in die optimiert werden muss gefunden (```s^([n])``` (```=-∇f(x_{P},y_{P})```)), jetzt muss noch entschieden werden wie weit wir in diese Richtung "wandern" (```α[n]```, liegt zwischen ```0``` und ```1```) (Hinweis: ```n``` ist die aktuelle Iteration).

Das kann man einerseits mit Hilfe eines fixen Werts machen, andererseits auch mit einer Folge (wie z.B ```1 / sqrt(n)```)
oder einer jedes mal neu errechneten Schrittweite. Zu beachten ist, dass die ausgewählte Folge auf jeden Fall divergent sein muss, da man sonst bis zu einer maximalen Entfernung "wandern"
kann und die Wahrscheinlichkeit groß ist das Optimum nie zu erreichen. Sollte man jedes mal einen fixen Wert verliert man zwar den Aufwand für die Berechnung der Schrittweite, jedoch ist die Wahrscheinlichkeit hoch, dass man viele Iterationen benötigt oder in einer endlosen Schleife festhängt.

Es gibt verschieden Arten wie man die Schrittweite berechnen kann, im Folgenden werde ich die Schrittweitenbestimmung nach Armijo erklären. Dieses Verfahren ist eines der sogenannten "line-search" Verfahren, das von mir gezeigte ist jedoch kein exaktes line-search Verfahren sondern lediglich eine einfache Heuristik.

## Die Armijo-Bedingung

    φ(α) ≤ φ(0) + c * α^{[n]} * φ(0)'.

Wobei ```c``` eine Konstante zwischen null und eins ist und dazu dient das die Bedingung nicht zu restriktiv ist, ```φ(α) = f(P^([n]) + α^([n]) * s^([n]))``` und ```φ(0)' = ∇f(P^{n}) * s^{[n]}```

*  Für den Anfang wird ```α=1``` gewählt und errechnet ob die Armijo-Bedingung erfüllt ist - sollte sie erfüllt sein haben wir ein passende ```α``` gefunden, wenn nicht wird ```α``` mit einem ```β```, welches ebenfalls zwischen null und eins liegt, multipliziert und wieder geprüft ob die Ungleichung erfüllt ist. (Hinweis: In der Praxis liefern die Werte ```c = 0.01``` und ```β = 0.9``` häufig gute Ergebnisse. [1](https://www.unibw.de/lrt1/gerdts/lehre/lpnlp/lp-nlp.pdf) [2](https://www.unibw.de/lrt1/gerdts/lehre/lpnlp/lp-nlp.pdf)
*  Wenn man einen neuen Punkt gefunden hat wird bei Punkt 3 fortgesetzt. Da es oft einen großen Rechenaufwand erfordert das genaue Minimum zu finden wird das Verfahren meist beendet wenn der Gradient einen akzeptablen Wert erreicht hat (So kann man als Abbruchbedingung z.B. die stärke des Anstiegs in einem Punkt (also den Betrag des Gradienten) nehmen).
