

//MOTOR IN1 2 3 4 PINES PLACA 8 9 10 11
//FINAL DE CARRERA PIN4

//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>      // incluye libreria stepper Motor
Stepper motor1(2048, 8, 10, 9, 11);   // pasos completos, pines de salida.
int val = 0;
int con = 0;
int pasos = 0;
int botoneta = 0;
int tiempoSegundos = 0;
int largoTotal = 10000;  //se mide en pasos de motor
int pasosxseg = 0; 
int contapasos= 0;
int tiempo1 = 0;
int tiempo2 = 0;
int case411 = 0;


//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display





/////Rotary Encoder Pin Assignment/////
#define rotaryPin_A 12
#define rotaryPin_B 13
#define rotaryButtonPin 5

#define inputTimeOut 10000   //time out for human input session, milliseconds

unsigned int maxMenuItems;     //number of menu items
unsigned char encoder_A = 0;  
unsigned char encoder_B = 0;
unsigned char encoder_A_prev = 0;
unsigned char encoder_C = 1;  //encoder button
unsigned char encoder_C_prev = 1;
unsigned long currentTime;
unsigned long loopTime;
/////End of Rotary Encoder Declarations/////

/////Global time variable, Hour : Minute Settings
unsigned int setHour = 0;
unsigned int setMinute = 0;
////////

//////Menu declarations///////
unsigned int menuPos = 0; //current menu position
unsigned int lastMenuPos = 0; //previous menu position
unsigned int parentMenuPos = 0; //parent menu position
bool humanInputActive = false;  //flag to indicate if input session is active
unsigned subMenuActive = 0;   //flag to indicate a sub selection menu session is active: 0 - main menu; 1 - number selection 1- 255; 2 - binary selection yes/no; 3 - time setting;
unsigned int subMenuPos = 0;  //sub menu rotary position
unsigned int subMenuClick = 0; //sub menu click counter
//unsigned long inputTimeOut = 10000;   //time out for human input session, milliseconds
unsigned long lastInputTime = millis(); //keep track of time of last human input

typedef struct menu_type
 {
     
     menu_type()
     : code(0), text("")
     {
       //do nothing
     }
     
     unsigned int code; //code that indicate address (position) in the menu tree
     String text; //text to be displayed for the menu selection
     
 }  menu_type;

menu_type menu[100] = {}; //initilizing menu array, use a number >= than the number of menu options


/////End of Menu declarations////

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(19200);
  rotaryEncoderInit();
  menuInit();

  lcd.init();
  lcd.backlight();
  
  //MOTOR
  motor1.setSpeed(11);       // en RPM (valores de 1, 2 o 3 para 28BYJ-48)
  pinMode(LED_BUILTIN, OUTPUT); //Led del arduino como chivo.
  pinMode(4,INPUT);             //Pin 4 entrada del Final de carrera
  // FIN MOTOR
}

void loop() {
  // put your main code here, to run repeatedly:
  rotaryEncoderUpdate();
  lcdBackLight();
}

void rotaryEncoderInit() {
//Rotary encoder initialization

  pinMode(rotaryPin_A, INPUT_PULLUP);
  pinMode(rotaryPin_B, INPUT_PULLUP);
  pinMode(rotaryButtonPin, INPUT_PULLUP);
  currentTime = millis();
  loopTime = currentTime;
}

