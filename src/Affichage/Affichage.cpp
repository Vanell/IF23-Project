#include "LiquidCrystal.h"

LiquidCrystal lcd(LCD_RS,LCD_Enable,LCD_D4,LCD_D5,LCD_D6,LCD_D7);

const long delay_LCD = 750; //Time refresh LCD
unsigned long previousMillis_LCD = 0;
bool state_LCD = true; //true : on ; false : off
bool changeData_LCD = true;

int SW = 4; // Value of last button press
//Configuration Button bounce
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

unsigned long lastMillis_BP = 0 ;
unsigned long lastMillis_BP = 0 ;


//Menu
char btn_push;

////Def structure menu
int const nb_level_menu(4);
int pos_menu[nb_level_menu];
int max_screen_lvl[nb_level_menu];

bool mode_itinerary = false;

// Define char for screen
byte char_arrow_left[8] = {0b00000,0b00000,0b11000,0b11100,0b11110,0b11100,0b11000,0b00000};
byte char_arrow_up[8]   = {0b00000,0b00000,0b00000,0b00100,0b01110,0b11111,0b11111,0b00000};
byte char_arrow_down[8] = {0b00000,0b00000,0b00000,0b11111,0b11111,0b01110,0b00100,0b00000};
byte char_select[8]     = {0b00000,0b00000,0b00001,0b00011,0b10110,0b11100,0b01000,0b00000};
byte char_back[8]       = {0b00000,0b00000,0b00001,0b00101,0b01101,0b11111,0b01100,0b00100};

//Battery
float Vbat; //Value of batterie
int percentBat;
float autonomy;
float autom;
float vmin = 4.5;
float v0;
unsigned long t0;


//Menu function
char ReadKeypad()
{
	char charBPN;
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
					charBPN = 'S';
				}else{//SW3
					charBPN = 'B';
				}
			}else{
				if(debouncerBP0.read()){//SW2
					charBPN = 'D';
				}else{//SW1
					charBPN = 'U';
				}
			} 
		// }
	}
	return charBPN;
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
				lcd.print((int)data_GPS[0]);
				lcd.setCursor(0,1);
				lcd.print("HDOP ");
				lcd.print((int)data_GPS[1]);
				break;
			case 2:
				lcd.print(data_GPS[4]);
				lcd.print(" m");
				lcd.setCursor(0,1);
				lcd.print(data_GPS[5]);
				lcd.print(" km/h");
				break;
			case 3:
				lcd.print(data_GPS[2]);//Lattitude
				lcd.setCursor(0,1);
				lcd.print(data_GPS[3]);//Longitude
				break;
			case 4:
				lcd.print("Date");
				lcd.setCursor(0,1);
				lcd.print("Time");
				break;
			case 5:
				Vbat = mapfloat(analogRead(pinBat),0,1023,0,6.2);
				percentBat = mapfloat(analogRead(pinBat),0,1023,0,100);
				autonomy = 18-(6.2 -Vbat)/0.11;
				lcd.setCursor(0,1);
		    	lcd.print("Bat:");
	    		lcd.print(Vbat);
	    		lcd.print((int)autonomy);
	    		lcd.print(percentBat);
	    		lcd.print("% ");
	    		lcd.print((int)autonomy);
	    		lcd.print(" h");
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
			// case 3:
			// 	lcd.write((uint8_t)4);
			// 	lcd.print("Option");
			// 	break;
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
				//appelle fonction prise point
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
					mode_itinerary = !mode_itinerary;
					//appelle function itineraire
				}else
				{
					lcd.print("Itinerary");
					lcd.setCursor(0,1);
					lcd.print("Progress");
					mode_itinerary = !mode_itinerary;
					//appelle function itineraire
				}
				delay(1000);
				//Facon sale pour le retour en arrière 
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			// case 3 : //Choose option
			// 	switch (pos_menu[0])
			// 	{
			// 		case 1 : 
			// 		//Option 1
			// 		lcd.print("Option 1");
			// 		break;
			// 		case 2 : 
			// 		//Option 2
			// 		lcd.print("Option 2");
			// 		break;
			// 		case 3 : 
			// 		//Option 3
			// 		lcd.print("Option 3");
			// 		break;

			// 	}
			// 	break;
		}
	}
	// else if (pos_menu[3] > 0 && pos_menu[2] == 3)//optioyn 
	// {
	// 	switch(pos_menu[1])
	// 	{
	// 		case 1 : 
	// 			//Option 1
	// 			lcd.print("Opt 1");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 		case 2 : 
	// 			//Option 2
	// 			lcd.print("Opt 2");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 		case 3 : 
	// 			//Option 3
	// 			lcd.print("Opt 3");
	// 			lcd.setCursor(0,1);
	// 			lcd.print("Choose");
	// 			delay(750);
	// 			break;
	// 	}
	// 	pos_menu[0] = pos_menu[3];
	// 	pos_menu[1] = 0;
	// 	pos_menu[2] = 0;
	// 	pos_menu[3] = 0;
	// 	changeData_LCD = true;
	// }
	else 
	{
		lcd.print("Error");
		lcd.setCursor(0,1);
		lcd.print("Menu");
		delay(250);
	}
}