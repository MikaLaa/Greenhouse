#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino_FreeRTOS.h>

#define SENS_1 A0
#define SENS_2 A1
#define motorAPin_A 8 //Arduino digital 8 is connected to HG7881's A-1A terminal
#define motorAPin_B 9 //Arduino digital 9 is connected to HG7881's A-1B terminal
#define motorBPin_A 10 //Arduino digital 10 is connected to HG7881's B-1A terminal
#define motorBPin_B 11 //Arduino digital 11 is connected to HG7881's B-1B terminal

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 
unsigned int result_1;
unsigned int result_2;

void setup() 
{
 
   xTaskCreate(
    TaskUpdateHumidity
    ,  "TaskUpdateHumidity"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
    RunMotors
    ,  "RunMotors"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
    Task_LCD
    ,  "Task_LCD"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

}


void loop() {
 ;
}

void Task_LCD( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  // initialize the lcd 
  lcd.init();               
  lcd.backlight();
  lcd.setCursor(1,0);

  for (;;) // A Task shall never return or exit.
  {
    lcd.setCursor(1,0);
    lcd.print("SENSOR_1");
    lcd.setCursor(10,0);
    lcd.print(result_1);

    lcd.setCursor(1,1);
    lcd.print("SENSOR_2");
    lcd.setCursor(10,1);
    lcd.print(result_2);
  }

    vTaskDelay(4000);  // one tick delay (15ms) in between reads for stability 30s
}
void TaskUpdateHumidity( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  for (;;) // A Task shall never return or exit.
  {
    result_1 = analogRead(SENS_1);
    result_2 = analogRead(SENS_2);
  }
    vTaskDelay(2000);  // one tick delay (15ms) in between reads for stability 30s
}

void RunMotors( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  pinMode(motorAPin_A, OUTPUT); //direction
  pinMode(motorAPin_B, OUTPUT); //speed
  pinMode(motorBPin_A, OUTPUT); //direction
  pinMode(motorBPin_B, OUTPUT); //speed

  for (;;) // A Task shall never return or exit.
  {
    if(result_1 > 300 || result_2 > 300)
    {
      analogWrite(motorAPin_B, 127);// start pumb
      analogWrite(motorBPin_B, 127);// start pumb
      unsigned long startMillis = millis();
      while(millis() - startMillis < 3000 )
      {
        // do nothing this needs to be re thinked
      }
      analogWrite(motorAPin_B, 0);// stop pumb
      analogWrite(motorBPin_B, 0);// stop pumb
    }
  }
  vTaskDelay(4000);  // one tick delay (15ms) in between reads for stability 60s
}
