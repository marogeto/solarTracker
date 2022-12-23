/*         _          _____              _             
          | |        |_   _|            | |            
 ___  ___ | | __ _ _ __| |_ __ __ _  ___| | _____ _ __ 
/ __|/ _ \| |/ _` | '__| | '__/ _` |/ __| |/ / _ \ '__|
\__ \ (_) | | (_| | |  | | | | (_| | (__|   <  __/ |   
|___/\___/|_|\__,_|_|  \_/_|  \__,_|\___|_|\_\___|_|   

Autor: Finn Wattenbach und Martin Rösner
email: finn.wattenbach@gmx.de
email: roesner@elektronikschule.de
Version: Alpha
Lizenz: GPL-3.0
*/

//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(12, 10, 11, 1);

// Motor Vertikal
#define VENABLE 5
#define VODIR 3
#define VUDIR 4
// Motor Horizontal
#define HENABLE 8
#define HLDIR 6
#define HRDIR 7
// LDR's
#define obenrechtsLDR 3
#define obenlinksLDR 4
#define untenrechtsLDR 5
#define untenlinksLDR 6
// Taster
#define horlinksTaster 22
#define horrechtsTaster 23
#define vertuntenTaster 24
#define vertobenTaster 25


#define matrix 13

/* image switching time */
unsigned long delaytime1 = 500;
unsigned long delaytime2 = 2000;
unsigned int  drehzeit = 50;
int i, ldr1, ldr2, ldrmax = 0;
int orldr, urldr, olldr, ulldr;
int orint, urint, olint, ulint;
bool voTaster, vuTaster, hlTaster, hrTaster;
byte pfeilLinks[8]       = { B00000000, B00010000, B00111000, B01111100, B00010000, B00010000, B00010000, B00000000 };
byte pfeilRechts[8]      = { B00000000, B00000000, B00010000, B00010000, B00010000, B01111100, B00111000, B00010000 };
byte pfeilUnten[8]       = { B00000000, B00000000, B00100000, B01100000, B11111100, B01100000, B00100000, B00000000 };
byte pfeilOben[8]        = { B00000000, B00000000, B00001000, B00001100, B01111110, B00001100, B00001000, B00000000 };
byte pfeilRechtsOben[8]  = { B00000000, B00000000, B00000000, B00100000, B00010000, B00001010, B00000110, B00001110 };
byte pfeilLinksUnten[8]  = { B00000000, B11100000, B11000000, B10100000, B00010000, B00001000, B00000000, B00000000 };
byte pfeilLinksOben[8]   = { B00000000, B00001110, B00000110, B00001010, B00010000, B00100000, B00000000, B00000000 };
byte pfeilRechtsUnten[8] = { B00000000, B00000000, B00000000, B00001000, B00010000, B10100000, B11000000, B11100000 };

void setup() {
  //--- Motor Vertikal ---
  pinMode(VENABLE, OUTPUT);
  pinMode(VODIR, OUTPUT);
  pinMode(VUDIR, OUTPUT);
  //--- Motor Horizontal ---
  pinMode(HENABLE, OUTPUT);
  pinMode(HLDIR, OUTPUT);
  pinMode(HRDIR, OUTPUT);
  // --- Taster
  pinMode(horlinksTaster, INPUT);   //22
  pinMode(horrechtsTaster, INPUT);  //23
  pinMode(vertuntenTaster, INPUT);  //24
  pinMode(vertobenTaster, INPUT);   //25
  //--- Monitor
  Serial.begin(9600);
  Serial.println("            _          _____              _             ");
  Serial.println("           | |        |_   _|            | |            ");
  Serial.println("  ___  ___ | | __ _ _ __| |_ __ __ _  ___| | _____ _ __ ");
  Serial.println(" / __|/ _ \| |/ _` | '__| | '__/ _` |/ __| |/ / _ \ '__|");
  Serial.println(" \__ \ (_) | | (_| | |  | | | | (_| | (__|   <  __/ |   ");
  Serial.println(" |___/\___/|_|\__,_|_|  \_/_|  \__,_|\___|_|\_\___|_|   ");
  Serial.println();
  //--- Fotodioden ---
  //analogRead(3)
  //analogRead(4)

  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Helligkeit des Mediums setzen
  lc.setIntensity(0, matrix);
  // Display löschen
  lc.clearDisplay(0);
}

