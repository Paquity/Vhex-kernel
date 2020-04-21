#include <kernel/fs/vfs.h>
#include <kernel/process.h>
#include <fcntl.h>

ssize_t sys_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
	extern struct process *process_current;

	// Check fd
	if (fd < 0 || fd - 3 >= PROCESS_NB_OPEN_FILE)
		return (-1);

	// Check virtual file (TTY) and open it if needed
	if (fd < 3) {
		if (process_current->tty.private == NULL &&
			vfs_open(&process_current->tty, "/dev/tty", O_RDWR) != 0)
			return (-1);
		return (vfs_pwrite(&process_current->tty, buf, count, offset));
	}

	// call VFS read primitive
	return (vfs_pwrite(&process_current->opfile[fd].file, buf, count, offset));
}