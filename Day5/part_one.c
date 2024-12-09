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

int validate_line(int64_t *numsLeft, int64_t *numsRight, char *buffer, size_t arraySize)
{
	int64_t	array[1024];
	size_t arrayIndex = 0;
	size_t loopCounter = 0;
	while(*buffer)
	{
		array[arrayIndex++] = atoll(buffer);
		while (*buffer != ',' && *buffer != '\n')
			buffer++;
		buffer++;
	}

	while ( loopCounter < arrayIndex)
	{
		for ( size_t rhs = 0; rhs < arraySize; rhs++)
			if (array[loopCounter] == numsRight[rhs])
				for(size_t x = loopCounter; x < arrayIndex; x++)
					if (array[x] == numsLeft[rhs])
						return 0;
		loopCounter++;
	}
	return array[arrayIndex/2];
}

int main(void){
	char		buffer[1024] = {0};
	int64_t		numsLeft[3000];
	int64_t		numsRight[3000];
	int64_t		answer = 0;
	size_t		numIndex = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;

	size_t lineSize = simple_get_line(fd, buffer);
	while (lineSize > 1)
	{
		sscanf(buffer, "%ld|%ld", &numsLeft[numIndex], &numsRight[numIndex]);
		numIndex++;
		lineSize = simple_get_line(fd, buffer);
	}
	lineSize = simple_get_line(fd, buffer);
	while(lineSize != -1)
	{
		answer += validate_line(numsLeft, numsRight, buffer, numIndex);
		lineSize = simple_get_line(fd, buffer);
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
