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
	static uint32_t correctMatch = 'M' << 16 | 'A' << 8 | 'S';
	char top[2];
	char bot[2];
	top[0] = matrix[index-lineSize-1];
	top[1] = matrix[index-lineSize+1];
	bot[0] = matrix[index+lineSize-1];
	bot[1] = matrix[index+lineSize+1];

	return (top[0] + bot[1] == 'M' + 'S') && (top[1] + bot[0] == 'M' + 'S');
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
		if (matrix[i] == 'A')
			answer += check_XMAS(i,lineSizeHolder + 9, matrix);
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
