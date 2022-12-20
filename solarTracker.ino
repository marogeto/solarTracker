/*         _          _____              _             
          | |        |_   _|            | |            
 ___  ___ | | __ _ _ __| |_ __ __ _  ___| | _____ _ __ 
/ __|/ _ \| |/ _` | '__| | '__/ _` |/ __| |/ / _ \ '__|
\__ \ (_) | | (_| | |  | | | | (_| | (__|   <  __/ |   
|___/\___/|_|\__,_|_|  \_/_|  \__,_|\___|_|\_\___|_|   

Autor: Finn Wattenbach und Martin Rösner
email: finn.wattenbach@gmx.de
email: roesner@elektronikschule.de

*/
//Motor Vertikal
#define VENABLE         5
#define VODIR           3
#define VUDIR           4
//Motor Horizontal
#define HENABLE         8
#define HLDIR           6
#define HRDIR           7
// LDR's
#define obenrechtsLDR   3
#define obenlinksLDR    4
#define untenrechtsLDR  5
#define untenlinksLDR   6 

#define matrix         13
int i, ldrmax = 0; 
byte oben=B11110000;
byte unten=B00001111;
  
byte ro[4]={B00100000,B00100000,B00010000,B00111110};
byte lu[4]={B00010010,B00100010,B00100010,B00011100};
byte ui[4]={B00000100,B00000010,B00000010,B00111100};

Serial.println("            _          _____              _             ");
Serial.println("           | |        |_   _|            | |            ");
Serial.println("  ___  ___ | | __ _ _ __| |_ __ __ _  ___| | _____ _ __ ");
Serial.println(" / __|/ _ \| |/ _` | '__| | '__/ _` |/ __| |/ / _ \ '__|");
Serial.println(" \__ \ (_) | | (_| | |  | | | | (_| | (__|   <  __/ |   ");
Serial.println(" |___/\___/|_|\__,_|_|  \_/_|  \__,_|\___|_|\_\___|_|   ");
Serial.println();

void setup() {
  //--- Motor Vertikal ---
  pinMode(VENABLE,OUTPUT);
  pinMode(VODIR,OUTPUT);
  pinMode(VUDIR,OUTPUT);
  //--- Motor Horizontal ---
  pinMode(HENABLE,OUTPUT);
  pinMode(HLDIR,OUTPUT);
  pinMode(HRDIR,OUTPUT);
  //--- Monitor
  Serial.begin(9600);
  //--- Fotodioden ---
  //analogRead(3)
  //analogRead(4)
  
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0,false);
  // Helligkeit des Mediums setzen
  lc.setIntensity(0,matrix);
  // Display löschen
  lc.clearDisplay(0);
}

void solarTracker(){
  // Intensität als Spannungswert...
  orldr = analogRead(obenrechtsLDR); //3
  urldr = analogRead(untenrechtsLDR);//5
  olldr = analogRead(obenlinksLDR);  //4
  ulldr = analogRead(untenlinksLDR); //6

  // Maximale Intensität bestimmen
/*  if(ldr1 >= ldr2 && ldr1 > ldrmax)
    ldrmax = ldr1;
  else if(ldr2 >= ldr1 && ldr2 > ldrmax)
    ldrmax = ldr2;
*/
  ldrmax = 300;
  // Intensitäten aufgrund der maximalen Itensität
  // Berechnen
  int orint = (int) (ldr1*15)/ldrmax;
  int urint = (int) (ldr2*15)/ldrmax;
  int olint = (int) (ldr1*15)/ldrmax;
  int ulint = (int) (ldr2*15)/ldrmax;

  // Motor"stärke" einstellen
  //digitalWrite(VENABLE,HIGH); // enable on
  //digitalWrite(HENABLE,HIGH); // enable on 
  analogWrite(VENABLE,180);
  analogWrite(HENABLE,180);

  // Horizontale Bewegung
  if( orint <= olint || urint <= ulint ){
    // Bewegung nach rechts
    Serial.print(" -> ");
    digitalWrite(HRDIR,HIGH); //one way
    digitalWrite(HLDIR,LOW);
    delay(100);
  }
  else if( olint <= orint || ulint <= urint ){
    //Bewegung nach links
    Serial.print(" <- ");
    digitalWrite(HRDIR,LOW);  //reverse
    digitalWrite(HLDIR,HIGH);
    delay(100);
  }

  // Vertikale Bewegung
  if( orint <= urint || olint <= ulint ){
    // Bewegung nach unten
    Serial.println(" v ");
    digitalWrite(VUDIR,HIGH); //one way
    digitalWrite(VODIR,LOW);
    delay(100);
  }
  else if( urint <= orint || ulint <= olint ){
    //Bewegung nach oben
    Serial.println(" ^ ");
    digitalWrite(VODIR,HIGH); //one way
    digitalWrite(VUDIR,LOW);
    delay(100);
  }

  lc.setIntensity(0, orint);
  for(int m=0; m<=4; m++) lc.setRow(0,m,oben);
  delay(delaytime2);
  lc.clearDisplay(0);
  
  lc.setIntensity(0, urint);
  for(int m=0; m<=4; m++) lc.setRow(0,m,unten);
  delay(delaytime2);
  lc.clearDisplay(0);


  lc.setIntensity(0, int1);
  for(int m=4; m<=7; m++) lc.setRow(0,m,unten);
  delay(delaytime2);
  lc.clearDisplay(0);

  lc.setIntensity(0, int2);
  for(int m=4; m<=7; m++) lc.setRow(0,m,oben);
  delay(delaytime2);
  lc.clearDisplay(0); 
}


void loop() {
  Serial.println("One way, then reverse");
  solarTracker();
}
   
