String command="";

void setup(){
	Serial.begin(9600);
}

void loop(){
	if(Serial.available()){
		command=Serial.readString();
		Serial.println(command);

	}
}