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
			answer += solve_path(i,lineSizeHolder + 9, matrix);
			break ;
		}
	}
	printf("The answer is: %lld", answer);
	return close(fd);
}
