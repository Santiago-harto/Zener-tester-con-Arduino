
//Santiago Harto Huertas 15/9/2020
//basado en voltimetro de hispavila.com
// display LCD_16x2_I2C
/*hardware: Arduino alimentado por USB. Del pin 5V del arduino alimentamos a un elevador de tensión boost converter (XL6009) con el POT al máximo,nos da unos 40V. 
 A la salida del boost le ponemos en serie una R 100k (R3) para limitar la corriente al zener. En serie iría el zener incógnita. En paralelo con el zener se le
 coloca un divisor de tensión que nos divide la tensión por 10 (R1=1M y R2=100k). Esta tensión dividida es la que introducimos en A7 del arduino. 
 Para proteger la entrada A7 le colocamos en paralelo un zener de 5V1. 
 v0.
 v1.Le colocamos un "test button" entre A3 y GND; y una R de 1k entre A3 y 5V. 
*/
#include <Arduino.h>
// poner la libreria de "NewliquidCrystal_1.3.4" de Malpartida en Arduino Libraries. contine lo necesario
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Push button
#define TEST_BUTTON           A3                 //Test/start push button (low active)
// Terminales de conexión del LCD
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //la direccion de mi dysplay es 0x27, la he buscado con "I2C_scan.ino"

#define NUMREADINGS 20 //20 los valores para leer 
int readings[NUMREADINGS]; //Las lecturas de la entrada analógica 
int index = 0; //Indice de la lectura actual
int total = 0; // lectura Total
int media = 0; //Media
float vcc = 4.45; // valor de la alimentacion de Arduino, Vcc, conecado por USB y con caída de tensión del boost marca eso con polímetro
float r1 = 985000; // 1M valor real medido en R1 con polímetro
float r2 = 94000; // 100K valor real medido en R2 con polímetro

void setup()
{ 
    pinMode(TEST_BUTTON, INPUT_PULLUP);          //Initialize the pushbutton pin as an input
    for (int i = 0; i < NUMREADINGS; i++) 
    {readings[i] = 0; }//Pone toda la matriz a 0, matriz de lecturas del voltímetro  
    
     Wire.begin(); // Comienza la comunicación I2C
  
 }

void loop()
{
 if (!(digitalRead(TEST_BUTTON)))             //If key is pressed
 {index = 0;
 total = 0; 

 for (int index = 0; index < NUMREADINGS; index++) 
    {readings[index] = analogRead(A7); //Lee los valores de A7 y guarda en la matriz 
     total += readings[index]; //Añade la lectura a total 
    }   
 media = total / NUMREADINGS; //Calcula la media 
 
float vout = (media * vcc) / 1024.0;
float vin = vout / (r2 / (r1 + r2));

  lcd.begin(16, 2);  // Inicializa el LCD para 16x2
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ZENER");
  lcd.setCursor(0, 1);
  if (vin > 38)  {lcd.print("No component");}
           else {lcd.print(vin);
                 lcd.print(" V");
                 }
delay(3000);
lcd.noBacklight(); //para apagar la luz
lcd.noDisplay(); 
}
}
 
