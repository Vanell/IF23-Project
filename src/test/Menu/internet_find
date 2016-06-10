/*+++++++++++++++++++++++++++++++++++++++++++++++++++
Author      : Fahmi Ghani
Date        : 5 July 2015 
Project     : LCD Menu Function
Component   : LCD Keypad shield
Description : Simple Menu function on LCD
              Button Function:
                Up/Down - change Menu Page
                Select - enter page
                Left - Exit

Video Link: http://youtu.be/cMqif5ICS5M

+++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);  

int keypad_pin = A0;
int keypad_value = 0;
int keypad_value_old = 0;

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 4;

void setup()
{
    lcd.begin(16,2);  //Initialize a 2x16 type LCD

    MainMenuDisplay();
    delay(1000);
}
void loop()
{
    btn_push = ReadKeypad();
    
    MainMenuBtn();
    
    if(btn_push == 'S')//enter selected menu
    {
        WaitBtnRelease();
        switch (mainMenuPage)
        {
            case 1:
              MenuA();
              break;
            case 2:
              MenuB();
              break;
            case 3:
              MenuC();
              break;
            case 4:
              MenuD();
              break;
        }

          MainMenuDisplay();
          WaitBtnRelease();
    }
    


    delay(10);
  
}//--------------- End of loop() loop ---------------------
void MenuA()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu A");
    
    while(ReadKeypad()!= 'L')
    {
        //Insert Task for Menu A here
       
    }
}
void MenuB()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu B");
    
    while(ReadKeypad()!= 'L')
    {
        //Insert Task for Menu B here
       
    }
}
void MenuC()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu C");
    
    while(ReadKeypad()!= 'L')
    {
        //Insert Task for Menu C here
       
    }
}
void MenuD()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Inside Menu D");
    
    while(ReadKeypad()!= 'L')
    {
        //Insert Task for Menu D here
       
    }
}

void MainMenuDisplay()
{
    lcd.clear();
    lcd.setCursor(0,0);
    switch (mainMenuPage)
    {
        case 1:
          lcd.print("1. Menu A");
          break;
        case 2:
          lcd.print("2. Menu B");
          break;
        case 3:
          lcd.print("3. Menu C");
          break;
        case 4:
          lcd.print("4. Menu D");
          break;
    }
}

void MainMenuBtn()
{
    WaitBtnRelease();
    if(btn_push == 'U')
    {
        mainMenuPage++;
        if(mainMenuPage > mainMenuTotal)
          mainMenuPage = 1;
    }
    else if(btn_push == 'D')
    {
        mainMenuPage--;
        if(mainMenuPage == 0)
          mainMenuPage = mainMenuTotal;    
    }
    
    if(mainMenuPage != mainMenuPageOld) //only update display when page change
    {
        MainMenuDisplay();
        mainMenuPageOld = mainMenuPage;
    }
}

char ReadKeypad()
{
  /* Keypad button analog Value
  no button pressed 1023
  select  741
  left    503
  down    326
  up      142
  right   0 
  */
  keypad_value = analogRead(keypad_pin);
  
  if(keypad_value < 100)
    return 'R';
  else if(keypad_value < 200)
    return 'U';
  else if(keypad_value < 400)
    return 'D';
  else if(keypad_value < 600)
    return 'L';
  else if(keypad_value < 800)
    return 'S';
  else 
    return 'N';

}

void WaitBtnRelease()
{
    while( analogRead(keypad_pin) < 800){} 
}