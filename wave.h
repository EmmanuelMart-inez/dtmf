// WAVE file header format
typedef struct HEADER {
	unsigned char riff[4]; 					//BIG	// ChunkID:RIFF string
	unsigned long overall_size;				//LIT	// ChunkSize: overall size of file in bytes
	unsigned char wave[4];					//BIG	// Format: "WAVE" string
	unsigned char fmt_chunk_marker[4];		//BIG	// SubChunkID: "fmt" string with trailing null char
	unsigned long length_of_fmt;			//LIT	// SubChunk1Size :length of the rest format chunk
	unsigned int format_type;				//LIT	// Audioformat: type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned int channels;					//LIT	// NumChannels:no.of channels
	unsigned int sample_rate;				//LIT	// sampling rate (blocks per second)
	unsigned int byterate;					//LIT	// SampleRate * NumChannels * BitsPerSample/8
	unsigned int block_align;				//LIT	// NumChannels * BitsPerSample/8
	unsigned int bits_per_sample;			//LIT	// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header[4];		//BIG	// SubChunk2ID:"DATA" string or FLLR string
	unsigned long data_size;				//LIT	//SubChunk2Size: Number of bytes in the data					// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	short *muestras;						//BIG	//Sound data
	unsigned char end[74];					//BIG 	//EndfileHEADER							

		unsigned char overall_sizeb[4];					//buffer = sufijo "buffer/BITS"
		unsigned char length_of_fmtb[4];
 		unsigned char format_typeb[2];
 		unsigned char channelsb[2];
 		unsigned char sample_rateb[4];
 		unsigned char byterateb[4];
 		unsigned char block_alignb[2];
 		unsigned char bits_per_sampleb[2];
 		unsigned char data_sizeb[4];
		long num_samples;
		long size_of_each_sample;
} wave;
   