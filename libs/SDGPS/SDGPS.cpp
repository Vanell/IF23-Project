#include <SD.h>


//SD variables
File myFile;
String fileName = "test.txt";
String command="";
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
		Serial.print("Error while opening SD for reading");
}

void writeWP2File(String name, String wpName,float lat,float lon, int nbSat,int dop){
	char filename[name.length()+1];
  	name.toCharArray(filename, sizeof(filename));
  	myFile = SD.open(filename,FILE_WRITE);
  	char data[10];
  	dtostrf(lat,1,6,data);
    if (myFile) {
    	myFile.print(wpName);
    	myFile.print(" ");
    	dtostrf(lat,1,6,data);
    	myFile.print(data);
    	myFile.print(" ");
    	dtostrf(lon,1,6,data);
    	myFile.print(data);
    	sprintf(data," %i %i",nbSat,dop);
    	myFile.println(data);
	    myFile.close();
  	} 
  	else {
    	Serial.print("Error while opening SD for writing");
  	}
}