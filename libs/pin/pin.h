//Pin Configuration File 

//GPS Pi
#define TX_GPS     2
#define RX_GPS     3
// PROGMEM const int TX_GPS=2;
// PROGMEM const int RX_GPS=3;

//LCD Pin
// #define LCD_RS     4
// #define LCD_Enable 5
// #define LCD_D4     6
// #define LCD_D5     7
// #define LCD_D6     8
// #define LCD_D7     9

//SD Pin
//#define SD_SS      10
PROGMEM const char SD_SS=10;
//#define SD_MOSI    11
//#define SD_MISO    12
//#define SD_SCK     13

//Buttons Pin
// PROGMEM const int BP0 = 16;
// PROGMEM const int BP1 = 15;
// PROGMEM const int BPEN = 17;
#define BP0        16
#define BP1        15
#define BPEN       17

//Others
//#define pinBat       0
PROGMEM const int pinBat=0;

void writeWP2File(String name, String wpName,float dataGPS[]);
void readFile(String name);