//SSD1306
  #include <SPI.h>
  #include "SSD1306Ascii.h"
  #include "SSD1306AsciiAvrI2c.h"
  #define SCREEN_ADDRESS1 0x3C 
  #define SCREEN_ADDRESS2 0x3D
  
  SSD1306AsciiAvrI2c display1;
  SSD1306AsciiAvrI2c display2;

//IBUS
  #include <IbusTrx.h>
  unsigned int messageSource = 0;
  unsigned int messageDestination = 0;
  unsigned int messageLength = 0;
  unsigned int messageCommand = 0;
  unsigned int message = 0;
  int a=0;
  int j=1;
  int IbusEnable = 2;
  IbusTrx ibusTrx;

//SD
  #include <SD.h>
  const int chipSelect = 53;
  int LOG=0;
  File sdcard_file;

//Mode select
  int buttonPushCounter = 0;   
  int button1State = 0;     
  int button2State = 0;      
  int lastButton1State = 0;
  int lastButton2State = 0;
  int button2wasPushed = 0;
  int firstLaunch1 = 0;
  int firstLaunch2 = 0;
  int firstLaunch3 = 0;
  int firstLaunch4 = 0;
  int firstLaunch5 = 0;
  int firstLaunch6 = 0;
  int firstLaunch7 = 0;
  int firstLaunch8 = 0;
  int firstLaunch9 = 0;

//Input Pins
  int Button1 = 29;
  int Button2 = 27;

//Led Pin
  int topLed = 31;
  int midLed = 30;
  int botLed = 25;

//IBUS MESSAGES without checksum
  //uint8_t IgnOFF[5] = { 0x80, 0x04, 0xBF, 0x11, 0x00 };
  //uint8_t IgnACC[5] = { 0x80, 0x04, 0xBF, 0x11, 0x01 };
  //uint8_t IgnON[5] = { 0x80, 0x04, 0xBF, 0x11, 0x03 };
  //uint8_t IgnSTART[5] = { 0x80, 0x04, 0xBF, 0x11, 0x07 };
  uint8_t Ready[5] = {0x3F, 0x04, 0xBF, 0x02, 0x01};
  uint8_t RlsStatus[4] = { 0x3F, 0x03, 0xE8, 0x00 };
  uint8_t RLSfault[4] = { 0x3F, 0x03, 0xE8, 0x04 };
  uint8_t RLSclearfault[4] = { 0x3F, 0x03, 0xE8, 0x05 };
  uint8_t RLSLightRead[7] = { 0x3F, 0x06, 0xE8, 0x06, 0x02, 0x54, 0x04};
  uint8_t RLSTemp[7] = { 0x3F, 0x06, 0xE8, 0x06, 0x00, 0xB4, 0x01};
  uint8_t WipeSens1[7] = { 0x3F, 0x06, 0xE8, 0x06, 0x00, 0x86, 0x08};
  //uint8_t Renforce[7] = { 0x3F, 0x06, 0xE8, 0x06, 0x01, 0x24, 0x01};  
  uint8_t Init1[8] = { 0x3F, 0x07, 0xE8, 0x07, 0x01, 0x20, 0x01, 0xFF};
  uint8_t Init2[8] = { 0x3F, 0x07, 0xE8, 0x07, 0x01, 0x04, 0x01, 0xFF};
  uint8_t Init3[8] = { 0x3F, 0x07, 0xE8, 0x07, 0x01, 0x05, 0x01, 0xFF};


void setup(){
  display1.begin(&Adafruit128x64, SCREEN_ADDRESS1);
  display2.begin(&Adafruit128x64, SCREEN_ADDRESS2);
  ibusTrx.begin(Serial);
  //Read bus enable
    pinMode(IbusEnable, OUTPUT);
    digitalWrite(IbusEnable, HIGH);
  //SD State
    pinMode(botLed, OUTPUT);
    digitalWrite(botLed, LOW);
  //Auto headlight state
    pinMode(midLed, OUTPUT);
    digitalWrite(midLed, LOW);
  //Screen init
    display1.clear();
    display2.clear();
    display1.setFont(Adafruit5x7);
    display2.setFont(Adafruit5x7);

  //Input pin
    pinMode(Button1, INPUT_PULLUP);
    pinMode(Button2, INPUT_PULLUP);


  //ibusTrx.write(Ignition);
  //ibusTrx.write(Wipe);
  display1.println("   IBUS Datalogger");
  display1.println("          &");
  display1.println("       decoder");
  display1.println(" ");
  display1.println(" ");
  display1.println("         V0.2");
  display1.println(" ");
  display1.println(" ");
  display1.print("2023           Diolum");

  pinMode(chipSelect, OUTPUT);
  
  if (SD.begin()){         
    display2.clear();
    display2.println("SD card ready");
    digitalWrite(botLed, HIGH);
    LOG=1;
  } 
  else{      
    display2.clear();
    display2.println("SD card init failed");
    LOG=0;
    return;
  }
  sdcard_file = SD.open("data.txt", FILE_WRITE);
  sdcard_file.println("NEW RECORD:");
  sdcard_file.close(); // close the file
}

