#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t got_usr1 = 0;

static void on_usr1(int sig) {
	(void)sig;
	got_usr1 = 1;
}

static int sleep_relative_ms(long ms) {
	struct timespec req = { .tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000L };
	struct timespec rem;
	while (nanosleep(&req, &rem) == -1) {
		if (errno == EINTR) {
			req = rem; //Restart sleep with remaining time
			continue;
		}
		return -1;
	}
	return 0;
}

static int sleep_periodic_absolute(struct timespec *deadline, long period_ms) {
	deadline->tv_nsec += period_ms * 1000000L;
	while (deadline->tv_nsec >= 1000000000L) {
		deadline->tv_sec++;
		deadline->tv_nsec -= 1000000000L;
	}
	int rc;
	while ((rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, deadline, NULL)) == EINTR);
	return (rc == 0) ? 0 : -1;
}

int main(void) {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_usr1;
	sigaction(SIGUSR1, &sa, NULL);

	printf("PID=%ld. Run 'kill -USR1 %ld' to interrupt\n", (long)getpid(), (long)getpid());

	printf("Sleeping 5s with restart loop...\n");
	sleep_relative_ms(5000);
	printf("Finished. Interrupted by USR1: %d\n", got_usr1);

	struct timespec next;
	clock_gettime(CLOCK_MONOTONIC, &next);
	for (int i = 1; i <= 5; i++) {
		sleep_periodic_absolute(&next, 1000);
		printf("tick %d\n", i);
	}
	return 0;
}
