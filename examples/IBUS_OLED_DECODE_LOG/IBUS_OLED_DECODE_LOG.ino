#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <IbusTrx.h>
#include <Wire.h>

#include <SD.h>

const int chipSelect = 53;
File sdcard_file;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

unsigned int messageSource = 0;
unsigned int messageDestination = 0;
unsigned int messageLength = 0;
unsigned int messageCommand = 0;
unsigned int message = 0;
int a=0;
int LOG=0;

#define bitmap_height   128
#define bitmap_width    64
const unsigned char spash [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x0e, 0x7f, 0x70, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x0c, 0x0f, 0xee, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3b, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xfb, 0xff, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x63, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x80, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00,
  0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x00, 
  0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x80, 0x00,
  0x00, 0x00, 0xff, 0xee, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x80, 0x00, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0xff, 0xe0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xbf, 0xf8, 0x00, 0xc0, 0x00, 
  0x00, 0x01, 0x03, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf1, 0xef, 0xff, 0x00, 0x00, 
  0x00, 0x03, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x6f, 0xff, 0xe0, 0x00, 
  0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 
  0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// create a new IbusTrx instance
IbusTrx ibusTrx;

uint8_t Ignition[5] = { 0x80, 0x04, 0xBF, 0x11, 0x03 };//without checksum
//uint8_t Wipe[5] = { 0x00, 0x04, 0xE8, 0x77, 0x05 };
uint8_t Wipe[15] = { 0xBF, 0x0E, 0x02, 0x03, 0x04, 0x07, 0x08 , 0x09 , 0x10, 0x11, 0x12, 0x08 , 0x09 , 0x10, 0x11};
//uint8_t ON[36] ={0x3F , 0x23 , 0xD0 , 0x0C , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27  , 0x28  , 0x29 , 0x30 , 0x31 , 0x32};


void setup(){
  ibusTrx.begin(Serial);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.drawBitmap(0, 0, spash, bitmap_height, bitmap_width, WHITE);
  display.display();
  delay(1000);
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE); 

  //SD
  pinMode(chipSelect, OUTPUT);
  if (SD.begin()){
    display.setCursor(0, 0);          
    display.clearDisplay();
    display.display();
    display.print("SD card is ready to use.");
    LOG=1;
    display.display();
    delay(1000);
  } 
  else{
    display.setCursor(0, 0);          
    display.clearDisplay();
    display.display();
    display.print("SD card initialization failed");
    LOG=0;
    display.display();
    delay(1000);
    return;
  }

  sdcard_file = SD.open("data.txt", FILE_WRITE);
  sdcard_file.println("NEW RECORD:");
  sdcard_file.close(); // close the file


  //ibusTrx.write(Ignition);
  //ibusTrx.write(Wipe);
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
      digitalWrite(13, HIGH);
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
    //



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
        sdcard_file.print(" Read Identity");
      }

/************************************
 *       ok after reset (02)        *
 ***********************************/
      if (message.b(0) == 0x02 && message.b(1) == 0x01){
        display.setCursor(0,24); 
        display.print("Ready after reset");
        sdcard_file.print(" Ready after reset");
      }

/**************************************
 *      Read fault memory (08)        *
 *************************************/
 if (message.b(0) == 0x08){
        display.setCursor(0,24); 
        display.print("Read fault memory");
        sdcard_file.print(" Read fault memory");
      }

/*********************************
 *       Read Coding (08)        *
 ********************************/
 if (message.b(0) == 0x08){
        display.setCursor(0,24); 
        display.print("Read Coding data");
        sdcard_file.print(" Read Coding data");
      }

/*********************************
 *    Write coding data (09)     *
 ********************************/
 if (message.b(0) == 0x09){
        display.setCursor(0,24); 
        display.print("Write coding data");
        sdcard_file.print(" Write coding data");
      }

/**************************
 *    IO status  (0B)     *
 *************************/
 if (message.b(0) == 0x0B){
        display.setCursor(0,24); 
        display.print("Diag IO Status");
        sdcard_file.print(" Diag IO Status");
      }


/************************************
 *      IGN Status request (10)     *
 ***********************************/
      if (message.b(0) == 0x10){
        display.setCursor(0,24); 
        display.print("IGN Request");
        sdcard_file.print(" IGN Request");
        ibusTrx.write(Ignition);
      }


 /***************************
 *      IGN Status (11)     *
 ***************************/
      if (message.b(0) == 0x11){
        display.setCursor(0,24); 
         display.print("IGN Status: ");
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
        display.setCursor(0,24); 
         display.print("IKE sensor status request");
         sdcard_file.print(" IKE sensor status request");
      }