void solarTracker() {
  // Intensität als Spannungswert...
  orldr = analogRead(obenrechtsLDR);   //3
  urldr = analogRead(untenrechtsLDR);  //5
  olldr = analogRead(obenlinksLDR);    //4
  ulldr = analogRead(untenlinksLDR);   //6
  // Schalter für die Drehbereiche
  // Die Schalter befinden sich im High Zustand
  // sobald sie geschlossen sind gehen sie auf LOW
  voTaster = digitalRead(vertobenTaster);
  vuTaster = digitalRead(vertuntenTaster);
  hlTaster = digitalRead(horlinksTaster);
  hrTaster = digitalRead(horrechtsTaster);

  // Maximale Intensität bestimmen
  if (orldr >= urldr && orldr > ldrmax)
    ldrmax = orldr;
  else if (urldr >= orldr && urldr > ldrmax)
    ldrmax = urldr;

  ldrmax = 200;
  // Intensitäten aufgrund der maximalen Itensität
  // Berechnen
  int orint = (int)(orldr * 1.5) / ldrmax;
  int urint = (int)(urldr * 1.5) / ldrmax;
  int olint = (int)(olldr * 1.5) / ldrmax;
  int ulint = (int)(ulldr * 1.5) / ldrmax;

  // Motor"stärke" einstellen
  //analogWrite(VENABLE,180);
  //analogWrite(HENABLE,180);
  digitalWrite(VENABLE, HIGH);
  digitalWrite(HENABLE, HIGH);
  
  // Horizontale Bewegung
  if ((orint < olint || urint < ulint) &&  hrTaster) {
    // Bewegung nach rechts
    Serial.print(" -> ");
    digitalWrite(HRDIR, HIGH);  //one way
    digitalWrite(HLDIR, LOW);
    delay(drehzeit);
    digitalWrite(HRDIR, LOW);
  } else if ((olint < orint || ulint < urint) && hlTaster) {
    //Bewegung nach links
    Serial.print(" <- ");
    digitalWrite(HRDIR, LOW);  //reverse
    digitalWrite(HLDIR, HIGH);
    delay(drehzeit);
    digitalWrite(HLDIR, LOW);
  } else {
    digitalWrite(HRDIR, LOW);
    digitalWrite(HLDIR, LOW);
    delay(100);
  }

  // Vertikale Bewegung
  if (orint < urint) {  //|| olint < ulint) && vuTaster){
    // Bewegung nach unten
    Serial.print(" v  : ObenRechts/UntenRechts: ");
    Serial.print(orint);
    Serial.print(" | ");
    Serial.println(urint);
    digitalWrite(VUDIR, HIGH);
    digitalWrite(VODIR, LOW);
    delay(drehzeit);
    digitalWrite(VUDIR, LOW);
  } else if (urint < orint) {  // || ulint < olint) && voTaster){
    //Bewegung nach oben
    Serial.print(" ^  : ObenRechts/UntenRechts: ");
    Serial.print(orint);
    Serial.print(" | ");
    Serial.println(urint);
    digitalWrite(VODIR, HIGH);
    digitalWrite(VUDIR, LOW);
    delay(drehzeit);
    digitalWrite(VODIR, LOW);
  } else {
    Serial.print(" -  : ObenRechts/UntenRechts: ");
    Serial.print(orint);
    Serial.print(" | ");
    Serial.println(urint);
    digitalWrite(VODIR, LOW);
    digitalWrite(VUDIR, LOW);
    delay(100);
  }

  lc.setIntensity(0, orint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilLinks[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, olint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilLinksUnten[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, olint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilUnten[m]);
  delay(delaytime2);
  lc.clearDisplay(0);
  
  lc.setIntensity(0, olint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilRechtsUnten[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, urint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilRechts[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, urint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilRechtsOben[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, ulint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilOben[m]);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, ulint);
  for (int m = 0; m <= 7; m++) lc.setRow(0, m, pfeilLinksOben[m]);
  delay(delaytime2);
  lc.clearDisplay(0);
}


void loop() {
  solarTracker();
}
