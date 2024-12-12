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
			break ;
		if (buffer[index - 1] == '\n')
			break ;
	}
	buffer[index] = '\0';
	return index;
}

int main(void){
	char		readBuffer[30000] = {0};
	uint64_t	constractedBuffer[300000] = {0};
	uint64_t		answer = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;

	size_t lineSize = simple_get_line(fd, readBuffer) - 1;
	uint64_t blockID = 1;
	size_t constractedBufferIndex = 0;
	for(size_t i = 0; i < lineSize; i++)
	{
		if (i % 2 == 0)
		{
			for (size_t x = 0; x < (int)readBuffer[i] - '0'; x++)
				constractedBuffer[constractedBufferIndex++] = blockID;
		blockID++;
		}
		else
		{
			for (size_t x = 0; x < readBuffer[i] - '0'; x++)
				constractedBuffer[constractedBufferIndex++] = 0;
		}
	}
	size_t end = constractedBufferIndex - 1;
	for(size_t y = 0; y < constractedBufferIndex; y++)
	{
		if (constractedBuffer[y] == 0)
		{
			while(end > y && constractedBuffer[end] == 0)
				end--;
			constractedBuffer[y] = constractedBuffer[end];
			constractedBuffer[end] = 0;

		}
	}
	size_t index = 0;
	while(constractedBuffer[index] != 0)
	{
		answer += index * (constractedBuffer[index] - 1);
		index++;
	}
	printf("The answer is: %llu", answer);
	return close(fd);
}
