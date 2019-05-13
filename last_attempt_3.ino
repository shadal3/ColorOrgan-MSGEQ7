#include <Adafruit_NeoPixel.h>

#define analogPin 0 // read from multiplexer using analog input 0
#define strobePin 2  // strobe is attached to digital pin 2
#define resetPin  3  // reset is attached to digital pin 3
#define dataPin   9  // send data to the led strip using digital output 6 //9
#define data_2Pin 6  // send data to the another led strip using digital output   //6 
#define button    5 // if the button is pressed, we get logical 1 to pin 5

#define number_of_pixels 310 //316
#define number_of_pixels_2 125 //126
#define filter 250
#define start_point 101
#define end_point 132
#define signal_limit 0
#define difference 184 //184
#define massive_length 40

int  spectrumBand[7];
int Spectrum_General; 

int i;
int j;

uint8_t  ColorOrgan_mode = 0;
uint8_t  reading;
uint8_t  previous = HIGH;
uint8_t  Waiting_mode = 0;

uint8_t number_of_frequency_red = 0;
uint8_t number_of_frequency_blue = 3;
uint8_t number_of_frequency_green = 5;
uint8_t frequency;

uint8_t number_of_led[5][massive_length];
uint8_t reverse[5];
uint8_t temporary[massive_length];
uint8_t number_of_values;
uint8_t color[3];
uint8_t white_color[massive_length];
uint8_t violet_values = 0;

uint8_t number_of_values_end[5];
boolean timer_running_mode_add[5] = {0,0,0,0,0};
boolean timer_running_mode = 0;


 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(number_of_pixels, dataPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(number_of_pixels_2, data_2Pin, NEO_GRB + NEO_KHZ800);

 
void setup()
{
 strip2.begin();
 strip2.show();
 strip.begin();
 strip.show();
 
 Serial.begin(19200);
 
 pinMode(analogPin, INPUT);
 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);
 pinMode(button, INPUT);
 
 analogReference(DEFAULT);
 randomSeed(analogRead(A0));
 
 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
}
 
void loop()
{
  reading = digitalRead(button);
 
  if (reading == HIGH && previous == LOW)
  {  
    if (ColorOrgan_mode == 0) ColorOrgan_mode = 1;
    
    else if (ColorOrgan_mode == 1)
    {
      ColorOrgan_mode = 2;
      for(i = 0; i < 5 ; i++)
      timer_running_mode_add[i] = 0;
    }
    
    else if (ColorOrgan_mode == 2)
    {
      ColorOrgan_mode = 0;
      timer_running_mode = 0;
    }   
  }

  previous = reading;
 
  if(ColorOrgan_mode == 0)
  {
    if(Boole() == 0)
    ColorOrgan();
    else
    if(Waiting() == 0)
    ColorOrgan();
    else
    if(Warming() == 0)
    ColorOrgan();
  }
 
  if(ColorOrgan_mode == 1)
  {
    if(Boole() == 0)
    ColorOrgan2();
    else
    if(Waiting() == 0)
    ColorOrgan2();
    else
    if(Warming() == 0)
    ColorOrgan2();
  }
 
  if(ColorOrgan_mode == 2)
  {
   
  if(Boole() == 0)
  ColorOrgan3();
  else
  if(Waiting() == 0)
  ColorOrgan3();
  else
  if(Warming() == 0)
  ColorOrgan3();
  }
 
 

}

int Boole()
 {
   uint8_t booling = 0;
   MSGEQ7();
   Mapping();

   if(timer_running_mode == 1)
   return 0;

   for (i = 0; i < 7 ; i++)
   if(spectrumBand[i] == 0) booling++;

   if(booling == 7)
   return 1;
   else
   return 0;
 }

int Waiting()
{
  for (int y = 0; y < 100 ; y++)
    {
      if(Boole() == 1)
      {
       ColorOrgan();
       continue;
      }
      else
      return 0;
    }
   return 1;
}

int Warming()
{
   while(1)
   {
    if(Waiting_mode == 0)
    if(White() == 0)
    return 0;
    if(Waiting_mode == 1)
    if(Sinus() == 0)
    return 0;
    if(Waiting_mode == 2)
    if(Rainbow() == 0)
    return 0;
    if(Waiting_mode == 3)
    if(Rainbow_moving() == 0)
    return 0;
   }
}

