# Einführung

Bei der Koordinatenabstiegsmethode wird bei jedem Schritt eine Koordinate ```i``` gewählt, nach der man optimiert. Es wird bei einem beliebigen Startpunkt begonnen und in jedem Iterationsschritt setzt man alle Koordinaten des aktuellen Punktes in die Funktion ein, nur die Koordinate, nach der man minimieren will, behält man als Variable bei.

Es wird immer in Richtung einer Achse minimiert und die Stelle, an der das Minimum ist, als neue Koordinate beibehalten und so gelangt man mit einer Anzahl an Iterationen zum Minimum der Funktion.

Wenn zum Beispiel die Funktion ```f(x,y)=2x^{2}+2xy+1.5y^{2}``` und der Startpunkt ```P=(1,\,1)``` gegeben ist, so muss man, wenn entlang der ```x```-Achse minimiert werden soll folgendes eingesetzt werden:

    x = 1+1d
    y = 1+0d

Soll entlang der $y$-Achse optimiert werden, so muss folgendes eingesetzt werden:

    x = 1+0d
    y = 1+1d

# Beispiel

Wir möchten die Funktion ```f(x,y)=2x^{2}+2xy+1.5y^{2}``` optimieren und wählen den Startpunkt ```P_{0}=(1,\,1)```. Wir minimieren zuerst entlang der ```x```-Achse und setzen ein

    f(d) = 2(1+d)^{2}+2(1+d)+1.5
         = 2d^{2}+6d+5.5

Diese soeben erhaltene Funktion müssen wir nun noch minimieren.

    f'(d) = 4d+6
    4d+6 = 0
    d = -3/2

Setzen wir dies in die Gleichung für die ```x```-Koordinate ein und verwenden die alte ```y```-Koordinate erhalten wir den neuen Punkt ```P_{1}=(-1/2 , 1)```. Nun minimieren wir nach ```y```, wir setzen also ein:

    x = -1/2 + 0d
    y = 1+1d

und erhalten

    f(d)=1.5d^{2}-d+6

Dies minimieren wir wiederum und erhalten die neue ```y```-Koordinate ```1/3```. Damit haben wir den neuen Punkt ```(-1/2 , 1/3)```.
Dieser Vorgang wird wiederholt, bis sich die errechneten Werte nurnoch marginal unterscheiden. Danach wird abgebrochen.
