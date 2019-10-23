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

#### Wie & Warum SSH?
Möchte man auf einen headless RasPi zugreifen, muss dennoch eine Verbindung hergestellt werden – und zwar mittels **Secure Shell** (kurz: SSH). Die Secure Shell ist ein sicheres Netzwerkprotokoll, mit dessen Hilfe man eine Netzwerkverbindung zwischen jegliche im Netzwerk befindlichen Geräten herstellen kann. Die Authentifizierung innerhalb des SSH-Protokolls erfolgt unter der Nutzung von **kryptografischen Key Pairs** wordurch die Kommunikation zwischen zwei durch SSH verbundenen Geräte (Passwärter, Daten, etc) verschlüsselt übertragen werden. Jedem Netzwerktteilnehmer wird ein **Public Key** und ein **Private Key** zugeteilt, wobei letzterer streng geheimgehalten werden soll. Möchte man sich mit einem anderem Host (bspw. dem Raspberry) verbinden, benötigt man dessen **Host Key**.

In unserem Falle möchten wir eine Verbindung zwischen unserem Client (Client == PC == Laptop) und dem RasPi herstellen, um ihn mittels der Mac-Konsole ansteuern zu können. **SSH ist unsere einzige Verbindung zum Raspberry und ermöglicht. Mit Hilfe der Kommandozeile des Macs können wir Befehle über SSH an den RasPi senden.**

## Step 1 - Betriebssystem auf dem Raspberry Pi 3 installieren
Wer seinen RasPi neu gekauft hat, muss zunächst erst einmal das Betriebssystem aufsetzen. Dazu kursieren bereits zahlreiche Anleitungen im Internet. Wer mit Montior, Tastatur & Maus arbeitet, dem empfehlen wir diesen Beitrag: https://www.datenreise.de/raspberry-pi-inbetriebnahme-howto/

