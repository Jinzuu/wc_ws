#ifndef __Stm32_CircularBuffer_H
#define __Stm32_CircularBuffer_H

typedef struct{
	int size;			// In bytes
	int nextWriteIdx;	// Index where next insert will occur
	int nextReadIdx;	// Index where next read will occur
	char* address;		// Pointer to the memory where the buffer is allocated
} Stm32_CircularBuffer;

// ##### Circular buffer functions #####
// Initializes the circular buffer. Used needs to provide a preallocated memory of sufficient size for the buffer
void CircularBuffer_Init( Stm32_CircularBuffer* buffer, int size, char* preallocatedMemoryForBuf );
// Stores data in the buffer. Returns an error if buffer is too full to store the data in the buffer (does not store any data then)
int CircularBuffer_Store( Stm32_CircularBuffer* buffer, char* sourceByteArray, int nBytesToStore );
// Reads and deletes data from the buffer. Returns an error if not enough data in the buffer (does not read/delete any data then)
int CircularBuffer_Read( Stm32_CircularBuffer* buffer, char* targetByteArray, int nBytesToRead );

// Returns the number of stored bytes within the buffer
int CircularBuffer_GetPopulation( Stm32_CircularBuffer* buffer );
// Flushes/Deletes the content of a circular buffer
int CircularBuffer_Flush( Stm32_CircularBuffer* buffer );

int CircularBuffer_SearchString( Stm32_CircularBuffer* buffer, const char* tofind);


#endif