void rotaryEncoderUpdate() {
//rotary encoder update, to be called from main loop()

  currentTime = millis();
  if (currentTime >= (loopTime + 1) ) {
    encoder_A = digitalRead(rotaryPin_A);
    encoder_B = digitalRead(rotaryPin_B);
    encoder_C = digitalRead(rotaryButtonPin);

    //handling knob rotation
    if ( (encoder_A == 0) && (encoder_A_prev == 1) ) {  //encoder changed position

      lastInputTime = millis();
      
      if (encoder_B == 1) { //B is high, so encoder moved clockwise
        //Serial.println("encoder rotated CW");

        switch (subMenuActive) { //send encoder action to appropriate menu handler
          case 0: //main menu
            menuUpdate(2);
            break;

          case 1: //subMenu1
            subMenu1Update(2);
            break;

          case 2: //subMenu2
            subMenu2Update(2);
            break;

          case 3:
            subMenu3Update(2);
            break;
            
          default:
            menuUpdate(2);
            break;
        }
        
        
      } else {  //else, encoder moved counter-clockwise
        //Serial.println("encoder rotated CCW");

        switch (subMenuActive) {  //call the appropriate menuupdater depending on which sub menu is active
          case 0:
            menuUpdate(3);
            break;

          case 1:
            subMenu1Update(3);
            break;

          case 2:
            subMenu2Update(3);
            break;

          case 3:
            subMenu3Update(3);
            break;

          default:
            menuUpdate(3);
            break;
        }
      }

    }
    
    //handling push button
    if ( (encoder_C == 0) && (encoder_C_prev == 1) ) { //button pushed
      //Serial.println("encoder button closed.");

      lastInputTime = millis();
      
      switch (subMenuActive) {  //call the appropriate menuupdater depending on which sub menu is active
          case 0:
            menuUpdate(1);
            break;

          case 1:
            subMenu1Update(1);
            break;

          case 2:
            subMenu2Update(1);
            break;

          case 3:
            subMenu3Update(1);
            break;

          default:
            menuUpdate(1);
            break;
        }
      
    } else if ( (encoder_C == 1) && (encoder_C_prev == 0) )  {  //button
      //Serial.println("encoder button opened.");
      
    }
    
    encoder_A_prev = encoder_A;
    encoder_C_prev = encoder_C;
    loopTime = currentTime;

//input time out
    if ( (millis() - lastInputTime) > inputTimeOut ) {
      humanInputActive = false;
      menuPos = 0;
      lastMenuPos = 0;
      menuUpdate(0);
      subMenuActive = 0;
      
    } else {
      humanInputActive = true;
    }
    
  }
  
}



unsigned int menuVerifyPos(unsigned int menuCode) {
//accepts a code that represents position in the menu
//checks against the menu, verify it exist, and returns it
//if the menu code given does not exist, 
//returns the closest code smaller than the one given
 
  bool confirmCode = false; //flag to keep track of whether code has been confirmed in menu tree

  for (unsigned int k = 0; k <= (maxMenuItems - 1); k++) {
    
    if (menuCode == menu[k].code) {  //found exact code, returns it
      menuPos = menu[k].code;
      confirmCode = true;
      lastMenuPos = menuCode;
      return menuPos;
    } 

  }

  if (confirmCode == false) {
    menuPos = lastMenuPos;
    return menuPos;   //cannot find menu option, go back to previous menu option
  }

}

void updateMenuDisplay(unsigned int menuCode) {
//prints menu selection to the LCD display
//in order to have a scrolling menu effect, this code looks at item before and after current menu item and display them in a row


  String curMenu;
  String curMenu2;
  String prevMenu;
  String nextMenu;

  curMenu = findMenuTextFromCode(menuCode);
  prevMenu = findMenuTextFromCode(menuCode - 1);
  nextMenu = findMenuTextFromCode((menuCode + 1));

//if string is 20 or more characters long (for 20 char wide LCD display)
 if( prevMenu.length() >= 20) {
  prevMenu.remove(19); //remove anything after the 19th character from string
 }

 if(nextMenu.length() >= 20) {
  nextMenu.remove(19); //remove anything after the 19th character from string
 }

//starts printing to LCD
  lcd.clear();
  lcd.setCursor(1,0); //char index, row index, on LCD
  lcd.print(prevMenu);
  lcd.setCursor(0,1);
  lcd.print(">");
  
//current menu option text is longer than 20 characters (on a 20 char wide LCD)
 if(curMenu.length() >= 20) {
  
  curMenu2 = curMenu;
  curMenu.remove(19); //remove char from index 19 (20th char)
  curMenu2.remove(0,19);  //remove the first 19 char starting from index 0
  
  lcd.setCursor(1,1); //print first 19 char of current menu text
  lcd.print(curMenu);
  lcd.setCursor(0,2);
  lcd.print("-");
  lcd.setCursor(1,2);
  lcd.print(curMenu2);  //print remainig char of current menu text
 
  lcd.setCursor(1,3); //print the next menu text in the 4th row
  lcd.print(nextMenu);
  
 } else {
  
  lcd.print(curMenu); //print the next menu text in the 3rd row
  lcd.setCursor(1,2);
  lcd.print(nextMenu);
 }
  

  
}