int button_control()
{
    reading = digitalRead(button);
    if(reading == HIGH && previous == LOW)
    {
      if (Waiting_mode == 0) Waiting_mode = 1;
      else if (Waiting_mode == 1) Waiting_mode  = 2;
      else if (Waiting_mode == 2) Waiting_mode = 3;
      else if (Waiting_mode == 3) Waiting_mode = 0;
      previous = reading;
      return 0;
    }
      previous = reading;
      return 1;
     
}

int White()
{
  while(1){
  uint8_t Number_of_leds = random(0,3);
  for(uint8_t i = 0; i < Number_of_leds; i++)
  {
    uint8_t Random_led = random(0,300);
    uint8_t a;
    uint8_t b;
    uint8_t c;
    do{
        a = random(0,2);
        b = random(0,2);
        c = random(0,2);
       }while((a == 1 && b == 1 && c == 1) || (a == 0 && b == 0 && c == 0) || (a == 1 && b == 0 && c == 1));
    for(uint8_t j = 0; j < 255; j++)
    { 
      strip.setPixelColor(Random_led, a*j, b*j, c*j);
      strip.setPixelColor(Random_led+1, a*j, b*j, c*j);
      Serial.print(i);
      Serial.print(" ");
      Serial.print(Number_of_leds);
      Serial.print(" ");
      Serial.print(Random_led);
      Serial.print(" ");
      Serial.print(a*j);
      Serial.print(" ");
      Serial.print(b*j);
      Serial.print(" ");
      Serial.println(c*j);
      strip.show();
      if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
      
    }
    
  }
  
   for(uint16_t z = 0; z < 264; z++)
   {
    strip.setPixelColor(z, 240, 0, 0);
    strip.setPixelColor(z-1, 0, 0, 0);
    delay(10);
    Serial.print(z);
    Serial.print(" ");
    if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
    strip.show();
    }
  }
  while(1){
  setAll2(0,0,0);
  //strip.setPixelColor(10, 200, 0, 0);
  //strip.setPixelColor(285, 0, 200, 0);
  strip2.show();
  strip.show();
  if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
  }
}

int Sinus()
{
  while(1)
  {
    int t = 0;
    int Position=0;
    int red = 100;
    int blue = 45;
    int green = 130;
    red = (random(5,70)*100) % 255;
    green = (random(5,70)*100) % 255 ;
    blue = (random(5,70)*100) % 255;

    for(int z = 0; z < number_of_pixels*2; z++)
    {
      if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
      Position = Position + 1;
      for(int i=0; i < number_of_pixels; i++)
      {
        strip.setPixelColor(i,((sin(i+Position) * 127 + 127)/255)*red,
                   ((sin(i+Position) * 127 + 127)/255)*green,
                   ((sin(i+Position) * 127 + 127)/255)*blue);
      }
      
      strip.show();
      delay(50);
    }
  }
}

int Rainbow() {
  while(1)
  {
    for (int color=0; color<255; color++) {
      if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
      for (int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(color));
       }
    strip.show();
    delay(10);
  }
  }
}

int Rainbow_moving()
{
  while(1)
  {
    uint16_t i, j;
    
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    if(Boole() == 0)
      return 0;
      if(button_control() == 0)
      return 1;
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels()) + j));
    }
    strip.show();
    delay(10);
  }
  }
}
 
 
 
void ColorOrgan3()
{
 setAll(0,0,0);
 setAll2(0,0,0);  
 Running_mode_single(0,0,35,71,0);
 Running_mode_single(1,1,101,132,0);
 Running_mode_single(2,3,167,203,0);
 Running_mode_single(3,4,34,64,199);
 Running_mode_single(4,5,35,58,250);

 for(i = 31, j = 0; i >= 0, j < 33; j++, i--)
      {
        
        if(i < 9 && i >= 0){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 204, 255, 0);
        }
        if(i < 17 && i > 8){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 255, 0, 0);
        }
        if(i < 25 && i > 16){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 255, 0);
        }
        if(i < 32 && i > 24){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 255);
        }
     }
      
 strip.show(); 
 strip2.show();    
}



