#include <stdio.h>
#include <stdint.h>

extern tankFireSoundRate;
extern tankFireSoundFrames;
extern tankFireSound[];

#define BITS_PER_SAMPLE 8
#define HEADER_SIZE 44-8

//I'VE GOT TO FIGURE OUT HOW TO FIX THE ENDIANNESS
// IT'S ALL WRONG

void write_wav_header(FILE *wavpointer) {
	//char* riff = "riff";	
	uint32_t data;
	fwrite("RIFF",sizeof(char),4,wavpointer);
	data = tankFireSoundFrames + HEADER_SIZE;
	fwrite(&data,sizeof(uint32_t),1,wavpointer); // placeholder zeros for file size
	fwrite("WAVE",sizeof(char),4,wavpointer); // file type header
	fwrite("fmt ",sizeof(char),4,wavpointer); // format chunk marker
	data = 16;
	fwrite(&data,sizeof(uint32_t),1,wavpointer); // length of format data
	data = 1;
	fwrite(&data,sizeof(uint16_t),1,wavpointer); // type - 1 is PCM
	fwrite(&data,sizeof(uint16_t),1,wavpointer); // number of channels - 1 for our purpose
	fwrite(&tankFireSoundRate,sizeof(uint32_t),1,wavpointer); // sample rate
	uint32_t bytes = tankFireSoundRate*BITS_PER_SAMPLE*1/8;
	fwrite(&bytes,sizeof(uint32_t),1,wavpointer);
	bytes = BITS_PER_SAMPLE *1 / 8;
	fwrite(&bytes,sizeof(uint16_t),1,wavpointer);
	data = BITS_PER_SAMPLE;
	fwrite(&data,sizeof(uint16_t),1,wavpointer);
	fwrite("data",sizeof(char),4,wavpointer);
	fwrite(&tankFireSoundFrames,sizeof(uint32_t),1,wavpointer);
}

int main() {
	FILE *wavpointer;
	int i;
	wavpointer = fopen("tankFire.wav","wb");
	if(!wavpointer) {
		printf("unable to open tankFire.wav.\n");
		return -1;
	}
	
	// write header section
	write_wav_header(wavpointer);
	
	printf("for\n");
	for (i = 0; i < tankFireSoundFrames; i++) {
		printf("tankFireSound[%d] = %d\n",i,tankFireSound[i]);
		fwrite(&tankFireSound[i],sizeof(uint8_t),1,wavpointer);
	}
	fclose(wavpointer);
}