String findMenuTextFromCode(unsigned int menuCode) {
//accepts a code representing the code in menu, and returns the corresponding text
  
  for (unsigned int j = 0; j <= (maxMenuItems - 1); j++ ) {
    if (menuCode == menu[j].code) {
      return menu[j].text;
      break;
    }
  }
}


void lcdBackLight(){
 //human inout timeout to turn off lcd back light
  if (humanInputActive == true) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}

void menuInit() {
  //menu text display

  unsigned int i = 0;

  menu[i].code = 0;
  menu[i++].text = "EXIT MENU";
  
  menu[i].code = 1;
  menu[i++].text = "BUSCAR CERO";
    //menu[i].code = 11;
    //menu[i++].text = "CURRENT TIME:";
  
  menu[i].code = 2;
  menu[i++].text = "PAUSAR MOTOR";
  /*
    menu[i].code = 20;
    menu[i++].text = "PREVIOUS MENU";
  
    menu[i].code = 21;
    menu[i++].text = "CONTINUOUS MODE";
      menu[i].code = 211;
      menu[i++].text = "ACTIVATE";
      
   
    menu[i].code = 22;
    menu[i++].text = "DAILY MODE";
      menu[i].code = 220;
      menu[i++].text = "PREVIOUS MENU";
      menu[i].code = 221;
      menu[i++].text = "ACTIVATE";
      menu[i].code = 222;
      menu[i++].text = "TARGET VOLUME (L)";
        menu[i].code = 2221;
        menu[i++].text = "SET LITRES PER DAY:";
      menu[i].code = 223;
      menu[i++].text = "SCHEDULE TIME";
        menu[i].code = 2231;
        menu[i++].text = "START DAILY WATER CHANGE AT:";
      
      
    
    menu[i].code = 23;
    menu[i++].text = "TOP-OFF ONLY MODE";
      menu[i].code = 230;
      menu[i++].text = "PREVIOUS MENU";
      menu[i].code = 231;
      menu[i++].text = "ACTIVATE";
    
    menu[i].code = 24;
    menu[i++].text = "DAILY TIMER MODE";
      menu[i].code = 240;
      menu[i++].text = "PREVIOUS MENU";
      menu[i].code = 241;
      menu[i++].text = "ACTIVATE";
      menu[i].code = 242;
      menu[i++].text = "DRAIN DURATION";
        menu[i].code = 2421;
        menu[i++].text = "DRAIN FOR THIS MANY MINUTES:";
      menu[i].code = 243;
      menu[i++].text = "SCHEDULE TIME";
        menu[i].code = 2431;
        menu[i++].text = "START DAILY WATER CHANGE AT:";
        
  */
  menu[i].code = 3;
  menu[i++].text = "IR Y VOLVER";
      menu[i].code = 30;
      menu[i++].text = "MENU ANTERIOR";
      menu[i].code = 31;
      menu[i++].text = "EN CUANTO?";
        menu[i].code = 311;
        menu[i++].text = "SEGUNDOS :";
           

  
  menu[i].code = 4;
  menu[i++].text = "MOVIMIENTO MANUAL";
      menu[i].code = 40;
      menu[i++].text = "MENU ANTERIOR";
      menu[i].code = 41;
      menu[i++].text = "MOVER MOTOR";
        menu[i].code = 411;
        menu[i++].text = "PASOS :";
     /* menu[i].code = 42;
      menu[i++].text = "DELAY DURATION";
        menu[i].code = 421;
        menu[i++].text = "DELAY DURATION IN MINUTES:";*/
  /*
  
  */
 maxMenuItems = i + 1;

}


//HANDLER

