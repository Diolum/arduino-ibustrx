//SSD1306
  #include <SPI.h>
  #include "SSD1306Ascii.h"
  #include "SSD1306AsciiAvrI2c.h"
  #define SCREEN_ADDRESS 0x3C 
  SSD1306AsciiAvrI2c display;

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

void setup(){
  display.begin(&Adafruit128x64, SCREEN_ADDRESS);
  ibusTrx.begin(Serial);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clear();
  display.setFont(Adafruit5x7);

  //ibusTrx.write(Ignition);
  //ibusTrx.write(Wipe);
  display.print("IBUS OLED DECODER");

  pinMode(chipSelect, OUTPUT);
  if (SD.begin()){         
    display.clear();
    display.print("SD card is ready to use.");
    LOG=1;
  } 
  else{      
    display.clear();
    display.print("SD card initialization failed");
    LOG=0;
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  sdcard_file = SD.open("data.txt", FILE_WRITE);
  sdcard_file.println("NEW RECORD:");
  sdcard_file.close(); // close the file
  digitalWrite(LED_BUILTIN, LOW);
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
    
    display.clear();
      
                    
    display.print("S:");
    if (message.source() < 16){display.print("0");}
    display.print(message.source(), HEX);
              
    display.print(" L:");
    if (message.length() < 16){display.print("0");}
    display.print(message.length(), DEC);
            
    display.print(" D:");
    if (message.destination() < 16){display.print("0");}
    display.print(message.destination(), HEX);

    display.print(" C:");
    if (message.b(0) < 16){display.print("0");}
    display.println(message.b(0), HEX);

    for (int i=1; i < message.length()-2; i++){
      if (j<7){
        if (message.b(i) < 16){display.print("0");}
          display.print(message.b(i), HEX);
          display.print(" ");
          j++;
        }
        else{
          if (message.b(i) < 16){display.print("0");}
          display.print(message.b(i), HEX);
          display.println(" ");
          j=1;
        }
    }
    j=1;
    display.println(" ");


    /***********************************
    *       ok after reset (00)        *
    ***********************************/
    if (message.b(0) == 0x00){
      display.print("Read Identity");
      sdcard_file.print(" Read Identity");
    }

    /************************************
    *       ok after reset (02)        *
    ***********************************/
    if (message.b(0) == 0x02 && message.b(1) == 0x01){
      display.print("Ready after reset");
      sdcard_file.print(" Ready after reset");
    }

    /**************************************
    *      Read fault memory (08)        *
    *************************************/
    if (message.b(0) == 0x08){
      display.print("Read fault memory");
      sdcard_file.print(" Read fault memory");
    }

    /*********************************
    *       Read Coding (08)        *
    ********************************/
    if (message.b(0) == 0x08){
      display.print("Read Coding data");
      sdcard_file.print(" Read Coding data");
    }

    /*********************************
    *    Write coding data (09)     *
    ********************************/
    if (message.b(0) == 0x09){
      display.print("Write coding data");
      sdcard_file.print(" Write coding data");
    }

    /**************************
    *    IO status  (0B)     *
    *************************/
    if (message.b(0) == 0x0B){
      display.print("Diag IO Status");
      sdcard_file.print(" Diag IO Status");
    }

    /************************************
    *      IGN Status request (10)     *
    ***********************************/
    if (message.b(0) == 0x10){
      display.print("IGN Request");
      sdcard_file.print(" IGN Request");
      // ibusTrx.write(Ignition);
    }

    /***************************
    *      IGN Status (11)     *
    ***************************/
    if (message.b(0) == 0x11){
      display.println("IGN Status: ");
      sdcard_file.print(" IGN Status: ");
      if (bitRead(message.b(1), 0)){ display.print("OFF");sdcard_file.print(" OFF");}        
      if (bitRead(message.b(1), 1)){ display.print("Pos1_Acc");sdcard_file.print(" Pos1_Acc");}
      if (bitRead(message.b(1), 2)){ display.print("Pos2_On");sdcard_file.print(" Pos2_On");}
      if (bitRead(message.b(1), 3)){ display.print("Pos3_Start");sdcard_file.print(" Pos3_Start");}
    }

    /******************************************
    *      IKE sensor status request (12)     *
    ******************************************/
    if (message.b(0) == 0x12){
      display.print("IKE sensor status request");
      sdcard_file.print(" IKE sensor status request");
    }

    /************************************
    *      IKE sensor status (13)       *
    ************************************/
    if (message.b(0) == 0x18){
      display.print("IKE sensor status");
      sdcard_file.print(" IKE sensor status");
    }

    /*************************
    *      Odometer (17)     *
    *************************/
    if (message.b(0) == 0x17){
      display.print("Odometer");
      sdcard_file.print(" Odometer");
    }

    /************************************
    *       Vehicule speed (18)        *
    ***********************************/
    if (message.b(0) == 0x18){
      display.println("Vehicule speed :");
      sdcard_file.print(" Vehicule speed");
      display.print(message.b(1), HEX);
    }
 
    /*********************
    *      Temp (19)     *
    *********************/
    if (message.b(0) == 0x19){
      display.print("Temp Out/Coolant");
      sdcard_file.print(" Temp Out/Coolant");
    }

    /************************************
    *          Temp request (1D)        *
    ************************************/
    if (message.b(0) == 0x1D){
      display.print("TEMP Request");
      sdcard_file.print(" TEMP Request");
    }

    /********************************
    *       Update text (24)        *
    ********************************/
    if (message.b(0) == 0x24){
      display.print("Update text");
      sdcard_file.print(" Update text");
    }

    /***********************************
    *    Vehicle data request (53)     *
    ***********************************/
    if (message.b(0) == 0x53){
      display.print("Vehicle data request");
      sdcard_file.print(" Vehicle data request");
    }

    /***********************************
    *         Wipe interval (58)       *
    ***********************************/
    if (message.b(0) == 0x58){
      display.println("Wipe: ");
      sdcard_file.print(" Wipe");
      display.print(message.b(1), HEX); display.print(" ");
      display.print(message.b(2), HEX); display.print(" ");
      display.print(message.b(3), HEX); display.print(" ");
    }

    /***********************************
    *             RLS (59)             *
    ***********************************/
    if (message.b(0) == 0x59){
      display.println("RLS:");
      sdcard_file.print(" RLS:");
      if (bitRead(message.b(1), 0)){display.print("ON");sdcard_file.print(" ON");}
      else{display.print("OFF");sdcard_file.print(" OFF");}
      display.print(" I:");  
      sdcard_file.print(" I:");  
      display.print((message.b(1)) >> 4, HEX);display.print(" ");
      if (bitRead(message.b(2), 0)){display.print("Twilight");sdcard_file.print(" Twilight");}
      if (bitRead(message.b(2), 1)){display.print("Darkness");sdcard_file.print(" Darkness");}
      if (bitRead(message.b(2), 2)){display.print("Rain");sdcard_file.print(" Rain");}
      if (bitRead(message.b(2), 3)){display.print("Tunnel");sdcard_file.print(" Tunnel");}
      if (bitRead(message.b(2), 4)){display.print("Garage");sdcard_file.print(" Garage");}
    }

    /***********************************
    *        LIGHT STATUS (5B)         *
    ***********************************/
    if (message.b(0) == 0x5B){
      display.print("LIGHT STATUS:");
      if (message.b(3) != 0 || message.b(4) != 0){ display.println("  FAULT");sdcard_file.print(" FAULT");}
      else{display.println(" ");}
      if (message.b(1) == 0){ display.print("OFF");sdcard_file.print(" OFF");}
      if (bitRead(message.b(1), 0)){ display.print("PK");sdcard_file.print(" PK");}
      if (bitRead(message.b(1), 1)){ display.print(" LO");sdcard_file.print(" LO");}
      if (bitRead(message.b(1), 2)){ display.print(" HI");sdcard_file.print(" HI");}
      if (bitRead(message.b(1), 3)){ display.print(" FF");sdcard_file.print(" FF");}
      if (bitRead(message.b(1), 4)){ display.print(" RF");sdcard_file.print(" RF");}
      if (bitRead(message.b(1), 5)){ display.print(" <-");sdcard_file.print(" <-");}
      if (bitRead(message.b(1), 6)){ display.print(" ->");sdcard_file.print(" ->");}
    }

    /************************************
    *       DIMMER DISPLAY (5C)         *
    ************************************/
    if (message.b(0) == 0x5C){
      display.println("DIMMER:");
      sdcard_file.print(" DIMMER: ");
      display.print(message.b(1), DEC);display.print(" ");
      display.print(message.b(2), DEC);
      sdcard_file.print((message.b(1)));
      sdcard_file.print(" ");
      sdcard_file.print((message.b(2)));
    }

    /*************************************************
    *       Light dimmer status request (5D)         *
    *************************************************/
    if (message.b(0) == 0x5D){
      display.print("Light dimmer status request");
      sdcard_file.print(" Light dimmer status request");
    }

    /**********************************
    *     Rain sensor status (71)     *
    **********************************/
    if (message.b(0) == 0x71){
      display.print("Rain sensor status");
      sdcard_file.print(" Rain sensor status");
    }
 
    /************************************
    *     Wiper Status request (75)     *
    ************************************/
    if (message.b(0) == 0x75){
      display.print("Wiper Request");
      sdcard_file.print(" Wiper Request");
      //ibusTrx.write(Wipe);
    }

    /**********************************************
    *    Diagnostic command acknowledged (A0)     *
    **********************************************/
    if (message.b(0) == 0xA0){
      display.print("Diag Com Ackowledged");
      sdcard_file.print(" Diag Com Ackowledged");
    }

    sdcard_file.println(" ");
    sdcard_file.close(); // close the file
    digitalWrite(LED_BUILTIN, LOW);
    
    
  }

}
