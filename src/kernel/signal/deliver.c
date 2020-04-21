#include <kernel/signal.h>
#include <kernel/scheduler.h>
#include <bits/waitstatus.h>
#include <sys/signal.h>

static int sig_deliver(struct process *proc, int sig)
{
	// Check ignored signal
	if (proc->signal[sig] == SIG_IGN)
		return (0);

	// Check default signal managment
	if (proc->signal[sig] == SIG_DFL)
	{
		switch (sig)
		{
			/* Change process state */
			case SIGSTOP:
				sched_task_stop(proc);
				return (1);
			case SIGCONT:
				if (proc->parent != NULL)
					signal_raise(proc->parent, SIGCHLD);
				return (0);

			/* Nothing, just wake up the process */
			case SIGCHLD:
				return (0);

			/* Process terination (default) */
			case SIGTERM:	// termination
			case SIGHUP:	// hang up
			case SIGKILL:	// kill
			case SIGBUS:	// bus error
			case SIGFPE:	// fatal arithmetic error
			case SIGSEGV:	// segmentation violation
			default:
				process_terminate(proc, __W_STOPCODE(sig));
				return (1);

		}
	}

	//TODO
	//TODO User handler management
	//TODO
	return (0);
}

int signal_deliver_pending(struct process *proc)
{
	int need_schedule;
	uint32_t sig;

	// Get all pending signals
	sig = proc->sig_pending & (~proc->sig_blocked);

	// Walk into pending signals
	need_schedule = 0;
	for (int i = 0 ; i < 32 && need_schedule == 0 ; ++i)
	{
		// Check signal pending
		if ((sig & (1 << i)) == 0)
			continue;

		// Check signal validity and try to
		// deliver the signal
		if (proc->signal[i] != __SIGUNDEF)
			sig_deliver(proc, i);

		// Clear the signal mask
		proc->sig_pending = proc->sig_pending & (~i);
	}
	return (need_schedule);
}
