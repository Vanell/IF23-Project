#include <SD.h>
#include <SDGPS.h>



void readFile(String name){
    char c;
    File myFile;
    char strchar[name.length()+1];
    name.toCharArray(strchar, sizeof(strchar));
    myFile = SD.open(strchar);
    if(myFile){
		while(myFile.available()){
      c = ((char)myFile.read());
			Serial.print(c);
		}
		myFile.close();
		Serial.print("ok");
	  }
    else
      Serial.println("err");
    return;
}


void writeWP2File(String name, String wpName,float dataGPS[]){
    File myFile;
    char strchar[name.length()+1];
    name.toCharArray(strchar, sizeof(strchar));
    myFile = SD.open(strchar, FILE_WRITE);
    char data[10];
    if (myFile) {
      myFile.print(wpName);
      myFile.print(F(","));
      //LATITUDE Longitude
      for(int i =2;i<4;i++){
        dtostrf(dataGPS[i],1,6,data);
        myFile.print(data);
        myFile.print(F(","));
      }
      for(int i =0;i<2;i++){
        dtostrf(dataGPS[i],1,6,data);
        myFile.print(data);
        myFile.print(F(","));
      }
      for(int i =4;i<13;i++){
        dtostrf(dataGPS[i],1,2,data);
        myFile.print(data);
        myFile.print(F(","));
      }
      float Vbat = mapfloat(analogRead(0),0,1023,0,6.2);
      dtostrf(Vbat,1,2,data);
      myFile.println(data);

      myFile.close();
    } 
    else
      Serial.println("err");
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