#include "../Headers/JpegHeader.h"

int main(int argc, const char **argv)
{
    if (argc > 2)
    {
        printf("More than 2 Command Line Arguments\n");
        return 1;
    }
    char filename[50] = {0};
    strcpy(filename, argv[1]);
    printf("%s\n", filename);

    struct JpegHeader *Header = (struct JpegHeader *)calloc(1, sizeof(struct JpegHeader));
    Header->valid = true;

    Header = ReadHeder(filename, Header);
    MCU(Header);

    return 0;
}

struct JpegHeader *ReadHeder(char *filename, struct JpegHeader *Header)
{

    FILE *jpg = fopen(filename, "rb");
    fseek(jpg, 0, SEEK_END);
    int len = ftell(jpg);
    printf("LEN -> %d\n", len);
    rewind(jpg);

    byte *imgBuffer = NULL;

    uint i = 0; // Index for Quantazion tables;
    uint j = 0; // Index for Huffman tables;
    uint k = 0; // Restart marker index
    uint l = 0; // SOS marker index;

    byte first = fgetc(jpg);
    byte secound = fgetc(jpg);

    if (first != 0xFF && secound != SOI)
    {

        fclose(jpg);
        Header->valid = false;
        return NULL;
    }

    while (Header->valid && !feof(jpg))
    {
        first = fgetc(jpg);
        secound = fgetc(jpg);

        if ((first == 0xFF) && (secound >= APP0 && secound <= APP15)) // JFIF marker
        {
            ReadAppoMarker(jpg, Header);
        }
        else if (first == 0xFF && secound == COM)
        {
            ReadComment(jpg, Header);
        }
        else if (first == 0xFF && secound == DQT)
        {
            Header->dqtMarker[i++] = ReadQuantazationTable(jpg, Header);
        }
        else if (first == 0xFF && secound == SOF0)
        {
            Header->sofMarker = ReadStartOfTheFrame(jpg, Header);
        }
        else if (first == 0xFF && secound == DRI)
        {
            Header->rstMarker[k++] = ReadRestartMarker(jpg, Header);
        }
        else if ((first == 0xFF && secound == SOS) || (first == SOS && secound == 0xFF))
        {
            Header->sosMarker[l++] = ReadStartOfScanMarker(jpg, Header);
            int i = 0;
            int curr = ftell(jpg);
            fseek(jpg, 0, SEEK_END);
            off_t n = ftell(jpg) - curr;
            fseek(jpg, curr, SEEK_SET);
            Header->imgBuffer = (byte *)malloc(n * sizeof(byte));
            while (i < n)
            {
                int m = fread(&Header->imgBuffer[i++], sizeof(byte), 1, jpg);
            }
            fclose(jpg);
            printf("%X %X\n", Header->imgBuffer[i - 2], Header->imgBuffer[i - 1]);
            return Header;
        }
        else if (first == 0xFF && secound == DHT)
        {
            Header->dhtMarker[j++] = ReadHuffmanTable(jpg, Header);
        }
    }

    return Header;
}

void ReadAppoMarker(FILE *filename, struct JpegHeader *Header)
{

    byte length = (fgetc(filename) << 8) + fgetc(filename); // this will be used to read 2 bytes and convert them to little endian
    byte appo[200] = {0};
    for (int i = 0; i < length - 2; i++)
    {
        appo[i] = fgetc(filename);
    }
    printf("=================APPN=====================\n");
    printf("APPN data -> %s\n", appo);
    printf("APPn length -> %d\n", length);
}

void ReadComment(FILE *file, struct JpegHeader *Header)
{

    byte length = (fgetc(file) << 8) + fgetc(file);

    byte comment[2000] = {0};
    for (int i = 0; i < length - 2; i++)
    {
        comment[i] = fgetc(file);
        printf("%c", comment[i]);
    }
    printf("\n%d\n", length);
}

struct DQT *ReadQuantazationTable(FILE *file, struct JpegHeader *Header)
{

    byte length = (fgetc(file) << 8) + fgetc(file);

    struct DQT *quantazationTable = (struct DQT *)malloc(sizeof(struct DQT));
    uint secound = 0;
    uint upperNibble = 0;
    uint lowerNibble = 0;