void Running_mode_single(int part, int sagedus, int alg_point, int lopp_point, int difference_additional)
{
  if(timer_running_mode_add[part] == 0)
  {

  for(j = 0; j < massive_length ; j++)
  {number_of_led[part][j] = alg_point ;}

   number_of_values = 0;

   number_of_values_end[part] = 0;

   if(spectrumBand[sagedus] > signal_limit)
   number_of_values_end[part] = 0;

  }

  

  if(spectrumBand[sagedus] > signal_limit || timer_running_mode_add[part] == 1)
  {     
    timer_running_mode_add[part] = 1;
    
   if(spectrumBand[sagedus] > signal_limit)
   number_of_values_end[part]++; 
  
   
    
    while (1) 
    {
      

      if(spectrumBand[sagedus] > signal_limit || number_of_values_end[part] > 0 )
      {
         if(part == 0)
         {
           color[0] = 255;
           color[1] = 255;
           color[2] = 0;
          }

         if(part == 1)
         {
           color[0] = 255;
           color[1] = 0;
           color[2] = 0;   
          }

          if(part == 2)
          { 
            color[0] = 255*0.4;
            color[1] = 255;
            color[2] = 0;
           }

           if (part == 3)
           {
              color[0] = 255;
              color[1] = 255*0.2;
              color[2] = 0;
           }

            if (part == 4)
            {
              color[0] = 0;
              color[1] = 255;
              color[2] = 255;
            }  
              
              
              
            strip.setPixelColor(number_of_led[part][number_of_values] + difference_additional, color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[part][number_of_values]-1 + difference_additional, 0 ,0, 0);
            

            reverse[part] = number_of_led[part][number_of_values] - alg_point;

            strip.setPixelColor(number_of_led[part][number_of_values]-(2*reverse[part]) + difference_additional, color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[part][number_of_values]-(2*reverse[part]) + 1 + difference_additional, 0, 0, 0);

           

            if(number_of_values_end[part] - 1 - number_of_values > 0)
            { 
              number_of_values++;    
              continue;
            }

            break;
        }    
      }           

        
     number_of_values = 0;
 

        if(number_of_led[part][number_of_values] == lopp_point)
        {
          strip.setPixelColor(lopp_point + difference_additional,0,0,0);
          strip.setPixelColor(alg_point - (lopp_point - alg_point) + difference_additional, 0, 0, 0);
              
          number_of_led[part][number_of_values] = alg_point; // unknown если не проходит по след условию тобишь последний элемент
          
          for (i = 1,number_of_values = 0; i < number_of_values_end[part]; i++, number_of_values++) 
          { 
             temporary[number_of_values] = number_of_led[part][i];
          } 

          for (number_of_values = 0; number_of_values < number_of_values_end[part] - 1; number_of_values++) 
          { 
             number_of_led[part][number_of_values] = temporary[number_of_values];
             number_of_led[part][number_of_values+1] = alg_point; 
          }
            
           number_of_values_end[part]--;
            
        }

           
          for(number_of_values = 0; number_of_values < number_of_values_end[part]; number_of_values++)
          number_of_led[part][number_of_values]++; 
          number_of_values = 0;

        

        
    
         if(number_of_values_end[part] == 0)
         {
           for(j = 0; j < massive_length ; j++)
           {
             number_of_led[part][j] = alg_point;
           }
             number_of_values = 0;
         }

       if(number_of_values_end[part] == 0)
       timer_running_mode_add[part] = 0; 

       number_of_values = 0;
       
   } 
}
 
 

 

void ColorOrgan2()
{
  setAll(0,0,0);
  setAll2(0,0,0);
  setAll(spectrumBand[0], spectrumBand[3] , spectrumBand[6]);
  
  for(i = 31, j = 0; i >= 0, j < 33; j++, i--)
      {
        
        if(i < 9 && i >= 0){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 204, 255, 0);
        }
        if(i < 17 && i > 8){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 255, 0, 0);
        }
        if(i < 25 && i > 16){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 255, 0);
        }
        if(i < 32 && i > 24){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 255);
        }
     }
      
    strip.show();
    strip2.show();
}


