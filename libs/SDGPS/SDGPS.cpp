#include <SD.h>
//#include <GPS.cpp>




//SD function
// char *convertStringToCharArray(String str){
//     char strchar[str.length()+1];
//     str.toCharArray(strchar, sizeof(strchar));
//     return strchar;
// }

void listFiles(){
  File root = SD.open("/");
  while(true){
    File entry = root.openNextFile();
    if(!entry)
      break;
    Serial.print(entry.name());
    Serial.print(" ");
    Serial.println(entry.size(),DEC);
    entry.close();
  }
}
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
	  //else
		//Serial.println("Err");
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
      myFile.print(F(" "));
      //LATITUDE Longitude
      for(int i =2;i<4;i++){
        dtostrf(dataGPS[i],1,6,data);
        myFile.print(data);
        myFile.print(F(" "));
      }
      for(int i =0;i<2;i++){
        dtostrf(dataGPS[i],1,6,data);
        myFile.print(data);
        myFile.print(F(" "));
      }
      for(int i =5;i<10;i++){
        dtostrf(dataGPS[i],1,0,data);
        myFile.print(data);
        myFile.print(F(" "));
      }
      //SECONDS
      dtostrf(dataGPS[10],1,0,data);
      myFile.println(data);

      myFile.close();
    } 
    //else {
      //Serial.println("err");
    //}
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