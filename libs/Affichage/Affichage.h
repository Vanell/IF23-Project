//const int delay_LCD = 750; //Time refresh LCD
//unsigned long previousMillis_LCD = 0;
//bool state_LCD = true; //true : on ; false : off
bool changeData_LCD = true;

//char SW = 4; // Value of last button press

//Menu
char btn_push;

////Def structure menu
//PROGMEM const int nb_level_menu = 4;

//PROGMEM const char slash = '/';
//PROGMEM const char dpoint = ':'; 

char pos_menu[4];
char max_screen_lvl[4];

bool mode_itinerary = false;
bool takePoint=false;

// Define char for screen
//byte char_arrow_left[8] = {0b00000,0b00000,0b11000,0b11100,0b11110,0b11100,0b11000,0b00000};
//byte char_arrow_left[2] = {0b11000,0b00000};
// byte char_arrow_up[8]   = {0b00000,0b00000,0b00000,0b00100,0b01110,0b11111,0b11111,0b00000};
// byte char_arrow_down[8] = {0b00000,0b00000,0b00000,0b11111,0b11111,0b01110,0b00100,0b00000};
// byte char_select[8]     = {0b00000,0b00000,0b00001,0b00011,0b10110,0b11100,0b01000,0b00000};


//float vmin = 4.5;
//float v0;
//unsigned long t0;

//unsigned long lastMillis_BP = 0 ;
