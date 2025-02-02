/*H************************************************************
*  FILENAME:      buzzer_driver.cpp
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
#include "buzzer_driver.hpp"

#include <avr/io.h>         // core avr functionality
#include <avr/pgmspace.h>

// note table (plus an initial 'wasted' entry for rests)
const unsigned int note[13] PROGMEM =
{
	// the rest command is technically note 0 and thus requires a frequency
	255,
	// one octave of notes, equal temperament in Gb
	1024,967,912,861,813,767,724,683,645,609,575,542
};

// location of individual samples in sample array
const unsigned char sample_index[6] PROGMEM =
{
	0,19,34,74,118,126
};

// raw PWM sample data
const unsigned char sample[SAMPLE_LENGTH] PROGMEM =
{
	// bwoop (0)
	0b10101010,0b10110110,0b10000111,0b11111000,
	0b10000100,0b00110111,0b11101000,0b11000001,
	0b00000111,0b00111101,0b11111000,0b11100000,
	0b10010001,0b10000111,0b00000111,0b00001111,
	0b00001111,0b00011011,0b00011110,
	// beep (19)
	0b10101010,0b00101010,0b00110011,0b00110011,
	0b00110011,0b00110011,0b00110011,0b11001101,
	0b11001100,0b11001100,0b11001100,0b10101100,
	0b10011001,0b00110001,0b00110011,
	// kick (34)
	0b10010101,0b10110010,0b00000000,0b11100011,
	0b11110000,0b00000000,0b11111111,0b00000000,
	0b11111110,0b00000000,0b00000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b00100101,
	0b00000000,0b00000000,0b00000000,0b00000000,
	0b11111111,0b11110111,0b11111111,0b11111111,
	0b11111111,0b10111111,0b00010010,0b00000000,
	0b10000000,0b00000000,0b00000000,0b00000000,
	0b00000000,0b11101110,0b11111111,0b11111111,
	0b11111111,0b11110111,0b11111111,0b11111110,
	// snare (74)
	0b10011010,0b10011010,0b10101010,0b10010110,
	0b01110100,0b10010101,0b10001010,0b11011110,
	0b01110100,0b10100000,0b11110111,0b00100101,
	0b01110100,0b01101000,0b11111111,0b01011011,
	0b01000001,0b10000000,0b11010100,0b11111101,
	0b11011110,0b00010010,0b00000100,0b00100100,
	0b11101101,0b11111011,0b01011011,0b00100101,
	0b00000100,0b10010001,0b01101010,0b11011111,
	0b01110111,0b00010101,0b00000010,0b00100010,
	0b11010101,0b01111010,0b11101111,0b10110110,
	0b00100100,0b10000100,0b10100100,0b11011010,
	// hi-hat (118)
	0b10011010,0b01110100,0b11010100,0b00110011,
	0b00110011,0b11101000,0b11101000,0b01010101,
	0b01010101,
	// end (126)
};


void init_buzzer_driver(BuzzerDriver* driver) {
  driver->current_byte = 0;
  driver->current_bit = 0;
  driver->sample_counter = 0;
  driver->current_sample = 0;

  driver->buffer1 = 0;
  driver->buffer2 = 0;
  driver->buffer2 = 0;
  driver->buffer4 = 0;

  driver->tick_counter = 0;
  driver->tick_speed = 1024;

  driver->music_data = nullptr;

  for (unsigned char i = 0; i < CHANNELS; i++) {
    driver->data_pointer[i] = 0;
    driver->frequency[i] = 255;
    driver->volume[i] = 1;
    driver->octave[i] = 3;
  }

  // initialise output pin
	DDRB = 0b00000001 << OUTPUT;  // == pinMode(8, OUTPUT);
}

void play_buzzer_driver(BuzzerDriver* driver, unsigned char* music_data) {
  driver->current_byte = 0;
  driver->current_bit = 0;
  driver->sample_counter = 0;
  driver->current_sample = 0;

  driver->buffer1 = 0;
  driver->buffer2 = 0;
  driver->buffer2 = 0;
  driver->buffer4 = 0;

  driver->tick_counter = 0;
  driver->tick_speed = 1024;

  driver->music_data = music_data;

  /* Set each channel's data pointer to that channel's data location in the core data array.
	 * Initialise each channel's frequencies. By default they're set to zero which causes out of
	 * tune notes (due to timing errors) until every channel is assigned frequency data.
	 * Additionally, default values are set should no volume or octave be specified */
	for (unsigned char i = 0; i < CHANNELS; i++) {
		driver->data_pointer[i] = pgm_read_byte(&music_data[i * 2]) << 8;
		driver->data_pointer[i] = driver->data_pointer[i] | pgm_read_byte(&music_data[i * 2 + 1]);
		driver->frequency[i] = 255; // random frequency (won't ever be sounded)
		driver->volume[i] = 1;      // default volume : 50% pulse wave
		driver->octave[i] = 3;      // default octave : o3
	}
}

