/*H************************************************************
*  FILENAME:      buzzer_driver.hpp
*  DESCRIPTION:   Micro Music Macro Language (μMML) Player for
*                 AVR microcontrollers.
*
*  NOTES:         To be compiled with AVR C.
*
*                 A four-channel MML inspired 1-bit music
*                 player. Three channels of harmonic pulse
*                 wave and a percussive sampler or noise
*                 generator.
*
*                 Eight different duty cycles possible:
*                 50%, 25%, 12.5%, 6.25%, 3.125%, 1.5625%,
*                 0.78125% and 0.390625%. The 'thinner' pulse
*                 widths are perceived as a change in waveform
*                 power (volume) as opposed to a change in
*                 timbre.
*
*                 Tested on:
*                   * Attiny13/45/85 @ 8MHz
*                   * Atmega168/328  @ 8MHz
*
*  AUTHOR:        Blake 'PROTODOME' Troise Killar
************************************************************H*/

#ifndef BUZZER_DRIVER_HPP
#define BUZZER_DRIVER_HPP

// stuff you can mess with
#define OUTPUT         0    // the PORTB hardware output pin

// stuff you shouldn't really mess with
#define CHANNELS       4    // the number of channels
#define SAMPLE_SPEED   5    // the sampler playback rate
#define SAMPLE_LENGTH  127  // the length of the sample array
#define MAXLOOPS       5    // the maximum number of nested loops

typedef struct BuzzerDriver {
  unsigned char out[CHANNELS];
  unsigned char octave[CHANNELS];
  unsigned char length[CHANNELS];
  unsigned char volume[CHANNELS];
  unsigned char loops_active[CHANNELS];
  unsigned char current_length[CHANNELS];

  unsigned int data_pointer[CHANNELS];
  unsigned int waveform[CHANNELS];
  unsigned int pitch_counter[CHANNELS];
  unsigned int frequency[CHANNELS];
  unsigned int loop_duration[MAXLOOPS][CHANNELS];
  unsigned int loop_point[MAXLOOPS][CHANNELS];
  unsigned int pointer_location[CHANNELS];

  // sampler variables
	unsigned char current_byte;
  unsigned char current_bit;
  unsigned char sample_counter;
  unsigned char current_sample;

	// temporary data storage variables
	unsigned char buffer1;
  unsigned char buffer2;
  unsigned char buffer3;
	unsigned int buffer4;

	// main timer variables
	unsigned int tick_counter;
  unsigned int tick_speed; //tempo

  unsigned int output_pin;

  unsigned char* music_data;
} BuzzerDriver;

void init_buzzer_driver(BuzzerDriver* driver);

void play_buzzer_driver(BuzzerDriver* driver, unsigned char* music_data);

void update_buzzer_driver(BuzzerDriver* driver);

void stop_buzzer_driver(BuzzerDriver* driver);

#endif
