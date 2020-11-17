#include "util.h"

char* substring(char* destination, const char* source, int beg, int n){
	while (n > 0)
	{
		*destination = *(source + beg);

		destination++;
		source++;
		n--;
	}
	*destination = '\0';
	return destination;
}
