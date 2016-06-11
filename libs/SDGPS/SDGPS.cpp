#include <SD.h>
//#include <GPS.cpp>


//SD variables
File myFile;
String fileName = "test.txt";
//String command="";
char c;

//SD function
void readFile(String name){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
	myFile = SD.open(filename);
	if(myFile){
		while(myFile.available()){
			c = ((char)myFile.read());
			Serial.print(c);
		}
		myFile.close();
		Serial.print("Done");
	}
	else
		Serial.println("Error while opening SD for reading");
}

void writeWP2File(String name, String wpName){
	  char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
  	// myFile = SD.open(filename,FILE_WRITE);
    myFile = SD.open("text.txt",FILE_WRITE);
  	char data[20];
  	//dtostrf(lat,1,6,data);
    if (myFile) {
    	myFile.print(wpName);
    	myFile.print(" ");
     //  //Write latitude
    	// dtostrf(data_GPS[2],1,6,data);
    	// myFile.print(data);
    	// myFile.print(" ");
     //  //write longitude
    	// dtostrf(data_GPS[3],1,6,data);
    	// myFile.print(data);
     //  //write nb Satellites and dop
    	// sprintf(data," %i %i",(int)data_GPS[0],(int)data_GPS[1]);
    	// myFile.println(data);
     //  //write date
     //  sprintf(data," %i/%i/%i",(int)data_GPS[7],(int)data_GPS[6],(int)data_GPS[5]); 
     //  myFile.println(data);
     //  //write time
     //  sprintf(data," %i:%i:%i",(int)data_GPS[8],(int)data_GPS[9],(int)data_GPS[10]);
     //  myFile.println(data);
	    myFile.close();
  	} 
  	else {
    	Serial.print("Error while opening SD for writing");
  	}
}

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