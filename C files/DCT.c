#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb-master/stb_image_write.h"
#define PI 3.14159265358979

void test();
double **DTC(double (*mat)[8], int n);

void writeYComponent(unsigned char *image, int width, int height, int channels)
{
    unsigned char *YBuffer = malloc(width * height * sizeof(unsigned char));
    if (!YBuffer)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int pixel = (y * width + x) * channels;
            unsigned char red = image[pixel];
            unsigned char green = image[pixel + 1];
            unsigned char blue = image[pixel + 2];

            int Y = 0.299 * red + 0.587 * green + 0.114 * blue;
            YBuffer[y * width + x] = Y;
        }
    }

    const char *filename = "YComponent.bmp";
    stbi_write_bmp(filename, width, height, 1, YBuffer);

    free(YBuffer);
}

void writeCbComponent(unsigned char *image, int width, int height, int channels)
{
    unsigned char *CbBuffer = malloc(width * height * sizeof(unsigned char));
    if (!CbBuffer)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    int Cb = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int pixel = (y * width + x) * channels;
            unsigned char red = image[pixel];
            unsigned char green = image[pixel + 1];
            unsigned char blue = image[pixel + 2];
            int Y = 0.299 * red + 0.587 * green + 0.114 * blue;

            Cb = 128 - (0.168746 * red) - (0.331264 * green) + (0.5 * blue);

            CbBuffer[y * width + x] = Cb;
        }
    }
    printf("%d ", Cb);
    const char *filename = "CbComponent.bmp";
    stbi_write_bmp(filename, width, height, 1, CbBuffer);

    free(CbBuffer);
}

void writeCrComponent(unsigned char *image, int width, int height, int channels)
{
    unsigned char *CrBuffer = malloc(width * height * sizeof(unsigned char));
    if (!CrBuffer)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    int Cr = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int pixel = (y * width + x) * channels;
            unsigned char red = image[pixel];
            unsigned char green = image[pixel + 1];
            unsigned char blue = image[pixel + 2];
            int Y = 0.299 * red + 0.587 * green + 0.114 * blue;

            Cr = 128 + (0.5 * red) - (0.418688 * green) - (0.081312 * blue);

            CrBuffer[y * width + x] = Cr;
        }
    }
    printf("%d ", Cr);
    const char *filename = "CrComponent.bmp";
    stbi_write_bmp(filename, width, height, 1, CrBuffer);

    free(CrBuffer);
}

int main()
{

    
    test();

    double matrix[8][8] =
        {{-76, -73, -67, -62, -58, -67, -64, -55},
         {-65, -69, -73, -38, -19, -43, -59, -56},
         {-66, -69, -60, -15, 16, -24, -62, -55},
         {-65, -70, -57, -6, 26, -22, -58, -59},
         {-61, -67, -60, -24, -2, -40, -60, -58},
         {-49, -63, -68, -58, -51, -60, -70, -53},
         {-43, -57, -64, -69, -73, -67, -63, -45},
         {-41, -49, -59, -60, -63, -52, -50, -34}};

    double **op;
    op = DTC(matrix, 8);
    return 0;
}

void test()
{

    int height = 0;
    int width = 0;
    int chanels = 0;

    unsigned char *image = stbi_load("../Images/PillarsOfCreation(1).bmp", &width, &height, &chanels, 3);

    writeYComponent(image, width, height, chanels);
    writeCbComponent(image, width, height, chanels);
    writeCrComponent(image, width, height, chanels);

    printf("%d\n", width);

    
    
        int Y = 0;
        int Cb = 0;
        int Cr = 0;
        int pixel = 0;
        unsigned char *out = malloc((width * height * chanels) * sizeof(unsigned char));
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                pixel = (y * width + x) * chanels;
                unsigned char red = image[pixel];
                unsigned char green = image[pixel + 1];
                unsigned char blue = image[pixel + 2];

                Y = 0.299 * red + 0.587 * green + 0.114 * blue;
                Cb = 128 - (0.168746 * red) - (0.331264 * green) + (0.5 * blue);
                Cr = 128 + (0.5 * red) - (0.418688 * green) - (0.081312 * blue);
                int out_pixel = (y * width + x) * chanels;
                out[out_pixel] = Y;
                out[out_pixel + 1] = Cb;
                out[out_pixel + 2] = Cr;
            }
        }
        const char *filename = "output1.bmp";
        stbi_write_bmp(filename, width, height, chanels, out);
        
}

double **DTC(double (*mat)[8], int n)
{
    int x = 0;
    double **outMat = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        outMat[i] = malloc(8 * sizeof(double));
    }

    double Cu, Cv = 0.0;
    double sum = 0.0;

    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            sum = 0;
            for (x = 0; x < n; x++)
            {

                for (int y = 0; y < n; y++)
                {
                    sum += mat[x][y] * cos(((2.0 * x + 1) * u * PI) / 16.0) *
                           cos(((2.0 * y + 1) * v * PI) / 16.0);
                }

                if (u == 0)
                    Cu = 1 / sqrt(2);
                else
                    Cu = 1;
                if (v == 0)
                    Cv = 1 / sqrt(2);
                else
                    Cv = 1;
            }
            outMat[u][v] = 1 / 4.0 * Cu * Cv * sum;
            printf("%8.1f ", outMat[u][v]);
        }
        printf("\n");
    }
    printf("\n");
    return outMat;
}