void menuUpdate(unsigned char encoderAction) {  //main menu navigation updater
//called on by rotary encoder actions
//parameter 0: no action from encoder
//parameter 1: encoder click
//parameter 2: encoder CW rotation
//parameter 3: encoder CCW rotation

//unsigned int verifiedMenuPos;
  
  if (encoderAction == 0) {
    //no action
    
  } else {  //triggered by action
  
    if (encoderAction == 1) {  //encoder click button
      
      //code below takes menu position to the next sub branch
      if (menuPos % 10 == 0) {  //if menu selection is at XX0 (zero at the end)
        menuPos = menuPos / 10; //go back up one level of menu (we use zero as the 'return' menu selection)
      } else {
        menuPos = menuPos * 10 + 1; //else, go further in one level of menu from the current selection
      }

      menuPos = menuVerifyPos(menuPos);
      menuAction(menuPos);  //click execution
      updateMenuDisplay(menuPos);
  
    } else if (encoderAction == 2) {  //encoder CW rotation
  
      if (menuPos % 10 == 9) {  //if current selection is at XX9
        //do nothing
      } else {
        menuPos ++;
      }
      
      menuPos = menuVerifyPos(menuPos);      
      updateMenuDisplay(menuPos);
  
      
    } else if (encoderAction == 3) {  //encoder CCW rotation
  
      if (menuPos % 10 == 0) {  //if current selection is at XX0
        //do nothing
      } else {
        menuPos --;
      }
      
      menuPos = menuVerifyPos(menuPos);
      updateMenuDisplay(menuPos);
    }

    lastMenuPos = menuPos;

    Serial.print("menuPos = ");
    Serial.println(menuPos);

  }
}

void subMenu1Update(unsigned char encoderAction) { //sub menu type 1 updater: number selection 1 - 255
//called on by rotary encoder actions with a parameter to indicate:
//parameter 0: no action from encoder
//parameter 1: encoder click
//parameter 2: encoder CW rotation
//parameter 3: encoder CCW rotation
  Serial.println("Entramos en el submenu1");

  if (encoderAction == 0) {
    // no action
  } else {
  
    if (encoderAction == 1) { //encoder click
      subMenuClick++;
      //Serial.print("subMenu1Update: click++; click = ");
      //Serial.println(subMenuClick);
  
      if(subMenuClick == 1) { //single click to exit sub menu
        lcd.clear();
        menuAction(menuPos);   //calls menuAction with last main menu tree position
      }
      
    } else if (encoderAction == 2) {  //CW rotation
        Serial.println("Hay rotación");
        if(case411==1){
          motor1.step(10);
          digitalWrite(8, LOW);     // desenergiza todas las bobinas
          digitalWrite(9, LOW);
          digitalWrite(10, LOW);
          digitalWrite(11, LOW);
        }

      if (subMenuPos >= 255) {
        subMenuPos = 1; 
      } else {
        subMenuPos++;
      }
      
    } else if (encoderAction == 3) {  //CCW rotation
        Serial.println("Hay Anti-rotación");
        if(case411==1){
          motor1.step(-10);
          digitalWrite(8, LOW);     // desenergiza todas las bobinas
          digitalWrite(9, LOW);
          digitalWrite(10, LOW);
          digitalWrite(11, LOW);
        }  
      if (subMenuPos <= 1) {
        subMenuPos = 255;
      } else {
        subMenuPos--;
      }
      
    }

    //update 4th row of LCD to display sub menu position
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print(subMenuPos); 
  }
}

void subMenu11Update(unsigned char encoderAction) { //sub menu type 11 updater: number selection -6000 and 6000
//called on by rotary encoder actions with a parameter to indicate:
//parameter 0: no action from encoder
//parameter 1: encoder click
//parameter 2: encoder CW rotation
//parameter 3: encoder CCW rotation
  Serial.println("Entramos en el submenu11");

  if (encoderAction == 0) {
    // no action
  } else {
  
    if (encoderAction == 1) { //encoder click
      subMenuClick++;
      //Serial.print("subMenu1Update: click++; click = ");
      //Serial.println(subMenuClick);
  
      if(subMenuClick == 1) { //single click to exit sub menu
        lcd.clear();
        menuAction(menuPos);   //calls menuAction with last main menu tree position
      }
      
    } else if (encoderAction == 2) {  //CW rotation
      Serial.println("hay rotacion");

      if (subMenuPos >= 6000) {
        subMenuPos = 1;  
        Serial.println("MAXIMO"); 
      } else {
        subMenuPos++;
        motor1.step(100);
        digitalWrite(8, LOW);     // desenergiza todas las bobinas
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);

      }
      
    } else if (encoderAction == 3) {  //CCW rotation
        Serial.println("hay Antirotacion");
      if (subMenuPos <= -6000) {
        subMenuPos = 6000;   
        Serial.println("Minimo");
      } else {
        subMenuPos--;
        motor1.step(-100);
        digitalWrite(8, LOW);     // desenergiza todas las bobinas
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
      }
      
    }

    //update 4th row of LCD to display sub menu position
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print(subMenuPos); 
  }
}

