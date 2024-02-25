#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include <dht11.h>

const int temp = 7; //ok
const int moisturePin = A0;   //ok
const int water = 13;   //ok
const int trigPin = 9;  //ok
const int echoPin = 10; //ok
const int bright = A1;   //ok 
const int led = 6;      //ok
const int ledButton = 8; //ok

int NUMPIXELS = 52;

unsigned long startMillis;
unsigned long getValueMillis =  0;
unsigned long eventMillis = 0;
unsigned long waitWater = 3600000;                  //un'ora
unsigned long eventOpenWater = 0;
unsigned long stopWater = 6000;                     //6 secondi
unsigned long ultrasonic_millis = 0;
unsigned long ultrasonic2_millis = 0;
unsigned int sonic_done = 0;
unsigned int open_water = 0;
unsigned int trigSonic = 0;
unsigned int duration = 0;
float maxDistance = 8.70;                       //aggiornato
unsigned int minMoisture = 560;                       //60% umidita' per innaffiare
unsigned long timer = 0;
unsigned long myTimer = 0;
unsigned long eventoLuci = 0;
int delayLuci = 180000;                             //3 minuti
int cont = 0;
unsigned long aux = 0;
int timerOver = 0;
int minBrightness = 70;      //da verificare
int ledOn = 0;

int tempLCD;
int umidLCD;
int lumLCD;

int tempMap, valTemp;
float distance = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    //ok
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, led, NEO_GRB + NEO_KHZ800);

int moisture = 0;
int tmp_moisture = 0;
int brightness = 0;
int brightness_map = 0;
String final_str;

dht11 DHT11;

void setup() {
  pinMode(temp, INPUT);
  pinMode(moisturePin, INPUT);
  pinMode(water, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(bright, INPUT);
  pinMode(ledButton, INPUT);
  
  digitalWrite(water, LOW);
  digitalWrite(water, HIGH);

  lcd.begin(16, 2);
  pixels.begin();

  Serial.begin(9600);
  startMillis = millis();
}

void loop() {
  if (getValueMillis + 5000 <= millis() || getValueMillis == 0){                     
    moisture = analogRead(moisturePin);
    tmp_moisture = moisture; //map(moisture, 0, 1023, 0, 100);

    int chk;
    chk = DHT11.read(temp);
    tempMap = DHT11.temperature;

    brightness = analogRead(bright);
    brightness_map = map(brightness, 0, 1023, 0, 100);  //"dipende" da resistenza usata

    tempLCD = tempMap;
    umidLCD = tmp_moisture;
    lumLCD = brightness_map;

    final_str = String(tmp_moisture) + "," + String(tempMap) + "," + String(distance) + ",";

    getValueMillis = millis();

    Serial.println(final_str);
    Serial.flush();
  }

  /* ------ WATER ------ */

  if (tmp_moisture < minMoisture && eventMillis + waitWater <= millis() && open_water == 0 && distance < maxDistance){
    digitalWrite(water, LOW);           
    open_water = 1;
    eventOpenWater = millis();
  }

  if (eventOpenWater + stopWater <= millis() && open_water == 1){
    digitalWrite(water, HIGH);
    open_water = 0;
    eventMillis = millis();
  }

  if(Serial.available()){
    char read = Serial.read();
    if (read == 'a' && open_water == 0/* && distance > maxDistance*/){
      digitalWrite(water, LOW);
      open_water = 1;
      eventOpenWater = millis();
      eventMillis = millis();
    }
  }

  /* ------ ULTRASONIC ------ */
  
  if (ultrasonic_millis + 0.002 <= millis() && trigSonic == 0){
    digitalWrite(trigPin, LOW);
    digitalWrite(trigPin, HIGH);
    ultrasonic_millis = millis();
    trigSonic = 1;
  }
  if (ultrasonic2_millis + 0.01 <= millis() && trigSonic == 1){
    digitalWrite(trigPin, LOW);
    ultrasonic2_millis = millis();
    trigSonic = 0;
    sonic_done = 1;
  }

  if (sonic_done == 1){
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    sonic_done = 0;
  }

  /* ------ LCD ------ */
  timer=millis();
  myTimer=timer-cont*aux;

  if(myTimer > 0 && myTimer<5000){
    timerOver = 0;
    lcd.setCursor(0, 0);
    lcd.print("   Serra  ITS");
    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.setCursor(12, 1);
    lcd.print(tempLCD);
    lcd.setCursor(15, 1);
    lcd.print("°");
  }

  if(myTimer>5000 && myTimer<5200)
    lcd.clear();

   if(myTimer >= 5200 && myTimer<10200){
    lcd.setCursor(0, 0);
    lcd.print("   Serra  ITS");
    lcd.setCursor(0, 1);
    lcd.print("Umidita':    ");
    lcd.setCursor(12, 1);
    lcd.print(umidLCD);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }

  if(myTimer>10200 && myTimer<10400)
    lcd.clear();
  
  if(myTimer>10400 && myTimer<15400){;
    lcd.setCursor(0, 0);
    lcd.print("   Serra  ITS");
    lcd.setCursor(0, 1);
    lcd.print("Luminosita': ");
    lcd.setCursor(12, 1);
    lcd.print(lumLCD);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }

  if(myTimer>15400 && timerOver == 0){
    cont=cont+1;
    lcd.clear();
    aux = myTimer;
    timerOver = 1;
  }

  /*LED AUTOMATICO*/
  if (brightness_map < minBrightness && ledOn == 0 && eventoLuci + delayLuci >= millis()){
    ledOn = 1;
    for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,255,10));
        pixels.show();
    }
  }
  
  if(brightness_map >= minBrightness && ledOn == 1 && eventoLuci + delayLuci >= millis()){
    ledOn = 0;
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();
    }
    pixels.show();
  }
}
