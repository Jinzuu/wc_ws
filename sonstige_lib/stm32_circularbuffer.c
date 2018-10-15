#include "Stm32_CircularBuffer.h"
#include <stm32_basicdefines.h>

void CircularBuffer_Init( Stm32_CircularBuffer* buffer, int size, char* preallocatedMemoryForBuf ){
	buffer->size = size;
	buffer->nextWriteIdx = 0;
	buffer->nextReadIdx = 0;
	buffer->address = preallocatedMemoryForBuf;
}

// Circular buffer functions
// Note: For code simplicity, only storing size-1 objects is possible
int CircularBuffer_Store( Stm32_CircularBuffer* buffer, char* sourceByteArray, int nBytesToWrite ){
	if ( buffer->size < nBytesToWrite + CircularBuffer_GetPopulation( buffer ) )
		return RESULT_ERROR;

	// Store bytes one after another
	int i;
	for ( i=0; i<nBytesToWrite; ++i ){
		buffer->address[buffer->nextWriteIdx] = sourceByteArray[i];
		buffer->nextWriteIdx = ( buffer->nextWriteIdx + 1 ) % buffer->size;
	}

	return RESULT_SUCCESS;
}

int CircularBuffer_Read( Stm32_CircularBuffer* buffer, char* targetByteArray, int nBytesToRead ){
	if ( nBytesToRead > CircularBuffer_GetPopulation( buffer ) )
		return RESULT_ERROR;

	// Read bytes one after another
	int i;
	for ( i=0; i<nBytesToRead; ++i ){
		targetByteArray[i] = buffer->address[buffer->nextReadIdx];
		buffer->nextReadIdx = ( buffer->nextReadIdx + 1 ) % buffer->size;
	}

	return RESULT_SUCCESS;
}

// Returns the number of stored bytes within the buffer
int CircularBuffer_GetPopulation( Stm32_CircularBuffer* buffer ){
	// Case write idx > read idx (no wrap around)
	int population = buffer->nextWriteIdx - buffer->nextReadIdx;
	if ( population < 0 )
		population += buffer->size;
	return population;
}

// Flushes/Deletes the content of a circular buffer
int CircularBuffer_Flush( Stm32_CircularBuffer* buffer ){
	buffer->nextWriteIdx = 0;
	buffer->nextReadIdx = 0;
	return RESULT_SUCCESS;
}

int CircularBuffer_SearchString( Stm32_CircularBuffer* buffer, const char* tofind)
{
	int len_to_search = strlen(tofind);
	int current_buffer_length = CircularBuffer_GetPopulation(buffer);

	if( tofind == 0 ) return -1;

	int Idx_to_read_from = buffer->nextReadIdx;
	int current_index_to_compare = 0;
	for( int i=0; i<current_buffer_length; ++i ) {

		if( buffer->address[Idx_to_read_from] == tofind[current_index_to_compare] ) {

			if( current_index_to_compare == (len_to_search - 1) ) {
				// if we get here, then the search string has been found!
				return i+1;
			}
			else if( current_index_to_compare < len_to_search )
			current_index_to_compare += 1;
		}
		else {
			// if we have found another start of the search string, start over
			if( buffer->address[Idx_to_read_from] == tofind[0] ) {
				current_index_to_compare = 1;
			}
		}

		// Increment to next Buffer element
		Idx_to_read_from = ( Idx_to_read_from + 1 ) % buffer->size;
	}

	// if we get here, then the search string has NOT been found
	return -1;
}