    int i = 0;
    secound = fgetc(file);
    printf("============DQT=========================\n");
    printf("length -> %d\n", length);
    printf("info -> %d\n", secound);
    upperNibble = (secound >> 4) & 0x0F;
    lowerNibble = secound & 0x0F; // Tels us the tabe ID.
    quantazationTable->dataLengt = upperNibble;
    quantazationTable->tableID = lowerNibble;
    for (i = 0; i < length - 3; i++)
    {
        quantazationTable->values[zigZagMap[i]] = fgetc(file);
    }
    printf("Data length = %d, Table ID = %d\n", quantazationTable->dataLengt, quantazationTable->tableID);

    for (int i = 0; i < 64; i++)
    {
        printf("%d,", quantazationTable->values[i]);
        if (i % 8 == 0 && i != 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    return quantazationTable;
}

struct SOF *ReadStartOfTheFrame(FILE *file, struct JpegHeader *Header)
{

    byte length = (fgetc(file) << 8) + fgetc(file);
    struct SOF *startOfTheFrame = (struct SOF *)malloc(sizeof(struct SOF));

    startOfTheFrame->precision = fgetc(file);
    startOfTheFrame->height = (fgetc(file) << 8) + fgetc(file);
    startOfTheFrame->width = (fgetc(file) << 8) + fgetc(file);
    startOfTheFrame->numOfComponents = fgetc(file);
    printf("===============SOF=====================\n");
    printf("Precision -> %d\n", startOfTheFrame->precision);
    printf("height -> %d\nwidth -> %d\n", startOfTheFrame->height, startOfTheFrame->width);
    printf("Number of Components -> %d\n", startOfTheFrame->numOfComponents);

    startOfTheFrame->componentID = malloc(startOfTheFrame->numOfComponents * sizeof(uint));
    startOfTheFrame->samepling = malloc(startOfTheFrame->numOfComponents * sizeof(uint));
    startOfTheFrame->qtIds = malloc(startOfTheFrame->numOfComponents * sizeof(uint));

    for (uint i = 0; i < startOfTheFrame->numOfComponents; i++)
    {
        startOfTheFrame->componentID[i] = fgetc(file);
        printf("ComponentID -> %d\n", startOfTheFrame->componentID[i]);
        startOfTheFrame->samepling[i] = fgetc(file);
        printf("Sampleing -> %d\n", startOfTheFrame->samepling[i]);
        startOfTheFrame->qtIds[i] = fgetc(file); // ID 1 is for luminance chanell(component) 2 and 3 are for Cb and Cr chanels.
        printf("QTID -> %d\n", startOfTheFrame->qtIds[i]);
    }

    return startOfTheFrame;
}

struct RST *ReadRestartMarker(FILE *file, struct JpegHeader *Header)
{

    byte length = (fgetc(file) << 8) + fgetc(file);

    struct RST *rstMarker = (struct RST *)malloc(sizeof(struct RST));
    printf("======================RST======================\n");

    rstMarker->restartInterval = (fgetc(file) << 8) + fgetc(file);

    if (length - 4 != 0)
    {
        perror("Invalid marker\n");
        Header->valid = false;
        return NULL;
    }
    printf("len -> %d\n", length);
    printf("Restart Interval -> %d\n", rstMarker->restartInterval);

    return rstMarker;
}

struct DHT *ReadHuffmanTable(FILE *file, struct JpegHeader *Header)
{

    struct DHT *dhtMarker = (struct DHT *)malloc(sizeof(struct DHT));

    byte length = (fgetc(file) << 8) + fgetc(file);

    byte upperNibble = 0;
    byte lowerNibble = 0;
    byte tableInfo = 0;

    memset(dhtMarker->numOfBytes, 0, 16);
    uint numOfSymbols = 0;     // Nummber of symbols in this marker

    tableInfo = fgetc(file);
    upperNibble = (tableInfo >> 4) & 0x0F;
    lowerNibble = tableInfo & 0x0F;

    dhtMarker->tableType = upperNibble;
    dhtMarker->tableID = lowerNibble;

    for (int i = 0; i < 16; i++)
    {
        dhtMarker->huffmanCodesLen[i] = NULL;
        dhtMarker->numOfBytes[i] = fgetc(file);
        if (dhtMarker->numOfBytes[i] != 0)
        {
            dhtMarker->huffmanCodesLen[i] = (byte *)malloc(dhtMarker->numOfBytes[i] * sizeof(byte));
            numOfSymbols += dhtMarker->numOfBytes[i];
        }
    }

    // array counter for codeslen
    int m = 0;
    // Index for symbol data array
    int k = 0;
    int u = 0;

    byte *symbols = (byte *)calloc(numOfSymbols, sizeof(byte));

    for (int i = 0; i < numOfSymbols; i++)
    {
        symbols[i] = fgetc(file);
    }

    for (int i = 0; i < 16; i++)
    {
        if (dhtMarker->numOfBytes[i] != 0)
        {
            for (int j = 0; j < dhtMarker->numOfBytes[i]; j++)
            {
                dhtMarker->huffmanCodesLen[i][m++] = symbols[u];
                dhtMarker->symbolsData[k++] = upperNibble = (symbols[u] >> 4) & 0x0F;
                dhtMarker->symbolsData[k++] = upperNibble = symbols[u] & 0x0F;
                u++;
            }
            m = 0;
        }
    }
    // If thre is random data after all that should be taken it will be read here is there isint.
    // The condition in for loop will evaluate to zero and it will skip it. (It's random because i dont know why is there data after what it read, for now)
    for (int i = 0; i < length - 3 - 16 - numOfSymbols; i++)
    {
        dhtMarker->otherData[i] = fgetc(file);
        // printf("%d ", m);
    }

    printf("===================DHT========================\n");
    printf("Number of bytes -> %d\n", dhtMarker->numOfBytes[2]);
    printf("Hf Table type -> %d\n", dhtMarker->tableType);
    printf("Hf Table Id -> %d\n", dhtMarker->tableID);
    printf("Number of symbols -> %d\n", numOfSymbols);

    for (int i = 0; i < 16; i++)
    {
        printf("%d -> :", i+1);
        if (dhtMarker->numOfBytes[i] == 0)
        {
            printf("\n");
            continue;
        }
        for (int j = 0; j < dhtMarker->numOfBytes[i]; j++)
        {
            printf("%X ", dhtMarker->huffmanCodesLen[i][j]);
        }
        printf("\n");
    }
    /*
    if (dhtMarker->tableType == 0)
    {
        for (int i = 0; i < 350; i++)
        {
            if (i % 2 == 0)
            {
                printf("%d\n", dhtMarker->symbolsData[i]);
            }

        }

    }
    */

    return dhtMarker;
}

struct SOS *ReadStartOfScanMarker(FILE *file, struct JpegHeader *Header)
{

    struct SOS *sosMarker = (struct SOS *)malloc(sizeof(struct SOS));

    uint lengt = (fgetc(file) << 8) + fgetc(file);
    printf("%d\n", lengt);

    sosMarker->numOfComponents = fgetc(file);

    int j = 0;
    byte upperNibble = 0;
    byte lowerNibble = 0;
    byte symbol = 0;
    sosMarker->tableIDs = (uint **)malloc(sizeof(uint *));
    for (int i = 0; i < sosMarker->numOfComponents; i++)
    {
        sosMarker->componentID[i] = fgetc(file);
        sosMarker->tableIDs[i] = (uint *)malloc(2 * sizeof(uint));
        symbol = fgetc(file);
        upperNibble = (symbol >> 4) & 0x0F;
        lowerNibble = symbol & 0x0F;
        sosMarker->tableIDs[i][j++] = upperNibble;
        sosMarker->tableIDs[i][j] = lowerNibble;
        j = 0;
    }
    sosMarker->startOfSelection = fgetc(file);
    sosMarker->endOfSelection = fgetc(file);
    symbol = fgetc(file);
    sosMarker->upperNibbleOfSuccessiveAproximation = upperNibble = (symbol >> 4) & 0x0F;
    sosMarker->lowerNibbleOfSuccessiveAproximation = lowerNibble = symbol & 0x0F;

    printf("================SOS===================\n");
    printf("Number of components -> %d\n", sosMarker->numOfComponents);
    for (int i = 0; i < sosMarker->numOfComponents; i++)
    {
        printf("Huffman Table ID(DC) -> %d\n", sosMarker->tableIDs[i][j++]);
        printf("Huffman Table ID(AC) -> %d\n", sosMarker->tableIDs[i][j]);
        j = 0;
    }
    printf("Start of Selection -> %d\n", sosMarker->startOfSelection);
    printf("End of Selection -> %d\n", sosMarker->endOfSelection);
    printf("Sucessive Aproximation -> %d\n", sosMarker->upperNibbleOfSuccessiveAproximation);
    printf("Sucessive Aproximation -> %d\n", sosMarker->lowerNibbleOfSuccessiveAproximation);

    return sosMarker;
}

void writeBMPHeader(FILE *file, int width, int height)
{
    // BMP File Header
    uint16_t fileType = 0x4D42;                  // "BM"
    uint32_t fileSize = width * height * 3 + 54; // Size of image data + BMP header size
    uint32_t reserved = 0;
    uint32_t offset = 54; // Offset to the image data

    fwrite(&fileType, sizeof(uint16_t), 1, file);
    fwrite(&fileSize, sizeof(uint32_t), 1, file);
    fwrite(&reserved, sizeof(uint32_t), 1, file);
    fwrite(&offset, sizeof(uint32_t), 1, file);

    // BMP Info Header
    uint32_t headerSize = 40;                // Size of the info header
    uint32_t imageSize = width * height * 3; // Size of the image data
    uint16_t planes = 1;
    uint16_t bitDepth = 24;       // 24 bits per pixel, assuming RGB color space
    uint32_t compression = 0;     // No compression
    uint32_t imageSizeRaw = 0;    // Not used if compression is 0
    uint32_t xResolution = 5905;  // Pixels per meter (default: 72 dpi)
    uint32_t yResolution = 5905;  // Pixels per meter (default: 72 dpi)
    uint32_t colorsUsed = 0;      // All colors are used
    uint32_t importantColors = 0; // All colors are important

    fwrite(&headerSize, sizeof(uint32_t), 1, file);
    fwrite(&width, sizeof(uint32_t), 1, file);
    fwrite(&height, sizeof(uint32_t), 1, file);
    fwrite(&planes, sizeof(uint16_t), 1, file);
    fwrite(&bitDepth, sizeof(uint16_t), 1, file);
    fwrite(&compression, sizeof(uint32_t), 1, file);
    fwrite(&imageSize, sizeof(uint32_t), 1, file);
    fwrite(&xResolution, sizeof(uint32_t), 1, file);
    fwrite(&yResolution, sizeof(uint32_t), 1, file);
    fwrite(&colorsUsed, sizeof(uint32_t), 1, file);
    fwrite(&importantColors, sizeof(uint32_t), 1, file);

    // Additional fields in the BMP Info Header
    uint32_t redMask = 0x00FF0000;         // Red channel mask
    uint32_t greenMask = 0x0000FF00;       // Green channel mask
    uint32_t blueMask = 0x000000FF;        // Blue channel mask
    uint32_t alphaMask = 0xFF000000;       // Alpha channel mask
    uint32_t colorSpaceType = 0x73524742;  // "sRGB" color space
    uint32_t colorSpaceEndpoints[9] = {0}; // Set color space endpoints if applicable

    fwrite(&redMask, sizeof(uint32_t), 1, file);
    fwrite(&greenMask, sizeof(uint32_t), 1, file);
    fwrite(&blueMask, sizeof(uint32_t), 1, file);
    fwrite(&alphaMask, sizeof(uint32_t), 1, file);
    fwrite(&colorSpaceType, sizeof(uint32_t), 1, file);
    fwrite(&colorSpaceEndpoints, sizeof(uint32_t), 9, file);
}

int *MCU(struct JpegHeader *jpeg)
{

    int NumOfMcuY = ceil(jpeg->sofMarker->width / 8.0);

    int y = ceil(jpeg->sofMarker->width / 8.0);
    int x = ceil(jpeg->sofMarker->height / 8.0);

    printf("(%d, %d)\n", x, y);
    printf("(%d, %d)\n", jpeg->sofMarker->height, jpeg->sofMarker->height);

    int ***MCU = (int ***)malloc((x * y) * sizeof(int **));

    for (int i = 0; i < x * y; i++)
    {
        MCU[i] = (int **)malloc(8 * sizeof(int *));
        for (int j = 0; j < 8; j++)
        {
            MCU[i][j] = (int *)malloc(8 * sizeof(int));
        }
    }

    int mx = 0; //MCU coordiantes
    int my = 0;

    for (int i = 0; i < jpeg->sofMarker->height; i++)
    {
        for (int j = 0; j < jpeg->sofMarker->width; j++)
        {
            int x = i / 8; // MCU coordinates.
            int y = j / 8;
            int k = x * NumOfMcuY + y; // MCU index
            if (i < 8)
            {
                my = i;
            }
            else
            {
                my = i % 8;
            }
            if (j < 8)
            {
                mx = j;
            }
            else
            {
                mx = j % 8;
            }
            //printf("%d: %d, %d\n", k, my, mx);
        }
    }

    return 0;
}