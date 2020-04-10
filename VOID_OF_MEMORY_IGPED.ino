/****************************/
/****************************/
/*VOID OF MEMORY*/
/*BY JULIA MAKIVIC*/
/****************************/

/*****This game consists of an IR receiver, two Tilt switches, and Arduino MEGA and a HX8357 TFT LCD screen*****/
/*****The player flips the tilt switches and aligns a device containing an infrared transmitter with the IR receiver
in order to trigger various events in the game***/


/***************************/
/*Adding core libraries*/
/****************************/
#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_HX8357.h" //Library for screen model
#include <SPI.h>
#include <SD.h>
//#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RS 8
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Declaring screen 
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RS);

#define SD_CS 4

/**Declaring array that will be used to read characters sent by Infrared to Serial*/
char args[1];

/****Declaring key variables to be used in the story***/
/**Variables mark milestones in the story*/
/**In setup, the are all declared as FALSE.  However, whenever a milestone is reached,
the corresponding variable for that milestone is declared as TRUE. Declaring the variable
as TRUE means that a milestone has been reached and the player can move on to the next part of the
story.**/
boolean start;
boolean message1;
boolean collect1;
boolean analysis1;
boolean intro;
boolean scene1;
boolean test1;
boolean tendril_1_test;
boolean tendril_2_test;
boolean eye_test;
boolean path1;
boolean path2;
boolean gamestate;
boolean spydecision;
boolean stalk1;
boolean stalk2;
boolean path1_step1;
boolean path2_step1;
boolean path_2_done;


/***********Initializing variables for speaker**************/
uint8_t speakerPin = 3;

/**********Initializing variables for tilt switches***********/
const uint8_t tilt1 = 7;
const uint8_t tilt2 = 6;

uint8_t reading_tilt_1;
uint8_t reading_tilt_2;

/**Declaring variables for colors used throughout the game**/
#define MACHINE_COLOR 0xF1EC
#define TINA_COLOR 0xCCFC

/**Declaring variables for New Game button . . .**/
const uint8_t buttonPin = 2;
volatile uint8_t buttonState =0;


 
uint8_t num;
  
void setup() {

  /**Setting baudrate at 1200 in order to receive IR signals**/
  
  Serial.begin(1200);
 
  /***Initializing Screen**/
  tft.begin(HX8357D);
  tft.fillScreen(HX8357_BLACK);

  
 
  /**Images are stored on an SD card inserted into the screen***/
  /**This code checks to see if the SD card is there/readable***/
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(SD_CS)) {
    Serial.println(F("failed!"));
  }
  Serial.println(F("OK!"));

    /*Setting up variables for speaker**/
   pinMode(speakerPin, OUTPUT);

   /*Setting up varaibles tilt siwthces*/
   pinMode(tilt1, INPUT);
   pinMode(tilt2, INPUT);

   /****Setting up story variables***/
   intro = false;
   message1 = false;
   collect1 = false;
   analysis1 = false; 
   scene1 = false;
   test1 = false;
   tendril_1_test = false;
   tendril_2_test = false;
   eye_test = false;
   path1 = false;
   path2 = false;
   spydecision = false;
   gamestate= false;
   stalk1 = false;
   stalk2 = false;
   path1_step1 = false;
   start = false;
   path2_step1 = false;
   path_2_done = false;

   /**This number will be used to keep track of messages sent over infrared**/
   num= 0;

   /***Code for New Game button***/
   /***Initializing interrupt capabilities in order to restart game whenever button is pressed**/
   //pinMode(buttonPin, INPUT_PULLUP);
   //attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, CHANGE);
   
 
}

