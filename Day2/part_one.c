#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

size_t simple_get_line(int fd, char *buffer){
	size_t	index = 0;
	while (1)
	{
		if (read(fd, &buffer[index++], 1) <= 0)
			return -1;
		if (buffer[index - 1] == '\n')
			break ;
	}
	buffer[index] = '\0';
	return index;
}

int	check_safe_line(int64_t *array, size_t arraySize)
{
	int direction = 0;
	for (size_t i = 1; i < arraySize; i++)
	{
		if (llabs(array[i -1] - array[i]) > 3 || llabs(array[i -1] - array[i]) == 0)
			return 0;
		if (direction == 0)
			direction = array[i] - array[i - 1];
		else
		{
			if (direction < 0 && array[i] > array[i - 1])
				return 0;
			if (direction > 0 && array[i] < array[i-1])
				return 0;
		}
	}
	return 1;

}

int main(void){
	uint64_t	answer = 0;
	int64_t		rowNumber[12] = {0};
	int64_t		bufferOne;
	char		lineBuffer[1024];
	size_t		index = 0;
	size_t		numIndex = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	while (simple_get_line(fd, lineBuffer) != -1)
	{
		numIndex = 0;
		index = 0;
		while(1)
		{
			sscanf(&lineBuffer[index], "%lld", &bufferOne);
			rowNumber[numIndex++] = bufferOne;
			while(lineBuffer[index] != ' ' && lineBuffer[index] != '\n')
				index++;
			if (lineBuffer[index] == '\n')
				break ;
			index++;
		}
		answer += check_safe_line(rowNumber, numIndex);

	}
	printf("The answer is %lld\n", answer);
	close(fd);
	return 0;
}
