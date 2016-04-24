#include "LiquidCrystal.h" //ajout de la librairie
#include "Bounce2.h"
#include "SD.h"

//Vérifier les broches !
LiquidCrystal lcd(4,5,6,7,8,9); //liaison 4 bits de données

const int BPEN=17;
const int BP0=16;
const int BP1=15;

const int pinBat = 0;

Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

int i = 0;

Sd2card sdCard


void setup()
{
   Serial.begin(115200);
   lcd.begin(8,2); //utilisation d'un écran 16 colonnes et 2 lignes
   lcd.print("Hello"); //petit test pour vérifier que tout marche
   
   pinMode(BPEN,INPUT_PULLUP);
   pinMode(BP0,INPUT_PULLUP);
   pinMode(BP1,INPUT_PULLUP);
   
   pinMode(pinBat,INPUT);
   
  float Vbat = map(analogRead(pinBat),0,1023,0,6);
  
  lcd.setCursor(0,0);
  lcd.print("Bat:");
  lcd.print(Vbat);
  
  Serial.println("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
  
  
   
}
 
void loop() {
    
  
}
