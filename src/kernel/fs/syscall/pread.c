#include <kernel/fs/vfs.h>
#include <kernel/process.h>
#include <fcntl.h>

ssize_t sys_pread(int fd, void *buf, size_t count, off_t offset)
{
	extern struct process *process_current;

	// Check fd
	if (fd < 0 || fd >= PROCESS_NB_OPEN_FILE)
		return (-1);

	// call VFS read primitive
	return (vfs_pread(&process_current->opfile[fd].file, buf, count, offset));
}
