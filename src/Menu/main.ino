//Main File

//Inculde file

////HomeMade
//#include "pins.h"


////Standard
#include "Bounce2.h"
#include "SoftwareSerial.h"
#include "LiquidCrystal.h"
#include "math.h"

#define RX_GPS     3  
#define TX_GPS     2

#define BPEN       17
#define BP0        16
#define BP1        15

#define pinBat     0

LiquidCrystal lcd(4,5,6,7,8,9);
// const long delay_LCD = 500; //Time refresh LCD
// unsigned long previousMillis_LCD = 0;
unsigned long lastMillis_BP = 0 ;
bool state_LCD = true; //true : on ; false : off
bool changeData_LCD = true;

int SW = 4; // Value of last button press
//Configuration Button bounce
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

char btn_push;

int const nb_level_menu(5);
int pos_menu[nb_level_menu];

// Define char for screen
byte char_arrow_left[8] = {
    0b00000,
    0b00000,
    0b11000,
    0b11100,
    0b11110,
    0b11100,
    0b11000,
    0b00000
};
byte char_arrow_up[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b01110,
    0b11111,
    0b11111,
    0b00000
};

byte char_arrow_down[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

byte char_select[8] = {
    0b00000,
    0b00000,
    0b00001,
    0b00011,
    0b10110,
    0b11100,
    0b01000,
    0b00000
};

byte char_back[8] = {
    0b00000,
    0b00000,
    0b00001,
    0b00101,
    0b01101,
    0b11111,
    0b01100,
    0b00100
};


char ReadKeypad()
{
    //Update bouton
    debouncerBPEN.update();
    debouncerBP0.update();
    debouncerBP1.update();

    if (debouncerBPEN.rose()){  
        if(debouncerBP1.read()){
            if(debouncerBP0.read()){//SW4
                SW = 4;
                return 'S';
            }else{//SW3
                SW = 3;}
                return 'B';
        }else{
            if(debouncerBP0.read()){//SW2
                SW = 2;
                return 'U';
            }else{//SW1
                SW = 1;}
                return 'D';
        } 
    }
}




void MainMenuBtn()
{
    if(btn_push == 'U')
    {
        pos_menu[0] ++;
    }
    else if(btn_push == 'D')
    {
        pos_menu[0] --;   
    }
    else if(btn_push == 'S')
    {
        for (int i(0) ; i < nb_level_menu ; i++)
        {
            pos_menu[i+1] = pos_menu[i];
        }   
    }
    
}


void setup()
{
    //Init tableau 
    for (int i(0) ; i < nb_level_menu ; i++)
    {
        pos_menu[i] = 0;
    }

    lcd.begin(8,2);  //Initialize a 2x8 type LCD

    // delay(1000);
    Serial.begin(115200);

    //LCD message when start
    lcd.createChar(0, char_arrow_up);
    lcd.createChar(1, char_arrow_down);
    lcd.createChar(2, char_select);
    lcd.createChar(3, char_back);
    lcd.createChar(4, char_arrow_left );

    //Affichage d'accueil
    lcd.setCursor(2,0);
    lcd.print("Hello");
    lcd.setCursor(3,1);
    lcd.print("GPS");

    delay(1000);

    lcd.clear();
    lcd.setCursor(0,1);
    lcd.write((uint8_t)0);
    lcd.setCursor(2,1);
    lcd.write((uint8_t)1);
    lcd.setCursor(5,1);
    lcd.write((uint8_t)2);
    lcd.setCursor(7,1);
    lcd.write((uint8_t)3);

    delay(1000);
    
    //MainMenuDisplay();
}

void loop()
{
    btn_push = ReadKeypad();
    Serial.println(btn_push);
    // MainMenuBtn();

    lcd.clear();
    for ( int i= nb_level_menu; i > 0; i--)
    {
        int a = 1+i ;
        lcd.setCursor(a,0);
        lcd.print(pos_menu[i]);
    }
    delay(100);
  
}