/************************************
 *      IKE sensor status (13)       *
 ************************************/
      if (message.b(0) == 0x18){
        display.setCursor(0,24); 
        display.print("IKE sensor status");
        sdcard_file.print(" IKE sensor status");
      }


 /*********************
 *      Odometer (17)     *
 *********************/
      if (message.b(0) == 0x17){
        display.setCursor(0,24); 
         display.print("Odometer");
         sdcard_file.print(" Odometer");
      }
/************************************
 *       Vehicule speed (18)        *
 ***********************************/
      if (message.b(0) == 0x18){
        display.setCursor(0,24); 
        display.print("Vehicule speed :");
        sdcard_file.print(" Vehicule speed");
        display.print(message.b(1), HEX);
      }

      
 /*********************
 *      Temp (19)     *
 *********************/
      if (message.b(0) == 0x19){
        display.setCursor(0,24); 
         display.print("Temp Out/Coolant");
         sdcard_file.print(" Temp Out/Coolant");
      }

 /************************************
 *          Temp request (1D)        *
 ************************************/
      if (message.b(0) == 0x1D){
        display.setCursor(0,24); 
        display.print("TEMP Request");
        sdcard_file.print(" TEMP Request");
      }


 /********************************
 *       Update text (24)        *
 ********************************/
      if (message.b(0) == 0x24){
        display.setCursor(0,24); 
        display.print("Update text");
        sdcard_file.print(" Update text");
      }

/************************************
 *    Vehicle data request (53)     *
 ***********************************/
      if (message.b(0) == 0x53){
        display.setCursor(0,24); 
        display.print("Vehicle data request");
        sdcard_file.print(" Vehicle data request");
      }


/************************************
 *         Wipe interval (58)       *
 ***********************************/
      if (message.b(0) == 0x58){
        display.setCursor(0,24); 
        display.print("Wipe: ");
        sdcard_file.print(" Wipe");
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
        sdcard_file.print(" RLS:");
        display.setCursor(25,24); 
        if (bitRead(message.b(1), 0)){display.print("ON");sdcard_file.print(" ON");}
        else{display.print("OFF");sdcard_file.print(" OFF");}
        display.setCursor(48,24);
        display.print("I:");    
        sdcard_file.print(" I:");
        display.print((message.b(1)) >> 4, HEX);
        sdcard_file.print((message.b(1)) >> 4);
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
        if (message.b(1) == 0){ display.print("OFF");sdcard_file.print(" OFF");}
        if (bitRead(message.b(1), 0)){ display.print("PK");sdcard_file.print(" PK");}
        if (bitRead(message.b(1), 1)){ display.print(" LO");sdcard_file.print(" LO");}
        if (bitRead(message.b(1), 2)){ display.print(" HI");sdcard_file.print(" HI");}
        if (bitRead(message.b(1), 3)){ display.print(" FF");sdcard_file.print(" FF");}
        if (bitRead(message.b(1), 4)){ display.print(" RF");sdcard_file.print(" RF");}
        if (bitRead(message.b(1), 5)){ display.print(" <-");sdcard_file.print(" <-");}
        if (bitRead(message.b(1), 6)){ display.print(" ->");sdcard_file.print(" ->");}
        display.setCursor(85,12); 
        if (message.b(3) != 0 || message.b(4) != 0){ display.print("FAULT");sdcard_file.print(" FAULT");}
      }


 /************************************
 *       DIMMER DISPLAY (5C)         *
 ************************************/
      if (message.b(0) == 0x5C){
        display.setCursor(0,18); 
        display.print("DIMMER:");
        sdcard_file.print(" DIMMER: ");
        display.setCursor(50,18); 
        display.print(message.b(1), DEC);
         sdcard_file.print((message.b(1)));
         sdcard_file.print(" ");
        display.setCursor(80,18); 
        display.print(message.b(2), DEC);
         sdcard_file.print((message.b(2)));
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
        sdcard_file.print(" Light dimmer status request");
      }


/***********************************
 *     Rain sensor status (71)     *
 **********************************/
      if (message.b(0) == 0x71){
        display.setCursor(0,24); 
        display.print("Rain sensor status");
         sdcard_file.print(" Rain sensor status");
      }
 
 /************************************
 *     Wiper Status request (75)     *
 ************************************/
      if (message.b(0) == 0x75){
        display.setCursor(0,24); 
        display.print("Wiper Request");
        sdcard_file.print(" Wiper Request");
       //ibusTrx.write(Wipe);
      }

 /**********************************************
 *    Diagnostic command acknowledged (A0)     *
 **********************************************/
      if (message.b(0) == 0xA0){
        display.setCursor(0,24); 
        display.print("Diag Com Ackowledged");
        sdcard_file.print(" Diag Com Ackowledged");
      }

    sdcard_file.println(" ");
    display.display();
    sdcard_file.close(); // close the file
    digitalWrite(13, LOW);
  }

}
