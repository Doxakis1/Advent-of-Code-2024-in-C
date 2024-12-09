#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define ORIGIN 1 << 4

typedef struct s_tileDetails
{
	size_t pos;
	uint8_t	flags;
} tileDetails;
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

size_t check_loop(size_t position, size_t lineSize, char *matrix)
{
	static const char *guardMoves = "^>v<";
	char	visitedTiles[300000] = {0};
	int64_t guardDisplacement[4] = {-lineSize, 1, lineSize, -1};
	int8_t currentDirection = 0;
	while (matrix[position] != guardMoves[currentDirection])
		currentDirection++;
	visitedTiles[position] |= currentDirection;
	while(1)
	{
		matrix[position] = 'X';
		size_t newPosition = position + guardDisplacement[currentDirection];
		if (matrix[newPosition] == '0')
			return 0;
		if (matrix[newPosition] == 'X')
		{
			position = newPosition;
			if (visitedTiles[newPosition] & (1 << currentDirection))
				return 1;
			visitedTiles[newPosition] |= (1 << currentDirection);
			continue;
		}
		if (matrix[newPosition] == '.')
		{
			position = newPosition;
			visitedTiles[newPosition] |= (1 << currentDirection);
			continue;
		}
		if (matrix[newPosition] == '#' || matrix[newPosition] == 'O')
		{
			currentDirection++;
			currentDirection %= 4;
			continue;
		}
	}
}

size_t solve_path(size_t position, size_t lineSize, char *matrix)
{
	static const char *guardMoves = "^>v<";
	int64_t guardDisplacement[4] = {-lineSize, 1, lineSize, -1};
	int8_t currentDirection = 0;
	size_t	answer = 1;
	while (matrix[position] != guardMoves[currentDirection])
		currentDirection++;
	while(1)
	{
		matrix[position] = 'X';
		size_t newPosition = position + guardDisplacement[currentDirection];
		if (matrix[newPosition] == '0')
			return answer;
		if (matrix[newPosition] == 'X')
		{
			position = newPosition;
			continue;
		}
		if (matrix[newPosition] == '.')
		{
			position = newPosition;
			answer++;
			continue;
		}
		if (matrix[newPosition] == '#')
		{
			currentDirection++;
			currentDirection %= 4;
			continue;
		}
	}
}

int main(void){
	char		buffer[1024] = {0};
	char		matrix[300000] = {0};
	int64_t		answer = 0;
	size_t		lineSize;
	size_t		matrixIndex = 0;
	size_t		originalStartingPosition = 0;
	char		originalDirection = 0;
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
		if (matrix[i] == '^' || matrix[i] == '>' || matrix[i] == 'v' || matrix[i] == '<')
		{
			originalDirection = matrix[i];
			originalStartingPosition = i;
			answer += solve_path(i,lineSizeHolder + 9, matrix);
			break ;
		}
	}
	tileDetails visitedTiles[answer];
	size_t	tileIndex = 0;
	for (size_t i = 0; i < matrixIndex; i++)
	{
		if (matrix[i] == 'X')
		{
			visitedTiles[tileIndex].pos = i;
			visitedTiles[tileIndex].flags = 0;
			if (i  == originalStartingPosition)
				visitedTiles[tileIndex].flags |= ORIGIN;
			tileIndex++;
			matrix[i] = '.';
		}
	}
	answer = 0;
	for (size_t y = 0; y < tileIndex; y++)
	{
		if ( visitedTiles[y].pos == originalStartingPosition)
			continue;
		matrix[visitedTiles[y].pos] = 'O';
		matrix[originalStartingPosition] = originalDirection;
		answer += check_loop(originalStartingPosition, lineSizeHolder + 9, matrix);
		matrix[visitedTiles[y].pos] = '.';
		for(size_t z = 0; z < matrixIndex; z++)
			if (matrix[z] == 'X')
				matrix[z] = '.';
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