void update_buzzer_driver(BuzzerDriver* driver) {
  if (driver->music_data == nullptr) {
    return;
  }

	/* The code below lowers the volume of the sample channel when the volume is changed,
	 * but slows the routine by a tone... Not desired right now, but could be interesting in
	 * future.
	 *
	 * if(volume[3] > 2)
	 * 	PORTB = 0;
	 */
	/**********************
	 *  Synthesizer Code  *
	 **********************/
	// sampler (channel D) code
	if (driver->sample_counter-- == 0) {
		if (driver->current_byte < driver->current_sample - 1) {
			// read individual bits from the sample array
			driver->out[3] = ((pgm_read_byte(&sample[driver->current_byte]) >> driver->current_bit++) & 1) << OUTPUT;
		} else {
			/* Waste the same number of clock cycles as it takes to process the above to
			 * prevent the pitch from changing when the sampler isn't playing. */
			for (unsigned char i = 0; i < 8; i++) {
				asm("nop;nop;");
      }
			// silence the channel when the sample is over
			driver->out[3] = 0;
		}
		// move to the next byte on bit pointer overflow
		if(driver->current_bit > 7) {
			driver->current_byte++;
			driver->current_bit = 0;
		}
		driver->sample_counter = SAMPLE_SPEED;
	}

	/* Port changes (the demarcated 'output' commands) are carefully interleaved with
	 * generation code to balance volume of outputs. */
	// channel A (pulse 0 code)
	PORTB = driver->out[0]; //output A (0)
	driver->pitch_counter[0] += driver->octave[0];
	if (driver->pitch_counter[0] >= driver->frequency[0])
    driver->pitch_counter[0] = driver->pitch_counter[0] - driver->frequency[0];
	if (driver->pitch_counter[0] <= driver->waveform[0])
		driver->out[0] = 1 << OUTPUT;
	PORTB = driver->out[1]; //output B (1)
	if (driver->pitch_counter[0] >= driver->waveform[0])
		driver->out[0] = 0;
	// channel B (pulse 1 code)
	driver->pitch_counter[1] += driver->octave[1];
	if (driver->pitch_counter[1] >= driver->frequency[1])
		driver->pitch_counter[1] = driver->pitch_counter[1] - driver->frequency[1];
	PORTB = driver->out[2]; //output C (2)
	if (driver->pitch_counter[1] <= driver->waveform[1])
		driver->out[1] = 1 << OUTPUT;
	if (driver->pitch_counter[1] >= driver->waveform[1])
		driver->out[1] = 0;
	// channel C (pulse 2 code)
	driver->pitch_counter[2] += driver->octave[2];
	if (driver->pitch_counter[2] >= driver->frequency[2])
		driver->pitch_counter[2] = driver->pitch_counter[2] - driver->frequency[2];
	PORTB = driver->out[3]; //output D (3)
	if (driver->pitch_counter[2] <= driver->waveform[2])
		driver->out[2] = 1 << OUTPUT;
	if (driver->pitch_counter[2] >= driver->waveform[2])
		driver->out[2] = 0;
  
	/**************************
	 *  Data Processing Code  *
	 **************************/
	if (driver->tick_counter-- == 0) {
		// Variable tempo, sets the fastest / smallest possible clock event.
		driver->tick_counter = driver->tick_speed;
		for (unsigned char voice = 0; voice < CHANNELS; voice++) {
			// If the note ended, start processing the next byte of data.
			if (driver->length[voice] == 0) {
				LOOP: // Yup, a goto, I know.
				// Temporary storage of data for quick processing.
				// first nibble of data
				driver->buffer1 = (pgm_read_byte(&driver->music_data[driver->data_pointer[voice]]) >> 4) & 15;
				// second nibble of data
				driver->buffer2 = pgm_read_byte(&driver->music_data[driver->data_pointer[voice]]) & 15;
				if (driver->buffer1 == 15) {
					// Another buffer for commands that require an additional byte.
					driver->buffer3 = pgm_read_byte(&driver->music_data[driver->data_pointer[voice] + 1]);
					// loop start
					if (driver->buffer2 == 0) {
						driver->loops_active[voice]++;
						driver->loop_point[driver->loops_active[voice] - 1][voice] = driver->data_pointer[voice] + 2;
						driver->loop_duration[driver->loops_active[voice] - 1][voice] = driver->buffer3 - 1;
						driver->data_pointer[voice]+= 2;
					} else if (driver->buffer2 == 1) {
            // loop end
						/* If we reach the end of the loop and the duration isn't zero,
						 * decrement our counter and start again. */
						if (driver->loop_duration[driver->loops_active[voice] - 1][voice] > 0) {
							driver->data_pointer[voice] = driver->loop_point[driver->loops_active[voice] - 1 ][voice];
							driver->loop_duration[driver->loops_active[voice] - 1][voice]--;
						} else {
              // If we're done, move away from the loop.
							driver->loops_active[voice]--;
							driver->data_pointer[voice]++;
						}
					} else if (driver->buffer2 == 2) {
            // macro
						driver->pointer_location[voice] = driver->data_pointer[voice] + 2;
						driver->data_pointer[voice] = pgm_read_byte(&driver->music_data[(driver->buffer3 + CHANNELS) * 2]) << 8;
						driver->data_pointer[voice] = driver->data_pointer[voice] | pgm_read_byte(&driver->music_data[(driver->buffer3 + CHANNELS) * 2 + 1]);
					} else if (driver->buffer2 == 3) {
            // tempo
						driver->tick_speed = driver->buffer3 << 5;
						driver->data_pointer[voice] += 2;
					} else if (driver->buffer2 == 4) {
            // transpose (currently unused)
						driver->data_pointer[voice] += 2; // skip data
          } else if (driver->buffer2 == 5) {
            // instrument (currently unused)
						driver->data_pointer[voice] += 2; // skip data
          } else if (driver->buffer2 == 6) {
            // tie command (currently unused)
						driver->data_pointer[voice]++; // skip data
          } else if (driver->buffer2 == 15) {
						// If we've got a previous position saved, go to it...
						if (driver->pointer_location[voice] != 0) {
							driver->data_pointer[voice] = driver->pointer_location[voice];
							driver->pointer_location[voice] = 0;
						} else {
              // ...If not, go back to the start.
							driver->data_pointer[voice] = pgm_read_byte(&driver->music_data[voice * 2]) << 8;
							driver->data_pointer[voice] = driver->data_pointer[voice] | pgm_read_byte(&driver->music_data[voice * 2 + 1]);
						}
					}
					/* For any command that should happen 'instantaneously' (e.g. anything
					 * that isn't a note or rest - in mmml notes can't be altered once
					 * they've started playing), we need to keep checking this loop until we
					 * find an event that requires waiting. */
					goto LOOP;
				}
				if (driver->buffer1 == 13 || driver->buffer1 == 14) {
					// octave
					if (driver->buffer1 == 13) {
						driver->octave[voice] = 1 << driver->buffer2;
          }
					// volume
					if (driver->buffer1 == 14) {
						driver->volume[voice] = driver->buffer2;
          }
					driver->data_pointer[voice]++;
					goto LOOP; //see comment above previous GOTO
				}
				// note value
				if (driver->buffer1 != 0 && driver->buffer1 < 14) {
					if (voice < 3) {
						driver->buffer4 = pgm_read_word(&note[driver->buffer1]);
						driver->frequency[voice] = driver->buffer4;
						/* Calculate the waveform duty cycle by dividing the frequency by
						 * powers of two. */
						driver->waveform[voice] = driver->buffer4 >> driver->volume[voice];
					} else {
						// reset the sampler
						driver->current_bit = 0;
						driver->current_byte = pgm_read_byte(&sample_index[driver->buffer1 - 1]);
						driver->current_sample = pgm_read_byte(&sample_index[driver->buffer1]);
					}
				} else {
          // rest
					driver->waveform[voice] = 0;
        }

				// note duration value
				if (driver->buffer2 < 8) {
					// standard duration
					driver->length[voice] = 127 >> driver->buffer2;
        } else {
					// dotted (1 + 1/2) duration
					driver->length[voice] = 95 >> (driver->buffer2 & 7);
        }
				// next element in data
				driver->data_pointer[voice]++;
			} else {
        // keep waiting until the note is over
				driver->length[voice]--;
      }
		}
	}
}

void stop_buzzer_driver(BuzzerDriver* driver) {
  init_buzzer_driver(driver);
}