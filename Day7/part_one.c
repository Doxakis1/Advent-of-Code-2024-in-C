#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t ascii_to_uint64(const char *str) {
	uint64_t result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return result;
}

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
//does not handle 0 case
uint64_t	power_two(uint64_t num)
{
	uint64_t answer = 2;
	for (uint64_t i = 1; i < num; i++)
		answer *= 2;
	return answer;
}

int64_t evaluate_answer(uint64_t *array, uint64_t combination, size_t arraySize)
{
	uint64_t answer = array[0];
	array++;
	arraySize--;
	for (size_t i = 0; i < arraySize; i++)
	{
		if (combination % 2)
			answer += *array;
		else
			answer *= *array;
		combination /= 2;
		array++;
	}
	return answer;
}

int64_t validate_line(char *buffer)
{
	uint64_t	array[1024];
	uint64_t desiredAnswer = ascii_to_uint64(buffer);
	size_t arrayIndex = 0;
	size_t loopCounter = 0;
	while (*buffer != ' ')
		buffer++;
	buffer++;
	while(*buffer)
	{
		array[arrayIndex++] = ascii_to_uint64(buffer);
		while (*buffer != ' ' && *buffer != '\n')
			buffer++;
		buffer++;
	}
	uint64_t	combination = power_two(arrayIndex - 1);
	while(loopCounter <= combination)
	{
		if (evaluate_answer(array, loopCounter, arrayIndex) == desiredAnswer)
		{
			return desiredAnswer;
		}
		loopCounter++;
	}
	return 0;
}

int main(void){
	char		buffer[1024] = {0};
	uint64_t		answer = 0;
	size_t		numIndex = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;

	size_t lineSize = simple_get_line(fd, buffer);
	while (lineSize != -1)
	{
		answer += validate_line(buffer);
		lineSize = simple_get_line(fd, buffer);
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
