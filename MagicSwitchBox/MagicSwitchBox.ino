#define LED_RED_RAW 504
#define LED_GREEN_RAW 562
#define LED_YELLOW_RAW 538

#define RED 1
#define GREEN 3
#define YELLOW 2

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  analogReference(EXTERNAL);
}

class LED {
  public:
    LED(int a_pin1, int a_pin2) {
      this->a_pin1 = a_pin1;
      this->a_pin2 = a_pin2;
      tm = millis();
      my_color = 1023.0;
    }
    ~LED() {}
    int get_color() {
      if (millis() - tm > 100) {
        tm = millis();

        if ((my_color = test(a_pin1, a_pin2)) < 700.0) {
          flip = false;
        } else if ((my_color = test(a_pin2, a_pin1)) < 700.0) {
          flip = true;
        } else {
          my_color = 1023.0;
        }
      }

      if (isColor(my_color, LED_RED_RAW)) return RED;
      else if (isColor(my_color, LED_YELLOW_RAW)) return YELLOW;
      else if (isColor(my_color, LED_GREEN_RAW)) return GREEN;
      return -1;
    }
    int get_my_color() {
      return test(a_pin1, a_pin2);
    }
    void set(int state) {
      get_color();
      if (flip) {
        turnOnOff(a_pin2, a_pin1, state);
      } else {
        turnOnOff(a_pin1, a_pin2, state);
      }
    }
    void set_if(int color, int state) {
      if (get_color() == color) {
        set(state);
      }
    }
  private:
    int a_pin1, a_pin2;
    float my_color;
    unsigned long tm;
    bool flip;

    void turnOnOff(int A_HIGH, int A_GND, int state) {
      pinMode(A_HIGH, OUTPUT);
      pinMode(A_GND, OUTPUT);

      digitalWrite(A_GND, LOW);
      digitalWrite(A_HIGH, state);
    }
    bool isColor(float color0, float color) {
      return absf(((color0 - color) / color) * 100.0) < 3.0;
    }
    float test(int A_INPUT, int A_GND) {
      pinMode(A_INPUT, INPUT_PULLUP);
      pinMode(A_GND, OUTPUT);

      digitalWrite(A_GND, LOW);
      return analogRead(A_INPUT);
    }
    float absf(float v) {
      return v < 0 ? v * (-1.0) : v;
    }
};

LED led1 = LED(A0, A1);
LED led2 = LED(A2, A3);
LED led3 = LED(A4, A5);

void loop() {

  if (digitalRead(2)) {
    led1.set_if(RED, HIGH);
    led2.set_if(RED, HIGH);
    led3.set_if(RED, HIGH);
  } else {
    led1.set_if(RED, LOW);
    led2.set_if(RED, LOW);
    led3.set_if(RED, LOW);
  }

  if (digitalRead(3)) {
    led1.set_if(YELLOW, HIGH);
    led2.set_if(YELLOW, HIGH);
    led3.set_if(YELLOW, HIGH);
  } else {
    led1.set_if(YELLOW, LOW);
    led2.set_if(YELLOW, LOW);
    led3.set_if(YELLOW, LOW);
  }

  if (digitalRead(4)) {
    led1.set_if(GREEN, HIGH);
    led2.set_if(GREEN, HIGH);
    led3.set_if(GREEN, HIGH);
  } else {
    led1.set_if(GREEN, LOW);
    led2.set_if(GREEN, LOW);
    led3.set_if(GREEN, LOW);
  }
  /*Serial.print(led1.get_my_color());Serial.print(" ");
    Serial.print(led2.get_my_color());Serial.print(" ");
    Serial.print(led3.get_my_color());Serial.println(" ");*/

}
