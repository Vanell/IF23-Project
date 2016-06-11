#include "TinyGPS.h"
#include "SoftwareSerial.h"
#include <pin.h>
#include <GPS.h>

// //Configuration GPS
// TinyGPS gps;


// //GPS
// SoftwareSerial ss(RX_GPS, TX_GPS);
// int const nb_data_GPS(13);
// float data_GPS[nb_data_GPS]; //sat;hdop;lat;long;alt;speed
// const long delay_GPS = 750; //Time refresh GPS
// unsigned long previousMillis_GPS = 0;





// bool newDataGPS = false;

// int nb_satGPS = 0;
// float latGPS = 0;
// float lonGPS = 0;
// int hdopGPS = 0;
// float altGPS = 0;
// float speedGPS = 0;





float truncateNumber(float x, int a){
	long y;
	y = pow(10,a)* x;
	return (float)y/pow(10,a);
}

//GPS function
float* get_data_GPS(SoftwareSerial ss, float data_GPS[])
{
	newDataGPS = false;
	float flat, flon;
	unsigned long fix_age, date, time,fix_age_date, chars = 0;
	unsigned short sentences = 0, failed = 0;
	unsigned long start = millis();
	int year;
	byte month, day, hour, minute, second, hundredths;

	// int const nb_data_GPS(13);
	// float data_GPS[nb_data_GPS];

	//impletment smartdelay
	do 
	{
		while (ss.available())
			if (gps.encode(ss.read()))
				{newDataGPS=true;}
	} while (millis() - start < 250);
	
	gps.stats(&chars, &sentences, &failed);

	//DEBUG
	// Serial.print(" CHARS=");
	// Serial.println(chars);
	// Serial.print(" SENTENCES=");
	// Serial.print(sentences);
	// Serial.print(" CSUM ERR=");
	// Serial.println(failed);
	
	if (chars == 0)
		Serial.println("** No characters received from GPS: check wiring **");
		newDataGPS = false;

	if (newDataGPS){
		
		//Nb satellites
		if (gps.satellites() != TinyGPS::GPS_INVALID_SATELLITES){data_GPS[0] = (float)gps.satellites();}
		else{data_GPS[0] = 8888;}

		//Hdop
		if(gps.hdop() != TinyGPS::GPS_INVALID_HDOP){data_GPS[1] = (float)gps.hdop();}
		else{data_GPS[1] = 8888;}

		gps.f_get_position(&flat, &flon, &fix_age);

		//Lattidue
		if (flat != TinyGPS::GPS_INVALID_F_ANGLE){data_GPS[2] = truncateNumber(flat,6);}
		else{data_GPS[2] = 8888;}
		//Longitude
		if (flon != TinyGPS::GPS_INVALID_F_ANGLE){data_GPS[3] = truncateNumber(flon,6);}
		else{data_GPS[3] = 8888;}
		//Fix age, last time (in ms) a position is fix 
		if (fix_age != TinyGPS::GPS_INVALID_AGE){data_GPS[4] = truncateNumber(flon,6);}
		else{data_GPS[4] = 8888;}
 
		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &fix_age_date);
		if (fix_age_date == TinyGPS::GPS_INVALID_AGE)
		{
			data_GPS[5] = 8888;//year
			data_GPS[6] = 8888;//month
			data_GPS[7] = 8888;//day
			data_GPS[8] = 8888;//hour
			data_GPS[9] = 8888;//minute
			data_GPS[10] = 8888;//seconde
		}else
		{
			data_GPS[5] = year;//year
			data_GPS[6] = (float)month;//month
			data_GPS[7] = (float)day;//day
			data_GPS[8] = (float)hour;//dhour
			data_GPS[9] = (float)minute;//minute
			data_GPS[10] = (float)second;//seconde
		}
		
		//GPS altitude
		if(gps.f_altitude() != TinyGPS::GPS_INVALID_F_ALTITUDE){altGPS = truncateNumber(gps.f_altitude(),2);}
		else{data_GPS[11]=8888;}

		//GPS Speed km/h
		if(gps.f_speed_kmph() != TinyGPS::GPS_INVALID_F_SPEED){speedGPS = truncateNumber(gps.f_speed_kmph(),2);}
		else{data_GPS[12]=8888;}

	}
	return data_GPS;
}