void loop() {
 
    /***This is the introductory part of the game that shows the title
    and some brief instructions***/
    if(start == false){
        drawRandomLines(HX8357_CYAN);
        delay(500);
        tft.fillScreen(HX8357_BLACK);
        tft.setTextSize(10);
        tft.setTextColor(MACHINE_COLOR);
        tft.setCursor(0,0);
        tft.setTextWrap(true);
   
        tft.print(F("Void of Memory"));
         /****Intro tune****/
        tone(3, 300);
        delay(500);
        tone(3, 50);
        delay(500);
        tone(3, 250);
        delay(500);
        tone(3, 100);
        delay(500);
        tone(3,20);
        delay(500);
        noTone(3);
   
        delay(2000);
  
   
  
        drawRandomLines(MACHINE_COLOR);
        drawRandomLines(HX8357_CYAN);
         tone(3, 262);
         delay(500);
         tone(3, 400);
         delay(500);
         tone(3, 250);
         delay(500);
         tone(3, 294);
         delay(500);
         tone(3,330);
         delay(500);
         noTone(3);
      
      
             
         tft.fillScreen(HX8357_CYAN);
         tft.setTextSize(4);
         tft.setCursor(0,10);
         tft.print(F("Welcome to Void of Memory . . . ."));
         delay(6000);
         drawRandomLines(0xCC5E);
         tft.fillScreen(HX8357_CYAN);
         tft.setCursor(0,10);
         tft.print(F("The game is simple . . ."));
         delay(6000);
         drawRandomLines(0xCC5E);
         tft.fillScreen(HX8357_CYAN);
         tft.setCursor(0,10);
         tft.print(F("You have three different devices, EYE-1, TENDRIL-1, and EXT-1, to interact with this machine . . ."));
         delay(6000);
         drawRandomLines(0xCC5E);
         tft.fillScreen(HX8357_CYAN);
         tft.setCursor(0,10);
         tft.print(F("The machine will tell you when and how to use each device  . . ."));
         delay(6000);
         tft.fillScreen(HX8357_CYAN);


         /**The encolosing if statement only execute if start is false**/
         /**start is set to true at the end so that the loop moves onto the next section 
         and does not loop through this conditional statement again**/
         start = true;
            
            
            
            
      
      }

  

    if(intro == false && start == true){
      
      
      tft.setTextSize(4);
      tft.setCursor(0,10);
      /***One of the extensions of this machine is called Tendril-1*/
      /**It consists of a tilt switch**/
      /**Players flip it over to perform actions in the game**/
      /**This is introduces them to one of the key mechanics early on**/
      tft.print(F("Flip Tendril-1 to begin the game . . . be sure to hold it for a few seconds . . ."));
      delay(2000);
      reading_tilt_1 = digitalRead(tilt1);

 
      /**If the tilt switch is upside down . . . ***/
      
    
      if(reading_tilt_1 == 0){
      /*.......Perform these actions*/

       
        tft.fillScreen(0x0000);
        drawRandomLines(HX8357_CYAN);
        drawRandomLines(0xD4FC);
        delay(2000);
      
        /***draws an image***/
        bmpDraw("intro1.bmp", 0, 0);
        delay(5000);
       
        
        drawRandomLines(HX8357_CYAN);
        delay(2000);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("Hello again, Marlena . . . "));
        delay(7000);
        

        drawRandomLines(HX8357_CYAN);
        delay(2000);   
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("I'm sure you remember me, LONE RANGER, . . . "));
        delay(2000);
        tft.print(F("Your smart guide for this Jerome-X53 . . ."));
        delay(10000);
     

        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0,80);
        tft.print(F("Welcome to the Jerome Information Analysis and Collection Center . . ."));
      
        delay(7000);
        drawRandomLines(HX8357_CYAN);
        

       /* drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0,60);
        tft.print(F("Rest a while as I calibrate all of my tendrils and receivers. . ."));
        /**Insert image of the Server Farm right here . . . **/
        /*delay(7000);
        drawRandomLines(HX8357_CYAN);*/
        

        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
         tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0,80);
        tft.print(F("My archive indicates that Jerome was a former ghost town . . ."));
        delay(10000);
        drawRandomLines(HX8357_CYAN);
       

        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0, 80);
        tft.print(F("And that is why all of the machines in this farm are Western-themed . . ."));
        delay(1000);
        tft.print(F("It embeds this Server Farm within the community and builds employee morale. . . "));
        delay(10000);
        drawRandomLines(HX8357_CYAN);

        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print("LONE RANGER: \n");
        tft.setCursor(0, 80);
        tft.print(F("My facial analysis lenses indicate you could use a boost in employee morale . . . "));
        delay(10000);
        drawRandomLines(HX8357_CYAN);

        /*drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print("LONE RANGER: \n");
        tft.setCursor(0, 80);
        tft.print(F("Alright Cowboy, let's get back on this horse!"));
        delay(10000);
        drawRandomLines(HX8357_CYAN);*/



       /* drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0,80);
        tft.print(F("Initializing connection to Cold-hand LukeX5892G and Clint EastwoodX-1 . . ."));
        /**Insert image of the Server Farm right here . . . **/
        /*delay(10000);
        drawRandomLines(HX8357_CYAN);*/
        

        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0,80);
        tft.print(F("This isn't your first rodeo! Let's begin some routine data scraping and analysis!"));
        /**Insert image of the Server Farm right here . . . **/
        delay(10000);
        drawRandomLines(HX8357_CYAN);
          
        drawRandomLines(MACHINE_COLOR);
        tft.fillScreen(HX8357_BLACK);

        /**setting milestone variable to true so that the loop moves onto the next section
        and doesn't loop over this conditional statement***/
        intro = true;
      
        }
      
     }


   /****************************/
   /**In this section the player is asked to perform a series of tests in order to calibrate the 
   machine**/
   /****************************/
   if(intro == true && test1== false){

      

        if(tendril_1_test == false){

           
        
           tft.setCursor(0,10);
           tft.print(F("Flip Tendril-1 to initiate regional data collection. . . . Remember to hold it for a few seconds . . ."));
           delay(2000);
          
           reading_tilt_1 = digitalRead(tilt1);
           reading_tilt_2 = digitalRead(tilt2);

           /***Checking to see if the correct tilt switch is flipped**/

           if(reading_tilt_1 == 0){

             /***Setting the variable to true***/
             /***If the plater flipped the correct tendril, then they achieve a milestone in the game**/
             /**They will be able to move on to the enxt section**/
             tendril_1_test= true;

             /***The rest of this code creates the text and animations on the screen***/
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             tft.print(F("Good draw, Cowboy!"));
             delay(2000);
             tft.print(F("We've identified a subject for analysis . . . ."));
             delay(7000);
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             tft.setCursor(0,100);
             tft.print(F("Individual 0X589034_UxZ"));
             delay(7000);
             bmpDraw("clean1.bmp",0,0);
             delay(5000);

             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);

             tft.setCursor(0,10);
             tft.print(F("Darn, that is one scrambled image, partner! . . ."));

             delay(8000);
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);


             tft.setCursor(0,10);
             tft.print(F("That was a composite map of his face . . . it allows us to map his fears, desires, intentions and future movements. . ."));;
             delay(8000);

              drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);



             tft.setCursor(0,10);
             tft.print(F("It gives us even more information than his real face . . ."));;
             delay(8000);

             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);

             
             tft.setCursor(0,10);
             tft.print(F("Time to mine my surveillance spores for traces of his whereabouts . . ."));;
             delay(8000);

            

             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             
             bmpDraw("clean2.bmp",0,0);
             delay(3000);

             /****For loops are used to make a short animation involving bitmapped image***/

             for(int i=0; i<300;i+=70){
              
                bmpDraw("blob1.bmp",i,270);
                tone(3, random(200));
                
             }

             for(int i=0; i<300;i+=70){
              
                bmpDraw("blob2.bmp",i,270);
                tone(3, random(200));
                 
             }

             for(int i=0; i<300;i+=70){
              
                bmpDraw("blob3.bmp",i,270);
                tone(3, random(200));
                
             }

            noTone(3);

            /***Draw the face now***/

             tft.setTextSize(4);
             drawRandomLines(MACHINE_COLOR);
             tft.fillScreen(HX8357_BLACK);
             
            }

            /***Feedback telling the player to flip the other switch in case they flipped the wrong one
            **/

            if(reading_tilt_2 == 0){

             
            
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             tft.setCursor(0,10);
             tft.print(F("Wrong one! Come on, you did this exact thing yesterday. No need to prolong this exciting process . . . "));
             delay(5000);
             drawRandomLines(MACHINE_COLOR);
             tft.fillScreen(HX8357_BLACK);

            
             
            }

           

          }

          /***The second test****/
          /****The player is asked to flip another one of the machine's extensions***/

          if(tendril_2_test == false && tendril_1_test== true){

            
           
            tft.setCursor(0,10);
            tft.print(F("Calibration Test 2:\n Flip EXT-1 to analyse the data we just mined . . . "));
           

            /****Reading the state of each tilt switch***/
            reading_tilt_1 = digitalRead(tilt1);
            reading_tilt_2 = digitalRead(tilt2);


            /***If the player flipped the correct extension***/

           if(reading_tilt_2 == 0){

            
            
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             tft.print(F("Excellent aim Outlaw!     Let's get to analyzin' straight away!"));
             delay(7000);


             /**Setting the variable to true, indicating that the player achieved a certain milestone and can move on***/
             tendril_2_test= true;

             drawRandomLines(HX8357_CYAN);
             delay(2000);
             tft.fillScreen(HX8357_BLACK);

             /*****For loop creates a text animation****/

               for(uint8_t i=0; i<10; i++){
                tft.setCursor(random(320),random(480));
                tft.setTextSize(5);
                tft.print(F("Contacts"));
          
                drawRandomLines(0xF04B);
                
                delay(100);
                drawRandomLines(0x039F);
                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(F("Location"));
                tone(3, random(600));
                
                delay(100);
                tft.setCursor(random(320),random(480));
                tft.print(F("Intent"));
                delay(100);
                tone(3, random(600));
                tft.setCursor(random(320),random(480));
                tft.print(F("Movement pinned . . . "));
                tone(3, random(600));
                delay(100);

                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(F("Sz%eCret F_a_Ce"));
          
                drawRandomLines(0xF04B);
                tone(3, random(600));
                
                delay(500);
                drawRandomLines(0x039F);
                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(F("Becoming seen . . . "));
                tone(3, random(600));
                delay(100);

                drawRandomLines(0xF04B);

               
               
          
                }

                noTone(3);



                /***The player receives a private call while they are interacting with the machine***/
                /***The "call" is an infrared signal that has been picked up by the TSOP receiver ***/
                /***The player needs to decide whether to see what was sent via the signal***/
                tft.fillScreen(0x0000);
                 drawRandomRects(0x389D, 0xF810);
                tft.setCursor(0,10);
                tft.setTextSize(4);
                tft.setTextColor(MACHINE_COLOR);
                tft.print(F("LONE RANGER: \n"));
                tft.setCursor(0,80);
                tft.print(F("Someone is trying to send you a private message . . . ."));
                delay(7000);

                tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                tft.print(F("LONE RANGER: \n"));
                tft.setCursor(0,80);
                tft.print(F("Taking calls while at work is looked down upon by corporate, Cowboy!"));
                delay(8000);

                /*tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                tft.print(F("LONE RANGER: \n"));
                tft.setCursor(0,80);
                tft.println(F("Don't want any signals to get mixed, the results could be disastrous!!"));
                delay(10000);*/

                tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                tft.print(F("LONE RANGER: \n"));
                tft.setCursor(0,80);
                tft.println(F("Lift EYE-1 and align it with SOCKET-1 to take the message, but flip TENDRIL-1 to get back to work . . . "));
                delay(10000);

                tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                tft.print(F("LONE RANGER: \n"));
                tft.setCursor(0,80);
                tft.println(F("Just a warning about EYE-1, sometimes it acts on its own, so you better be fast!!"));
                delay(10000);

          
               
                /*****The code above informs the player on how to accept the "call" or what to do if they want to get back to work*****/
                
                drawRandomLines(0xF04B);
                tft.fillScreen(0x0000); 
            }


            /******Feedback incase the player flipped the wrong switch****/
            if(reading_tilt_1 == 0){
            
             drawRandomLines(HX8357_CYAN);
             tft.fillScreen(HX8357_BLACK);
             tft.print(F("Nope. You already did that one . . . "));
             delay(5000);

             
             drawRandomLines(HX8357_CYAN);
             delay(1000);
             tft.fillScreen(HX8357_BLACK);
             
            }
            
            
           }


           /*****************************************************/
           /*****************************************************/
           /*******In this section the player determines whether or not to take the call 
           they received in the previous section*********/
           /**************************************************/
           if(eye_test == false && tendril_2_test == true){

              
              tft.setCursor(0,20);
              tft.print(F("Flip TENDRIL-1 to go back to work OR align EYE-1 with SOCKET-1 to take the message . . ."));
              delay(4000);

              /**Reading state of tilt siwthc**/
              reading_tilt_1 = digitalRead(tilt1);
              

              /******Checking to see if the infrared signal was received******/
              if(Serial.available()){

                char text = char(Serial.read());
              
                Serial.println(text);
                 args[0] = text;

                 
                /**If a clear signal was received . . . ***/
                /****This means that the player has agreed to take the call****/
                 
                if(args[0] == 'y'){

                    drawRandomLines(HX8357_CYAN);
                    drawRandomLines(HX8357_CYAN);
                    delay(2000);
                    tft.fillScreen(HX8357_BLACK);
                    tft.setCursor(0,10);
                    tft.print(F("LONE RANGER: \n"));
                    tft.setCursor(0,80);
                    tft.println(F("Well alright then, Jesse James! The message went through!"));
                    delay(8000);
                    drawRandomLines(HX8357_CYAN);
                    drawRandomLines(HX8357_CYAN);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("LONE RANGER: \n"));
                    tft.setCursor(0,80);
                    tft.println(F("Just remember - this could have unforeseen consequences!"));
                    delay(8000);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("LONE RANGER: \n"));
                    tft.setCursor(0,80);
                    tft.println(F("I've mined about 80,000 hours of spaghetti westerns in order to develop these believable speech patterns . . ."));
                    delay(8000);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("LONE RANGER: \n"));
                    tft.setCursor(0,80);
                    tft.println(F("And the ending is the same every time . . . "));
                    delay(8000);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("LONE RANGER: \n"));
                    tft.setCursor(0,80);
                    tft.println(F("All outlaws get shot in the end . . ."));
                    delay(8000);

                    drawRandomLines(HX8357_CYAN);
                    drawRandomLines(HX8357_CYAN);
                    drawRandomRects(0xFB56, 0xFFF0);
                    tft.fillScreen(0x0000);
                    
                    tft.print(F("INTERCEPTING . . . "));
                    delay(2000);
                    tft.println(F("Message from .. .hmm that's odd . . .. þŧƃƃƛƣƔ . . ."));
                    delay(7000);
                    drawRandomLines(HX8357_CYAN);
                   


                    /*******IIf the player has agreed to take the call, then these milestone variables become TRUE******/
                    /*******These variables lead the player down one branch of the story*****/
                    eye_test = true;
                    path2 = true;
                  

                    
                  }else{

                    Serial.println(num);
                    num++; 
                     /******This section takes into account the fact that infrared signals can sometimes become muddy*******/
                       /******if the signal is unclear, but has been received for a while, then move the player down the same branch as
                       if they answered the call********/
                       /******As a game designer, I am assuming that the signal is muddy because they player is trying to answer the message
                       but something else is interfering with it****/
                       
                    if(num>6){
                      tft.fillScreen(HX8357_BLACK);
                      
                      
                       tft.setCursor(0,60);
                       tft.print(F("Despite mustering my strongest firewall the message still went through!!!"));
                       delay(8000);
                       drawRandomLines(HX8357_CYAN);
                       tft.fillScreen(0x0000);
                       tft.setCursor(0,0);
                       tft.print(F("There will be consequences for this, Billy the Kid!!!!"));
                       delay(10000);
                       drawRandomLines(HX8357_RED);
                       tft.fillScreen(0x0000);

                       tft.print(F("INTERCEPTING . . . "));
                        delay(2000);
                        tft.println(F("Message from .. .hmm that's odd . . .. þŧƃƃƛƣƔ . . ."));
                        delay(9000);
                        drawRandomLines(HX8357_CYAN);
                        tft.fillScreen(0x0000);
                       
                       /*****Sets the milestone variables to TRUE***/
                       path2 = true; 
                       eye_test = true;  
                      }
                    
                    }
                
                }else{
                  
                  //Serial.println("N");
                  
                  }

                /****This means that they player has chosen not to take the call****/
                if(reading_tilt_1 == 0){

                  /******The milestone variable is set to TRUE****/
                  /******This will lead the player down another branch of the game**********/
                  path1 = true;
                 
                  
                  
                  }       
            }

            if(tendril_1_test == true && tendril_1_test == true && eye_test == true){
              /****This indicates that the player has completed the "test" and is ready to move onto the next section****/
              test1 = true;
              }

   }
       

    

   /***************************************/
   /******PATH ONE*****/
   /******This is one of the branching paths in the story**********************/
   /*************************************************/
   /****************************************/
   if(scene1 == false && path1 == true && path1_step1 == false){


    
      tft.fillScreen(0x4DB2);
      drawRandomLines(MACHINE_COLOR);
      delay(200);
      drawRandomLines(0xFFF6);
      delay(200);
      drawRandomLines(0xDE9D);
      delay(1000);

      tft.fillScreen(0x0000);
      drawRandomLines(0x000B);
      tft.setCursor(10,10);
      tft.setTextColor(MACHINE_COLOR);
      tft.print(F("LONE RANGER: \n"));
      tft.setCursor(0, 80);
      tft.print(F("Good choice, Cowboy! It's good protocol to keep your personal life out of the ranch . . ."));
      delay(6000);
      drawRandomLines(0x07F0);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("Working in this industry, you know that privacy is a precious commodity . . . Better to keep as much of it as you can . . ."));
      delay(10000);
      drawRandomLines(0xFFE0);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("If you accepted that call who knows what sort of data about your personal life would have revealed itself . . . "));
      delay(10000);
      drawRandomLines(0x07F0);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("Of course, you're great at herding data out on the range . . .  "));
      delay(8000);
      drawRandomLines(0xFFE0);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("But I imagine even you have secrets I haven't quite collected yet, that you would prefer remain hidden . . . "));
      delay(8000);
      drawRandomLines(0xFFE0);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("Buried deep like the treasure in the Good, the Bad and the Ugly!"));
      delay(8000);
      drawRandomLines(0xFB56);
      drawRandomRects(0x87FF, 0xFC18);

      /***Moving onto the actual stalking bit***/
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.print(F("LONE RANGER: \n"));
      tft.setCursor(0,80);
      tft.println(F("Okay, Cowboy. Here is why we pay you the big bucks . . ."));
      delay(7000);
      drawRandomLines(0xFB56);
      tft.fillScreen(0x0000);
      tft.setCursor(0,10);
      tft.println(F("There is not enough networked coverage on individual 0X589034_UxZ . . ."));
      delay(8000);
      drawRandomLines(0xFB56);
      tft.fillScreen(0x0000);
      
        
      tft.setCursor(0,10);
      tft.print(F("LONE RANGER: \n"));
      tft.setCursor(0,80);
      tft.println(F("Okay, Cowboy . . . You decide which of my tracking methods to use . . ."));
      delay(8000);
      drawRandomLines(0xFFE0);
      tft.fillScreen(0x0000);

    
      
      
      /******Setting variables to true in order to be able to move onto the next section*****/
      scene1 = true;
      test1 = true;
      path1_step1 = true;
   

    
     
    }


      
   /***************************************/
   /*********This is the second branching path in the story*****************************/
   /******PATH TWO*****/
   /****************************************/
    
      if(scene1 == false && path2 == true && eye_test == true && path2_step1 == false){
        tft.fillScreen(0x4DB2);
        drawRandomLines(MACHINE_COLOR);
        delay(200);
        drawRandomLines(0xFFF6);
        delay(200);
        drawRandomLines(0xDE9D);
        delay(1000);

        tft.fillScreen(0x0000);
        drawRandomLines(0x000B);
        tft.setCursor(10,10);
        tft.setTextColor(MACHINE_COLOR);
        tft.print(F("LONE RANGER: \n"));
        tft.setCursor(0, 80);
        tft.print(F("This is a precarious situation you've found yourself in . . . ."));
        delay(8000);
        drawRandomLines(0x000B);
        tft.fillScreen(0x0000);
        tft.setCursor(0,80);
        tft.print(F("I can't decipher who the message is from . . . it's like I have no record of their existence . . . "));
        tft.setCursor(0,10);
        delay(8000);
        drawRandomLines(0xF000);
        tft.fillScreen(0x0000);
        tft.setCursor(0,10);
        tft.setTextColor(MACHINE_COLOR);
        tft.print(F("LONE RANGER: \n"));
        tft.println(F("You will have to carefully decrypt the content of the message . . . "));
        delay(10000);
        drawRandomLines(0xF000);
        tft.fillScreen(0x0000);
       
        

        /*****Setting milestone variables to true*****/
        scene1 = true;
        test1 = true;
        path2_step1 = true;

       
        
        
      } 

   /***************************************************************/
   /******This is a continuationof PATH 2, the second branching path in the story***********/
   /*************************************************/
   if(path2 == true && path2_step1 == true && path_2_done == false){
    
        tft.setCursor(0,10);
        tft.println(F("Flip EXT-1 in order to decrypt the message . . . ."));
        reading_tilt_2 = digitalRead(tilt2);

        /****Flip the correct tilt switch to see what is inside the message .  ..****/
        
        if(reading_tilt_2 == 0){

           drawRandomLines(0xF000);
           tft.setCursor(0,10);
           tft.fillScreen(0xF810);
           tft.setTextColor(0x8E3F);
           tft.setCursor(0,60);
           tft.println(F("I knew it was you . . ."));
           delay(2000);
           tft.println(F("Not by your face . . . I can't see you because I no longer have eyes . . ."));
           delay(8000);
           drawRandomLines(0x8E3F);
           tft.fillScreen(0xF810);
           tft.setCursor(0,80);
           tft.println(F("But I guess I recognise your presence . . . at a molecular level . . ."));
           tft.println(F("Do . . . you . . . recognise me . . . ?"));
           delay(9000);
           drawRandomLines(0x8E3F);
           drawRandomLines(0xFD55);
           drawRandomLines(0x4008);
           
           tft.fillScreen(0x0000);

           
           tft.setTextColor(0x8E3F);
           tft.setCursor(0,10);
           tft.print(F("LONE RANGER: \n"));
           tft.print(F("Okay pardner this migghhhhttt freak you out. . . "));
           delay(9000);
           drawRandomLines(0x4008);
          
           tft.fillScreen(0x0000);
           tft.setCursor(0,10);
           tft.print(F("LONE RANGER: \n"));
           tft.print(F("After performing a brief analysis of my archive . . . "));
           delay(9000);
           drawRandomLines(0x4008);
           tft.fillScreen(0x0000);
           
          
           tft.setCursor(0,10);
         
           tft.print(F("It seems that this message is from someone under the DECEASED database . . ."));
           delay(9000);
           drawRandomLines(0x4008);
           tft.fillScreen(0x0000);
           
           tft.setCursor(0,10);
           tft.print(F("LONE RANGER: \n"));
           tft.print(F("Hmmmm the name is . . . ¬VþIʱCTOʭʭRI˽˽A"));
           tft.println(F("Does it ring a bell?"));
           delay(10000);

           drawRandomLines(0x4008);
           drawRandomLines(0x4008);
           tft.fillScreen(0x0000);

           tft.setCursor(0,10);
           tft.println(F("You have ten seconds to reply . . . ."));
           tft.println(F("Flip Tendril-1 to send the message, otherwise do nothing . . ."));
           delay(10000);

           reading_tilt_1 = digitalRead(tilt1);

           if(reading_tilt_1 == 0){

            /*****Triangle animation******/
            /*****Taken from the graphicstest example that came with the screen library*******/
            /*****Used the animation because it fits the aesthetic of my game******/
            tft.fillScreen(0xF810);
             /***Make a triangle illustration . . . ***/

                int           n, i, cx = tft.width()  / 2 - 1,
                              cy = tft.height() / 2 - 1;
        
                tft.fillScreen(HX8357_BLACK);
                n     = min(cx, cy);
                //start = micros();
                for(i=0; i<n; i+=5) {
                tft.drawTriangle(
                  cx    , cy - i, // peak
                  cx - i, cy + i, // bottom left
                  cx + i, cy + i, // bottom right
                  tft.color565(200, 20, i));
        
                  
                }
                  delay(3000);
                  tft.fillScreen(0x0000);
                  tft.setCursor(0,10);
                  tft.print(F("LONE RANGER: \n"));
                  tft.println(F("The mysterious sender got your message, Cowboy . . . ."));
                  tft.println(F("My internal surveillance spores indicate that you in fact did know her. . ."));
                  delay(10000);
                  drawRandomRects(0xF810, 0x07FF);
                  delay(3000);

                  tft.fillScreen(0x0000);
                  tft.setCursor(0,10);
                  tft.print(F("LONE RANGER: \n"));
                  tft.println(F("Her name was Victoria . . .  She was your friend, wasn't she?"));
                  delay(8000);
                  drawRandomLines(0xF810);
                  drawRandomLines(0x07E0);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("LONE RANGER: \n"));
                  tft.println(F("She disappeared mysteriously . . . when you were together in the forest . . ."));
                  delay(8000);
                  drawRandomLines(0xF810);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("She was last seen in a green fog that came out of nowhere . . ."));
                  delay(10000);
                  drawRandomRects(0x07E0, 0x0400);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.setTextColor(0xF810);
                  tft.print(F("LONE RANGER: \n"));
                  tft.println(F("I have a secret . . . ."));
                  tft.print(F("It sounds odd . . . but I was the fog . . ."));
                  tft.println(F("There was a leak . . ."));
                  delay(10000);
                  drawRandomLines(0x8010);
                  drawRandomLines(0xF800);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("I'm sorry . . . I lost control of my surveillance spores . . ."));
                  delay(10000);
                  drawRandomLines(0x8010);
                  drawRandomLines(0xF800);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("It's kind of hard to explain pardner, she just kind of dissolved, became a part of the environment"));
                  delay(10000);
                  drawRandomLines(0x4008);
                  drawRandomLines(0x8008);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("But she's still here in some sense, she's alive in a way . . . and she received your message . . ."));
                  delay(10000);
                  drawRandomLines(0x4008);
                  drawRandomLines(0x8008);
                  tft.fillScreen(0x0000);

                  tft.setCursor(0,10);
                  tft.print(F("She wants to send you a response . . . Flip EXT-1 to see it, you have ten seconds . . ."));
                  delay(10000);

                  reading_tilt_2 = digitalRead(tilt2);

                  if(reading_tilt_2 == 0){

                    drawRandomRects(0x07E0, 0xF810);
                    drawRandomRects(0xFFE0, 0xFC18);
                    tft.fillScreen(0xF810);
                    tft.setTextColor(0x87FF);
                    tft.setCursor(0,10);
                    tft.print(F("VICTORIA: \n"));
                    tft.println(F("˥ I am glad you reme˥mber me afterՇall . . . ժժՇ"));
                    delay(10000);
                    drawRandomLines(0xFFE0);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("˥˥˥ I'm not mad˥ at you ˥for leaving me i˥n the for˥˥˥est, there is no way you could have kno˥˥˥˥wn˥˥˥ . . ."));
                    delay(12000);
                    drawRandomLines(0xFFE0);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("I am dispersed particles in the environment now . . ."));
                    delay(8000);
                    drawRandomLines(0xFFE0);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("Sometimes the air interferes with my signal, so I can't always say hi . . ."));
                    delay(10000);
                    drawRandomLines(0xFFE0);
                    tft.fillScreen(0x0000);
                    tft.setCursor(0,10);
                    tft.print(F("But when I have a strong enough signal, I will always let you know I'm there . . ."));
                    delay(10000);
                    drawRandomRects(0x07E0, 0xF810);
                    drawRandomRects(0xFFE0, 0xFC18);
                    tft.fillScreen(0x0000);
                    
                    
                    

                    ending();
                    return 0;
                    
                    }else{
                      drawRandomRects(0x07E0, 0xF810);
                      tft.fillScreen(0x0000);
                      tft.setCursor(0,10);
                      tft.print(F("LONE RANGER: \n"));
                      tft.println(F("Not fast enough, Cowboy!!! She seemed eager to talk to you, perhaps she will try to reach you again . . ."));
                      delay(10000);
                      drawRandomLines(0x07E0);
                      tft.fillScreen(0x0000);

                      ending();
                      return 0;
                      
                      
                      
                      }
                 
                  
                  
                  

                 
                  
          
          
                  /*****Set milestone variable to TRUE,so that loop does not go over this section again*****/
                  path_2_done = true;
          
          
                  /***Reached the end of the branching path****/
                  /***Call function to reset all variables and loop back to the beginning*******/
                  ending();
                  /****Exits conditional statement******/
                  return 0;
          


            
            }else{

                   tft.fillScreen(0x0000);
                   tft.setTextColor(MACHINE_COLOR);
                   tft.setCursor(0,10);
                   tft.print(F("LONE RANGER: \n"));
                   tft.println(F("It's for the best, Cowboy"));
                   delay(8000);
                   tft.setCursor(0,80);
                   drawRandomLines(0xF000);
                   tft.fillScreen(0x0000);
                   
                   tft.println(F("I know who it was . . . "));
                   delay(8000); 
                   drawRandomLines(0xF000);
                   tft.fillScreen(0x0000);
                   tft.setCursor(0,80);
                   tft.println(F("It's better to ignore messages  from the past . . ."));
                   delay(10000);
                   drawRandomLines(0xF000);
                   drawRandomLines(0xF810);
                   
                   
                  
                     /*****Set milestone variable to TRUE,so that loop does not go over this section again*****/
                  path_2_done = true;
        
                   /***Reached the end of the branching path****/
                /***Call function to reset all variables and loop back to the beginning*******/
                  ending();
                  }}
       
    }

   /******************************************************************************/
   /*****This is a continuation of PATH 1, the first branching of the story******/
   /*******************************************************************/
   /**In this section the player makes a choice by lifting a particular tilt siwtch **/
   

   if(path1 == true && path1_step1 == true && stalk1 == false && stalk2 == false){

    tft.setCursor(0,10);
    tft.println(F("Flip either TENDRIL-1 or EXT-1 to track 0X589034_UxZ"));
    tft.println(F("Use you best judgment, Cowboy!!!"));

    /***Checking status of each tilt switch**/
    reading_tilt_1 = digitalRead(tilt1);
    reading_tilt_2 = digitalRead(tilt2);

    /**********************************/
    /*******If the first tilt switch is upside down, execute this*******/
    if(reading_tilt_1 == 0){

       stalk1 = true;
       

       drawRandomLines(0xF810);
       tft.fillScreen(0x0000);
       tft.setCursor(0,10);
       tft.print(F("LONE RANGER:\n"));
       tft.setCursor(0,80);
       tft.println(F("Excellent choice . . ."));
       tft.println(F("This is a very thorough method of data extraction . . ."));
       delay(8000);
       drawRandomLines(0xF986);
       tft.fillScreen(0x0000);
       tft.setCursor(0,10);
       tft.println(F("I will program my spores to make small incisions in his skin and lodge themselves inside . . ."));
       delay(9000);
       drawRandomLines(0xF986);
       tft.fillScreen(0xF810);
       /***Triangle animation from the graphicstest example code that came with the library for the screen***/

        int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

        tft.fillScreen(HX8357_BLACK);
        n     = min(cx, cy);
        //start = micros();
        for(i=0; i<n; i+=5) {
        tft.drawTriangle(
          cx    , cy - i, // peak
          cx - i, cy + i, // bottom left
          cx + i, cy + i, // bottom right
          tft.color565(200, 20, i));

          
        }

        delay(3000);
        drawRandomRects(0x07FF, 0x07FF);
        tft.fillScreen(0x0000);
        tft.setCursor(0,10);
        tft.print(F("LONE RANGER: \n"));
        tft.println(F("Perfect . . . this little sheep won't wander too far from the herd . . . Way to lasso him up, Cowboy!!!"));
        delay(8000);
        drawRandomLines(0xF810);
        tft.fillScreen(0x0000);
        tft.setCursor(0,10);
        tft.println(F("I'd say that's a good days work, Cowboy . . . "));
        tft.println(F("Goodnight! And I will see you again, bright and early tomorrow . . . "));
        delay(8000);
        drawRandomLines(0xF810);
        drawRandomRects(0x4813, 0x0010);
        tft.fillScreen(0x0000);
        
        /*******END********/
        /*****Reset all variables******/
        ending();
      
      }

    /*******************/
    /**If the player chooses the other option and lifts the other tilt switch instead***/
    /*******************************/

    if(reading_tilt_2 == 0){

       stalk2 = true;
       drawRandomRects(0xF810, 0xFE3C);
       tft.fillScreen(0x0000);
       tft.setCursor(0,10);
       tft.print(F("LONE RANGER: \n"));
       tft.setCursor(0,80);
       tft.println(F("This is an interest choice, Cowboy . . . A more complex method for sure, but it will provide extensive infomation on the subject . . . ."));
       delay(10000);
       drawRandomRects(0xF810, 0xFE3C);
       tft.fillScreen(0x0000);
       tft.setCursor(0,10);
       tft.println(F("Embedding micro-tendrils in subject's contacts and associates . . . "));
       delay(8000);
       drawRandomLines(0xF810);
       tft.fillScreen(0xFC10);

       /*****For loop creates text animation*****/
       for(uint8_t i=0; i<10; i++){
                tft.setCursor(random(320),random(480));
                tft.setTextSize(5);
                tft.print(F("Contacts"));
          
                drawRandomLines(0xF04B);
                
                delay(100);
                drawRandomLines(0x039F);
                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(F("FACES"));
                tone(3, random(600));
                
                delay(100);
                tft.setCursor(random(320),random(480));
                tft.print(F("EYES"));
                delay(100);
                tone(3, random(600));
                tft.setCursor(random(320),random(480));
                tft.print(F("DESIRE TO"));
                tone(3, random(600));
                delay(100);

                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(i*random(600)+"ux_IT1");
          
                drawRandomLines(0xF04B);
                tone(3, random(600));
                
                delay(500);
                drawRandomLines(0x039F);
                tft.setCursor(random(320),random(480));
                tft.setTextSize(random(10));
                tft.print(F("Becoming known . . . ."));
                tone(3, random(600));
                delay(100);

                drawRandomLines(0xF04B);

               
               
          
                }
          
                noTone(3);
                tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                tft.setTextSize(4);
                tft.print(F("LONE RANGER: \n"));
                tft.println(F("Brilliant, Cowboy!!! It took a bit of computational effort on my end, but I've coralled all of the subject's known associates . . ."));
                delay(10000);
                tft.fillScreen(0x0000);
                tft.setCursor(0,10);
                
                tft.println(F("This was a good day's work . . . "));
                tft.println(F("I'll see you again bright and early tomorrow. As sure as the grass shall grow . . . "));
                delay(10000);

                /*****Reached the end, reset all variables to FALSE to make loop start from beginning******/
                ending(); 
      
      }
    }
    
      
}