void subMenu2Update(unsigned char encoderAction){ //sub menu type 2, yes-no selection
//called on by rotary encoder actions
//parameter 0: no action from encoder
//parameter 1: encoder click
//parameter 2: encoder CW rotation
//parameter 3: encoder CCW rotation

 if (encoderAction == 1) { //encoder click
    
    subMenuClick++;

    if(subMenuClick == 1) { //single click to exit sub menu
      lcd.clear();
      menuAction(menuPos);   //calls menuAction with last main menu tree position
    }
    
  } else {  //encoder rotation
  if (encoderAction != 0) {
    if (subMenuPos != 1) { //flip subMenuPos between 0 and 1
      subMenuPos = 1;      
    } else {
      subMenuPos = 0;
    }
  }

    //update 4th row of LCD to display sub menu position
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    
    if (subMenuPos == 1) {
      lcd.print("YES"); 
    } else if (subMenuPos == 0) {
      lcd.print("NO");
    }
  
  }

}

void subMenu3Update(unsigned char encoderAction) { //sub menu type 3 updater: time setting
//called on by rotary encoder actions with a parameter to indicate:
//parameter 0: no action from encoder
//parameter 1: encoder click
//parameter 2: encoder CW rotation
//parameter 3: encoder CCW rotation

  if (encoderAction == 0) {
    // no action
  } else {
  
    if (encoderAction == 1) { //encoder click
      subMenuClick++;

      if(subMenuClick == 1) { //first click to finish setting hour, move on to set minutes
        
        subMenuPos = 0; //reset subMenuPos to be used for setting minute
        
      } else if(subMenuClick == 2) { //second click to record minute, exit menu
         
        //:::code to set hour and minute to global variables:::
        
        lcd.clear();
        menuAction(menuPos);   //calls menuAction with last main menu tree position
      }
      
    } else if (encoderAction == 2) {  //CW rotation

      if (subMenuClick == 0) {  //setting hour
        
        if (subMenuPos >= 24) { //scrolling end limits
          subMenuPos = 0; 
        } else {
          subMenuPos++;
        }
        
        setHour = subMenuPos;
        
      } else if (subMenuClick == 1) { //setting minute
        
        if (subMenuPos >= 59) { //scrolling end limits
          subMenuPos = 0; 
        } else {
          subMenuPos++;
        }

        setMinute = subMenuPos;
      }
      
    } else if (encoderAction == 3) {  //CCW rotation

      if (subMenuClick == 0) {  //setting hour
        
        if (subMenuPos <= 0) {  //scrolling end limits
          subMenuPos = 24;
        } else {
          subMenuPos--;
        }

        setHour = subMenuPos;

      } else if (subMenuClick == 1) { //setting minute

        if (subMenuPos <= 0) {  //scrolling end limits
          subMenuPos = 59;
        } else {
          subMenuPos--;
        }

        setMinute = subMenuPos;
      }
      
    }
    
    //update 4th row of LCD to display sub menu position
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print(setHour);
    lcd.print(":");
    lcd.print(setMinute);
  }
}



//ACTION

