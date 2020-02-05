#include "util.h"
#include "builtin.h"
#include <lib/string.h>
#include <lib/display.h>
#include <lib/unistd.h>

// Internal builtin list
// FIXME: due to PIE binary format, we can not
// FIXME: use address of builtin_proc !!
/*struct builtin_s list[1] = {
	{
		.name = "proc",
		.entry = (void*)&builtin_proc
	},
	//{
	//	.name = "ram",
	//	.entry = (void *)&builtin_ram
	//}
};*/


//TODO: use agc, argv.
int check_builtin(char *cmd)
{

	// Process test
	dclear();
	dprint(0, 0, "Process Load Test !");

	// Load process test
	pid_t pid = fexecve("/mnt/smemfs/VHEX/test.elf");
	if (pid < 0)
	{
		dprint(0, 1, "Process error !");
		dupdate();
		for(int i = 0 ; i < 3000000 ; ++i);
		return (-1);
	}

	int counter = 0;
	dclear();
	dprint(0, 0, "Process Test !");;
	while (1)
	{
		dclr_str_area(0, 1, 20, 1);
		dprint(0, 1, "P process: %d", counter);
		dupdate();
		counter = counter + 1;
		for (int i = 0 ; i < 30000 ; ++i);
	}

	//int wstatus;
	//pid_t pid;
	//int i;

	// Try to find builtin
/*	for (int i = 0 ; i < 1 ; i++)
	{
		if (strcmp(list[i].name, cmd) != 0)
			continue;

		// Execute builtin
		list[i].entry(0, NULL);
		return (0);
	}*/
	return (-1);
	// Create subprocess
	//pid = fork();
	//if (pid < 0)
	//	return (1);

	// If we are the child execute
	// the builtins.
	//if (pid == 0)
	//		//{
	//			dclear();
	//			dprint(0, 0, "Child process !!");
	//			dprint(0, 1, "PID  = %d", getpid());
	//			dprint(0, 2, "PPID = %d", getppid());
	//			dupdate();

	//	} else {
	//		waitpid(pid, &wstatus, WCONTINUED);
	//TODO: signal handling.
	//	}
	//	}
	//	return (1);
	}
