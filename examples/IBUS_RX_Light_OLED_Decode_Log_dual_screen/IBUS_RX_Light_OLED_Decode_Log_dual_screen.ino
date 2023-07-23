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
  IbusTrx ibusTrx;

//SD
  #include <SD.h>
  const int chipSelect = 53;
  int LOG=0;
  File sdcard_file;


//IBUS MESSAGES without checksum
  //uint8_t IgnOFF[5] = { 0x80, 0x04, 0xBF, 0x11, 0x00 };
  //uint8_t IgnACC[5] = { 0x80, 0x04, 0xBF, 0x11, 0x01 };
  //uint8_t IgnON[5] = { 0x80, 0x04, 0xBF, 0x11, 0x03 };
  //uint8_t IgnSTART[5] = { 0x80, 0x04, 0xBF, 0x11, 0x07 };

  //uint8_t Wipe[5] = { 0x00, 0x04, 0xE8, 0x77, 0x05 };


  uint8_t RlsStatus[4] = { 0x3F, 0x03, 0xE8, 0x00 };
  //uint8_t RLSfault[4] = { 0x3F, 0x03, 0xE8, 0x04 };
  //uint8_t RLSclearfault[4] = { 0x3F, 0x03, 0xE8, 0x05 };
  uint8_t RLSFontUnfilter[7] = { 0x3F, 0x06 , 0xE8 , 0x06 , 0x02 , 0x55 , 0x01};
  uint8_t RLSAmbiantUnfilter[7] = { 0x3F, 0x06 , 0xE8 , 0x06 , 0x02 , 0x54 , 0x01};
  uint8_t RLSTemp[7] = { 0x3F, 0x06 , 0xE8 , 0x06 , 0x02 , 0x5E , 0x01};


void setup(){
  display1.begin(&Adafruit128x64, SCREEN_ADDRESS1);
  display2.begin(&Adafruit128x64, SCREEN_ADDRESS2);
  ibusTrx.begin(Serial);
  //Read bus enable
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  //SD State
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
  //Auto headlight state
  pinMode(30, OUTPUT);
  digitalWrite(30, LOW);
  //Screen init
  display1.clear();
  display2.clear();
  display1.setFont(Adafruit5x7);
  display2.setFont(Adafruit5x7);

  //ibusTrx.write(Ignition);
  //ibusTrx.write(Wipe);
  display1.println("   IBUS Datalogger");
  display1.println("          &");
  display1.println("       decoder");
  display1.println(" ");
  display1.println(" ");
  display1.println("         V0.1");
  display1.println(" ");
  display1.println(" ");
  display1.print("2023           Diolum");

  pinMode(chipSelect, OUTPUT);
  
  if (SD.begin()){         
    display2.clear();
    display2.println("SD card ready");
    digitalWrite(25, HIGH);
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








void loop(){

  // available() has to be called repeatedly, with no delay() in between
  // this function returns true if a new message is available for reading
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

}//End of loop