void menuAction(unsigned int menuCode) {
 //gets called when rotary encoder is clicked,
  //check the current location inside the menu,
  //execute approriate actions
  //returns true if some action is executed
  //returns false if nothing is done
 Serial.print("menuAction called with menuCode = ");
 Serial.println(menuCode);
  
      switch (menuCode) {


        case 1: //BUSCAR CERO
        con = 0;
        pasos= -24; //Le doy un valor a pasos para que vuelva mientras busca el Final de carrera
        val = digitalRead(4); //leo el FC
        delay(2);           //Antirebote
        val = digitalRead(4); //leo el FC
        lcd.clear();
        lcd.setCursor(1,0); //
        lcd.print("Buscando el Final");
        lcd.setCursor(0,1);
        lcd.print("de carrera");

        while(val == 0 && con == 0) // el con es mi flag, cuando toque el Final de carrera val sera 1 y con se pone a 1,
                                    //asì cuando vuelve a pasar por el while, con nunca mas será 0.
        {  
        motor1.step(pasos); //arranca girando el motor hacia la izquierda en busca del FC.
        val = digitalRead(4); //leo el FC
        delay(1);           //Antirebote
        val = digitalRead(4); //leo el FC

        Serial.println("Buscando el FC");
        
        if(digitalRead(4)== 1 && con == 0 )
          {
            Serial.println("FC encontrado");
            
            digitalWrite(8, LOW);     // desenergiza todas las bobinas
            digitalWrite(9, LOW);
            digitalWrite(10, LOW);
            digitalWrite(11, LOW);

            Serial.println("Motor Detenido"); //Fin .
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("FC Encontrado");
            lcd.setCursor(0,1);
            lcd.print("Motor Detenido");
            delay(2000);
          }
          
        }
        break;

        case 2: //PARAR MOTOR

        digitalWrite(8, LOW);     // desenergiza todas las bobinas
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);

        Serial.println("Motor Detenido");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Motor detenido");
        delay(1000);

        break;

        case 311:  //IR Y VENIR

        if (subMenuActive != 1) {    //if subMenu1 is not active, activate it

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("TIEMPO EN SEG:");
            subMenuActive = 1;    //activate subMenu1
            subMenuPos = 60;     //sub menu initialization, set it to current daily volume target
            subMenuClick = 0;
            subMenu1Update(0);  //calls subMenu1Update with 0 (no action)

          } else if (subMenuActive == 1) {    //if subMenu1 is already active, execute action from sub menu1 and activate main menu

            subMenuActive = 0;  //deactivate submenu, activate main menus
            
            //set daily target variable to subMenuPos;
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("IR Y VENIR");
            lcd.setCursor(0,1);
            lcd.print("EN ");
            lcd.print(subMenuPos);
            lcd.print(" SEGUNDOS");
            delay(3000);

            pasosxseg = 0; 
            contapasos= 0;
            tiempoSegundos = subMenuPos / 2; //mitad para ir , mitad para volver
          
            pasosxseg = largoTotal / tiempoSegundos;

            Serial.println(tiempoSegundos);
            Serial.println(pasosxseg);
            delay(3000);
            while (contapasos < largoTotal )
            {
               
                motor1.step(pasosxseg);
                Serial.println(contapasos);
                contapasos = contapasos + pasosxseg;
    
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Volviendo");
            Serial.println("Volviendo");
            contapasos = 0;
            pasosxseg = -pasosxseg;
            while (contapasos < largoTotal )
            {
                
                motor1.step(pasosxseg);
                Serial.println(contapasos);
                contapasos = contapasos - pasosxseg;
                
            }              

                digitalWrite(8, LOW);     // desenergiza todas las bobinas
                digitalWrite(9, LOW);
                digitalWrite(10, LOW);
                digitalWrite(11, LOW);

            Serial.println("Motor Detenido"); //Fin .
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Motor Detenido");
            delay(2000);

            menuPos = 3;  //return to previous main menu position
            
            delay(1000);
            updateMenuDisplay(menuPos);

          }
        
        break;

        case 411:  //MOVER MOTOR MANUAL
        Serial.println("caso411)");
        case411=1;

        if (subMenuActive != 1) {    //if subMenu1 is not active, activate it

            lcd.clear();
            lcd.setCursor(0,0);
            Serial.println("Moviendo:");
            subMenuActive = 1;    //activate subMenu1
            subMenuPos = 0;     //sub menu initialization, set it to current daily volume target
            subMenuClick = 0;
            subMenu1Update(0);  //calls subMenu1Update with 0 (no action)

          } else if (subMenuActive == 1) {    //if subMenu1 is already active, execute action from sub menu1 and activate main menu

            subMenuActive = 0;  //deactivate submenu, activate main menus
            
            //set daily target variable to subMenuPos;
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Nos fuimos");
            Serial.println("Nos fuimos");
            
            delay(3000);

            

            menuPos = 4;  //return to previous main menu position
            
            delay(1000);
            updateMenuDisplay(menuPos);
            case411=0;
          }
        break;

        

        default:
          Serial.print("Main menu case not found: ");
          Serial.println(menuCode);

          break;
      }
}
