#include <kernel/fs/vfs.h>
#include <kernel/process.h>

off_t sys_lseek(int fd, off_t offset, int whence)
{
	extern struct process *process_current;

	// Check fd
	if (fd < 0 || fd >= PROCESS_NB_OPEN_FILE)
		return (-1);

	// Call VFS lseek primitive
	return (vfs_lseek(&process_current->opfile[fd].file, offset, whence));
}
