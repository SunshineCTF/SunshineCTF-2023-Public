/*
This example program makes use of the simple
sound library to generate a sine wave and write the
output to sound.wav.
For complete documentation on the library, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2013/wavfile
Go ahead and modify this program for your own purposes.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "wavfile.h"

const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND * 0.2);

int main(int argc, char *argv[])
{
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;

	if (argc > 1)
	{
		sscanf(argv[1], "%lf", &frequency);
		if (frequency > 32000 || frequency < 60)
		{
			printf("Bad frequency: %lf\n", frequency);
			return EXIT_FAILURE;
		}
	}
	// i'm ignoring buffer overflows; we're not intending the user to be able to run this file.
	// if you're reading this on the server, congrats! stop wasting time, there are no easter egg
	// flags! :D
	char filename[100];
	if (argc < 3)
	{
		int rounded = round(frequency);
		snprintf(filename, sizeof(filename), "%d.wav", rounded);
	}
	else
	{
		snprintf(filename, sizeof(filename), "%s", argv[2]);
	}
	printf("Filename: %s, frequency: %lf\n", filename, frequency);
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for (i = 0; i < length; i++)
	{
		double t = (double)i / WAVFILE_SAMPLES_PER_SECOND;
		waveform[i] = volume * sin(frequency * t * 2 * M_PI);
	}

	FILE *f = wavfile_open(filename);
	if (!f)
	{
		printf("couldn't open %s for writing: %s\n", filename, strerror(errno));
		return EXIT_FAILURE;
	}

	wavfile_write(f, waveform, length);
	wavfile_close(f);

	return EXIT_SUCCESS;
}
