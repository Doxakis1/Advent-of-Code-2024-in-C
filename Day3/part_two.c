#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char		buffer[300000] = {0};
	int64_t		answer = 0, numOne, numTwo, skipChars, toDoOrNotToDo = 1;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0 || read(fd, buffer, 300000) <= 0)
		return 1;
	for (size_t i = 0; buffer[i]; i++)
	{
		if (sscanf(&buffer[i], "mul(%ld,%ld%n",&numOne,&numTwo, &skipChars) == 2 && buffer[i+skipChars] == ')')
			answer += (numOne * numTwo * toDoOrNotToDo);
		if (strncmp(&buffer[i], "do()",4) == 0)
			toDoOrNotToDo = 1;
		if (strncmp(&buffer[i], "don't()",7) == 0)
			toDoOrNotToDo = 0;
	}
	printf("The answer is %lld\n", answer);
	return close(fd);
}
