#include "TinyGPS.h"
#include "SoftwareSerial.h"
//#include <pin.h>
#include <GPS.h>




float truncateNumber(float x, int a){
	long y;
	y = pow(10,a)* x;
	return (float)y/pow(10,a);
}

//GPS function

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

void get_data_GPS(float data_GPS[])
{
	newData = false;
	float flat, flon;
	unsigned long fix_age, date, time,fix_age_date, chars = 0;
	unsigned short sentences = 0, failed = 0;
	unsigned long start = millis();
	int year;
	byte month, day, hour, minute, second, hundredths;

	//impletment smartdelay
	
	
	while (ss.available()&&millis()-start<pgm_read_byte(&delay_GPS))
	{
		char c = ss.read();
	    // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
	    if (gps.encode(c)) // Did a new valid sentence come in?
	      newData = true;
	}
	
	gps.stats(&chars, &sentences, &failed);

	//DEBUG
	// Serial.print(" CHARS=");
	// Serial.println(chars);
	// Serial.print(" SENTENCES=");
	// Serial.print(sentences);
	// Serial.print(" CSUM ERR=");
	// Serial.println(failed);
	
	if (chars == 0){
		//Serial.println("NocharGPS");
		//newData = false;
	}

	
  

	if (newData){
	    gps.f_get_position(&flat, &flon, &fix_age);
	    

		//Nb satellites
		if (gps.satellites() != TinyGPS::GPS_INVALID_SATELLITES){
			data_GPS[0] = (float)gps.satellites();
		}
		else{
			data_GPS[0] = 8888;
		}

		//Hdop
		if(gps.hdop() != TinyGPS::GPS_INVALID_HDOP){
			data_GPS[1] = (float)gps.hdop();
		}
		else{
			data_GPS[1] = 8888;
		}

		//gps.f_get_position(&flat, &flon, &fix_age);

		//Lattidue
		if (flat != TinyGPS::GPS_INVALID_F_ANGLE){
			data_GPS[2] = truncateNumber(flat,6);
		}
		else{
			data_GPS[2] = 8888;
		}
		//Longitude
		if (flon != TinyGPS::GPS_INVALID_F_ANGLE){
			data_GPS[3] = truncateNumber(flon,6);
		}
		else{
			data_GPS[3] = 8888;
		}
		//Fix age, last time (in ms) a position is fix 
		if (fix_age != TinyGPS::GPS_INVALID_AGE){
			data_GPS[4] = truncateNumber(flon,6);
		}
		else{
			data_GPS[4] = 8888;
		}

		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &fix_age_date);
		if (fix_age_date == TinyGPS::GPS_INVALID_AGE)
		{
			for(int i =5;i<11;i++){
				data_GPS[i]=8888;
			}
			// data_GPS[5] = 8888;//day
			// data_GPS[6] = 8888;//month
			// data_GPS[7] = 8888;//year
			// data_GPS[8] = 8888;//hour
			// data_GPS[9] = 8888;//minute
			// data_GPS[10] = 8888;//seconde
		}else
		{
			data_GPS[5] = (float)day;//day
			data_GPS[6] = (float)month;//month
			data_GPS[7] = year;//year
			data_GPS[8] = (float)hour;//dhour
			data_GPS[9] = (float)minute;//minute
			data_GPS[10] = (float)second;//seconde
		}
		
		//GPS altitude
		if(gps.f_altitude() != TinyGPS::GPS_INVALID_F_ALTITUDE){
			data_GPS[11] = truncateNumber(gps.f_altitude(),2);
		}
		else{
			data_GPS[11]=8888;
		}

		//GPS Speed km/h
		if(gps.f_speed_kmph() != TinyGPS::GPS_INVALID_F_SPEED){
			data_GPS[12] = truncateNumber(gps.f_speed_kmph(),2);
		}
		else{
			data_GPS[12]=8888;
		}



  

	}
	return;// data_GPS;
}