TinyGPS gps;

//GPS
SoftwareSerial ss(3,2);//ss(RX_GPS, TX_GPS);
const PROGMEM int delay_GPS = 750; //Time refresh GPS
bool newData = false;