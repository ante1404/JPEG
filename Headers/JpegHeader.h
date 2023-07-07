#ifndef JPEG_HEADER_H
#define JPEG_HEADER_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

struct JpegHeader *ReadHeder(char *filename, struct JpegHeader *Header);
void ReadAppoMarker(FILE *file, struct JpegHeader *Header);
void ReadComment(FILE *file, struct JpegHeader *Header);
struct DQT *ReadQuantazationTable(FILE *file, struct JpegHeader *Header);
struct SOF *ReadStartOfTheFrame(FILE *file, struct JpegHeader *Header);
struct DHT *ReadHuffmanTable(FILE *file, struct JpegHeader *Header);
struct RST *ReadRestartMarker(FILE *file, struct JpegHeader *Header);
struct SOS *ReadStartOfScanMarker(FILE *file, struct JpegHeader *Header);
void writeBMPHeader(FILE *file, int width, int height);
void writeMCUToFile(int x, int y, const char *filename);
int *MCU(struct JpegHeader *jpeg);

typedef unsigned char byte;
typedef unsigned int uint;
// typedef struct DQT *dqtM;

// Start of Frame markers, non-differential, Huffman coding
const byte SOF0 = 0xC0; // Baseline DCT
const byte SOF1 = 0xC1; // Extended sequential DCT
const byte SOF2 = 0xC2; // Progressive DCT
const byte SOF3 = 0xC3; // Lossless (sequential)

// Start of Frame markers, differential, Huffman coding
const byte SOF5 = 0xC5; // Differential sequential DCT
const byte SOF6 = 0xC6; // Differential progressive DCT
const byte SOF7 = 0xC7; // Differential lossless (sequential)

// Start of Frame markers, non-differential, arithmetic coding
const byte SOF9 = 0xC9;  // Extended sequential DCT
const byte SOF10 = 0xCA; // Progressive DCT
const byte SOF11 = 0xCB; // Lossless (sequential)

// Start of Frame markers, differential, arithmetic coding
const byte SOF13 = 0xCD; // Differential sequential DCT
const byte SOF14 = 0xCE; // Differential progressive DCT
const byte SOF15 = 0xCF; // Differential lossless (sequential)

// Define Huffman Table(s)
const byte DHT = 0xC4;

// JPEG extensions
const byte JPG = 0xC8;

// Define Arithmetic Coding Conditioning(s)
const byte DAC = 0xCC;

// Restart interval Markers
const byte RST0 = 0xD0;
const byte RST1 = 0xD1;
const byte RST2 = 0xD2;
const byte RST3 = 0xD3;
const byte RST4 = 0xD4;
const byte RST5 = 0xD5;
const byte RST6 = 0xD6;
const byte RST7 = 0xD7;

// Other Markers
const byte SOI = 0xD8; // Start of Image
const byte EOI = 0xD9; // End of Image
const byte SOS = 0xDA; // Start of Scan
const byte DQT = 0xDB; // Define Quantization Table(s)
const byte DNL = 0xDC; // Define Number of Lines
const byte DRI = 0xDD; // Define Restart Interval
const byte DHP = 0xDE; // Define Hierarchical Progression
const byte EXP = 0xDF; // Expand Reference Component(s)

// APPN Markers
const byte APP0 = 0xE0;
const byte APP1 = 0xE1;
const byte APP2 = 0xE2;
const byte APP3 = 0xE3;
const byte APP4 = 0xE4;
const byte APP5 = 0xE5;
const byte APP6 = 0xE6;
const byte APP7 = 0xE7;
const byte APP8 = 0xE8;
const byte APP9 = 0xE9;
const byte APP10 = 0xEA;
const byte APP11 = 0xEB;
const byte APP12 = 0xEC;
const byte APP13 = 0xED;
const byte APP14 = 0xEE;
const byte APP15 = 0xEF;

// Misc Markers
const byte JPG0 = 0xF0;
const byte JPG1 = 0xF1;
const byte JPG2 = 0xF2;
const byte JPG3 = 0xF3;
const byte JPG4 = 0xF4;
const byte JPG5 = 0xF5;
const byte JPG6 = 0xF6;
const byte JPG7 = 0xF7;
const byte JPG8 = 0xF8;
const byte JPG9 = 0xF9;
const byte JPG10 = 0xFA;
const byte JPG11 = 0xFB;
const byte JPG12 = 0xFC;
const byte JPG13 = 0xFD;
const byte COM = 0xFE;
const byte TEM = 0x01;

struct DQT
{
    uint dataLengt;  // Tells us if the data ifn qt is defined as 8.bit values of 16. If its 8-bit then the value of valid is 0 and 1 if its 16.
    uint tableID;    // For storing table ID. Values can be from 0 to 3.
    uint values[64]; // Values for a quantization table.
};

struct DHT
{
    uint tableType;            // AC table = 1, DC table = 0.
    uint tableID;              // values for Huffman tables are 0,1,2,3
    uint numberOfSymbols;      // This is the sum of the 16 values in huffmanCodesLen array
    byte *huffmanCodesLen[16]; // Contains number of codes with coresponding lenght
    uint symbolsData[325];     // Contains data of each symbol. index 1 and 2 contain the upper and lower nibble data respectivly. Upper nibble is number of preceaading zeros
    byte otherData[300];       // And lower nibble contains the length of coefficient in bits(the lenght of that coefficients code)
                               // For AC  table there can max be 162 symnols and DC max number of symbols is 12
    uint numOfBytes[16];       // This will be to store the values of next 16 bytes, each byte tells us how many symboles are there of lenght of i-th n in this 16 int array
};

struct RST
{
    uint markerNumber;
    uint restartInterval;
};

struct SOF
{
    uint precision;       // Must be 8 because collor chanell values are 8 bit
    uint16_t height;      // Height of the Image
    uint16_t width;       // Width of Image
    uint numOfComponents; // Number of color chanels. 1 if the image is grayscale and 3 if it's Y,Cb,Cr
    uint *componentID;    // componentID (1, 2, 3) for each Y, Cb, Cr
    uint *samepling;      // Will define this later.
    uint *qtIds;          // Quantazation table ID. Y chanel get walue of 0, Cb and Cr get value of 1
};

struct SOS
{
    uint numOfComponents; // Number of color components
    uint componentID[3];
    uint **tableIDs;       // at index of i % 2 == 0 its a DC table id, otherwise its a AC for huffman table
    uint startOfSelection; // must be 0
    uint endOfSelection;   // must be 63
    uint upperNibbleOfSuccessiveAproximation;
    uint lowerNibbleOfSuccessiveAproximation;
};

struct JpegHeader
{
    bool valid;
    struct SOF *sofMarker;
    struct DQT *dqtMarker[4];
    struct DHT *dhtMarker[4];
    struct RST *rstMarker[8];
    struct SOS *sosMarker[8]; // I made it to supporte more than 1 because progressive jpeg have more than 1, but Baseline will always have 1 SOS marker.
    byte *imgBuffer;          // Used to store the rest of the image data after we read the SOS marker, so we can close the file
};

int zigZagMap[] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28, // Index map for traversing the qt and huffman tables in zig-zag order, the values are used as indexes.
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63};

#endif // !JPEG_DEADER_H