void loop() {
  button1State = digitalRead(Button1);
  if (button1State != lastButton1State) {
    if (button1State == LOW) {
      buttonPushCounter++;
    } 
    lastButton1State = button1State;
  }
  
  button2State = digitalRead(Button2);
  if (button2State != lastButton2State) {
    if (button2State == LOW) {
      button2wasPushed=1;
    } 
    lastButton2State = button2State;
  }
  
  if (buttonPushCounter == 1) {
    firstLaunch8 = 0;
    if (firstLaunch1 == 0) {
      ibusTrx.write(Ready);
      display1.clear();
      display2.clear(); 
      display2.println("Live Read and log");
      firstLaunch1 = 1;
     } 
    loop1();
    
  }
  if (buttonPushCounter == 2) {
    firstLaunch1 = 0;
     if (firstLaunch2 == 0) {
      digitalWrite(midLed, LOW);
      display1.clear();
      display2.clear(); 
      display2.println("RLS INFO");
      firstLaunch2 = 1;
     } 
    loop2();
  }
  if (buttonPushCounter == 3) {
      firstLaunch2 = 0;
      if (firstLaunch3 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS Light values");
        firstLaunch3 = 1;
      } 
      loop3();
    }
  if (buttonPushCounter == 4) {
      firstLaunch3 = 0;
      if (firstLaunch4 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS Wipe values");
        firstLaunch4 = 1;
      } 
      loop4();
    }
  if (buttonPushCounter == 5) {
      firstLaunch4 = 0;
      if (firstLaunch5 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS Temp value");
        firstLaunch5 = 1;
      } 
      loop5();
    }

  if (buttonPushCounter == 6) {
      firstLaunch5 = 0;
      if (firstLaunch6 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS Faults");
        firstLaunch6 = 1;
      } 
      loop6();
    }


      if (buttonPushCounter == 7) {
      firstLaunch6 = 0;
      if (firstLaunch7 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS Erase Faults");
        display1.println("RLS Erase Faults ?");
        display1.println("Ready ? ");
        firstLaunch7 = 1;
      } 
      loop7();
    }

    if (buttonPushCounter == 8) {
      firstLaunch7 = 0;
      if (firstLaunch8 == 0) {
        display1.clear();
        display2.clear(); 
        display2.println("RLS INIT");
        display1.println("RLS Erase ADAPTATIONS?");
        display1.println("Sure ? ");
        firstLaunch8 = 1;
      } 
      loop8();
    }

    if (buttonPushCounter == 9) {
      firstLaunch8 = 0;
      if (firstLaunch9 == 0) {
        display1.clear();
        display2.clear(); 
        firstLaunch9 = 1;
        buttonPushCounter = 0;
        display2.println("         END");
      } 
    }

}

