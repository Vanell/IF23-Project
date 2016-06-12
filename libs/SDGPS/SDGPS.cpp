#include <SD.h>
//#include <GPS.cpp>




//SD function
void readFile(String name){
    char c;
    File myFile;
    char filename[name.length()+1];
    name.toCharArray(filename, sizeof(filename));
    myFile = SD.open(filename);
    if(myFile){
		while(myFile.available()){
      c = ((char)myFile.read());
			Serial.print(c);
		}
		myFile.close();
		Serial.print("done");
	  }
	  else
		Serial.println("Error while opening SD for reading");
    return;
}




void writeWP2File(String name, String wpName,float dataGPS[]){
    File myFile;
    char filename[name.length()+1];
    name.toCharArray(filename, sizeof(filename));
    myFile = SD.open(filename, FILE_WRITE);
    char data[10];
    if (myFile) {
      //Serial.println("wrinting in file!");
      myFile.print(wpName);
      myFile.print(" ");
      //LATITUDE
      dtostrf(dataGPS[2],1,6,data);
      myFile.print(data);
      myFile.print(" ");
      //LOGITUDE
      dtostrf(dataGPS[3],1,6,data);
      myFile.print(data);
      myFile.print(" ");
      //NB SATELLITES
      dtostrf(dataGPS[0],1,0,data);
      myFile.print(data);
      myFile.print(" ");
      //HDOP
      dtostrf(dataGPS[1],1,0,data);
      myFile.print(data);
      myFile.print(" ");
      //DAY
      dtostrf(dataGPS[7],1,0,data);
      myFile.print(data);
      myFile.print("/");
      //MONTH
      dtostrf(dataGPS[6],1,0,data);
      myFile.print(data);
      myFile.print("/");
      //YEAR
      dtostrf(dataGPS[5],1,0,data);
      myFile.print(data);
      myFile.print(" ");
      //HOURS
      dtostrf(dataGPS[8],1,0,data);
      myFile.print(data);
      myFile.print(":");
      //MINUTES
      dtostrf(dataGPS[9],1,0,data);
      myFile.print(data);
      myFile.print(":");
      //SECONDS
      dtostrf(dataGPS[10],1,0,data);
      myFile.println(data);

      myFile.close();
    } 
    else {
      Serial.println("error while opening SD for writing");
    }
    return;
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