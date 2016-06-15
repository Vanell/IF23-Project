//Pin Configuration File 

//GPS Pins
#define TX_GPS     2
#define RX_GPS     3

//LCD Pin
#define LCD_RS     4
#define LCD_Enable 5
#define LCD_D4     6
#define LCD_D5     7
#define LCD_D6     8
#define LCD_D7     9

//SD Pin
PROGMEM const char SD_SS=10;

//Buttons Pin
#define BP0        16
#define BP1        15
#define BPEN       17

//Others
PROGMEM const int pinBat=0;

void writeWP2File(String name, String wpName,float dataGPS[]);
void readFile(String name);
