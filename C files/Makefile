CC=gcc
CFLAGS += -O3 -g3 -g -W -Wall -Wextra -Wuninitialized -Wstrict-aliasing -std=c18
# Libmath
LDFALGS = -lm


SOURCES=Huffman.o SaveHm.o HashTable.o uneque.o hf.o idk2.o Decompress.o DCT.o
OBJECTS=$(SOURCES:.c=.o=.exe)
EXECUTABLE=read write test dc DCT

hp: hf1
hf1: Huffman.o SaveHm.o HashTable.o uneque.o
	$(CC) $(CFLAGS) -o read huffman.o SaveHm.o HashTable.o uneque.o

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

SaveHm.o: SaveHm.c
	$(CC) $(CFLAGS) -c SaveHm.c

HashTable.o: HashTable.c
	$(CC) $(CFLAGS) -c HashTable.c

uneque.o: uneque.c
	$(CC) $(CFLAGS) -c uneque.c


hm: hf 
hf: hf.o SaveHm.o HashTable.o
	$(CC) $(CFLAGS) -o write hf.o SaveHm.o HashTable.o

hf.o: hf.c
	$(CC) $(CFLAGS) -c hf.c


DC: SaveHm.o HashTable.o Huffman.o Decompress.o
	$(CC) $(CFLAGS) -o dc SaveHm.o HashTable.o Huffman.o Decompress.o

Decompress.o: Decompress.c
	$(CC) $(CFLAGS) -c Decompress.c


DCT:dct
dct:DCT.o
	$(CC) $(CFLAGS) -o DCT DCT.o $(LDFALGS)

DCT.o: DCT.c
	$(CC) $(CFLAGS) -c DCT.c $(LDFALGS)


JPG: jpg
jpg:JpegDecoder.o
	$(CC) $(CFLAGS) -o jpg JpegDecoder.o $(LDFALGS)
jpg.o: JpegDecoder.o
	$(CC) $(CFLAGS) -c JpegDecoder.c $(LDFALGS)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

