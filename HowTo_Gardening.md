<img align="right" width="160" src="images/logo-technologiestiftung.jpg">
<img align="right" width="160" src="images/citylab-logo-2160x550.jpg">


&nbsp;
# Bauanleitung Gardening Box
## Workshop vom 29. November 2019

<img align="top" width="100%" src="images/Gardening/Gardening_Schaltplan.png">

#### Schaltplan für die Gardening Box bestehend aus (v.l.n.r.): Pumpe, MOSFET Modul, Mikrocontroller und Feuchtigkeitssensor 
### Die Idee
Die Novemberedition der offenen Werkstatt widmet sich dem Bau winterfester Kleingartenparadiese. Wir konstruieren vollautomatisch bewässerte Beete für die Fensterbank. Eine Vollspektrum LED, Feuchtigkeitssensoren und Pumpen helfen zeitgenössischen Stadtgärtner\*innen bei der Arbeit. 

### Hardware
* Arduino Nano Mikrocontroller
* Feuchtigkeitssensor
* Pumpe
* MOSFET (zur Regelung des Arbeitsstromkreis' der Pumpe)
* Netzteil (für LED & Arduino)

Die einzelnen Bauelemente müssen entsprechend des Schaltkreises (siehe Abbildung oben) miteinander verkabelt werden. Die Vollspektrum LED taucht im Schaltplan nicht auf, da sie eigenständig, über ein externes Netzkabel versorgt wird (aber dazu später mehr).

### Software – Setup
Damit die Pumpe automatisiert durch den Arduniona angeteuert werden kann, muss dieser entsprechend programmiert werden. Dazu schließt man den Mikrocontroller zunächst via Mini-USB-Anschluss an seinen Client (Laptop, ThinClient, PC, etc.) an und öffnet die Arduino IDE. Wer die Entwicklungsumgebung von Arduino bis dato noch nicht installiert hat, sollte das an dieser Stelle mit Hilfe der [offiziellen Website](https://www.arduino.cc/en/main/software) tun.

Damit das man seinen Sketch (aka. Programmcode aka. Algorithmus) auf den Arduino Nano via USB übertragen kann, müssen zunächst drei Voreinstellungen in der Arduino IDE getroffen werden. Unter:
**1.** Tools -> **Board** -> "Arduino Nano" auswählen
**2.** Tools -> **Prozessor** -> "Atmega 328P" auswählen (Achtung: wenn Arduino nicht original, dann "Old Bootloader" auswählen )
**3.** Tools -> **Port** -> USB Port des Computers auswählen

Falls deine Arduino IDE den Arduino Nano (CH340) trotz aller Konfigurationen nicht erkennt, dann downloade den für Deinen Computer passenden USB Treiber. Entweder die Windows Version, Mac OS oder Linux.
* **Windows:** [CH341SER WIN USB Treiber](http://www.wch.cn/download/CH341SER_ZIP.html)
* **Mac:** [CH341SER MAC OS Treiber](http://www.wch.cn/download/CH341SER_MAC_ZIP.html)
* **Linux:** [CH341 Linux auf Github](https://github.com/juliagoda/CH341SER)

### Software – Code verstehen
Ihr findet den Code "Gardening.ino" in diesem GitHub Repository im Order [Codes und Sketches](https://github.com/technologiestiftung/werkstatt/tree/master/codes_sketches). Nun ein paar Worte zum Aufbau. 

Zunächst werden alle Variablen initialisiert, die wir für den Programmcode benötigen
```javascript
int messwert=0; //Integer Variable die später unseren Feuchtigkeits-Messwert speichert
int temp = 0;//Integer Variable, die wir benötigen, um den Wasserfüllstand zu überprüfen
int Pump=5; //Integer Pumpe wird mit dem Wert 5 inititalisiert
#define Sensor A0 //#define weißt dem Pin "A0" den konstanten Namen "Sensor" zu
```
Gemäß des standardmäßigen Aufbaus einer .ino-Datei folgen im Anschluss genau zwei Funktionen: **die setup() und die loop() Funktion**. Die setup() wird jedes Mal aufgerufen, wenn der Sketch startet. Sie soll benutzt werden, um Variablen, Pinmodi, Bibliotheken, usw. zu initialisieren. Nach der setup() Funktion folgt die loop() Funktion. Sie beinhaltet Programmcode, der kontinuierlich in einer unendlichen Schleife ausgeführt wird - Eingänge auslesen, Ausgänge triggern, etc. Diese Funktion erledigt die Hauptarbeit des Sketches.

#### setup()
Mit der Funktion pinMode() werden verschiedene Pins entweder als INPUT, OUTPUT oder INPUT_PULLUP konfigugiert. Schauen wir uns die beiden Pins an, die wir entsprechend unserer Schaltung belegt haben.
```javascript
  pinMode(5, OUTPUT); // setzt den Digital-Pin 5 als Ausgang
  pinMode(Sensor,INPUT_PULLUP); // setze den (analaogen) Pin A0 als Eingang und aktiviert das Pullup
```
Das Pullup aktiviert die internen Pullup-Widerstände auf dem Arduino Board. Hat zur Folge, dass unser Sensor im unbeschalteten Zustand auf HIGH liegt und entsprechend Messwerte sammelt. Genauer wird an dieser Stelle nicht auf die Innenwiederstände Potentiale eingeagnen.

#### loop()
Mit der Funktion analogRead() kann der Messwert an einem analogen Pin ausgelesen werden. Da unser Sensor an Pin A0 anliegt, wollen wir den Sensor an Pin A0 auslesen. 
```javascript
 messwert=analogRead(Sensor); //Messwert an der Stelle "Sensor" wird ausgelesen und in der Variablen "messwert gespeichert"
```

#### if – else if – else
In if-else-Schleifen kann mit Hilfe von Bedingungen auf bestimmte Ereignisse – in unserem Fall "Erde trocken" bzw. "Erde ausreichend feucht" – reagiert werden, wobei der Programmcode **innerhalb** der Schleife nur dann ausgeführt wird, wenn die Bedingung der Schleife erfüllt ist. Um die Bedingungen für die Schleife zu denifieren müssen wir zunächst wissen, welche Werte unser Sensormesswert annehmen kann. Nach ein paar Testläufen haben wir uns dafür entschieden den Schwellenwert auf 960 zu setzen, da bei Wert 960 die Erde trocken war. Heißt: Ist der Messwert, den der Sensor misst größer als 960, dann schalten wir die Pumpe an und lassen sie für 10sec pumpen.

```javascript
if (messwert>960) { //Bedingung
digitalWrite(Pump, HIGH); //Pumpe pumpt...
delay(10000); //... fuer 10 sec
temp=analogRead(Sensor); //neuen Sensormesswert nach pumpen messen
}
```

In der **zweiten Schleife (else if)** fragen wir unsere zweite Bedingung ab: Wenn der Messwert, den wir nach dem pumpen gemessen haben ("temp") immer noch größer als 960 ist, dann schalten wir die Pumpe aus. Für unsere Bewässerungssystem kann das mehere Bedeutungen haben: entweder ist die Pumpe kaputt oder der Wassertank ist leer. In beiden Fällen jedoch macht es Sinn, dass man die Pumpe ausschaltet.

```javascript
else if (temp>960){ // zweite Bedingung
  digitalWrite(Pump, LOW);
}
```
In der **dritten Schleife (else)** sagen wir dem Programmcode, was er machen soll, wenn die erste und zweite Bedingung jeweils **nicht** erfüllt ist. In unserem Falle hat der Sensor einen Wert gemessen, der kleiner als 960 ist ("Erde feuchter") und wir können die Pumpe ausschalten.

```javascript
else{
  digitalWrite(Pump, LOW);
}
```
Hat man den Code fertig geschrieben muss er nur noch compiliert werde (auf das Häckchen oben links in der Arduino IDE klicken) und ihn auf das Board laden (Upload-Pfeil daneben anklicken). 

## Finally: Strom anschließen
lorem ipsum

Bildung war hier
