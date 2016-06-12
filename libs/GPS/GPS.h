
TinyGPS gps;


//GPS
SoftwareSerial ss(RX_GPS, TX_GPS);
//ss.begin(4800);
// int const nb_data_GPS(13);
// float data_GPS[13]; //sat;hdop;lat;long;alt;speed
const PROGMEM int delay_GPS = 5000; //Time refresh GPS
unsigned long previousMillis_GPS = 0;





bool newData = false;

// int nb_satGPS = 0;
// float latGPS = 0;
// float lonGPS = 0;
// int hdopGPS = 0;
//float altGPS = 0; //necessary
//float speedGPS = 0; //necessary
// float age = 0;
// int year = 0;
// int month = 0;
// int day = 0;
// int hours = 0;
// int minutes = 0;
// int seconds = 0;
// int altitude = 0;
// int speed = 0;
