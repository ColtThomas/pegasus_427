#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define READBINARY "rb"
#define WRITETEXT "w"
#define WAVARG 1
#define CARG 2
#define BEGIN_TO_NUMBER_OF_CHANNELS 22
#define RATE_TO_SAMPLE_SIZE 6
#define SAMPLE_SIZE_TO_DATA_SIZE 4
#define BITS_PER_BYTE 8
#define BYTE 8
#define TWOBYTE 16

uint16_t num_channels;
uint32_t sample_rate;
uint16_t bits_per_sample;
uint32_t frame_count;
char* c_prefix;


void handle_header(FILE *wavpointer, FILE *cpointer) {
// begin by skipping to the first thing we care about: the number of channels
	fseek(wavpointer, BEGIN_TO_NUMBER_OF_CHANNELS, SEEK_SET);
	fread(&num_channels,sizeof(uint16_t),1,wavpointer);
	fread(&sample_rate,sizeof(uint32_t),1,wavpointer);
	fseek(wavpointer, RATE_TO_SAMPLE_SIZE, SEEK_CUR);
	fread(&bits_per_sample,sizeof(uint16_t),1,wavpointer);
	fseek(wavpointer,SAMPLE_SIZE_TO_DATA_SIZE,SEEK_CUR);
	fread(&frame_count,sizeof(uint32_t),1,wavpointer);
	frame_count /= bits_per_sample;
	frame_count *= BITS_PER_BYTE;
	// write header
	fprintf(cpointer,"// %s file generated from wav file by Dallen Ford and Colt Thomas.\n",c_prefix);
	fprintf(cpointer,"\n#include<stdint.h>\n");
	fprintf(cpointer,"\nuint32_t %sRate = %d;\n",c_prefix,sample_rate);
	fprintf(cpointer,"\nuint32_t %sFrames = %d;\n",c_prefix,frame_count);
	fprintf(cpointer,"\nuint32_t %s[] = {\n",c_prefix);
}

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Invalid arguments. Usage: soundconvert infile.wav outfile.c\n");
		return -1;
	}
	FILE *wavpointer;
	FILE *cpointer;
	int i;
// setup the prefix based on the filename
	c_prefix = (char*)malloc(sizeof(char)*(strlen(argv[CARG]) - 2));
	for(i = 0; i < strlen(argv[CARG])-2; i++) {
		c_prefix[i] = argv[CARG][i];
}
	wavpointer = fopen(argv[WAVARG],READBINARY);
	if(!wavpointer) {
		printf("Unable to open wav file!\n");
		return -2;
	}
	cpointer = fopen(argv[CARG],WRITETEXT);
	if(!cpointer) {
		printf("Unable to open c file!\n");
		return -2;
	}

	// read through header	
	handle_header(wavpointer,cpointer);
	
	// iterate through file, reading samples
	uint16_t sample;
	for(i = 0; i < frame_count; i ++) {
		switch(bits_per_sample) {
		case BYTE:
			fread(&sample,sizeof(uint8_t),1,wavpointer);
			break;
		case TWOBYTE:
			fread(&sample,sizeof(uint16_t),1,wavpointer);
			break;
		}
		fprintf(cpointer,"%d",sample);
		if(i != frame_count-1) {
			fprintf(cpointer,",");
		}
		if(!((i+1)%8)) { // 8 samples per line
			fprintf(cpointer,"\n");
		}
	}
	fprintf(cpointer,"};\n");
}