void ColorOrgan()
 {
      //setAll(0,0,0);
      setAll2(0,0,0);
      Running_mode();
      for(i = 0; i < 4 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[5], spectrumBand[5]);
      for(i = 4; i < 8 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[4], 0);
      for(i = 8; i < 12 ; i++)
      strip.setPixelColor(i, 0,0, spectrumBand[3]);
      for(i = 12; i < 16 ; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 16; i < 20 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 20; i < 24 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i =24 ; i < 28 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 28; i < 32 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 32; i < 36 ; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 36; i < 40; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 40; i < 44 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 44; i < 48 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 48; i < 52 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 52; i < 56; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 56; i < 60 ; i++)
      strip.setPixelColor(i, 0,0, spectrumBand[3]);
      for(i = 60; i < 64 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[4], 0);
      for(i = 64; i < 68 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[5], spectrumBand[5]);
      for(i = 68; i < 71 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[6]*0.2,spectrumBand[6]);
      
      //for(i = 71; i < 132 ; i++)
      //strip.setPixelColor(i, 150, 150, 150);
      for(i = 132; i < 136 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[6]*0.2,spectrumBand[6]);
      for(i = 136; i < 140; i++)
      strip.setPixelColor(i, 0, spectrumBand[5], spectrumBand[5]);
      for(i = 140; i < 144 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[4], 0);
      for(i = 144; i < 148 ; i++)
      strip.setPixelColor(i, 0,0, spectrumBand[3]);
      for(i = 148; i < 152 ; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 152; i < 156 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 156; i < 160 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 160; i < 164 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 164; i < 168 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 168; i < 172 ; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 172; i < 176 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 176; i < 180 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 180; i < 184 ; i++)
      strip.setPixelColor(i, spectrumBand[0],0, 0);
      for(i = 184; i < 188 ; i++)
      strip.setPixelColor(i,spectrumBand[1], 0, spectrumBand[1]/5.0);
      for(i = 188; i < 192 ; i++)
      strip.setPixelColor(i, spectrumBand[2],spectrumBand[2]*0.5, 0);
      for(i = 192 ; i < 196 ; i++)
      strip.setPixelColor(i, 0,0, spectrumBand[3]);
      for(i = 196; i < 200 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[4], 0);
      for(i = 200; i < 203 ; i++)
      strip.setPixelColor(i, 0, spectrumBand[5], spectrumBand[5]);
      
      for(i = 203; i < 263; i++)
      strip.setPixelColor(i, spectrumBand[0],0,0);
      
      
      
      /*for(i = 0; i < 105 ; i++)
      strip2.setPixelColor(i, 100, 0 ,0);*/

       //160cm = 1A
       //1m = 0.625A
       //7,5W per meter*/
       /*
      for(i = 31, j = 0; i >= 0, j < 33; j++, i--)
      {
        
        if(i < 7 && i >= 0){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 200, 200);
        }
        if(i < 12 && i > 6){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 200, 0, 200);
        }
        if(i < 18 && i > 11){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 200, 0, 0);
        }
        if(i < 25 && i > 17){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 200, 0);
        }
        if(i < 30 && i > 24){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 200);
        }
     }

      for(i = 60, j = 0; i >= 30, j < 33; j++, i--)
      {
        
        if(i < 37 && i > 29){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 200, 200);
        }
        if(i < 42 && i >= 36){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 200, 0, 200);
        }
        if(i < 48 && i > 41){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 200, 0, 0);
        }
        if(i < 54 && i > 47){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 200, 0);
        }
        if(i < 60 && i > 53){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 200);
        }
     }
     

     for(i = 91, j = 0; i >= 60, j < 33; j++, i--)
      {
        
        if(i < 69 && i >= 60){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 204, 255, 0);
        }
        if(i < 77 && i > 68){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 255, 0, 0);
        }
        if(i < 85 && i > 76){
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 255, 0);
        }
        if(i < 92 && i > 84){
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 255);
        }
     }

     for(i = 92, j = 0; i <= 121, j < 33; j++, i++)
      {
        
        if(i < 99 && i >= 92){ //101 92
        if(Spectrum_General >= (1 + 40*j) )
        strip2.setPixelColor(i, 0, 0, 255);
        }
        if(i < 107 && i > 98){ // 109 100
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 0, 255, 0);
        }
        if(i < 115 && i > 106){ // 117 108
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 255, 0, 0);
        }
        if(i < 123 && i > 114){ //124 116
        if(Spectrum_General >= (0 + 40*j) )
        strip2.setPixelColor(i, 204, 255, 0);
        }
     }
     */
     
     

     

     
    strip.show();
    strip2.show();
 }

