#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

size_t simple_get_line(int fd, char *buffer){
	size_t index = 0;
	while(1)
	{
		if (read(fd, &buffer[index++],1) <= 0)
			return -1;
		if (buffer[index - 1] == '\n')
			break ;
	}
	buffer[index] = '\0';
	return index;
}

int check_XMAS(size_t index, size_t lineSize, char *matrix)
{
	static uint32_t correctMatch = 'X' << 24 | 'M' << 16 | 'A' << 8 | 'S';
	uint32_t forwardHorizontal;
	uint32_t forwardVertical;
	uint32_t forwardDiagonal;
	uint32_t forwardDiagonalBack;

	uint32_t backwordHorizontal;
	uint32_t backwardVertical;
	uint32_t backwardDiagonal;
	uint32_t backwardDiagonalBack;
	
	backwardDiagonalBack = matrix[index] << 24 | matrix[index-lineSize-1] << 16 | matrix[index-lineSize*2-2] << 8| matrix[index-lineSize*3-3];
	
	backwardDiagonal = matrix[index] << 24 | matrix[index-lineSize+1] << 16 | matrix[index-lineSize*2+2] << 8| matrix[index-lineSize*3+3];

	backwardVertical = matrix[index] << 24 | matrix[index-1] << 16 | matrix[index-2] << 8| matrix[index-3];

	backwordHorizontal = matrix[index] << 24 | matrix[index-lineSize] << 16 | matrix[index-lineSize*2] << 8| matrix[index-lineSize*3];

	forwardHorizontal = matrix[index] << 24 | matrix[index+lineSize] << 16 | matrix[index+lineSize*2] << 8| matrix[index+lineSize*3];

	forwardVertical = matrix[index] << 24 | matrix[index+1] << 16 | matrix[index+2] << 8| matrix[index+3];

	forwardDiagonal = matrix[index] << 24 | matrix[index+lineSize+1] << 16 | matrix[index+lineSize*2+2] << 8| matrix[index+lineSize*3+3];

	forwardDiagonalBack = matrix[index] << 24 | matrix[index+lineSize-1] << 16 | matrix[index+lineSize*2-2] << 8| matrix[index+lineSize*3-3];

//	if(forwardDiagonalBack == correctMatch)
//		printf("index %ld\n", index);
//
	return (backwardDiagonalBack == correctMatch) + (backwardDiagonal == correctMatch) + (backwardVertical == correctMatch) + (forwardHorizontal == correctMatch) + (forwardVertical == correctMatch) + (forwardDiagonal == correctMatch) + (forwardDiagonalBack == correctMatch) + (backwordHorizontal == correctMatch);
}

int main(void){
	char		buffer[1024] = {0};
	char		matrix[300000] = {0};
	int64_t		answer = 0;
	size_t		lineSize;
	size_t		matrixIndex = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	const size_t lineSizeHolder = simple_get_line(fd, buffer);
	lineSize = lineSizeHolder;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t x = 0; x <= lineSizeHolder + 8; x++)
			matrix[matrixIndex++] = '0';
//		matrix[matrixIndex++] = '\n';
	}
	while(lineSize != -1)
	{
		for (size_t i = 0; i < 5; i++)
			matrix[matrixIndex++] = '0';
		for (size_t x = 0; x < lineSize - 1; x++)
			matrix[matrixIndex++] = buffer[x];
		for (size_t i = 0; i < 5; i++)
			matrix[matrixIndex++] = '0';
//		matrix[matrixIndex++] = '\n';
		lineSize = simple_get_line(fd, buffer);
	}
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t x = 0; x <= lineSizeHolder + 8; x++)
			matrix[matrixIndex++] = '0';
//		matrix[matrixIndex++] = '\n';
	}
	for ( size_t i = 0; i < matrixIndex; i++)
	{
		if (matrix[i] == 'X')
			answer += check_XMAS(i,lineSizeHolder + 9, matrix);
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
