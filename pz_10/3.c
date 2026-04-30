#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void subscriber() {
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &set, NULL); //Must block before sigwaitinfo

	printf("Subscriber PID=%ld, waiting for RT signal...\n", (long)getpid());
	for (;;) {
		siginfo_t si;
		int r = sigwaitinfo(&set, &si);
		if (r == -1) continue;
		printf("Received value: %d from PID: %ld\n", si.si_value.sival_int, (long)si.si_pid);
		if (si.si_value.sival_int < 0) break; //Stop on negative
	}
}

int main(int argc, char **argv) {
	if (argc < 2) return 1;
	if (strcmp(argv[1], "sub") == 0) {
		subscriber();
	} else if (strcmp(argv[1], "pub") == 0 && argc == 4) {
		union sigval val;
		val.sival_int = atoi(argv[3]);
		sigqueue(atoi(argv[2]), SIGRTMIN, val); //Send signal with data
	}
	return 0;
}