void loop1(){//Display and log to SD


  bool messageWaiting = ibusTrx.available();

  // if there's a message waiting, check it out
  if (messageWaiting) {
    // read the incoming message (this copies the message and clears the receive buffer)
    IbusMessage message = ibusTrx.readMessage();

    sdcard_file = SD.open("data.txt", FILE_WRITE);
    if (sdcard_file== true & LOG==1) {  
      digitalWrite(LED_BUILTIN, HIGH);
      sdcard_file.print(" S: ");
      if (message.source() < 16){sdcard_file.print("0");}
      sdcard_file.print(message.source(), HEX);
      sdcard_file.print(" L: ");
      if (message.length() < 16){sdcard_file.print("0");}
      sdcard_file.print(message.length(), HEX);
      sdcard_file.print(" D: ");
      if (message.destination() < 16){sdcard_file.print("0");}
      sdcard_file.print(message.destination(), HEX);
      sdcard_file.print(" C: ");
      if (message.b(0) < 16){sdcard_file.print("0");}
      sdcard_file.print(message.b(0), HEX);
      sdcard_file.print(" O: ");
      for (int i=1; i < message.length()-2; i++){
        if (message.b(i) < 16){sdcard_file.print("0");}
        sdcard_file.print(message.b(i), HEX);
        sdcard_file.print(" ");
      }
    }
    
    display1.clear();
    display2.clear();      
    display2.println("RAW HEX DATA:");               
    display2.print("S:");
    if (message.source() < 16){display2.print("0");}
    display2.print(message.source(), HEX);
              
    display2.print(" L:");
    if (message.length() < 16){display2.print("0");}
    display2.print(message.length(), DEC);
            
    display2.print(" D:");
    if (message.destination() < 16){display2.print("0");}
    display2.print(message.destination(), HEX);

    display2.print(" C:");
    if (message.b(0) < 16){display2.print("0");}
    display2.println(message.b(0), HEX); 
    for (int i=1; i < message.length()-2; i++){
      if (j<7){
        if (message.b(i) < 16){display2.print("0");}
          display2.print(message.b(i), HEX);
          display2.print(" ");
          j++;
        }
        else{
          if (message.b(i) < 16){display2.print("0");}
          display2.print(message.b(i), HEX);
          display2.println(" ");
          j=1;
        }
    }
    j=1;
    display2.println(" ");


    /***********************************
    *       ok after reset (00)        *
    ***********************************/
    if (message.b(0) == 0x00){
      display1.print("Read Identity");
      sdcard_file.print(" Read Identity");
    }

    /************************************
    *       ok after reset (02)        *
    ***********************************/
    if (message.b(0) == 0x02 && message.b(1) == 0x01){
      display1.print("Ready after reset");
      sdcard_file.print(" Ready after reset");
    }

    /**************************************
    *      Read fault memory (08)        *
    *************************************/
    if (message.b(0) == 0x08){
      display1.print("Read fault memory");
      sdcard_file.print(" Read fault memory");
    }

    /*********************************
    *       Read Coding (08)        *
    ********************************/
    if (message.b(0) == 0x08){
      display1.print("Read Coding data");
      sdcard_file.print(" Read Coding data");
    }

    /*********************************
    *    Write coding data (09)     *
    ********************************/
    if (message.b(0) == 0x09){
      display1.print("Write coding data");
      sdcard_file.print(" Write coding data");
    }

    /**************************
    *    IO status  (0B)     *
    *************************/
    if (message.b(0) == 0x0B){
      display1.print("Diag IO Status");
      sdcard_file.print(" Diag IO Status");
    }

    /************************************
    *      IGN Status request (10)     *
    ***********************************/
    if (message.b(0) == 0x10){
      display1.print("IGN Request");
      sdcard_file.print(" IGN Request");
      // ibusTrx.write(Ignition);
    }

    /***************************
    *      IGN Status (11)     *
    ***************************/
    if (message.b(0) == 0x11){
      display1.println("IGN Status: ");
      sdcard_file.print(" IGN Status: ");
      if (bitRead(message.b(1), 0)){ display1.print("OFF");sdcard_file.print(" OFF");}        
      if (bitRead(message.b(1), 1)){ display1.print("Pos1_Acc");sdcard_file.print(" Pos1_Acc");}
      if (bitRead(message.b(1), 2)){ display1.print("Pos2_On");sdcard_file.print(" Pos2_On");}
      if (bitRead(message.b(1), 3)){ display1.print("Pos3_Start");sdcard_file.print(" Pos3_Start");}
    }

    /******************************************
    *      IKE sensor status request (12)     *
    ******************************************/
    if (message.b(0) == 0x12){
      display1.print("IKE sensor status request");
      sdcard_file.print(" IKE sensor status request");
    }

    /************************************
    *      IKE sensor status (13)       *
    ************************************/
    if (message.b(0) == 0x18){
      display1.print("IKE sensor status");
      sdcard_file.print(" IKE sensor status");
    }

    /*************************
    *      Odometer (17)     *
    *************************/
    if (message.b(0) == 0x17){
      display1.print("Odometer");
      sdcard_file.print(" Odometer");
    }

    /************************************
    *       Vehicule speed (18)        *
    ***********************************/
    if (message.b(0) == 0x18){
      display1.println("Vehicule speed :");
      sdcard_file.print(" Vehicule speed");
      display1.print(message.b(1), HEX);
    }
 
    /*********************
    *      Temp (19)     *
    *********************/
    if (message.b(0) == 0x19){
      display1.print("Temp Out/Coolant");
      sdcard_file.print(" Temp Out/Coolant");
    }

    /************************************
    *          Temp request (1D)        *
    ************************************/
    if (message.b(0) == 0x1D){
      display1.print("TEMP Request");
      sdcard_file.print(" TEMP Request");
    }

    /********************************
    *       Update text (24)        *
    ********************************/
    if (message.b(0) == 0x24){
      display1.print("Update text");
      sdcard_file.print(" Update text");
    }

    /***********************************
    *    Vehicle data request (53)     *
    ***********************************/
    if (message.b(0) == 0x53){
      display1.print("Vehicle data request");
      sdcard_file.print(" Vehicle data request");
    }

    /***********************************
    *         Wipe interval (58)       *
    ***********************************/
    if (message.b(0) == 0x58){
      display1.println("Wipe: ");
      sdcard_file.print(" Wipe");
      display1.print(message.b(1), HEX); display1.print(" ");
      display1.print(message.b(2), HEX); display1.print(" ");
      display1.print(message.b(3), HEX); display1.print(" ");
    }

    /***********************************
    *             RLS (59)             *
    ***********************************/
    if (message.b(0) == 0x59){
      display1.println("RLS:");
      sdcard_file.print(" RLS:");
      if (bitRead(message.b(1), 0)){display1.print("ON");sdcard_file.print(" ON");  digitalWrite(30, HIGH);}
      else{display1.print("OFF");sdcard_file.print(" OFF");  digitalWrite(30, LOW);}
      display1.print(" I:");  
      sdcard_file.print(" I:");  
      display1.print((message.b(1)) >> 4, HEX);display1.print(" ");
      if (bitRead(message.b(2), 0)){display1.print("Twilight");sdcard_file.print(" Twilight");}
      if (bitRead(message.b(2), 1)){display1.print("Darkness");sdcard_file.print(" Darkness");}
      if (bitRead(message.b(2), 2)){display1.print("Rain");sdcard_file.print(" Rain");}
      if (bitRead(message.b(2), 3)){display1.print("Tunnel");sdcard_file.print(" Tunnel");}
      if (bitRead(message.b(2), 4)){display1.print("Garage");sdcard_file.print(" Garage");}
    }

    /***********************************
    *        LIGHT STATUS (5B)         *
    ***********************************/
    if (message.b(0) == 0x5B){
      display1.print("LIGHT STATUS:");
      if (message.b(3) != 0 || message.b(4) != 0){ display1.println("  FAULT");sdcard_file.print(" FAULT");}
      else{display1.println(" ");}
      if (message.b(1) == 0){ display1.print("OFF");sdcard_file.print(" OFF");}
      if (bitRead(message.b(1), 0)){ display1.print("PK");sdcard_file.print(" PK");}
      if (bitRead(message.b(1), 1)){ display1.print(" LO");sdcard_file.print(" LO");}
      if (bitRead(message.b(1), 2)){ display1.print(" HI");sdcard_file.print(" HI");}
      if (bitRead(message.b(1), 3)){ display1.print(" FF");sdcard_file.print(" FF");}
      if (bitRead(message.b(1), 4)){ display1.print(" RF");sdcard_file.print(" RF");}
      if (bitRead(message.b(1), 5)){ display1.print(" <-");sdcard_file.print(" <-");}
      if (bitRead(message.b(1), 6)){ display1.print(" ->");sdcard_file.print(" ->");}
    }

    /************************************
    *       DIMMER display1 (5C)         *
    ************************************/
    if (message.b(0) == 0x5C){
      display1.println("DIMMER:");
      sdcard_file.print(" DIMMER: ");
      display1.print(message.b(1), DEC);display1.print(" ");
      display1.print(message.b(2), DEC);
      sdcard_file.print((message.b(1)));
      sdcard_file.print(" ");
      sdcard_file.print((message.b(2)));
    }

    /*************************************************
    *       Light dimmer status request (5D)         *
    *************************************************/
    if (message.b(0) == 0x5D){
      display1.print("Light dimmer status request");
      sdcard_file.print(" Light dimmer status request");
    }

    /**********************************
    *     Rain sensor status (71)     *
    **********************************/
    if (message.b(0) == 0x71){
      display1.print("Rain sensor status");
      sdcard_file.print(" Rain sensor status");
    }
 
    /************************************
    *     Wiper Status request (75)     *
    ************************************/
    if (message.b(0) == 0x75){
      display1.print("Wiper Request");
      sdcard_file.print(" Wiper Request");
      //ibusTrx.write(Wipe);
    }

    /**********************************************
    *    Diagnostic command acknowledged (A0)     *
    **********************************************/
    if (message.b(0) == 0xA0){
      display1.print("Diag Com Ackowledged");
      sdcard_file.print(" Diag Com Ackowledged");
    }

    sdcard_file.println(" ");
    sdcard_file.close(); // close the file
    
    
  }
}//End of loop1

