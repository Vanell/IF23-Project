#include "LiquidCrystal.h"
#include "Arduino.h"
#include "Bounce2.h"
#include <pin.h>
#include <Affichage.h>
//#include <SDGPS.cpp>
//#include <SDGPS.cpp>

//LiquidCrystal lcd(LCD_RS,LCD_Enable,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
LiquidCrystal lcd(4,5,6,7,8,9);

Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce debouncerBP0 = Bounce(BP0,5);
Bounce debouncerBP1 = Bounce(BP1,5);

//Content of dataGPS:
// 0 NBSAT
// 1 HDOP
// 2 LATITUDE
// 3 LONGITUDE
// 4 AGE
// 5 YEAR
// 6 MONTH
// 7 DAY
// 8 HOURS
// 9 MINUTES
// 10 SECONDS
// 11 ALTITUDE
// 12 SPEED


////FUNCTION////

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
		for(int i = 3;i>0;i--){
			pos_menu[i]=pos_menu[i-1];
		}
		// pos_menu[3] = pos_menu[2];
		// pos_menu[2] = pos_menu[1];
		// pos_menu[1] = pos_menu[0];
		pos_menu[0] = 1 ;
	}
	else if(btn_push == 'B')
	{
		if (pos_menu[1] > 0)
		{
			//faire une putain de boucle for ...=
			for(int i =0;i<3;i++){
				pos_menu[i] = pos_menu[i+1];
			}
			// pos_menu[0] = pos_menu[1]; 
			// pos_menu[1] = pos_menu[2]; 
			// pos_menu[2] = pos_menu[3]; 
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
				lcd.setCursor(0,1);
		    	lcd.print(F("Bat:"));
	    		lcd.print(Vbat);
	    		lcd.print((int)autonomy);
	    		lcd.print(percentBat);
	    		lcd.print(F("% "));
	    		lcd.print((int)autonomy);
	    		lcd.print(F(" h"));
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
				lcd.print(F("Pr iti"));
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
				lcd.print(F("Point"));
				lcd.setCursor(0,1);
				lcd.print(F("Took !"));
				delay(750);
				//appelle fonction prise point
				takePoint=true;
				// writeWP2File("waypoint.txt", "testWayPoint",data_GPS);
				// Serial.println("Return from writing function");
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
					lcd.print(F("Itinerary"));
					lcd.setCursor(0,1);
					lcd.print(F("Stop"));
					mode_itinerary = !mode_itinerary;
					//appelle function itineraire
				}else
				{
					lcd.print(F("Itinerary"));
					lcd.setCursor(0,1);
					lcd.print(F("Progress"));
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
		lcd.print(F("Error"));
		lcd.setCursor(0,1);
		lcd.print(F("Menu"));
		delay(250);
	}
	return;
}