/*******************/
/***Function that resets all mileston variables to FALS***/
/*Makes loop start from beginning**/
void ending(){

     tft.fillScreen(0x0000);
        tft.setCursor(0,10);
        tft.setTextColor(0xFFFF);
        tft.print(F("This branch of the game has ended . . . The game will restart in a few seconds . . . "));
        delay(12000);
        
        
       
        
        
        tft.fillScreen(HX8357_CYAN);
        tft.setTextColor(MACHINE_COLOR);
         /****Setting up story variables***/
           intro = false;
            message1 = false;
            collect1 = false;
            analysis1 = false; 
            scene1 = false;
            test1 = false;
            tendril_1_test = false;
            tendril_2_test = false;
            eye_test = false;
            spydecision = false;
            path1 = false;
            path2 = false;
            stalk1 = false;
            stalk2 = false;
            path1_step1 = false;
            start = false;
            path2_step1 = false;
            path_2_done = false;

         /*********************************/
        
  
  
  }

/*****Draws rectangles of random sizes and locations on the screen****/
/*****Used to create glitchy animations****/
void drawRandomRects(uint16_t color1, uint16_t color2){

  for(int i=0; i<8; i++){
    
     tft.fillRect(random(320),random(480),random(50), random(80),color1);
     tone(3, random(300,500));
     delay(500);
     tft.fillRect(random(320),random(480),random(50), random(80),color2);
     tone(3, random(300,500));
     delay(500);
    }

    noTone(3);
  
  
  }

/*****Function that determine what happens when the New Game button is pushed*****/

int pin_ISR(){
  buttonState = digitalRead(buttonPin);

        tft.fillScreen(HX8357_CYAN);
        tft.setTextColor(MACHINE_COLOR);

    /****resets story variables***/
           ending();

            return 0;

         /*********************************/

  
  }

/******Draws random lines in order to create glitch effect*********/
void drawRandomLines(uint16_t color){

  for(int i=0; i<50; i++){

      int xinit =0;
      int xfin = 320;
      int y = random(480);
      tft.drawLine(xinit,y,xfin,y, color);
    }
    
  }
  
/*********************************************/
/**Code for drawing BMP**/
/*********************************************/
/***Taken from example code that came with library specific to screen*****/

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite(); // Start TFT transaction
        tft.setAddrWindow(x, y, w, h);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite(); // End TFT transaction
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
            tft.startWrite(); // Start new TFT transaction
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              tft.endWrite(); // End TFT transaction
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite(); // Start new TFT transaction
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        tft.endWrite(); // End last TFT transaction
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