void loop2(){//Read RLS INFO

  //Sensor Status
  if (ibusTrx.available()) {    
      ibusTrx.write(RlsStatus); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.setRow(0);
        display1.print("Teilenummer: ");
        display1.print(message.b(1), HEX); 
        display1.print(message.b(2), HEX); 
        display1.print(message.b(3), HEX); 
        display1.println(message.b(4), HEX); 

        display1.print("HW: ");
        display1.println(message.b(5), HEX); 
        display1.print("SW: ");
        display1.println(message.b(12), HEX); 

        display1.print("Coding index: ");
        display1.println(message.b(6), HEX); 

        display1.print("Diag index: ");
        display1.println(message.b(7), HEX);   

        display1.print("BUS index: ");
        display1.println(message.b(8), HEX); 

        display1.print("Herstelldatum:  ");
        display1.print(message.b(9), HEX); 
        display1.print("/");
        display1.println(message.b(10), HEX); 

      }
    }
}//End of loop2

void loop3(){//Read RLS Light
  //Sensor light
  if (ibusTrx.available()) {    
      ibusTrx.write(RLSLightRead); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.setRow(0);
        for(int i=1 ; i<5; i++){
          switch (i) {
            case 1:
                display1.print("Ambi. unfiltered: "); 
            break;
            case 2:
                display1.print("Front unfiltered: "); 
            break;
            case 3:
                display1.print("Ambi. filtered:   "); 
            break;
            case 4:
                display1.print("Front filtered:   "); 
            break;
          }
        if (message.b(i) < 100){display1.print(" ");}
        if (message.b(i) < 10){display1.print(" ");}
        display1.println(message.b(i), DEC); 
      }
    }
  }
}//End of loop3

