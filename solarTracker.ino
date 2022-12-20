
//Motor Vertikal
#define VENABLE         5
#define VODIR           3
#define VUDIR           4
//Motor Horizontal
#define HENABLE         8
#define HLDIR           6
#define HRDIR           7
// LDR's
#define obenrechtsLDR   9
#define obenlinksLDR   10
#define untenrechtsLDR 11
#define untenlinksLDR  12

#define matrix         13
int i, ldrmax = 0; 
byte oben=B11110000;
byte unten=B00001111;
  
byte ro[4]={B00100000,B00100000,B00010000,B00111110};
byte lu[4]={B00010010,B00100010,B00100010,B00011100};
byte ui[4]={B00000100,B00000010,B00000010,B00111100};


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
  //Serial.print(analogRead(3));
  //Serial.println(analogRead(4))
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,matrix);
  /* and clear the display */
  lc.clearDisplay(0);

}

void solarTracker(){
  // Intensität als Spannungswert...
  orldr = analogRead(obenrechtsLDR);
  urldr = analogRead(untenrechtsLDR);
  olldr = analogRead(obenlinksLDR);
  ulldr = analogRead(untenlinksLDR);
  
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

  // Horizontale Bewegung
  if( orint <= olint || urint <= ulint ){
    // Bewegung nach rechts
  }
  else if( olint <= orint || ulint <= urint ){
    //Bewegung nach links
  }

  // Vertikale Bewegung
  if( orint <= urint || olint <= ulint ){
    // Bewegung nach unten
  }
  else if( urint <= orint || ulint <= olint ){
    //Bewegung nach oben
  }

    //digitalWrite(ENABLE,HIGH); // enable on
  analogWrite(ENABLE,180);



  for (i=0;i<5;i++) {
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    delay(1000);
    digitalWrite(DIRA,LOW);  //reverse
    digitalWrite(DIRB,HIGH);
    delay(1000);
  }
  Serial.print(ldr1);
  Serial.print(" ---- ");
  Serial.print(ldr2);
  Serial.print(" ---- ");
  Serial.print(ldrmax);
  Serial.print(" ---- ");
  Serial.print(int1);
  Serial.print(" ---- ");
  Serial.println(int2);

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
  solartracker();
}
   
