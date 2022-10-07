#include <dht_nonblocking.h>
#include<DS3231.h>
#include <LiquidCrystal.h>
#include<Wire.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11

DHT_nonblocking dht_sensor(23, DHT_SENSOR_TYPE);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DS3231 clock;
RTCDateTime dt;

float temp;
float humid;

bool flip;

void setup() {
  clock.begin();
  lcd.begin(16, 2);
  pinMode(2, INPUT);
  delay(200);
  if(digitalRead(2) == HIGH){
    clock.setDateTime(__DATE__, __TIME__); 
  }
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop() {
  measure_environment(&temp, &humid);
  dt = clock.getDateTime();

  flip = !flip;

  if(flip){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humid: ");
    lcd.print(humid);
    lcd.print("%");
  }else{
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print(dt.year); lcd.print("-");
    lcd.print(dt.month); lcd.print("-");
    lcd.print(dt.day); lcd.setCursor(0,1);
    lcd.print(dt.hour); lcd.print(":");
    if(dt.minute < 10){
      lcd.print("0");
    }
    
    lcd.print(dt.minute);
  }
  delay(3000);
}
