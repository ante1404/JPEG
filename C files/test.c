#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb-master/stb_image_write.h "

int i = -1;
int size = 0;

void calculateCbValue(const char *inputImagePath, const char *outputImagePath, int x, int y)
{
    int width, height, channels;
    unsigned char *image = stbi_load(inputImagePath, &width, &height, &channels, 0);

    if (image == NULL)
    {
        printf("Error loading the image.\n");
        return;
    }

    if (channels != 3)
    {
        printf("Input image is not in RGB format.\n");
        stbi_image_free(image);
        return;
    }

    // Convert RGB image to YCbCr color space
    unsigned char *ycbcrImage = malloc(width * height * channels);
    for (int i = 0; i < width * height; i++)
    {
        unsigned char r = image[i * 3];
        unsigned char g = image[i * 3 + 1];
        unsigned char b = image[i * 3 + 2];

        ycbcrImage[i * 3] = 0.299 * r + 0.587 * g + 0.114 * b;               // Y
        ycbcrImage[i * 3 + 1] = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b; // Cb
        ycbcrImage[i * 3 + 2] = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b; // Cr
    }

    // Access and print the Cb value of the specified pixel
    int pixelIndex = y * width + x;
    unsigned char cbValue = ycbcrImage[pixelIndex * 3 + 1];
    printf("Cb value at pixel (%d, %d): %d\n", x, y, cbValue);

    // Save the Cb channel as a BMP image
    unsigned char *cbChannel = malloc(width * height);
    for (int i = 0; i < width * height; i++)
    {
        cbChannel[i] = ycbcrImage[i * 3 + 1];
    }

    stbi_write_bmp(outputImagePath, width, height, 1, cbChannel);

    // Free memory
    stbi_image_free(image);
    free(ycbcrImage);
    free(cbChannel);
}



void push(int value, int arr[]){

    arr[++i] = value;

}

int pop(int arr[]){

    if (i < 0)
    {
        perror("Stack is empty\n");
        return 1;
    }

    int retunrVal = arr[i--];

    return retunrVal;
}


int *createArray(int capacity){

    int *arr = (int *)calloc(capacity, sizeof(int));
    return arr;
}

int insertElement(int *arr, int element, int capacity){

    
    if (capacity == size)
    {
        capacity += capacity + 10;
        arr = realloc(arr, 10);
        arr[size++] = element;
        return capacity;
    }
    arr[size++] = element;

    return capacity;
}

int main()
{
    int capacity = 2;

    int *arr = createArray(5);

    capacity = insertElement(arr, 1, capacity);
    capacity = insertElement(arr, 2, capacity);
    capacity = insertElement(arr, 3, capacity);
    capacity = insertElement(arr, 4, capacity);
    capacity = insertElement(arr, 5, capacity);
    capacity = insertElement(arr, 6, capacity);

    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int n = 0xF7;
    int u = 0;
    int m = (n >> 4) & 0x0F;
    u = n & 0x0F;
    printf("upper -> %d\n", m);
    printf("lower -> %d\n", u);
    return 0;
}