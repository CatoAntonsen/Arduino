//example use of LCD4Bit_mod library

#include <LCD4Bit_mod.h>

//create object to control an LCD.
//number of lines in display=1
LCD4Bit_mod lcd = LCD4Bit_mod(2);

//Key message
int  adc_key_val[5] = {30, 150, 360, 535, 760 };
int ipNumber[] = { 192, 168, 1, 177 };
bool ipChanged = true;
int xpos = 0;
long unsigned lastMillis;

int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -1;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 12; i++) {
    ipNumber[i] = ipNumber[i];

  }
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat

  lcd.init();
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)
  lcd.clear();
  lcd.printIn("Set IP:");

  printIp();
}

void loop() {

  adc_key_in = analogRead(0);    // read the value from the sensor
  digitalWrite(13, HIGH);
  key = get_key(adc_key_in);		        // convert into key press
  digitalWrite(13, LOW);

  if (key != oldkey) {				    // if keypress is detected
    delay(50);		// wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor
    key = get_key(adc_key_in);		        // convert into key press
    if (key != oldkey)
    {
      oldkey = key;
      if (key >= 0) {
        processKey(key, 1);
        printIp();
      }
    }
    lastMillis = millis();
  } else {
    delay(50);		// wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor
    key = get_key(adc_key_in);		        // convert into key press
    if (key >= 0 && key == oldkey) {
        if (lastMillis + 500 < millis()) {
          Serial.println("Holding: " + String(key));
          processKey(key, 10);
          printIp();
        }
    }
  }
}

// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;

  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {

      return k;
    }
  }

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}


void processKey(int key, int numStep) {
  switch (key)
  {
    case 0: // Right
      xpos++;
      if (xpos > 3) xpos = 3;
      break;
    case 1: // Up
      ipNumber[xpos] = ipNumber[xpos] + numStep;
      if (ipNumber[xpos] > 255) ipNumber[xpos] = 0;
      ipChanged = true;
      break;
    case 2: // Down
      ipNumber[xpos] = ipNumber[xpos] - numStep;
      if (ipNumber[xpos] < 0) ipNumber[xpos] = 255;
      ipChanged = true;
      break;
    case 3: // Left
      xpos--;
      if (xpos < 0) xpos = 0;
      break;
    case 4: // Select
      break;
  }
}

void printIp() {
  if (ipChanged) {
    lcd.cursorTo(2, 0);  //line=2, x=0
    for (int i = 0; i < 4; i++) {
      String s = String(ipNumber[i]);
      int sLength = s.length() + 1;
      char charBuff[sLength];
      s.toCharArray(charBuff, sLength);
      lcd.printIn(charBuff);
      if (i < 3) lcd.printIn(".");
    }
    ipChanged = false;
  }
}


