//Configuration GPS
TinyGPS gps;


//GPS
SoftwareSerial ss(RX_GPS, TX_GPS);
int const nb_data_GPS(13);
float data_GPS[13]; //sat;hdop;lat;long;alt;speed
const long delay_GPS = 750; //Time refresh GPS
unsigned long previousMillis_GPS = 0;





bool newDataGPS = false;

int nb_satGPS = 0;
float latGPS = 0;
float lonGPS = 0;
int hdopGPS = 0;
float altGPS = 0;
float speedGPS = 0;