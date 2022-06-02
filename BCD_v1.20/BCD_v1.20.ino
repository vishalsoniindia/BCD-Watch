
// ST_CP pin 12
const int latchPin = 3;
// SH_CP pin 11
const int clockPin = 1;
// DS pin 14
const int dataPin = 0;


#define button_pin 2
#define volt_adc_pin A2

unsigned long milli_seconds = 1000;
unsigned long current_time = 0;
unsigned long current_time_2 = 0;
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

unsigned long inncrease_time_interval = 100;
unsigned long increase_time_started = 0;
int hold_increasing_time = 0;

unsigned long low_led_on_interval = 500;
unsigned long low_led_on_started = 0;

unsigned long minutes_debouncing_interval = 100;
unsigned long minutes_debouncing_started = 0;
int is_minutes_released = 1;

int battery_low_indication = 0;
int blink_for_low = 0;
int battery_low_off_hold = 0;
int battery_low_blink_count = 0;

int button = HIGH;
int seconds = 1;
int minuts = 1;
int hours = 1;
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
int volt_adc_value = 0;
float volt = 0;
int battery_low = 0;
float volt_after_devider = 0;

unsigned long test;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(volt_adc_pin, INPUT);
//  Serial.begin(115200);
  alloff();
  previous_time = millis();
}

void loop() {
  current_time = millis();
  current_time_2 = millis();

  button_detect();
  battery();

  if ((current_time - previous_time) >= milli_seconds && set_mode == 0) { //
    previous_time = millis();
    previous_time = (previous_time - (previous_time - current_time));
    seconds ++;
//    test = current_time;
  }

  if (seconds > 60) {
    minuts ++;
    seconds = 1;
    if((minuts%2) == 0){
      seconds = seconds + 1;
    }
    if((minuts%5) == 0){
      seconds = seconds + 1;
    }
    if((minuts%20) == 0){
      seconds = seconds + 1;
    }
  }
  if (minuts > 60) {
    hours ++;
    minuts = 1;
  }
  if (hours > 12) {
    hours = 1;
//    if((minuts%6) == 0){
//      seconds = seconds - 5;
//    }
  }

//  Serial.print(hours);
//  Serial.print(':');
//  Serial.print(minuts);
//  Serial.print(':');
//  Serial.print(seconds);
//  Serial.print(":");
//  Serial.print(test);
//  Serial.println("   ");
}

void battery() {
  volt_adc_value = analogRead(volt_adc_pin);
  volt_after_devider = float(3.3 * volt_adc_value) / 1023;
  volt = float(4.2 * volt_after_devider) / 2.8; // 2.8 is calculated by voltage devider with input of 4.2 volt
  if (volt <= 3.5 && volt >= 3.4 && (battery_low == 0 || battery_low == 2)) { //3.4
    battery_low = 1;
  }

  if (volt < 3.4 && (battery_low == 1 || battery_low == 0)) { //3.4
    battery_low = 2;
  }

  if (volt > 3.5 && (battery_low == 1 || battery_low == 2)) {
    battery_low = 0;
  }

  if (button == LOW && battery_low == 2) {
    battery_low_indication = 1;
  }

  if (battery_low_indication == 1) {
    if (battery_low_blink_count < 3) {
      blink_for_battery_low();
    }
    else {
      alloff();
      battery_low_indication = 0;
      battery_low_blink_count = 0;
    }

  }

}

