#include <stdio.h>
#include <stdlib.h>

int main() {
	char buff[1024];
	FILE *fp = popen("ps aux", "r");
	FILE *more = popen("more", "w");
	if (!fp || !more) return 1;
	while (fgets(buff, sizeof(buff), fp)) fputs(buff, more);
	pclose(fp);
	pclose(more);
	return 0;
}
