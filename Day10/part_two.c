#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define HAS_NODE 1


typedef struct s_tile
{
	int hasVisited;
} tile;

tile	visitedTiles[1024 * 1024] = {0};

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
size_t possible_paths_from(size_t y, size_t x, size_t yMax, size_t xMax, char (*matrix)[1024])
{
	int currentNumber = matrix[y][x] - '0';
	if (currentNumber == 9)
	{
		return 1;
	}
	size_t answer = 0;
	//going up
	if (y > 0 && matrix[y-1][x] - '0' == currentNumber + 1)
		answer += possible_paths_from(y - 1, x, yMax, xMax, matrix);
	//going down
	if (y < yMax - 1 && matrix[y+1][x] - '0' == currentNumber + 1)
		answer += possible_paths_from(y + 1, x, yMax, xMax, matrix);
	//going left
	if (x > 0 && matrix[y][x-1] - '0' == currentNumber + 1)
		answer += possible_paths_from(y, x - 1, yMax, xMax, matrix);
	//going right
	if (x < xMax - 1 && matrix[y][x+1] - '0' == currentNumber + 1)
		answer += possible_paths_from(y, x + 1, yMax, xMax, matrix);
	return answer;
}

int main(void){
	char		matrix[1024][1024] = {0};
	uint64_t	answer = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	size_t y = 0;
	const size_t xMax = simple_get_line(fd, matrix[y++]) - 1;
	while (simple_get_line(fd, matrix[y++]) != -1)
		;
	const size_t yMax = y - 1;
	for (size_t y = 0; y < yMax; y++)
	{
		for (size_t x = 0; x < xMax; x++)
		{
			
			if (matrix[y][x] == '0')
			{
				for (size_t z = 0; z < 1024 * 1024; z++)
					visitedTiles[z].hasVisited = 0;
				answer += possible_paths_from(y, x, yMax, xMax, matrix);		
			}
		}
	}
	printf("The answer is: %llu", answer);
	return close(fd);
}
