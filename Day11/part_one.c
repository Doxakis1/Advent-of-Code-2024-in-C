#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


size_t maxDepth = 25;

size_t find_num_digits(uint64_t num)
{
	size_t answer = 0;
	while (num)
	{
		answer++;
		num /= 10;
	}
	return answer;
}

uint64_t upper(uint64_t num, size_t digits)
{
	uint64_t divisor = 1;
	while (digits)
	{
		divisor *= 10;
		digits--;
	}
	return num/divisor;
}

uint64_t lower(uint64_t num, size_t digits)
{
	uint64_t divisor = 1;
	while (digits)
	{
		divisor *= 10;
		digits--;
	}
	return num%divisor;
}

uint64_t calculate_children_of(uint64_t num, size_t depth)
{
	if (depth == maxDepth)
		return 0;
	if (num == 0)
		return calculate_children_of(1, depth + 1);
	size_t numDigits = find_num_digits(num);
	if (numDigits % 2 == 0)
		return 1 + calculate_children_of(upper(num, numDigits / 2), depth + 1) + calculate_children_of(lower(num, numDigits / 2), depth + 1);
	return calculate_children_of(num * 2024, depth + 1);
}

size_t simple_get_line(int fd, char *buffer){
	size_t index = 0;
	while(1)
	{
		if (read(fd, &buffer[index++],1) <= 0)
			return index;
		if (buffer[index - 1] == '\n')
			break ;
	}
	buffer[index] = '\0';
	return index;
}

int main(void){
	char		buffer[1024] = {0};
	uint64_t	answer = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	size_t lineSize = simple_get_line(fd, buffer);
	size_t index = 0;
	while (buffer[index])
	{
		answer += 1 + calculate_children_of(atoll(&buffer[index]), 0);
		while(buffer[index] && buffer[index] != ' ')
			index++;
		index++;
	}
	printf("The answer is: %llu", answer);
	return close(fd);
}
