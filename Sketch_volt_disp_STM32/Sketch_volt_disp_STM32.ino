#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
// Define analog input
#define ANALOG_IN_PIN PA0
#define AC_IN_PIN PA3

// The Arduino Nano pin connected to the IN pin of relay module
#define RELAY_PIN PA11 //load output control
#define RELAY_PIN2 PA12  //charging control
//LOW & HIGH
#define LED1 PB8  //LOW BATTERY
#define LED2 PB9  //FULL CHARGE
//%%%%
#define LED3 PB12  //25%
#define LED4 PB13  //50%
#define LED5 PB14  //75%
#define LED6 PB15  //90-100%

//lowest battery working voltage && Full Charge voltage
float low_voltage = 10.5;
float low_hold = 11;

float full_charge = 14.0;
float charging_hold = 13.0; //charging hold value
//==============================================================================
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;
// Integer for ADC value
int adc_value = 0;
// Float for Reference Voltage
float ref_voltage = 3.3;
//float AC_in = 0.0;

bool hold = false;
bool hold2 = false;
 
void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(ANALOG_IN_PIN, INPUT);
  pinMode(AC_IN_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setCursor(15, 25);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.println("RC product BD.com");  //Start-UP Display
  display.display();
  delay(2500);                           //Start-up Time
  display.clearDisplay();
}
 
void loop() {
  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);

  // Determine voltage at ADC input
  adc_voltage  = ((adc_value * ref_voltage) / 4096); 
  // Calculate voltage at divider input
  in_voltage = (adc_voltage / (R2 / (R1 + R2)));


  if (digitalRead(AC_IN_PIN) == HIGH)  //Power ON
  {
    if ((hold == true) && (in_voltage <= charging_hold)){
      hold = false;
    }
    if ((in_voltage <= low_voltage)&&(in_voltage > 2)){  
      hold = false;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, HIGH);
      digitalWrite(LED1, LOW);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10); 
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(" Charging ON  ");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else if(in_voltage >= full_charge){ // Full charge
      hold = true;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, LOW);  //LOW
      digitalWrite(LED2, HIGH); //FULL
      display.setCursor(25, 10);
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println("Fully Charged!");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else if (in_voltage <= 2){
      hold = false;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, HIGH);  //LOW
      digitalWrite(LED2, LOW); //FULL 
      display.setCursor(25, 10); //ON
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println("  NO Battery! ");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else if ((hold == true) && (in_voltage > charging_hold)){ //Hold function
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, LOW);  //LOW 
      digitalWrite(LED2, HIGH); //FULL
      display.setCursor(25, 10);
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println("Fully Charged!");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else{
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, HIGH);
      digitalWrite(LED1, LOW);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10);
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(" Charging ON  ");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
  }
   //if (AC_in < 512)=========  NO POWER  =========
  else if (digitalRead(AC_IN_PIN) == LOW)
  {
   // display.clearDisplay();
    if ((hold2 == true) && (in_voltage >= low_hold)){
      hold2 = false;
    }
    if ((in_voltage <= low_voltage)&&(in_voltage > 2)){ // LOW
      hold2 = true;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, HIGH);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10); //LOW
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(" Battery Low!");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else if ((hold2 == true) && (in_voltage < low_hold)){ //Hold function
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, HIGH);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10); //LOW
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(" Battery Low!");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else if (in_voltage <= 2){
      hold2 = false;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, HIGH);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10); //ON
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println("Battery Error");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);
    }
    else{
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(LED1, LOW);  //LOW
      digitalWrite(LED2, LOW); //FULL
      display.setCursor(25, 10); //On
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println("  Output ON!  ");
      display.setCursor(35, 25);  //- x = 35, y =25
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.print(in_voltage, 1);
      display.print(" Volt");
      display.display();
      delay(300);   
    }
  }
  //%% 
  if (in_voltage > 13.20 ){
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, HIGH);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println("Batt: 100%");
    display.display();
    delay(300);
  }
  else if (in_voltage <= 13.20 && in_voltage > 13.00 ){
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, HIGH);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println(" Batt: 90%");
    display.display();
    delay(300);
  }
  else if (in_voltage > 12.80 && in_voltage <= 13.00){
  // per = "Batt: 75%";
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, LOW);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println(" Batt: 75%");
    display.display();
    delay(300); 
  }
  else if ((in_voltage > 12.40) && (in_voltage <= 12.80)){
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println(" Batt: 50%");
    display.display();
    delay(300); 
  }
  else if ((in_voltage > 11.8) && (in_voltage <= 12.40)){
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println(" Batt: 25%");
    display.display();
    delay(300); 
  }
  else if (in_voltage > 10.50 && in_voltage <= 11.80){
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    display.setCursor(30, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.println(" Batt: 10%");
    display.display();
    delay(300); 
  }
  else {    //if (in_voltage < 10.50)
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    display.setCursor(30, 40);
    display.setTextColor(WHITE, BLACK);
    display.setTextSize(1);
    display.println(" Warning! ");
    display.display();
    delay(300); 
  }
}