#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct s_cachedNode
{
	uint64_t	num;
	size_t		depth;
	uint64_t	answer;
} cachedNodes;

cachedNodes myCache[3000000];
size_t cachedNodesIndex = 0;

void addCached(uint64_t num, size_t depth, uint64_t answer)
{

	if (cachedNodesIndex >= 3000000)
		return ;
	myCache[cachedNodesIndex].num = num;
	myCache[cachedNodesIndex].depth = depth;
	myCache[cachedNodesIndex].answer = answer;
	cachedNodesIndex++;
}

size_t maxDepth = 75;

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

ssize_t find_in_cache(uint64_t num, size_t depth)
{
	for(size_t i = 0; i < cachedNodesIndex; i++)
	{
		if (myCache[i].num == num && myCache[i].depth == depth)
			return myCache[i].answer;
	}
	return -1;
}

uint64_t calculate_children_of(uint64_t num, size_t depth)
{
	size_t answer = find_in_cache(num, depth);
	if (answer != -1)
		return answer;
	if (depth == maxDepth)
		return 0;
	if (num == 0)
	{
		answer = calculate_children_of(1, depth + 1);
		addCached(num, depth, answer);
		return answer;
	}
	size_t numDigits = find_num_digits(num);
	if (numDigits % 2 == 0)
	{
		answer = 1 + calculate_children_of(upper(num, numDigits / 2), depth + 1) + calculate_children_of(lower(num, numDigits / 2), depth + 1);
		addCached(num, depth, answer);
		return answer;
	}
	answer = calculate_children_of(num * 2024, depth + 1);
	addCached(num, depth, answer);
	return answer;
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
