## Nelder-Mead-Verfahren

Das Nelder-Mead-Verfahren wird zur Optimierung nichtlinearer Funktionen eingesetzt und ist in der Praxis weit verbreitet. Die Grundidee besteht darin, dass n+1 unabhängige Punkte gewählt werden (also ein Simplex gebildet wird) aus denen der „schlechteste“ Punkt ermittelt wird und dann durch einen anderen ersetzt wird. Dadurch nähert sich der Algorithmus schrittweise dem Optimum an, wobei er in manchen Fällen auf lokale Nebenoptima konvergieren kann. Hierfür gibt es Anpassungen des Nelder-Mead-Verfahrens, um auch solche Probleme zu lösen.

## Abstiegsverfahren

Das Abstiegsverfahren ist ein weiteres Verfahren zur Optimierung einer Funktion. Ein Vektor d heißt Abstiegsrichtung von Funktion f in einem Punkt x, falls es ein T gibt, sodass gilt: ```f (x + td) < f (x)``` für jedes ```0 < t < T```. Dieser Vektor zeigt also in die Richtung, in welcher der Wert der Funktion in allen Punkten kleiner als T weniger als der Funktionswert bei x ist. Wir können auch sagen, dass wenn ```∇f (x)T d < 0``` gilt, dann ist d die Abstiegsrichtung von f im Punkt x. Um ein lokales Minimum mit dieser Methode zu finden, muss man zuerst die Abstiegsrichtung in einem beliebigen Startpunkt finden. Dann bewegt man sich zu dem Punkt, in dessen Richtung der Abstiegsvektor zeigt, und berechnet dort erneut den Abstiegsvektor. Man wiederholt dies, bis kein neuer Vektor berechnet werden kann oder die erforderliche Genauigkeit erreicht wurde.

## Gradientenverfahren

Grundsätzlich macht sich das Gradientenverfahren den Fakt zunutze, dass der Gradient immer in die Richtung des stärksten Anstiegs zeigt. Dementsprechend zeigt natürlich der negative Gradient in die Richtung des stärksten Abstiegs. Nun „geht“ man eine gewisse Zeit in diese Richtung bis man einen neuen Punkt hat von dem man abermals den Gradienten ausrechnet. Die unterschiedlichen Gradientenverfahren unterscheiden sich nun darin wie weit man in die Richtung des negativen Gradienten gehen soll. Im theoretischen Teil wird sowohl stärker auf das Gradientenverfahren an sich, als auch auf die Unterschiede zwischen den verschiedenen Arten eingegangen werden.

## Koordinatenabstiegsmethode

Bei der Koordinatenabstiegsmethode wird in jedem Schritt eine Koordinate i gewählt, nach der optimiert wird. Der Startpunkt ist beliebig wählbar und der folgende Vorgang wird mehrmals wiederholt. Bei jedem Durchgang werden alle Koordinaten des aktuellen Punktes x k in die Funktion eingesetzt, nur die gewählte Koordinate i behält man als Variable bei. Die dadurch entstandene eindimensionale Funktion wird minimiert und die Koordinate i wird mit dem gefundenen Wert ersetzt. Dieser Vorgang wird mit allen Koordinaten wiederholt. Nun kann man diesen gesamten Vorgang erneut anwenden, bis sich der Punkt nicht mehr oder nur kaum verändert; danach wird abgebrochen.
