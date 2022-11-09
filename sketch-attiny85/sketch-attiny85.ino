#include <avr/eeprom.h>

#define INPUT_PIN A1
#define QUARTER_ANALOG_READ 204
#define INPUT_DEADZONE 20

#define BLINK_DELAY 300
#define SEQUENCE_DELAY 300
#define TURN_DELAY 1500

#define BUZZER_PIN 4

#define GAME_SIZE 20

byte sequence[GAME_SIZE];
unsigned int sounds[4] = { 1047, 1319, 1568, 1976 };

byte leds[3] = { 0, 1, 3 };
byte ledsMap[4] = { 3, 2, 1, 0 };

void setLed(int number, int state) {

  if (number == 3) {
    digitalWrite(leds[1], state);
    digitalWrite(leds[2], state);
  } else {
    digitalWrite(leds[number], state);
  }
}

void getRandomSeed() {
  uint16_t seed = eeprom_read_word(0);
  randomSeed(seed++);
  eeprom_write_word(0, seed);
}

void setup() {
  for (byte i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
  }

  while (true) {

    for (byte i = 0; i < 4; i++) {
      tone(BUZZER_PIN, sounds[i]);
      delay(500);
      noTone();
      delay(500);
    }
  }

  //Gera sequencia de números aleatórios
  getRandomSeed();

  for (byte i = 0; i < GAME_SIZE; i++) {
    sequence[i] = random(0, 4);
  }
}

void blink(byte index) {
  setLed(index, HIGH);
  tone(BUZZER_PIN, sounds[index]);
  delay(BLINK_DELAY);
  setLed(index, LOW);
  noTone(BUZZER_PIN);
}

void win() {
  unsigned int notes[4] = { 2093, 2637, 3136, 3951 };

  while (true) {
    for (byte i = 0; i < 4; i++) {
      tone(BUZZER_PIN, notes[i]);
      setLed(i, HIGH);
      delay(50);
      setLed(i, LOW);
    }
  }
}

void lose() {
  byte sequence[4] = { 0, 2, 1, 3 };

  while (true) {
    for (byte i = 0; i < 4; i++) {
      tone(BUZZER_PIN, 300);
      setLed(sequence[i], HIGH);
      delay(200);
      noTone(BUZZER_PIN);
      setLed(sequence[i], LOW);
      delay(200);
    }
  }
}

int input() {

  byte inputMap[4] = { 3, 2, 1, 0 };

  while (true) {
    int currentRead = analogRead(INPUT_PIN);

    for (int i = 1; i < 5; i++) {
      if ((currentRead > (i * QUARTER_ANALOG_READ) - INPUT_DEADZONE) && (currentRead < (i * QUARTER_ANALOG_READ) + INPUT_DEADZONE))
        return inputMap[i - 1];
    }
  }
}

int nonBlockingInput() {
  int currentRead = analogRead(INPUT_PIN);

  for (int i = 1; i < 5; i++) {
    if ((currentRead > (i * QUARTER_ANALOG_READ) - INPUT_DEADZONE) && (currentRead < (i * QUARTER_ANALOG_READ) + INPUT_DEADZONE))
      return i - 1;
  }

  return -1;
}

void loop() {
  byte turn = 0;
  for (turn = 0; turn < GAME_SIZE; turn++) {
    //Executa sequencia atual
    for (byte item = 0; item <= turn; item++) {
      blink(sequence[item]);
      delay(SEQUENCE_DELAY);
    }

    //Recebe sequencia de inputs
    for (byte i = 0; i <= turn; i++) {
      if (input() == sequence[i]) blink(sequence[i]);
      else lose();
    }

    if (turn != GAME_SIZE - 1) delay(TURN_DELAY);
  }
  win();
}