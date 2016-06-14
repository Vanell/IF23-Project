
TinyGPS gps;


//GPS
SoftwareSerial ss(3,2);//ss(RX_GPS, TX_GPS);
//ss.begin(4800);
// int const nb_data_GPS(13);
// float data_GPS[13]; //sat;hdop;lat;long;alt;speed
const PROGMEM int delay_GPS = 750; //Time refresh GPS
//unsigned long previousMillis_GPS = 0;





bool newData = false;

