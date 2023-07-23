#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <IbusTrx.h>
#include <Wire.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


unsigned int messageSource = 0;
unsigned int messageDestination = 0;
unsigned int messageLength = 0;
unsigned int messageCommand = 0;
unsigned int message = 0;
int a=0;
// create a new IbusTrx instance
IbusTrx ibusTrx;


void setup(){
  ibusTrx.begin(Serial);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); 
}



void loop(){
  // available() has to be called repeatedly, with no delay() in between
  // this function returns true if a new message is available for reading
  bool messageWaiting = ibusTrx.available();

  // if there's a message waiting, check it out
  if (messageWaiting) {
    // read the incoming message (this copies the message and clears the receive buffer)
    IbusMessage message = ibusTrx.readMessage();


   
    display.clearDisplay();
    display.display();
           
    display.setCursor(0, 0);          
    display.print("S:");
    if (message.source() < 16){display.print("0");}
    display.print(message.source(), HEX);
    
    display.setCursor(30, 0);          
    display.print("L:");
    if (message.length() < 16){display.print("0");}
    display.print(message.length(), DEC);
    
    display.setCursor(60, 0);          
    display.print("D:");
    if (message.destination() < 16){display.print("0");}
    display.print(message.destination(), HEX);
    
    display.setCursor(90, 0); 
    display.print("C:");
    if (message.b(0) < 16){display.print("0");}
    display.print(message.b(0), HEX);

    display.setCursor(0,9);
    for (int i=1; i < message.length()-2; i++){
    if (message.b(i) < 16){display.print("0");}
    display.print(message.b(i), HEX);
    display.print(" ");
}


/************************************
 *       ok after reset (00)        *
 ***********************************/
 if (message.b(0) == 0x00){
        display.setCursor(0,24); 
        display.print("Read Identity");
      }

/************************************
 *       ok after reset (02)        *
 ***********************************/
      if (message.b(0) == 0x02 && message.b(1) == 0x01){
        display.setCursor(0,24); 
        display.print("Ready after reset");
      }

/**************************************
 *      Read fault memory (08)        *
 *************************************/
 if (message.b(0) == 0x08){
        display.setCursor(0,24); 
        display.print("Read fault memory");
      }

/*********************************
 *       Read Coding (08)        *
 ********************************/
 if (message.b(0) == 0x08){
        display.setCursor(0,24); 
        display.print("Read Coding data");
      }

/*********************************
 *    Write coding data (09)     *
 ********************************/
 if (message.b(0) == 0x09){
        display.setCursor(0,24); 
        display.print("Write coding data");
      }

/**************************
 *    IO status  (0B)     *
 *************************/
 if (message.b(0) == 0x0B){
        display.setCursor(0,24); 
        display.print("Diag IO Status");
      }


/************************************
 *      IGN Status request (10)     *
 ***********************************/
      if (message.b(0) == 0x10){
        display.setCursor(0,24); 
        display.print("IGN Request");
       // ibusTrx.write(Ignition);
      }


 /***************************
 *      IGN Status (11)     *
 ***************************/
      if (message.b(0) == 0x11){
        display.setCursor(0,24); 
         display.print("IGN Status: ");
         if (bitRead(message.b(1), 0)){ display.print("OFF");}        
         if (bitRead(message.b(1), 1)){ display.print("Pos1_Acc");}
         if (bitRead(message.b(1), 2)){ display.print("Pos2_On");}
         if (bitRead(message.b(1), 3)){ display.print("Pos3_Start");}
      }

 /******************************************
 *      IKE sensor status request (12)     *
 ******************************************/
      if (message.b(0) == 0x12){
        display.setCursor(0,24); 
         display.print("IKE sensor status request");
      }

/************************************
 *      IKE sensor status (13)       *
 ************************************/
      if (message.b(0) == 0x18){
        display.setCursor(0,24); 
        display.print("IKE sensor status");
      }


 /*********************
 *      Odometer (17)     *
 *********************/
      if (message.b(0) == 0x17){
        display.setCursor(0,24); 
         display.print("Odometer");
      }
/************************************
 *       Vehicule speed (18)        *
 ***********************************/
      if (message.b(0) == 0x18){
        display.setCursor(0,24); 
        display.print("Vehicule speed :");
        display.print(message.b(1), HEX);
      }

      
 /*********************
 *      Temp (19)     *
 *********************/
      if (message.b(0) == 0x19){
        display.setCursor(0,24); 
         display.print("Temp Out/Coolant");
      }

 /************************************
 *          Temp request (1D)        *
 ************************************/
      if (message.b(0) == 0x1D){
        display.setCursor(0,24); 
        display.print("TEMP Request");
      }


 /********************************
 *       Update text (24)        *
 ********************************/
      if (message.b(0) == 0x24){
        display.setCursor(0,24); 
        display.print("Update text");
      }

/************************************
 *    Vehicle data request (53)     *
 ***********************************/
      if (message.b(0) == 0x53){
        display.setCursor(0,24); 
        display.print("Vehicle data request");
      }


/************************************
 *         Wipe interval (58)       *
 ***********************************/
      if (message.b(0) == 0x58){
        display.setCursor(0,24); 
        display.print("Wipe: ");
        display.setCursor(50,24); 
        display.print(message.b(1), HEX);
        display.setCursor(80,24); 
        display.print(message.b(2), HEX);
        display.setCursor(110,24); 
        display.print(message.b(3), HEX);
      }


/************************************
 *             RLS (59)             *
 ***********************************/
      if (message.b(0) == 0x59){
        display.setCursor(0,24); 
        display.print("RLS:");
        display.setCursor(25,24); 
        if (bitRead(message.b(1), 0)){display.print("ON");}
        else{display.print("OFF");}
        display.setCursor(48,24);
        display.print("I:");    
        display.print((message.b(1)) >> 4, HEX);
        display.setCursor(76,24);
        if (bitRead(message.b(2), 0)){display.print("Twilight");}
        if (bitRead(message.b(2), 1)){display.print("Darkness");}
        if (bitRead(message.b(2), 2)){display.print("Rain");}
        if (bitRead(message.b(2), 3)){display.print("Tunnel");}
        if (bitRead(message.b(2), 4)){display.print("Garage");}
      }



/************************************
 *        LIGHT STATUS (5B)         *
 ***********************************/
      if (message.b(0) == 0x5B){
        display.setCursor(0,24); 
        if (message.b(1) == 0){ display.print("OFF");}
        if (bitRead(message.b(1), 0)){ display.print("PK");}
        if (bitRead(message.b(1), 1)){ display.print(" LO");}
        if (bitRead(message.b(1), 2)){ display.print(" HI");}
        if (bitRead(message.b(1), 3)){ display.print(" FF");}
        if (bitRead(message.b(1), 4)){ display.print(" RF");}
        if (bitRead(message.b(1), 5)){ display.print(" <-");}
        if (bitRead(message.b(1), 6)){ display.print(" ->");}
        display.setCursor(85,12); 
        if (message.b(3) != 0 || message.b(4) != 0){ display.print("FAULT");}
      }


 /************************************
 *       DIMMER DISPLAY (5C)         *
 ************************************/
      if (message.b(0) == 0x5C){
        display.setCursor(0,18); 
        display.print("DIMMER:");
        display.setCursor(50,18); 
        display.print(message.b(1), DEC);
        display.setCursor(80,18); 
        display.print(message.b(2), DEC);
        display.drawRect(4, 26, 115, 6, WHITE);
        display.fillRect(4, 27, int(message.b(1)*0.45),4, WHITE);
        display.drawLine(int(message.b(2)*0.45)+4, 26, int(message.b(2)*0.45)+4,32, BLACK);
        display.drawLine(int(message.b(2)*0.45)+5, 26, int(message.b(2)*0.45)+5,32, BLACK);
      }


 /*************************************************
 *       Light dimmer status request (5D)         *
 *************************************************/
      if (message.b(0) == 0x5D){
        display.setCursor(0,24); 
        display.print("Light dimmer status request");
      }


/***********************************
 *     Rain sensor status (71)     *
 **********************************/
      if (message.b(0) == 0x71){
        display.setCursor(0,24); 
        display.print("Rain sensor status");
      }
 
 /************************************
 *     Wiper Status request (75)     *
 ************************************/
      if (message.b(0) == 0x75){
        display.setCursor(0,24); 
        display.print("Wiper Request");
       //ibusTrx.write(Wipe);
      }

 /**********************************************
 *    Diagnostic command acknowledged (A0)     *
 **********************************************/
      if (message.b(0) == 0xA0){
        display.setCursor(0,24); 
        display.print("Diag Com Ackowledged");
      }

 
    display.display();
    
  }

}
