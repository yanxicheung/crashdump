#include "crashdump.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define DEFAULT_SIG_SIZE 11

static inline void print_except_header(int signo)
{

}

static void print_registers(ucontext_t *ct)
{

}

static void print_backtrace(ucontext_t *ct)
{

}

static int print_maps(pid_t pid)
{
	return 0;
}

static void exception_action(int signo, siginfo_t* info, void* ct)
{
	struct sigaction sigact_defalt;
	print_except_header(signo);
	print_registers((ucontext_t *)ct);
	print_backtrace((ucontext_t *)ct);
	print_maps(gettid());

	// 恢复默认处理
	memset(&sigact_defalt, 0, sizeof (struct sigaction));
	sigemptyset(&sigact_defalt.sa_mask);
	sigact_defalt.sa_flags = SA_ONESHOT;
	sigact_defalt.sa_sigaction = SIG_DFL;

	sigaction(signo, &sigact_defalt, NULL);
	// 将信号再次抛出,触发操作系统对信号的默认处理
	kill(getpid(), signo);
}

static int install_signals(int *signals, int size)
{
	int i = 0;
	struct sigaction sigact;

	stack_t ss;
	ss.ss_sp = malloc(SIGSTKSZ);
	if (ss.ss_sp == NULL)
		return -1;

	ss.ss_flags = 0;
	ss.ss_size = SIGSTKSZ;
	if (sigaltstack (&ss, NULL) < 0)
	{
		free(ss.ss_sp);
		return -1;
	}

	sigemptyset(&sigact.sa_mask);
	memset(&sigact, 0, sizeof (struct sigaction));
	sigact.sa_flags = SA_ONSTACK | SA_SIGINFO;
	sigact.sa_sigaction = exception_action;

	for (i = 0; i < size; i++)
		sigaction(signals[i], &sigact, NULL);

	return 0;
}

int crashdump_init()
{
	int signals[DEFAULT_SIG_SIZE] = {
		SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT,
		SIGBUS, SIGFPE, SIGSEGV, SIGPWR, SIGSYS };
	return install_signals(signals, DEFAULT_SIG_SIZE);
}
