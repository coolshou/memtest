#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// unsigned long long memory_to_eat = 1024 * 1024* 50; //50M
unsigned long long memory_to_eat = 0;
size_t eaten_memory = 0;
void *memory = NULL;
// char *memory = NULL;

// void write_kilobyte(void *pointer, size_t offset)
// {
//     int size = 0;
//     while (size < 1024)
//     {   // writing one byte per page is enough, this is overkill
//         pointer[offset + (size_t) size++] = 1;
//     }
// }

int eat_kilobyte()
{
	if (memory == NULL)
	{
		memory = malloc(1024);
	}
	else
	{
		memory = realloc(memory, (eaten_memory * 1024) + 1024);
	}
	if (memory == NULL)
	{
		// realloc failed here - we probably can't allocate more memory for whatever reason
		return 1;
	}
	else
	{
		// Force the kernel to map the containing memory page.
		((char *)memory)[1024 * eaten_memory] = 42;
		// write_kilobyte(memory, eaten_memory * 1024);
		eaten_memory++;
		return 0;
	}
}

int memeat(int memusage, int iwait)
{
	eaten_memory = 0;
	memory = NULL;

	memory_to_eat = 1024 * memusage;
	printf("I will try to eat %lli kb of ram\n", memory_to_eat);
	int megabyte = 0;
	while (memory_to_eat > 0)
	{
		memory_to_eat--;
		if (eat_kilobyte())
		{
#if defined _WIN32
			printf("Failed to allocate more memory! Stucked at %lli kb :(\n", eaten_memory);
#else
			printf("Failed to allocate more memory! Stucked at %li kb :(\n", eaten_memory);
#endif
			return 200;
		}
		if (megabyte++ >= 1024)
		{
#if defined _WIN32
			printf("Eaten %lld / %d MB of ram\n", eaten_memory / (size_t)1024, memusage);
#else
			printf("Eaten %ld / %d MB of ram\n", eaten_memory / (size_t)1024, memusage);
#endif
			megabyte = 0;
		}
	}
	printf("Successfully eaten requested memory!\n");
	printf("wait %d sec then free the eaten memory!\n", iwait);
	sleep(iwait);
	free(memory);
	printf("Test end!\n");
	return 0;
}
