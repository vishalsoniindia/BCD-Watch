
// ST_CP pin 12
const int latchPin = 3;
// SH_CP pin 11
const int clockPin = 1;
// DS pin 14
const int dataPin = 0;


#define button_pin 2

unsigned long seconds = 60000;
unsigned long current_time = 0;
unsigned long previous_time = 0;

unsigned long show_time_interval = 1000;
unsigned long show_time_started = 0;

unsigned long long_press_interval = 3000;
unsigned long long_press_started = 0;

unsigned long long_press_interval_for_minuts = 3000;
unsigned long long_press_started_for_minuts = 0;

unsigned long long_press_interval_for_out_setmode = 3000;
unsigned long long_press_started_for_out_setmode = 0;

unsigned long blink_interval = 300;
unsigned long blink_started = 0;

int button = HIGH;
int minuts = 22;
int hours = 10;
int show_time = 0;
int show_hours = 0;
int show_minuts = 0;
int decimal_to_led = 0;
int set_hours = 1;
int set_minuts = 0;
int button_pressed = 0;
int button_pressed_set = 0;
int entered_in_setmode = 0;
int entered_in_minuts_set = 0;
int out_from_setmode = 0;
int set_mode = 0;
int led_pin = 0;
int blink_flag = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  //  Serial.begin(9600);
  alloff();
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


  //  Serial.print(hours);
  //  Serial.print(":");
  //  Serial.println(minuts);

}

void button_detect() {
  button = digitalRead(button_pin);
  if (button == LOW && button_pressed == 0 && set_mode == 0) {
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
    hours_led(1);
    show_time = 0;
    show_hours = 1;
    current_time = millis();
    show_time_started = millis();
  }

  if ((current_time - show_time_started > show_time_interval) && button_pressed == 2 && show_hours == 1) {
    alloff();
    BCD(minuts);
    minuts_led(1);
    show_hours = 0;
    show_minuts = 1;
    current_time = millis();
    show_time_started = millis();
  }

  if ((current_time - show_time_started > show_time_interval) && button_pressed == 2 && show_minuts == 1) {
    alloff();
    if (button == HIGH) {
      button_pressed = 0;
      out_from_setmode = 0;
      current_time = millis();
    }
  }

  if (button == LOW &&  button_pressed == 2 && set_mode == 0 && out_from_setmode == 0) {
    if (current_time - long_press_started > long_press_interval ) {
      set_mode = 1;
      entered_in_setmode = 1;
      //      Serial.println("set_mode");
    }
  }

  if (set_mode == 1) {
    settime();
  }
  //  Serial.print(button_pressed);
}


void settime() {
  if (set_hours == 1) {
    blink('h');
    alloff();
    BCD(hours);
  }
  if (set_minuts == 1) {
    blink('m');
    alloff();
    BCD(minuts);
  }

  if (set_hours == 1 && button == LOW && button_pressed_set == 0) {
    button_pressed_set = 1;
    current_time = millis();
    long_press_started_for_minuts = millis();
  }

  if (set_hours == 1 && button == HIGH && button_pressed_set == 1) {
    button_pressed_set = 0;
    if (entered_in_setmode == 0) {
      hours = hours + 1;
      if (hours > 12) {
        hours = 1;
      }
    }
    delay(100);
    entered_in_setmode = 0;

  }

  if (button == LOW && button_pressed_set == 1 && set_hours == 1 && entered_in_setmode == 0) {
    if (current_time - long_press_started_for_minuts > long_press_interval_for_minuts) {
      set_hours = 0;
      set_minuts = 1;
      button_pressed_set = 0;
      entered_in_minuts_set = 1;
    }
  }



  if (set_minuts == 1 && button == LOW && button_pressed_set == 0) {
    button_pressed_set = 1;
    current_time = millis();
    long_press_started_for_out_setmode = millis();
  }

  if (set_minuts == 1 && button == HIGH && button_pressed_set == 1) {
    button_pressed_set = 0;
    if (entered_in_minuts_set == 0) {
      minuts = minuts + 1;
      if (minuts > 60) {
        minuts = 1;
      }
    }
    delay(100);
    entered_in_minuts_set = 0;
  }

  if (button == LOW && button_pressed_set == 1 && set_minuts == 1 && entered_in_minuts_set == 0) {
    if (current_time - long_press_started_for_out_setmode > long_press_interval_for_out_setmode) {
      set_hours = 1;
      set_minuts = 0;
      button_pressed_set = 0;
      set_mode = 0;
      out_from_setmode = 1;
    }
  }

}

void alloff() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
}

void BCD(int decimal) {
  int decimal_break = decimal;
  decimal_to_led = 0;
  alloff();
  while (decimal_break != 0) {
    if (decimal_break >= 32) {
      decimal_break = decimal_break - 32;
      decimal_to_led = decimal_to_led + 1;
    }
    else if (decimal_break >= 16) {
      decimal_break = decimal_break - 16;
      decimal_to_led = decimal_to_led + 4;
    }
    else if (decimal_break >= 8) {
      decimal_break = decimal_break - 8;
      decimal_to_led = decimal_to_led + 16;
    }
    else if (decimal_break >= 4) {
      decimal_break = decimal_break - 4;
      decimal_to_led = decimal_to_led + 64;
    }
    else if (decimal_break >= 2) {
      decimal_break = decimal_break - 2;
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000010);
      decimal_to_led = decimal_to_led + 2;
    }
    else if (decimal_break >= 1) {
      decimal_break = decimal_break - 1;
      decimal_to_led = decimal_to_led + 8;
    }

  }
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, decimal_to_led);
  digitalWrite(latchPin, HIGH);
}

void blink(char blink_led) {
  if (led_pin == 0 && blink_flag == 0) {
    current_time = millis();
    blink_started = millis();
    blink_flag = 1;
  }
  if ((current_time - blink_started > blink_interval) && led_pin == 0 ) {
    led_pin = 1;
    current_time = millis();
    blink_started = millis();
  }
  if ((current_time - blink_started > blink_interval) && led_pin == 1 ) {
    led_pin = 0;
    blink_flag = 0;
  }

  if (blink_led == 'h' && led_pin == 1) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (32 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else if(blink_led == 'm' && led_pin == 1){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (128 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (0 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
}

void hours_led(int h_state) {
  if (h_state == 1) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (32 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else {
    if (h_state == 1) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
      digitalWrite(latchPin, HIGH);
    }
  }
}

void minuts_led(int m_state) {
  if (m_state == 1) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (128 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else {
    if (m_state == 1) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
      digitalWrite(latchPin, HIGH);
    }
  }
}