void button_detect() {
  button = digitalRead(button_pin);
  if (button == LOW && button_pressed == 0 && set_mode == 0 && battery_low != 2 && battery_low_indication != 1) {
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
    current_time_2 = millis();
    show_time_started = millis();
  }

  if ((current_time_2 - show_time_started > show_time_interval) && button_pressed == 2 && show_hours == 1) {
    alloff();
    BCD(minuts);
    minuts_led(1);
    show_hours = 0;
    show_minuts = 1;
    current_time_2 = millis();
    show_time_started = millis();
  }

  if ((current_time_2 - show_time_started > show_time_interval) && button_pressed == 2 && show_minuts == 1) {
    alloff();
    if (button == HIGH) {
      if (battery_low == 1 && set_mode == 0) {
        battery_low_indication = 1;
      }
      button_pressed = 0;
      out_from_setmode = 0;
      current_time_2 = millis();
    }
  }

  if (button == LOW &&  button_pressed == 2 && set_mode == 0 && out_from_setmode == 0 && battery_low != 2) {
    if (current_time_2 - long_press_started > long_press_interval ) {
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

  if (set_hours == 1 && button == LOW && button_pressed_set == 0 && hold_increasing_time == 0) {
    button_pressed_set = 1;
    current_time_2 = millis();
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
    //delay(100);
    hold_increasing_time = 1;
    entered_in_setmode = 0;
    current_time_2 = millis();
    increase_time_started = millis();
  }



  if (button == LOW && button_pressed_set == 1 && set_hours == 1 && entered_in_setmode == 0) {
    if (current_time_2 - long_press_started_for_minuts > long_press_interval_for_minuts) {
      set_hours = 0;
      set_minuts = 1;
      button_pressed_set = 0;
      entered_in_minuts_set = 1;
      hold_increasing_time = 0;
    }
  }

  if (set_minuts == 1 && button == LOW && button_pressed_set == 0 && hold_increasing_time == 0) {
    button_pressed_set = 1;
    current_time_2 = millis();
    long_press_started_for_out_setmode = millis();
  }

  if (set_minuts == 1 && button == HIGH && button_pressed_set == 1) {
    button_pressed_set = 0;
    if (entered_in_minuts_set == 0) {
      if(is_minutes_released == 1){
          minuts = minuts + 1;
          if (minuts > 60) {
              minuts = 1;
          }
      }
      else{
        is_minutes_released = 0;
        current_time_2 = millis();
        minutes_debouncing_started = millis();
      }
    }
    //delay(100);
    hold_increasing_time = 1;
    entered_in_minuts_set = 0;
    current_time_2 = millis();
    increase_time_started = millis();
  }

  if ((current_time_2 - minutes_debouncing_started  > minutes_debouncing_interval) && is_minutes_released == 0 ) {
    button_pressed_set = 1;
    is_minutes_released = 1;
  }


  if (button == LOW && button_pressed_set == 1 && set_minuts == 1 && entered_in_minuts_set == 0) {
    if (current_time_2 - long_press_started_for_out_setmode > long_press_interval_for_out_setmode) {
      button_pressed_set = 0;
      set_hours = 1;
      set_minuts = 0;
      set_mode = 0;
      out_from_setmode = 1;
      hold_increasing_time = 0;
      alloff();
      seconds = 1;
      previous_time = millis();
    }
  }

  if (battery_low == 2) {
    set_hours = 1;
    set_minuts = 0;
    button_pressed_set = 0;
    set_mode = 0;
    out_from_setmode = 1;
    hold_increasing_time = 0;
    alloff();
  }

  if ((current_time_2 - increase_time_started > inncrease_time_interval) && hold_increasing_time == 1 ) {
    hold_increasing_time = 0;
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
    current_time_2 = millis();
    blink_started = millis();
    blink_flag = 1;
  }
  if ((current_time_2 - blink_started > blink_interval) && led_pin == 0 ) {
    led_pin = 1;
    current_time_2 = millis();
    blink_started = millis();
  }
  if ((current_time_2 - blink_started > blink_interval) && led_pin == 1 ) {
    led_pin = 0;
    blink_flag = 0;
  }

  if (blink_led == 'h' && led_pin == 1) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (32 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else if (blink_led == 'm' && led_pin == 1) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (128 + decimal_to_led));
    digitalWrite(latchPin, HIGH);
  }
  else {
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

void blink_for_battery_low() {
  current_time_2 = millis();

  if (blink_for_low == 0 ) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (128 + 32));
    digitalWrite(latchPin, HIGH);
    blink_for_low = 1;
    current_time_2 = millis();
    low_led_on_started = millis();
  }
  if ((current_time_2 - low_led_on_started > low_led_on_interval) && blink_for_low == 1 && battery_low_off_hold == 0 ) {
    alloff();
    battery_low_off_hold = 1;
    current_time_2 = millis();
    low_led_on_started = millis();
  }
  if ((current_time_2 - low_led_on_started > low_led_on_interval) && blink_for_low == 1 && battery_low_off_hold == 1) {
    alloff();
    battery_low_off_hold = 0;
    blink_for_low = 0;
    battery_low_blink_count++;
  }
}
