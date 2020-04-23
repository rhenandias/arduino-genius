#include <Arduino.h>

#define buzzer_pin      6		  // Pino do buzzer
#define blink_delay		  300 	// Delay entre os leds
#define sequence_delay 	300		// Delay entre os elementos da sequencia
#define turn_delay  	  1500	// Delay entre as rodadas

#define game_size 		  20	// Tamanho da sequencia gerada

byte sequence[game_size];							              // Array para armazenar a sequencia aleatoria gerada para a partida
byte inputs[4] = {A2, A3, A4, A5};					        // Pinos para os botões
byte leds[4] = {8, 9, 10, 11};						          // Pinos dos leds
unsigned int sounds[4] = {2093, 2349, 2637, 3136};	// Frequencias das notas

void setup() {
	//Configura pinos dos leds como saida
	for(byte i = 0; i < 4; i++){
		pinMode(leds[i], OUTPUT);
	}

	//Configura pinos das chaves como entrada
	for(byte i = 0; i < 4; i++){
		pinMode(inputs[i], INPUT_PULLUP);
	}

	//Gera sequencia de números aleatórios
	randomSeed(analogRead(A0)); 
	for(byte i = 0; i < game_size; i++){
		sequence[i] = random(0, 4);
	}
}

void blink(byte index){
	digitalWrite(leds[index], HIGH);	// Liga Led
	tone(buzzer_pin, sounds[index]);	// Aciona Som
	delay(blink_delay);					      // Delay
	digitalWrite(leds[index], LOW);		// Desliga Led
	noTone(buzzer_pin);					      // Desliga Som
}

byte input(){
	while(true){
		for(byte i = 0; i < 4; i++){
			if(!digitalRead(inputs[i])) return i;
		}
	}
}

void lose(){
	while(true){
		tone(buzzer_pin, 300);
		for(byte i = 0; i < 4; i++){
			digitalWrite(leds[i], HIGH);
		}
		delay(200);
		noTone(buzzer_pin);		
		for(byte i = 0; i < 4; i++){
			digitalWrite(leds[i], LOW);
		}
		delay(200);
	};
}

void win(){
	unsigned int notes[4] = {2093, 2637, 3136, 3951};
	while(true){
		for(byte i = 0; i < 4; i++){
			tone(buzzer_pin, notes[i]);
			digitalWrite(leds[i], HIGH);
			delay(50);
			digitalWrite(leds[i], LOW);
		}
	}
}

void loop() {
	byte turn = 0;
	for(turn = 0; turn < game_size; turn++){
		//Executa sequencia atual
		for(byte item = 0; item <= turn; item++){
			blink(sequence[item]);
			delay(sequence_delay);
		}

		//Recebe sequencia de inputs
		for(byte i = 0; i <= turn; i++){
			if(input() == sequence[i])	blink(sequence[i]);
			else lose();
		}

		if(turn != game_size - 1) delay(turn_delay);
	}
	win();
}