void Running_mode()
{
  int helper_of_frequnecy;
  
  if(timer_running_mode == 0)
  {
   for (i = 0; i < 3 ; i++)
   {
    for(j = 0; j < massive_length ; j++)
    {number_of_led[i][j] = start_point;}
   }
   
   number_of_values = 0;

   for(i = 0; i < 3 ; i++)
   number_of_values_end[i] = 0;

  }

  if((spectrumBand[0]  || spectrumBand[1]  || spectrumBand[2]  || spectrumBand[3]  || spectrumBand[4]  || spectrumBand[5] || spectrumBand[6]) || timer_running_mode == 1)
  {

   if(spectrumBand[number_of_frequency_red] || spectrumBand[number_of_frequency_red + 1] )
   number_of_values_end[0]++;

   if(spectrumBand[number_of_frequency_blue] || spectrumBand[number_of_frequency_blue + 1] || spectrumBand[number_of_frequency_blue - 1])
   number_of_values_end[1]++;

   if(spectrumBand[number_of_frequency_green] || spectrumBand[number_of_frequency_green + 1])
   number_of_values_end[2]++; 
       
   timer_running_mode = 1;
   
    i = 0;
    violet_values = 0;
    
    while (i < 3) 
    {
      if (i == 0)
      {
        frequency = number_of_frequency_red;
        helper_of_frequnecy = 0;
      }
      if (i == 1)
      {
        frequency = number_of_frequency_blue;
        helper_of_frequnecy = 1;
      }
      if (i == 2)
      {
        frequency = number_of_frequency_green;
        helper_of_frequnecy = 0;
      }


      if(spectrumBand[frequency] || spectrumBand[frequency + 1] || spectrumBand[frequency - helper_of_frequnecy] || number_of_values_end[i] > 0 )
      {
         if(i == 0)
         {
           color[0] = 255;
           color[1] = 0;
           color[2] = 0;
          }

         if(i == 1)
         {
           color[0] = 0;
           color[1] = 255;
           color[2] = 0;   
          }

          if(i == 2)
          { 
            color[0] = 0;
            color[1] = 0;
            color[2] = 255;
           }

           if (i == 1)
           {
              for(j = 0 ; j < number_of_values_end[0] ;  j++)
              {
                if(number_of_led[i][number_of_values] == number_of_led[i-1][j])
                {
                  color[0] = 255;
                  color[1] = 255;
                  color[2] = 0;
                  white_color[violet_values] = number_of_led[i][number_of_values];
                  violet_values++;  
                  break;
                  }
                }
            }

            if (i == 2)
            {
               for(j = 0 ; j < number_of_values_end[i-1] ;  j++)
               {
                 if(number_of_led[i][number_of_values] == number_of_led[i-1][j])
                 {
                   color[0] = 0;
                   color[1] = 255;
                   color[2] = 255;
                   break;
                 }
               }

                for(j = 0 ; j < number_of_values_end[i-2] ;  j++)
                {
                  if(number_of_led[i][number_of_values] == number_of_led[i-2][j])
                  {
                    color[0] = 255;
                    color[1] = 0;
                    color[2] = 255;
                    break;
                  }
                }

                for(j = 0; j < violet_values;  j++)
                {        
                  if(number_of_led[i][number_of_values] == white_color[j])
                  {                  
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;  
                    break;
                   }
                 } 
              }  
              
              
              
            strip.setPixelColor(number_of_led[i][number_of_values], color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[i][number_of_values]-1, 0 ,0, 0);
            

            reverse[i] = number_of_led[i][number_of_values] - start_point;

            strip.setPixelColor(number_of_led[i][number_of_values]-(2*reverse[i]), color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[i][number_of_values]-(2*reverse[i]) + 1, 0, 0, 0);

            strip.setPixelColor(number_of_led[i][number_of_values] + difference, color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[i][number_of_values]-1 + difference, 0 ,0, 0);

            strip.setPixelColor(number_of_led[i][number_of_values]-(2*reverse[i]) + difference, color[0], color[1], color[2]); 
            strip.setPixelColor(number_of_led[i][number_of_values]-(2*reverse[i]) + 1 + difference , 0, 0, 0);

            if(number_of_values_end[i] - 1 - number_of_values > 0)
            { 
              number_of_values++;   
              continue;
            }  
        }    
       i++;
       number_of_values = 0;
      }           

         
     number_of_values = 0;
          
     for(j = 0; j < 3; j++) // when the value of number_of_led arrives to end point, size of the massive will be decreased in 1 
     {
        if(number_of_led[j][number_of_values] == end_point)
        {
          strip.setPixelColor(end_point,0,0,0);
          strip.setPixelColor(start_point - (end_point - start_point), 0, 0, 0);

          strip.setPixelColor(end_point + 184,0,0,0);
          strip.setPixelColor(start_point - (end_point - start_point) + 184, 0, 0, 0);
              
          number_of_led[j][number_of_values] = start_point; // if the variable "number_of_values_end" is 1, the next condition doesn't work. in this case the first element of massive has to get start point

          for (i = 1,number_of_values = 0; i < number_of_values_end[j]; i++, number_of_values++) 
          { 
             temporary[number_of_values] = number_of_led[j][i];
          } 

          for (number_of_values = 0; number_of_values < number_of_values_end[j] - 1; number_of_values++) 
          { 
             number_of_led[j][number_of_values] = temporary[number_of_values];
             number_of_led[j][number_of_values+1] = start_point; 
          }                  
           number_of_values = 0;
           number_of_values_end[j]--;
            
         }
      }

      for(i = 0; i < 3 ; i++) 
      {   
       
          for(number_of_values = 0; number_of_values < number_of_values_end[i]; number_of_values++)
          number_of_led[i][number_of_values]++; 
          number_of_values = 0;
          continue;
      }
    
      for(i = 0; i < 3 ; i++)
      {
         if(number_of_values_end[i] == 0)
         {
           for(j = 0; j < massive_length ; j++)
           {
             number_of_led[i][j] = start_point;
             number_of_values = 0;
           }
         }
      }

       if(number_of_values_end[0] == 0 && number_of_values_end[1] == 0 && number_of_values_end[2] == 0)
       timer_running_mode = 0; 
   } 
}


