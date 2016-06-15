//Main File

//Inculde file



////Standard
#include <avr/pgmspace.h>
////HomeMade
#include <GPS.cpp>
#include <SDGPS.cpp>
#include <Affichage.cpp>




unsigned long previousMillis_Iti =0;
float data_GPS_Loop[13];
String wpFile="waypoint.txt";
String itiFile="itinary.txt";




////MAIN////

void setup()
{
	//Input button 
	pinMode(BPEN,INPUT_PULLUP);
	pinMode(BP0,INPUT_PULLUP);
	pinMode(BP1,INPUT_PULLUP);

	//Input batterie
	pinMode(pinBat,INPUT);
	
	//LCD begin
	lcd.begin(8,2);	
	lcd.clear();
	lcd.setCursor(0,0);

	////Create Char for LCD
	// lcd.createChar(0, char_arrow_up);
	// lcd.createChar(1, char_arrow_down);
	// lcd.createChar(2, char_select);
	// lcd.createChar(3, char_back);
	// lcd.createChar(4, char_arrow_left );

	//Begin serial computer
	Serial.begin(19200);

	//Begin serial GPS
	ss.begin(4800);

	//Init menu 
	for (int i(0) ; i <4 ; i++)
	{
		pos_menu[i] = 0;

	}
	pos_menu[0] = 1;
	max_screen_lvl[0] = 5;//Root menu
	max_screen_lvl[1] = 2;// Menu
	//max_screen_lvl[2] = 3;//Option

	for(int i=0;i<13;i++){
		data_GPS_Loop[i]=0;
		//Serial.println(data_GPS_Loop[i]);
	}

	//INITIALISATION SD
	pinMode(SD_SS,OUTPUT);

	if (!SD.begin(pgm_read_word(&SD_SS))){
		lcd.setCursor(0,0);
		lcd.print(F("SD NO OK"));
   		//Serial.println("initfail");
		delay(5000);	
    	return;
	}

	//Serial.println("SD Init Ok");
	lcd.setCursor(2,0);
	lcd.print(F("SD OK"));
	delay(500);

	//LCD message when start
	lcd.setCursor(2,0);
	lcd.print(F("Hello"));
	lcd.setCursor(3,1);
	lcd.print(F("GPS"));

	delay(750);

	lcd.clear();
	lcd.setCursor(0,1);
	// lcd.write((uint8_t)0);
	// lcd.setCursor(2,1);
	// lcd.write((uint8_t)1);
	// lcd.setCursor(5,1);
	// lcd.write((uint8_t)2);
	// lcd.setCursor(7,1);
	// lcd.write((uint8_t)3);

	delay(750);
	Serial.println("in");

}

void loop()
{
	
	btn_push = ReadKeypad();
	MainMenuBtn();

	if (changeData_LCD || ( pos_menu[1]<= 0 && newData))
	{

		changeData_LCD = false;
		MainMenuDisplay(data_GPS_Loop);
		//previousMillis_LCD=millis();

	}

	//Serial
	

	String command="";
	String name = "";
	while(Serial.available()>0){
		command=command+(char)Serial.read();
	}

	if(command.length()>5){
		name = command.substring(4);
		//Serial.println(name);
		//Serial.println(command.substring(0,3));
		if(command.substring(0,3) == "sen"){
			//Serial.println("gotta send");
			readFile(name);

		}
		if(command.substring(0,3) == "rem"){
			char strchar[name.length()+1];
    		name.toCharArray(strchar, sizeof(strchar));
			SD.remove(strchar);
		}

	}
	else{

		if(command == "rd"){
			//Check serial
			Serial.println("yp");
			
		}
	}
	Serial.flush();
	
	//GPS
	
	get_data_GPS(data_GPS_Loop);
	if(takePoint){

		writeWP2File(wpFile, "twp",data_GPS_Loop);
		takePoint=false;

	}
	if(mode_itinerary){
		float deltaD = (millis()-previousMillis_Iti)*data_GPS_Loop[11]*3.6;
		if((deltaD>5&&data_GPS_Loop[11]>2)|| (millis()-previousMillis_Iti)>60000){
			writeWP2File(itiFile,"tit",data_GPS_Loop);
			previousMillis_Iti=millis();
		}
	}
	delay(10);
}