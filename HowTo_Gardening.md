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

### Software – Vorbereitungen
Damit die Pumpe automatisiert durch den Arduniona angeteuert werden kann, muss dieser entsprechend programmiert werden. Dazu schließt man den Mikrocontroller zunächst via Mini-USB-Anschluss an seinen Client (Laptop, ThinClient, PC, etc.) an und öffnet die Arduino IDE. Wer die Entwicklungsumgebung von Arduino bis dato noch nicht installiert hat, sollte das an dieser Stelle mit Hilfe der [offiziellen Website](https://www.arduino.cc/en/main/software) tun.

Damit das man seinen Sketch (aka. Programmcode aka. Algorithmus) auf den Arduino Nano via USB übertragen kann, müssen zunächst drei Voreinstellungen in der Arduino IDE getroffen werden. Unter:
1. Tools -> Board -> "Arduino Nano" auswählen
2. Tools -> Prozessor -> "Atmega 328P" auswählen (Achtung: wenn Arduino nicht original, dann "Old Bootloader" auswählen )
3. Tools -> Port -> USB Port des Computers auswählen

Falls deine Arduino IDE den Arduino Nano (CH340) trotz aller Konfigurationen nicht erkennt, dann downloade den für Deinen Computer passenden USB Treiber. Entweder die Windows Version, Mac OS oder Linux.
* **Windows:** [CH341SER WIN USB Treiber](http://www.wch.cn/download/CH341SER_ZIP.html)
* **Mac:** [CH341SER MAC OS Treiber](http://www.wch.cn/download/CH341SER_MAC_ZIP.html)
* **Linux:** [CH341 Linux auf Github](https://github.com/juliagoda/CH341SER)

### Software – Code verstehen
Den Code haben wir natürlich schon erstellt und getestet. Ihr findet den Code "Gardening.ino" in diesem GitHub Repository im Order [Codes und Sketches](https://github.com/technologiestiftung/werkstatt/tree/master/codes_sketches). Nun ein paar Worte zum Aufbau. 

Zunächst werden alle Variablen initialisiert, die wir für den Programm code benötigen
```python
int messwert=0; #Variable die später unseren Feuchtigkeits-Messwert speichert
int temp = 0; #Temporäre Variable, die wir benötigen, um den Wasserfüllstand zu überprüfen
int Pump=5; #Variable Pumpe wird mit dem Wert 5 inititalisiert
define Sensor A0 #hier wird die Variable "Sensor" dem analogen Pin "A0" zugeordnet (gem. Schaltplan)
```
Gemäß des standardmäßigen Aufbaus einer .ino-Datei folgen im Anschluss genau zwei Funktionen: die setup() und die loop() Funktion. Die setup() wird jedes Mal aufgerufen, wenn der Sketch startet. Sie soll benutzt werden, um Variablen, Pinmodi, Bibliotheken, usw. zu initialisieren. Nach der setup() Funktion folgt die loop() Funktion. Sie beinhaltet Programmcode, der kontinuierlich in einer unendlichen Schleife ausgeführt wird - Eingänge auslesen, Ausgänge triggern, etc. Diese Funktion erledigt die Hauptarbeit des Sketches.

## Step 2 - lorem ipsum
lorem ipsum
```console
$ lorem ipsum
```


Bildung war hier