void loop4(){//Read RLS Wipe
  if (ibusTrx.available()) {    
      ibusTrx.write(WipeSens1); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.setRow(0);
        for(int i=1 ; i<9; i++){
          switch (i) {
            case 1:
                display1.print("Wipe1:            "); 
            break;
            case 2:
                display1.print("Unkn1:            "); 
            break;
            case 3:
                display1.print("Wipe2:            "); 
            break;
            case 4:
                display1.print("Unkn2:            "); 
            break;
            case 5:
                display1.print("Wipe3:            "); 
            break;
            case 6:
                display1.print("Unkn3:            "); 
            break;
            case 7:
                display1.print("Wipe4:            "); 
            break;
            case 8:
                display1.print("Unkn4:            "); 
            break;
          }
        if (message.b(i) < 100){display1.print(" ");}
        if (message.b(i) < 10){display1.print(" ");}
        display1.println(message.b(i), DEC); 
        }
      }
  }
}//End of loop4

void loop5(){//Read RLS TEMP
  //Sensor temp
  if (ibusTrx.available()) {  
      ibusTrx.write(RLSTemp); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.setRow(0);
        display1.print("Sensor Temp:      "); 
        if (message.b(1) < 100){display1.print(" ");}
        if (message.b(1) < 10){display1.print(" ");}
        display1.println(message.b(1), DEC); 

      }
    }
}//End of loop5

void loop6(){//Read Defaults
  if (ibusTrx.available()) {  
      ibusTrx.write(RLSfault); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
      display1.clear();
      for (int i=1; i < message.length()-2; i++){
        if (j<7){
         if (message.b(i) < 16){display1.print("0");}
          display1.print(message.b(i), HEX);
          display1.print(" ");
          j++;
        }
        else{
          if (message.b(i) < 16){display1.print("0");}
          display1.print(message.b(i), HEX);
          display1.println(" ");
          j=1;
        }
     }
      }
      }
}//End of loop6

void loop7(){//Erase Defaults
  if (ibusTrx.available()) {  
    if (button2wasPushed==1){
      ibusTrx.write(RLSclearfault); 
      IbusMessage message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.println("Done");
        button2wasPushed =0;
      }
    }
  }
}//End of loop7

void loop8(){//Erase Defaults
  if (ibusTrx.available()) {  
    if (button2wasPushed==1){
      ibusTrx.write(Init1); 
      IbusMessage message = ibusTrx.readMessage(); 
      ibusTrx.write(Init2); 
      message = ibusTrx.readMessage(); 
      ibusTrx.write(Init3); 
      message = ibusTrx.readMessage(); 
      if (message.b(0) == 0xA0){
        display1.println("INIT 1: Done");
        button2wasPushed =0;
      }
      else{display1.println("INIT FAIL");button2wasPushed =0;}
    }
  }
}//End of loop8






