
#define one 7
#define two 11
#define four 8
#define eight 9
#define sixteen A5
#define thirtytwo 10

#define button_pin 2

unsigned long seconds = 60000;
unsigned long current_time = 0;
unsigned long previous_time = 0;

unsigned long show_time_interval = 3000;
unsigned long show_time_started = 0;

int button = HIGH;
int minuts = 22;
int hours = 10;
int show_time = 0;
int button_pressed = 0;

void setup() {
  pinMode(one, OUTPUT);
  pinMode(two, OUTPUT);
  pinMode(four, OUTPUT);
  pinMode(eight, OUTPUT);
  pinMode(sixteen, OUTPUT);
  pinMode(thirtytwo, OUTPUT);
  pinMode(button, INPUT_PULLUP);
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
    button_pressed = 1;
  }
  if (button == HIGH && button_pressed == 1 ) {
    show_time = 1;
    button_pressed = 2; 
  }
  if (button == HIGH && show_time == 1) {
    alloff();
    BCD(minuts);
    show_time = 0;
    show_time_started = millis();
  }
  if(current_time - show_time_started > show_time_interval){
    alloff();
    button_pressed = 0;
  }

}


void alloff() {
  digitalWrite(one, LOW);
  digitalWrite(two, LOW);
  digitalWrite(four, LOW);
  digitalWrite(eight, LOW);
  digitalWrite(sixteen, LOW);
  digitalWrite(thirtytwo, LOW);
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