**Headless (für Mac) sieht der Ablauf wie folgt aus:**
* Micro-SD-Karte (mind. 8GB) mit Adapter in den Mac stecken und formatieren (wenn nicht gänzlich neu)
* Raspbian-Installationsdatei auf [www.raspberrypi.org](https://www.raspberrypi.org/) herunterladen
* die SD-Karte "flashen" (also in unserem Falle die Raspbian-Software in den ROM-Speicherbereich der SD-Karte schreiben)
  * folgender Kommand wird dazu benötigt, wobei die beiden Pfade individuell angepasst werden müssen
  ```
  sudo dd bs=1m if=/dein/pfad/zur/Installationsdatei.img of=/dev/deineDisk123
  ```
  * Kommand um die Bezeichnung der aktiven disks herauszufinden
  ```
  diskutil list
  ```
  * vor dem flashen der SD-Karte, muss man die entsprechende disk (respektive die SD-Karte) unmounten
  ```
  // Wenn "/dev/disk5: Resource busy", dann SDk-Karte unmounte
  sudo diskutil unmountDisk /dev/deineDisk123
  ```
Achtung: nach ausführen des Kommands wird die Disk geflasht -> es erscheint vorerst keine Ausgabe auf der Konsole, da dieser Vorgang    etwas Zeit in Anspruch nimmt. Mit der Tastenkombination Ctrl+T kann jedoch der Fortschritt abgefragt werden. Ist unser Mac mit dem flashen fertig, steckt man die Micro-SD-Karte in den RasPi, verbindet ihn via Netztteil mit Strom – und fertig.

## Step 2 - SSH für den Raspberry aktivieren (headless in der Boot-Partition)

Jetzt, da Raspbian auf unserer SD-Karte installiert wurde, können wir erste Einstellungen vornehmen. Die SD-Karte verbleibt dabei zunächst im Mac. Um unseren Raspberry per SSH mit unserem MAC verknüpfen zu können, müssen wir zunächst SSH auf dem Raspberry aktivieren, den SSH ist aus Sichheitsgründen (Stichwort: gleiche Benutzername und gleiches Passwort für alle Raspberries) standardmäßig deaktiviert. Zum aktivieren von SSH ohne Tastatur, Maus und Bildschirm kann man auf der SD-Speicherkarte in der Boot-Partition eine leere Datei mit dem Namen "ssh" (keine Datei-Endung!) erstellen. Wenn Raspbian später gestartet wird, dann wird SSH aktiviert und die Datei automatisch gelöscht.

## Step 3 - Raspberry in Betrieb nehmen

* Micro-SD-Karte in den Raspberry Pi stecken
* Raspberry mit Strom versorgen

**Hinweis rote LED:** Die rote LED zeigt den Status der Stromversorgung an. Wenn diese durchgehend leuchtet, liegt eine ausreichende Stromversorung von 5V vor. Sollte die rote LED anfangen zu blinken ist die Stromversorgung unter 4,63 Volt gefallen und es liegt ein Problem mit der Spannungsversorung vor.

**Hinweis grüne LED:** Die grüne LED zeigt den Status der SD-Karte an. Die LED leuchtet auf, wenn auf die SD-Karte zugegriffen wird (Lesen/Schreiben von Dateien). Sollte die grüne LED durchgängig leuchtenn, kann nicht auf die SD-Karte zugegriffen werden. Im schlimmsten falle bedeutet dieser Zustand, dass die SD-Karte lerr oder gar defekt ist.


## Step 4 - Raspberry Pi 3 via SSH mit dem Laptop verbinden
Den RasPi kann man auf zwei Wege mit dem Client verbinden: per **IP-Adresse** des Raspberry's im Netzwerk oder per **Hostnamen**. 

**Default-User: pi**
**Default-Host: raspberrybi
**Default-Passwort: raspberry**

**Via IP-Adresse:** Die Netzwerkadresse kann schnell und einfach über das Webinterface eures Routers ausgelesen werden. Wahlweise gibt es unzählige MAC-CLI-Befehle, unter anderem:

```
$ arp -a | grep raspberry
$ arp -a | grep raspi
```

Die einfachste **Variante (MIT Head)** ist es die IP-Adresse direkt in der Konsole des Raspberry's auszulesen
```
$ hostname -I
// wahlweise kann man auch nach der inet Adresse suchen
$ ifconfig
```

Der Username ist nach der ersten Inbetriebnahme standardmäßig gesetzt, sodass sich folgender Befehl ergibt: ssh *[username]*@*[IP-Adresse]*
```
$ ssh pi@192.168.x.xxx
```
Unter Usmständen gibt die Konsole folgenen Ausgabe zurück.

```
The authenticity of host '192.168.x.xxx (192.168.x.xxx)' can't be established.
ECDSA key fingerprint is SHA256:JH+C1P6D1lg6bOxSs7An56RIYHFAzIEnG4FN2pR8DWI.
Are you sure you want to continue connecting (yes/no)?
```
In unserem Falle ist das "genau richtig", denn es bestätigt, dass das kryptografische Verfahren von SSH prinzipiell funktioniert und wir uns zum aller ersten mal mit einem nicht authentifizierten Gerät (also dem RasPi) verbinden wollen und unser MAC den entsprechenden **Public Host Key** vom RasPi noch nicht authentifiziert hat. **ECDSA ist ein Algorithmus** der vom Host Key verwendet wird. Lange rede kurzer Sinn: wir bestätigen die Anfrage der Konsole mit "y + Enter", um den **Public Host Key** vom RasPi zu den **Known Host Keys** hinzuzufügen.

**Via Hostname:** Der Hostname ist ebenso wir der Username per default vom Raspberry vordefiniert und lautet rasberrypi. Möchte man sich via Hostnamen mit dem RasPI verbinden, ist folgender Befehl notwendig:
```
ssh pi@raspberrypi
```
Bevor es mit Step 5 weitergeht muss eine gültige SSH-erbingung zwischen Mac (Laptop) und RasPi  bestehen.
<img align="right" width="80%" src="images/WebRadio/SSH-success.png">


## Step 5 - Finally: mit dem Radio verbinden
Um sich mit dem Radio zu verbinden benötigt man zunächst die MAC-Adresse des Radios. Der Einfacheit halber kann man diese direkt über die Systemeinstellungen des MAC (Laptop) auslesen: Systemeinstellungen --> Bluetooth --> Geräte scannen --> Rechtsklick auf das Radio --> Adresse notieren. Die MAC-Adresse wir immer im *kanonischen Format* angegeben: 12-34-56-78-9A-BC





