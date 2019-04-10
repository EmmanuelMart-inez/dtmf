// WAVE file header format
typedef struct HEADER {
	unsigned char riff[4];						// RIFF string
	unsigned int overall_size;					// overall size of file in bytes
	unsigned char overall_sizeb[4];					//buffer = sufijo "b"
	unsigned char wave[4];						// WAVE string
	unsigned char fmt_chunk_marker[4];			// fmt string with trailing null char
	unsigned int length_of_fmt;					// length of the format data
	unsigned char length_of_fmtb[4];
	unsigned int format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned char format_typeb[2];
	unsigned int channels;						// no.of channels
	unsigned char channelsb[2];
	unsigned int sample_rate;					// sampling rate (blocks per second)
	unsigned char sample_rateb[4];
	unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8
	unsigned int byterateb[4];
	unsigned int block_align;					// NumChannels * BitsPerSample/8
	unsigned int block_alignb[2];
	unsigned int bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned int bits_per_sampleb[2];
	unsigned char data_chunk_header[4];			// DATA string or FLLR string
	unsigned int data_size;
	unsigned int data_sizeb[4];
	long num_samples;
	long size_of_each_sample;					// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	short *muestras;
	unsigned char end[74];
} wave;
