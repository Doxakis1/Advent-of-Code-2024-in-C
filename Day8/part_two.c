#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define HAS_NODE 1

typedef struct	s_pos{
	size_t x;
	size_t y;
} pos;

typedef struct s_tile
{
	uint8_t hasNode;
} tile;

int64_t find_common_divisor(int64_t a, int64_t b)
{
	if (b == 0)
		return llabs(a);
	return find_common_divisor(llabs(b), llabs(a) % llabs(b));
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

void mark_unique(tile (*uniqueTiles)[1024], size_t y, size_t x, size_t startY, size_t startX, size_t yMax, size_t xMax)
{
	int64_t differenceY;
	int64_t differenceX;
	int64_t currentY = startY;
	int64_t currentX = startX;
	uniqueTiles[startY][startX].hasNode = HAS_NODE;
	differenceY = y - startY;
	if ( x < startX)
	{
		differenceX = startX - x;
		differenceX *= -1;
	}
	else
		differenceX = x - startX;
	int64_t commonDivisor  = find_common_divisor(differenceX, differenceY);
	differenceY /= commonDivisor;
	differenceX /= commonDivisor;
	//mark before:
	int64_t tempNodeY = currentY - differenceY;
	int64_t tempNodeX = currentX - differenceX;
	while (1)
	{
		if (tempNodeY >= 0 && tempNodeY < yMax && tempNodeX >= 0 && tempNodeX < xMax)
			uniqueTiles[tempNodeY][tempNodeX].hasNode = HAS_NODE;
		else
			break ;
		tempNodeY -= differenceY;
		tempNodeX -= differenceX;
	}
	//mark after
	tempNodeY = currentY + differenceY;
	tempNodeX = currentX + differenceX;
	while (1)
	{
		if (tempNodeY >= 0 && tempNodeY < yMax && tempNodeX >= 0 && tempNodeX < xMax)
			uniqueTiles[tempNodeY][tempNodeX].hasNode = HAS_NODE;
		else
			break ;
		tempNodeY += differenceY;
		tempNodeX += differenceX;
	}
	return ;
}

void find_possible_nodes(char (*matrix)[1024], size_t startY, size_t startX, char antennaType, size_t yMax, size_t xMax, tile (*uniqueTiles)[1024])
{
	size_t x = 0;
	for(size_t y = startY; y < yMax; y++)
	{
		if (y == startY)
			x = startX + 1;
		else
			x = 0;
		while (x < xMax)
		{
			if ( matrix[y][x] == antennaType)
				mark_unique(uniqueTiles, y, x, startY, startX, yMax, xMax);
			
			x++;
		}
	}
}

int main(void){
	char		matrix[1024][1024];
	uint64_t	answer = 0;
	tile		uniqueTiles[1024][1024] = {0};
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
			if (matrix[y][x] != '.')
				find_possible_nodes(matrix, y, x, matrix[y][x], yMax, xMax, uniqueTiles);
		}
	}
	for (size_t i = 0; i < yMax; i++)
		for (size_t z = 0; z < xMax; z++)
			answer += uniqueTiles[i][z].hasNode;
	printf("The answer is: %llu", answer);
	return close(fd);
}
