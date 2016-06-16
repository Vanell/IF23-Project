#include "LiquidCrystal.h"
#include "Arduino.h"
#include "Bounce2.h"
#include <pin.h>
#include <Affichage.h>
LiquidCrystal lcd(LCD_RS,LCD_Enable,LCD_D4,LCD_D5,LCD_D6,LCD_D7);

Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);


////FUNCTIONS////

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



//Menu function
char ReadKeypad()
{
	char charBPN;
	//Update bouton
	debouncerBPEN.update();
	debouncerBP0.update();
	debouncerBP1.update();
	if (debouncerBPEN.rose()){
		changeData_LCD = true;
		if(debouncerBP1.read()){
			if(debouncerBP0.read()){//SW4
				charBPN = 'S';
			}else{//SW3
				charBPN = 'B';
			}
		}
		else{
			if(debouncerBP0.read()){//SW2
				charBPN = 'D';
			}else{//SW1
				charBPN = 'U';
			}
		} 
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
		for(int i = 3;i>0;i--){
			pos_menu[i]=pos_menu[i-1];
		}
		pos_menu[0] = 1 ;
	}
	else if(btn_push == 'B')
	{
		if (pos_menu[1] > 0)
		{
			for(int i =0;i<3;i++){
				pos_menu[i] = pos_menu[i+1];
			}
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

void MainMenuDisplay(float data_GPS[])
{
	lcd.clear();        
	lcd.setCursor(0,0);
	//Battery
	float Vbat; //Value of batterie
	char percentBat;
	float autonomy;

	if (pos_menu[1] <= 0)//Root menu
	{
		switch (pos_menu[0])
		{
			case 1:
				lcd.print(F("NbSat "));
				lcd.print((int)data_GPS[0]);
				lcd.setCursor(0,1);
				lcd.print(F("HDOP "));
				lcd.print((int)data_GPS[1]);
				break;
			case 2:
				lcd.print(data_GPS[11]);
				lcd.print(F(" m"));
				lcd.setCursor(0,1);
				lcd.print(data_GPS[12]);
				lcd.print(F(" km/h"));
				break;
			case 3:
				lcd.print(data_GPS[2]);//Lattitude
				lcd.setCursor(0,1);
				lcd.print(data_GPS[3]);//Longitude
				break;
			case 4:
				lcd.print((int)data_GPS[5]);
				lcd.print(F("/"));
				lcd.print((int)data_GPS[6]);
				lcd.print(F("/"));
				lcd.print((int)data_GPS[7]);
				lcd.setCursor(0,1);
				lcd.print((int)data_GPS[8]);
				lcd.print(F(":"));
				lcd.print((int)data_GPS[9]);
				lcd.print(F(":"));
				lcd.print((int)data_GPS[10]);
				break;
			case 5:
				Vbat = mapfloat(analogRead(pgm_read_word(&pinBat)),0,1023,0,6.2);
				percentBat = mapfloat(analogRead(pgm_read_word(&pinBat)),0,1023,0,100);
				autonomy = 18-(6.2 -Vbat)/0.11;
				lcd.setCursor(0,0);
		    	lcd.print(F("Bat:"));
	    		lcd.print(Vbat);
	    		lcd.setCursor(0,1);
	    		lcd.print(percentBat);
	    		lcd.print(F("% "));
	    		lcd.print((int)autonomy);
	    		lcd.print(F("h"));
				break;
		}
	}else if (pos_menu[1] > 0 && pos_menu[2] <= 0) // Menu
	{
		switch(pos_menu[0])
		{
			case 1:
				lcd.write((uint8_t)4);
				lcd.print(F("Pr pts"));
				lcd.setCursor(1,1);
				if(mode_itinerary)
				{
					lcd.print(F("Stop iti"));
				}else
				{
					lcd.print(F("Pr iti"));
				}
				break;
			case 2:
				lcd.setCursor(1,0);
				lcd.print(F("Pr pts"));
				lcd.setCursor(0,1);
				lcd.write((uint8_t)4);

				if(mode_itinerary)
				{
					lcd.print(F("Stop iti"));
				}else
				{
					lcd.print(F("Pr iti"));
				}
			break;
		}
	}else if ( pos_menu[2] > 0 && pos_menu[3] <= 0) //Menu choose
	{
		switch(pos_menu[1])
		{
			case 1 : 
				//Launch function take point
				lcd.print(F("Point"));
				lcd.setCursor(0,1);
				lcd.print(F("Took !"));
				delay(750);
				takePoint=true;
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

			case 2 : 
				//Launch function take itinerary
				if (mode_itinerary)
				{
					lcd.print(F("Itinerary"));
					lcd.setCursor(0,1);
					lcd.print(F("Stop"));
					mode_itinerary = !mode_itinerary;
				}else
				{
					lcd.print(F("Itinerary"));
					lcd.setCursor(0,1);
					lcd.print(F("Progress"));
					mode_itinerary = !mode_itinerary;
				}
				delay(1000);
				pos_menu[0] = pos_menu[2];
				pos_menu[1] = 0;
				pos_menu[2] = 0;
				changeData_LCD = true;
				break;

		}
	}
	else 
	{
		lcd.print(F("Error"));
		lcd.setCursor(0,1);
		lcd.print(F("Menu"));
		delay(250);
	}
	return;
}