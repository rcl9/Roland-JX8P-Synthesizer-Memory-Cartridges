// JX8P Binary-Memory-Image to SysEx File Converter, by RCL9

// This utility program loads in 2k JX8P binary memory images of the memory cartridge and turns them into "SysEx"
// files which can be sent back to the JX8P via a normal patch librarian. The 32 patches can be output as a single
// .syx file (+ a MIDI message to write each patch into the JX8P memory), or as 32 separate .syx files (with no
// MIDI message to write the patch into the JX8P memory).

// To download to the JX8P, use the "CJXP" Windows utility program. Turn on "System Exclusive" on the JX8P first.   

// -------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static void Usage();

	int
main(int argc, char *argv[])
{
	FILE	*fpi, *fpo;
	short	done, fnum, break_apart = 0;
	char	*str, *in_name;
	long	offset=0;

	if (argc < 1)
		Usage();

	/* Pick up any command line options */
	fnum = 0;
	done = 0;
	while (argc > 1 && !done) {
		str = argv[1];
		if (str[0] == '-') {
			switch (str[1]) {
				case 'b':
				case 'B':
					break_apart = 1;
					break;

#if 00
				case 'h':
				case 'H':
					if (argc <= 2) {
						printf("Height parameter required for -w option\n");
						exit(1);
					}
					argv++;
					argc--;
 					height = atoi(argv[1]);
					break;
#endif
			}
		} else {
			if (fnum > 0) 
				Usage();
			else {
				in_name = argv[1];
				++fnum;
			}
			done = 1;
		}
		argv++;
		argc--;
	}

	if (fnum != 1)
		Usage();

	if ((fpi = fopen(in_name, "rb")) == (FILE *) NULL) {
		fprintf(stderr, "Error opening input file '%s'.\n", in_name);
		exit(1);
	}

	if ((fpo = fopen("file.syx", "wb")) == (FILE *) NULL) {
		fprintf(stderr, "Error opening output file '%s'.\n", "file.syx");
		exit(1);
	}

	// Skip over the junk at the start of the JX8P cartridge data
loop2:	for (int i=0; i < 0xA0; ++i)
		fgetc(fpi);
	offset += 0xA0;

	short	patch_num = 0;
	while (!feof(fpi)) {
		char patch_name[20];

		// Output header of the Roland JX8P's System Exclusive message
		fputc( 0xF0, fpo );
		fputc( 0x41, fpo );	// Roland ID
		fputc( 0x35, fpo );	// Opcode: Tone button pressed
		fputc( 0x00, fpo );	// Channel/Unit number 
		fputc( 0x21, fpo );	// Format type
		fputc( 0x20, fpo );	// Level: Patch button pressed
		fputc( 0x01, fpo );	// Group

		// Process the 59 bytes of the tone (in pairs of parameter/value)

		for (int i=0; i < 10; ++i) {
        		if ( feof(fpi) )
				goto done;

			patch_name[i] = fgetc(fpi); 

			fputc( patch_name[i], fpo );
		}

		for (int i=0; i < 49; ++i) {
        		if ( feof(fpi) )
				goto done;

			short val = fgetc(fpi);
			if ( val == EOF )
				break;

			if (val > 0x80)	
				val &= ~0x80;

			fputc( val, fpo );

		}

		// Output end of the Roland JX8P's System Exclusive message
		fputc( 0xF7, fpo );

		offset += 59;

		// -------------------------

#if 00
		// Individual Patch Parameter (for single tone SYS-EX files)

		fputc( 0xF0, fpo );
		fputc( 0x41, fpo );	// Roland ID
		fputc( 0x36, fpo );	// Opcode: 
		fputc( 0x00, fpo );	// MIDI channel
		fputc( 0x21, fpo );	// Format type = JX8P
		fputc( 0x30, fpo );	// Level = 2
		fputc( 0x01, fpo );	// Group #

		fputc( 00, fpo );	// Parameter (0-8)	- Bend range 
		fputc( 0x60, fpo );	// Value (0-127) 

		fputc( 01, fpo );	// Portamento time	 
		fputc( 00, fpo ); 

		fputc( 02, fpo );	// Portamento SW 
		fputc( 00, fpo ); 

		fputc( 03, fpo ); 	// Assign select mode
		fputc( 00, fpo ); 

		fputc( 04, fpo );	 // After touch select 
		fputc( 00, fpo ); 

		fputc( 05, fpo );	// Bend LFO 
		fputc( 00, fpo ); 

		fputc( 06, fpo );	// Unison detune (0-127) 
		fputc( 0x40, fpo );

		fputc( 0xF7, fpo );
#endif

		// -------------------------

		// Write the patch to a specific memory location if we are outputting all the bank patches into a single .syx file

		if (!break_apart) {
			fputc( 0xF0, fpo );
			fputc( 0x41, fpo );	// Roland ID
			fputc( 0x34, fpo );	// Opcode: write patch to memory location 
			fputc( 0x00, fpo );	// MIDI channel
			fputc( 0x21, fpo );	// Format type = JX8P
			fputc( 0x20, fpo );	// Level = 1
			fputc( 0x01, fpo );	// Group #

			fputc( 00, fpo );		// Extension of program # 
			fputc( patch_num, fpo );	// This is the 0-based patch number 
			fputc( 02, fpo );		// Function # (write to memory) 

			fputc( 0xF7, fpo );
		}

		if (break_apart) {
			char	filename[20];

			patch_name[10] = 0;
			strcpy(filename, patch_name);

			// Remove any illegal characters
			for (int i =0; i < 10; ++i) {
				if (filename[i] == '/')
					filename[i] = '-' ;
				else if (filename[i] == '\\')
					filename[i] = '-' ;
			}

			// Remove whitespace at end of the patch name
			for (int i=9; i >=0; --i) {
				if (filename[i] == ' ')
					filename[i] = 0;
				else
					break;
			}

			// Remove whitespace at start of the patch name
loop:			while ( filename[0] == ' ' ) {
				strcpy(filename, filename+1);
				goto loop;
			}
			if (!strlen(filename))
				strcpy(filename, "(empty)");

			strcat(filename, ".syx");

			fclose(fpo);		
			rename ("file.syx", filename);

			if ( !feof(fpi) )
				fpo = fopen("file.syx", "wb");
		} 
		++patch_num;

		// This might be 8 banks in one image (8 x 32 tones, 64k cartridge)
		if (patch_num >= 32) {
			patch_num=0;
			goto loop2;
		}
	}

done:	fclose(fpi);
	fclose(fpo);

	if (break_apart)
		remove("file.syx");
	else {
		char	filename[20];

		strcpy(filename, in_name);
		strcat(filename, ".syx");
		rename("file.syx", filename);
	}

}

	static void
Usage()
{
	printf("\nJX8P Cartridge Memory Image to MIDI SYS-EX File Converter, v1.0 (c) RCL9.\n");
	printf("Usage: `convert [-b] filename.raw'\n");
	printf("       -b = Break apart the binary image into individual Sys-EX files based on the patch names (+ no 'write to memory' SysEx code)\n");
	exit(0);
}
