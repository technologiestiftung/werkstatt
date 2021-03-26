# Display 

## Text ausgeben

1. Lade displaytext.ino (liegt bei dialogstarter/code)

```
void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Morgen ist");	// write something to the internal memory
  u8g2.drawStr(50,20,"Samstag");
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(3000);  
}
```
2. Schreibe eigenen Text:
`u8g2.drawStr(x, y, "blabla");`  x,y geben die Position des Texts an. Wobei x eine Zahle zwischen 0 und 128 ist und y eine Zahl zwischen 0 und 32

## Bilder ausgeben

1. Lade displayxpm.ino
2. Füge deinen eigenen Icon ein

## Das .xbm Format funktioniert gut! So sieht ein Pferd aus


```
#define tier_width 32
#define tier_height 32
static unsigned char tier_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00,
   0x60, 0x03, 0x00, 0x00, 0x30, 0x06, 0x00, 0x00,
   0x18, 0x0c, 0x00, 0x00, 0xcc, 0x19, 0x00, 0x00,
   0xf6, 0xf1, 0xff, 0x03, 0x3e, 0xe3, 0xff, 0x07,
   0x0c, 0x06, 0xc0, 0x06, 0x00, 0x06, 0xc0, 0x06,
   0x00, 0x06, 0xc0, 0x06, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0xfe, 0xff, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   */
```
## Wo bekomme ich .xbm Bilddatein her?

1. im Ordner : https://github.com/Sarapedia/werkstatt/blob/master/dialogstarter/xbmexamples.md

   
   


## xbm Editor

just run it in your python environment
https://sourceforge.net/projects/simple-xbm-image-editor/files/

<img align="center" width="80%" src="images/editor.png">

### Outputexample

```
#define tier_width 32
#define tier_height 32
static unsigned char tier_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00,
   0x60, 0x03, 0x00, 0x00, 0x30, 0x06, 0x00, 0x00,
   0x18, 0x0c, 0x00, 0x00, 0xcc, 0x19, 0x00, 0x00,
   0xf6, 0xf1, 0xff, 0x03, 0x3e, 0xe3, 0xff, 0x07,
   0x0c, 0x06, 0xc0, 0x06, 0x00, 0x06, 0xc0, 0x06,
   0x00, 0x06, 0xc0, 0x06, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0xfe, 0xff, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x06, 0xc0, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
```

https://imagemagick.org/script/command-line-processing.php



Kann ich Ihnen
duzen?


Warum spreche
ich nicht? Willst
du das wissen?,
„Ja, ich bin taub
und es ist auch
nicht schlimm,
dafür gibt es ja
hier.


Du wirst auf eine
einsame Insel
verbannt und
darfst nur einen
Gegenstand
mitnehmen.
Welcher wäre
das?


Erzähl drei lustige
Sachen über dich
selbst in
Gestikulieren, was
ich wissen soll!

Warum sprichst
du mich an?

Wie, glaubst du,
hört sich Musik
an?


Darfst du Auto
fahren?


Was ist taube
Kultur?


Mann, hier ist es
voll, nicht wahr?


Sieht’s aus, dass
wir zur Party
gehen, vielleicht
zur derselben
Party? :)

Was ist deine
Labels?

Haben wir
eigentlich wirklich
Kommunikationspr
oblem?

Kannst Du mir was
ganz Einfaches in
Gebärdensprache
beibringen?


Wie ist, Sex zu
haben, ohne das
Stöhnen des
Partners zu
hören?


Wie flirtest du?


Zeig mir wie ich
mit dir
kommunizieren
kann!


Wie fühlst du dich
denn als du grad
in der Situation
mit einer tauben
Person bist?


Was ist dir lieber:
Taub oder blind?

Kannst du
schreiben oder
gebärden?


Ich spreche in
Gebärdensprache
und was ist deine
Muttersprache?

Ist die
Gebärdensprache
international?

Wie war es für
deine Eltern zu
merken, dass du
gehörlos bist?

Wenn du ein
Geräusch hören
könntest, welches
würdest du dir
wünschen?

Gibt es eine
Kulturkonflikt
zwischen tauben
und hörenden
Menschen?


Wenn du Chef der
BVG wärst, was
würdest du
ändern?

Gibt es eine
Erfahrung, die
dein Leben
nachhaltig
verändert hat?

Hast du dich
schon einmal mit
taube Person
getroffen?

Ist eine
Paarbeziehung
zwischen tauben
und hörenden
möglich?

Kannst du den
Lippen ablesen?


Lebst du in einer
Parallelgesellschaf
t?

Wann macht dir
die Stille Angst?

Ist es schwer,
Gebärdensprache
zu lernen?

Wie kann die
BVG uns „lieben“,
obwohl sie sich
immer verspätet
und sich als
unzuverlässig
erweist?


Was haltest du
davon, dass ein/e
Fremde/r dich
anspricht?

Bist du jetzt frisch,
mit mir zur Café
zu gehen?

Ist es schwer,
Lautsprache und
lippelesen zu
lernen?


Ist es unangenehm,
in der Öffentlichkeit
Gebärdensprache
zu benutzen, weil
dich alle anstarren?


Gebärdensprache
wird vorgeworfen,
sehr primitiv zu
sein. Redest du
primitiv?

Was muss ich
noch beachten,
bevor ich mit dir
zur Café gehe?

Was denkst du
über die
Hörenden?

