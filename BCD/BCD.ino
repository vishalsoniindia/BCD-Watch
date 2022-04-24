
#define one 7
#define two 11
#define four 8
#define eight 9
#define sixteen A5
#define thirtytwo 10

#define hours_led 3
#define minuts_led 4

#define button_pin 2

unsigned long seconds = 60000;
unsigned long current_time = 0;
unsigned long previous_time = 0;

unsigned long show_time_interval = 1000;
unsigned long show_time_started = 0;

unsigned long long_press_interval = 3000;
unsigned long long_press_started = 0;

unsigned long blink_interval = 300;
unsigned long blink_started = 0;

int button = HIGH;
int minuts = 22;
int hours = 10;
int show_time = 0;
int show_hours = 0;
int show_minuts = 0;
int set_hours = 1;
int set_minuts = 0;
int button_pressed = 0;
int set_mode = 0;
int led_pin = 0;
int blink_flag = 0;

void setup() {
  pinMode(one, OUTPUT);
  pinMode(two, OUTPUT);
  pinMode(four, OUTPUT);
  pinMode(eight, OUTPUT);
  pinMode(sixteen, OUTPUT);
  pinMode(thirtytwo, OUTPUT);
  pinMode(hours_led, OUTPUT);
  pinMode(minuts_led, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  Serial.begin(9600);
  previous_time = millis();
}

void loop() {
  current_time = millis();
  button_detect();
  
  if (current_time - previous_time >= seconds) {
    minuts ++;
    previous_time = millis();
  }
  if (minuts > 60) {
    hours ++;
    minuts = 1;
  }
  if (hours > 12) {
    hours = 1;
  }
  

  Serial.print(hours);
  Serial.print(":");
  Serial.println(minuts);

}

void button_detect() {
  button = digitalRead(button_pin);
  if (button == LOW && button_pressed == 0) {
    button_pressed = 2;
     show_time = 1;
     long_press_started = millis();
  }
//  if (button == HIGH && button_pressed == 1 ) {
//    show_time = 1;
//    button_pressed = 2; 
//  }
  if (show_time == 1) {
    alloff();
    BCD(hours);
    digitalWrite(hours_led,HIGH);
    show_time = 0;
    show_hours = 1;
    show_time_started = millis();
  }

  if((current_time - show_time_started > show_time_interval) && button_pressed == 2 && show_hours == 1){
    alloff();
    BCD(minuts);
    digitalWrite(minuts_led,HIGH);
    show_hours = 0;
    show_minuts = 1;
    show_time_started = millis();
  }
  
  if((current_time - show_time_started > show_time_interval) && button_pressed == 2 && show_minuts == 1){
    alloff();
    if(button == HIGH){
    button_pressed = 0;
    }
  }

  if(button == LOW &&  button_pressed == 2 && set_mode == 0){
  if(current_time - long_press_started > long_press_interval ){
     set_mode = 1;
  }
  }
//  Serial.print(button_pressed);
}


void alloff() {
  digitalWrite(one, LOW);
  digitalWrite(two, LOW);
  digitalWrite(four, LOW);
  digitalWrite(eight, LOW);
  digitalWrite(sixteen, LOW);
  digitalWrite(thirtytwo, LOW);
  digitalWrite(hours_led, LOW);
  digitalWrite(minuts_led,LOW);
}

void BCD(int decimal) {
  int decimal_break = decimal;
  alloff();
  while (decimal_break != 0) {
    if (decimal_break >= 32) {
      decimal_break = decimal_break - 32;
      digitalWrite(thirtytwo, HIGH);
    }
    else if (decimal_break >= 16) {
      decimal_break = decimal_break - 16;
      digitalWrite(sixteen, HIGH);
    }
    else if (decimal_break >= 8) {
      decimal_break = decimal_break - 8;
      digitalWrite(eight, HIGH);
    }
    else if (decimal_break >= 4) {
      decimal_break = decimal_break - 4;
      digitalWrite(four, HIGH);
    }
    else if (decimal_break >= 2) {
      decimal_break = decimal_break - 2;
      digitalWrite(two, HIGH);
    }
    else if (decimal_break >= 1) {
      decimal_break = decimal_break - 1;
      digitalWrite(one, HIGH);
    }

  }
}

int blink(){ 
  if(led_pin == 0 && blink_flag == 0){
  blink_started = millis();
  blink_flag = 1;
  }
  if((current_time - blink_started > blink_interval) && led_pin == 0 ){
    led_pin = 1;
    blink_started = millis();
  }
  if((current_time - blink_started > blink_interval) && led_pin == 1 ){
    led_pin = 0;
    blink_flag = 0;
  }

  return led_pin;
}


void settime(){
  if(set_hours == 1){
    digitalWrite(hours_led, blink());
  }
  
}
