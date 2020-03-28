#include <kernel/fs/vfs.h>
#include <kernel/devices/earlyterm.h>

ssize_t vfs_read(FILE *file, void *buf, size_t count)
{
	//TODO: Check permission !!

	// Check error
	if (file == NULL || buf == NULL)
		return (-1);

	// Get / check file's informations
	if (file->private == NULL ||
			file->file_op == NULL ||
			file->file_op->read == NULL)
		return (-1);

	// Check error
	if (file->cursor == 0xffffffff)
		earlyterm_write("pos = %#x\n", file->cursor);
	
	// Read with FS specifique primitive and return the numbe of reading bytes.
	ssize_t read = file->file_op->read(((struct dentry*)file->private)->inode, buf, count, file->cursor);
	if (read > 0)
		file->cursor = file->cursor + read;
	return (read);
}