void MSGEQ7()
 {
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(1); // to allow Reset Pulse width- requires at least 100nS
    digitalWrite(resetPin, LOW);
    delayMicroseconds(100); // to allow Reset to Strboe - requires at least 72uS
 
    for (i = 0; i < 7; i++)
    {
      digitalWrite(strobePin, LOW); // to change the frequency
      delayMicroseconds(50); //to allow output to settle - requires at least 36uS 
      spectrumBand[i] = analogRead(analogPin);
         
      digitalWrite(strobePin, HIGH);
      delayMicroseconds(50); // to allow Strob to Strob delay - requires at least 36uS
    }
    Serial.println();
 }
 
void Mapping()
 {
    int temp;
    for(i = 0; i < 7; i++)
    {
      spectrumBand[i] = constrain(spectrumBand[i], filter , 1024); // every value which is less than filter value will get filter's value
      spectrumBand[i] = map(spectrumBand[i], filter,1024, 0,255); // the value from one range become the value of another one
      temp = spectrumBand[i] * 1.15;
      if(temp > 255)
      spectrumBand[i] = 255;
      else
      spectrumBand[i] = temp;
      Serial.print(" ");
      Serial.print(spectrumBand[i]);
      
    }

    Spectrum_General = (spectrumBand[0] + spectrumBand[1] + spectrumBand[2] + spectrumBand[3] + spectrumBand[4] + spectrumBand[5] + spectrumBand[6]) * 3;
 }


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) { 
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < number_of_pixels; i++ ) {
    strip.setPixelColor(i, red, green, blue);
  }
}

void setAll2(byte red, byte green, byte blue) {
  for(int i = 0; i < number_of_pixels_2; i++ ) {
    strip2.setPixelColor(i, red, green, blue);
  }
}


