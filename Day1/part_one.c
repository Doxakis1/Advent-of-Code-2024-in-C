#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int compare_func(const void *a,const void *b)
{
	return (*(int *)a > *(int *)b);
}

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

int main(void){
	uint64_t	answer = 0;
	int64_t		columnOne[1024] = {0};
	int64_t		columnTwo[1024] = {0};
	int64_t		bufferOne;
	int64_t		bufferTwo;
	char		lineBuffer[1024];
	size_t		index = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	while (simple_get_line(fd, lineBuffer) != -1)
	{
		sscanf(lineBuffer, "%ld %ld\n", &bufferOne, &bufferTwo);
		columnOne[index] = bufferOne;
		columnTwo[index++] = bufferTwo;
	}
	qsort(columnOne, index, sizeof(int64_t), compare_func);
	qsort(columnTwo, index, sizeof(int64_t), compare_func);
	for (size_t i = 0; i < index; i++)
	{
		answer += llabs(columnOne[i] - columnTwo[i]);
	}
	printf("The answer is %lld\n", answer);
	close(fd);
	return 0;
}
