#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_node
{
	uint64_t	nodeID;
	int		nodeSize;
	_Bool		hasBeenTested;
	struct s_node	*next;
} node;

size_t simple_get_line(int fd, char *buffer){
	size_t index = 0;
	while(1)
	{
		if (read(fd, &buffer[index++],1) <= 0)
			break ;
		if (buffer[index - 1] == '\n')
			break ;
	}
	buffer[index] = '\0';
	return index;
}

node *find_previous(node *head, node *desiredNode)
{
	node *tmp = head;
	while (tmp && tmp->next != desiredNode)
		tmp = tmp->next;
	return tmp;
}

node *find_next_not_tested(node *head)
{
	node *previous = NULL;
	node *tmp = head;
	while (tmp != NULL)
	{
		if (tmp->nodeID != 0 && tmp->hasBeenTested == false)
			previous = tmp;
		tmp = tmp->next;
	}
	return previous;
}


node *find_next_fitting_place(node *head, node *testingNode)
{
	uint8_t	desiredSize = testingNode->nodeSize;
	node *tmp = head;
	while (tmp && tmp != testingNode)
	{
		if (tmp->nodeID == 0 && tmp->nodeSize >= desiredSize)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


int main(void){
	char		readBuffer[30000] = {0};
	node		nodeLList[80000] = {0};
	uint64_t	constractedBuffer[300000] = {0};
	uint64_t	answer = 0;
	int fd = open("input.txt", O_RDONLY);
	if (fd < 0)
		return 1;

	size_t lineSize = simple_get_line(fd, readBuffer) - 1;
	uint64_t blockID = 1;
	size_t nodeIndex = 0;
	size_t constractedBufferIndex = 0;
	for(size_t i = 0; i < lineSize; i++)
	{
		if (i % 2 == 0)
		{
			nodeLList[nodeIndex].nodeID = blockID;
			nodeLList[nodeIndex].nodeSize = readBuffer[i] - '0';
			nodeLList[nodeIndex].next = &nodeLList[nodeIndex + 1];
			nodeLList[nodeIndex].hasBeenTested = false;
			blockID++;
		}
		else
		{
			nodeLList[nodeIndex].nodeID = 0;
			nodeLList[nodeIndex].nodeSize = readBuffer[i] - '0';
			nodeLList[nodeIndex].next = &nodeLList[nodeIndex + 1];
		}
		nodeIndex++;
	}
	nodeLList[nodeIndex - 1].next = NULL;
	node *head = &nodeLList[0];
	node *tmp = head;
	head->hasBeenTested = true;
	node *nextTest = find_next_not_tested(head);
	node *fittingPlace = NULL;
	while (nextTest)
	{
		fittingPlace = find_next_fitting_place(head, nextTest);
		if (fittingPlace)
		{
			nodeLList[nodeIndex].nodeID = 0;
			nodeLList[nodeIndex].nodeSize = nextTest->nodeSize;
			nodeLList[nodeIndex].next = nextTest->next;
			find_previous(head, nextTest)->next = &nodeLList[nodeIndex];
			find_previous(head, fittingPlace)->next = nextTest;
			nextTest->next = fittingPlace;
			fittingPlace->nodeSize -= nextTest->nodeSize;
			if (fittingPlace->nodeSize == 0)
				nextTest->next = fittingPlace->next;
			nodeIndex++;
		}
		nextTest->hasBeenTested = true ;
		nextTest = find_next_not_tested(head);
	}
	size_t index = 0;
	while (head)
	{
		for (size_t i = 0; i < head->nodeSize; i++)
		{
			if (head->nodeID)
			{
				answer += index * (head->nodeID - 1);
			}
			index++;
		}
		head = head->next;
	}
	printf("The answer is: %llu", answer);
	return close(fd);
}
