<img align="right" width="160" src="images/logo-technologiestiftung.jpg">
<img align="right" width="160" src="images/citylab-logo-2160x550.jpg">


&nbsp;
# Bauanleitung Web Radio
## Remote Workshop für zu Hause

<img align="top" width="100%" src="images/WebRadio/Header_WebRadio.jpg">

#### In dieser Bauanleitung wollen wir zeigen, wie man aus einem Bluetooth-fähigen Radio und einem Raspberry Pi 3 ein Web Radio baut. Genauer: konfiguriert. Denn gebaut wurde das Radio ja schon.

### Die Idee
Man nehme ein Bluetooth-fähiges Radio, verknüpft dieses via Bluetooth mit einen RasPi, welcher wiederum mit dem lokalen WIFI verknüpft ist, und steuert über den Raspberry einen x-beliebigen Webradio-Sender an. In diesem Beispiel hier, haben wir die Konfigurationen in ein Shell-Skript geschrieben und dieses schließlich mit dem Autostart des Raspberrys gepackt. Und fertig ist der "autonome" RasPi ;-)

### Hardware
* (d)ein Laptop/PC
* ein beliebiges Radio **mit Bluetooth-Funktion**
* Raspberry Pi 3 + SD-Karte + Netzteil (https://www.rasppishop.de/RPi3-A-Plus-Bundle)

In dieser Bauanleitung arbeiten wir mit dem Raspberry Pi 3. Der Vorteil im Vergleich zu den Vorgängermodellen: ab Modell Nummer 3 wurde ein boardinternes Chipset für die WIFI- und Bluetooth-Funktion verbaut, sodass wir keine zusätzlichen Komponenten benötigen. Wer lieber mit dem Arduino arbeitet und/oder ein paar Euro sparen möchte, der kann das Radio mit einem Arduino Uni WIFI (built-in WIFI) konfigurieren. Die Commands dafür werden hier allerdings nicht erläutert.

### First Things First: Headless-Betrieb mit Secure Shell
Bevor es losgeht muss eine Entscheidung getroffen werden: möchte ich meinen RasPi headless oder "mit head" benutzen? Der Headless-Betrieb bedeutet, dass der Mikrocontroller ohnen Monitor, Maus und ohne Tastatur betrieben wird. Im Gegensatz dazu kann man an die USB bzw. HDMI-Anschlüssen des Mini-Rechners auch entsprechende Hardware anschließen. Aus Platz- und Kostengründen wird der RasPi jedoch oft headless betrieben.  

Möchte man auf einen headless RasPi zugreifen, muss dennoch eine Verbindung hergestellt werden – und zwar mittels **Secure Shell** (kurz: SSH). Die Secure Shell ist ein Netzwerkprotokoll, mit dessen Hilfe man eine Netzwerkverbindung zwischen allen im Netzwerk befindlichen Geräten herstellen kann. In unserem Falle möchten wir eine Verbindung zwischen unserem Client (Client == PC == Laptop) und dem RasPi herstellen, um ihn mittels der Konsole (CLI) ansteuern zu können.

## Step 1 - Betriebssystem auf dem Raspberry Pi 3 installieren
Wer seinen RasPi neu gekauft hat, muss zunächst erst einmal das Betriebssystem aufsetzen. Dazu kursieren bereits zahlreiche Anleitungen im Internet. Wer mit Montior, Tastatur & Maus arbeitet, dem empfehlen wir diesen Beitrag: https://www.datenreise.de/raspberry-pi-inbetriebnahme-howto/

Headless (für Mac) sieht der Ablauf der Installation wie folgt aus:
* Micro-SD-Karte (mind. 8GB) mit Adapter in den Mac stecken und formatieren (wenn nicht gänzlich neu)
* Raspbian-Installationsdatei auf [www.raspberrypi.org](https://www.raspberrypi.org/) herunterladen
* die SD-Karte "flashen" (also in unserem Falle die Raspbian-Software in den ROM-Speicherbereich der SD-Karte schreiben)
  * folgender Kommand wird dazu benötigt, wobei die beiden Pfade individuell angepasst werden müssen
  ```
  sudo dd bs=1m if=/dein/pfad/zur/Installationsdatei.img of=/deine/disk123
 
  ```
  * Kommand um die Bezeichnung der aktiven disks herauszufinden
  ```
  diskutil list
  ```
  * vor dem flashen der SD-Karte, muss man die entsprechende disk (respektive die SD-Karte) unmounten
  ```
  dd: /dev/disk5: Resource busy
  ```
  * Achtung: nach ausführen des Kommands wird die Disk geflasht -> es erscheint vorerst keine Ausgabe auf der Konsole, da dieser Vorgang    etwas Zeit in Anspruch nimmt. Mit der Tastenkombination Ctrl+T kann jedoch der Fortschritt abgefragt werden

* Micro-SD-Karte


## Step 2 - Raspberry Pi 3 via SSH mit dem Laptop verbinden
Um den RasPi mit dem Client zu verbinden, benötigt man zunächst die Netzwerkadresse des Mikrokontrollers.
**MAC Konsole** 
```console
$ arp -a | grep raspberry
```




