//Main File

//Inculde file

////HomeMade
//#include "pins.h"


////Standard
#include "Bounce2.h"
//#include "SoftwareSerial.h"
#include "LiquidCrystal.h"
//#include "math.h"

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

int const nb_level_menu(4);
int pos_menu[nb_level_menu];
int max_screen_lvl[nb_level_menu];

bool mode_itinerary = false;
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

		// lastMillis_BP = millis();
		
		// if (!state_LCD)
		// {
		// 	lcd.display();
		// 	state_LCD = true;
		// }
		// else
		// {
			changeData_LCD = true;
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
					return 'D';
				}else{//SW1
					SW = 1;}
					return 'U';
			} 
		// }
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
		//faire une putain de boucle for ...
		pos_menu[3] = pos_menu[2];
		pos_menu[2] = pos_menu[1];
		pos_menu[1] = pos_menu[0];
		pos_menu[0] = 1 ;
	}
	else if(btn_push == 'B')
	{
		if (pos_menu[1] > 0)
		{
			//faire une putain de boucle for ...
			pos_menu[0] = pos_menu[1]; 
			pos_menu[1] = pos_menu[2]; 
			pos_menu[2] = pos_menu[3]; 
			pos_menu[3] = 0;
		}else{
			pos_menu[0] = 5 ; // Menu batterie 
		}
	}

	//Gestion des boucles de menu
	if (pos_menu[1] <= 0 && pos_menu[0] > max_screen_lvl[0])
	{
		pos_menu[0] = 1;
	}
	else if (pos_menu[1] <= 0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[0];
	}
	
	else if (pos_menu[1] > 0 && pos_menu[0] > max_screen_lvl[1])
	{
		pos_menu[0] = 1;
	}else if (pos_menu[1] >0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[1];
	}

	else if (pos_menu[2] > 0 && pos_menu[0] > max_screen_lvl[2])
	{
		pos_menu[0] = 1;
	}else if (pos_menu[2] >0 && pos_menu[0] <= 0 )
	{
		pos_menu[0] = max_screen_lvl[2];
	}
}

void MainMenuDisplay()
{
	lcd.clear();        
	lcd.setCursor(0,0);

	if (pos_menu[1] <= 0)//Root menu
	{
		switch (pos_menu[0])
		{
			case 1:
				lcd.print("Nb Sat ");
				lcd.setCursor(0,1);
				lcd.print("HDOP ");
				break;
			case 2:
				lcd.print("Altitude");
				lcd.setCursor(0,1);
				lcd.print("Vitesse");
				break;
			case 3:
				lcd.print("Lattitude");
				lcd.setCursor(0,1);
				lcd.print("Longitude");
				break;
			case 4:
				lcd.print("Date");
				lcd.setCursor(0,1);
				lcd.print("Time");
				break;
			case 5:
				lcd.print("Voltage");
				lcd.setCursor(0,1);
				lcd.print("Autonomie");
				break;
		}
	}else if (pos_menu[1] > 0 && pos_menu[2] <= 0) // Menu
	{
		switch(pos_menu[0])
		{
			case 1:
				lcd.write((uint8_t)4);
				lcd.print("Pr pts");
				lcd.setCursor(1,1);
				lcd.print("Pr iti");
				break;
			case 2:
				lcd.setCursor(1,0);
				lcd.print("Pr pts");
				lcd.setCursor(0,1);
				lcd.write((uint8_t)4);

				if(mode_itinerary)
				{
					lcd.print("Stop iti");
				}else
				{
					lcd.print("Pr iti");
				}
			break;
			case 3:
				lcd.write((uint8_t)4);
				lcd.print("Option");
				break;
		}
	}else if ( pos_menu[2] > 0 && pos_menu[3] <= 0) //Menu choose
	{
		switch(pos_menu[1])
		{
			case 1 : 
				//Launch functuion take point
				lcd.print("Point");
				lcd.setCursor(0,1);
				lcd.print("Took !");
				delay(750);
				//Facon sale pour le retour en arrière 
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			case 2 : 
				//Launch function take itinerary
				if (mode_itinerary)
				{
					lcd.print("Itinerary");
					lcd.setCursor(0,1);
					lcd.print("Stop");
				}else
				{
				lcd.print("Itinerary");
				lcd.setCursor(0,1);
				lcd.print("Progress");
				}
				delay(750);
				//Facon sale pour le retour en arrière 
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			case 3 : //Choose option
				switch (pos_menu[0])
				{
					case 1 : 
					//Option 1
					lcd.print("Option 1");
					break;
					case 2 : 
					//Option 2
					lcd.print("Option 2");
					break;
					case 3 : 
					//Option 3
					lcd.print("Option 3");
					break;

				}
				break;
		}
	}else if (pos_menu[3] > 0 && pos_menu[2] == 3)//optioyn 
	{
		switch(pos_menu[1])
		{
			case 1 : 
				//Option 1
				lcd.print("Opt 1");
				lcd.setCursor(0,1);
				lcd.print("Choose");
				delay(750);
				break;
			case 2 : 
				//Option 2
				lcd.print("Opt 2");
				lcd.setCursor(0,1);
				lcd.print("Choose");
				delay(750);
				break;
			case 3 : 
				//Option 3
				lcd.print("Opt 3");
				lcd.setCursor(0,1);
				lcd.print("Choose");
				delay(750);
				break;
		}
		pos_menu[0] = pos_menu[3];
		pos_menu[1] = 0;
		pos_menu[2] = 0;
		pos_menu[3] = 0;
		changeData_LCD = true;
	}else 
	{
		lcd.print("Error");
		lcd.setCursor(0,1);
		lcd.print("Menu");
		delay(250);
	}
}

void setup()
{
	//Init tableau menu 
	for (int i(0) ; i < nb_level_menu ; i++)
	{
		pos_menu[i] = 0;
	}
	pos_menu[0] = 1;
	max_screen_lvl[0] = 5;//Root menu
	max_screen_lvl[1] = 3;// Menu
	max_screen_lvl[2] = 3;//Option

	lcd.begin(8,2);  //Initialize a 2x8 type LCD

	// delay(1000);
	Serial.begin(9600);

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

	delay(750);

	lcd.clear();
	lcd.setCursor(0,1);
	lcd.write((uint8_t)0);
	lcd.setCursor(2,1);
	lcd.write((uint8_t)1);
	lcd.setCursor(5,1);
	lcd.write((uint8_t)2);
	lcd.setCursor(7,1);
	lcd.write((uint8_t)3);

	delay(750);
	
}

void loop()
{
	unsigned long currentMillis = millis();

	btn_push = ReadKeypad();
	MainMenuBtn();

	if (changeData_LCD) 
	{
		changeData_LCD = false;
		MainMenuDisplay();
	}
	
	// lcd.clear();
	// lcd.setCursor(2,0);
	// lcd.print(pos_menu[2]);
	// lcd.print(pos_menu[1]);
	// lcd.print(pos_menu[0]);
	
	
	// if (currentMillis - lastMillis_BP >= 10000 && state_LCD)//Turn of screen after 5 min 300 000 ms
	// {
	// 	lcd.noDisplay();
	// 	state_LCD = false;
	// }

	delay(10);
}