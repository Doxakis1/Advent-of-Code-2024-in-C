#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){
	uint64_t	answer;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;
	close(fd);
	return 0;
}
