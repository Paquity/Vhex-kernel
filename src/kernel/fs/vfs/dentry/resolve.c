#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/process.h>
#include <kernel/devices/earlyterm.h>
#include <lib/string.h>

static int get_name(int *name_lenght, const char *path, char *name)
{
	// Get file name
	*name_lenght = 0;
	while (*name_lenght < 24 &&
			path[*name_lenght] != '/' &&
			path[*name_lenght] != '\0')
	{
		name[*name_lenght] = path[*name_lenght];
		*name_lenght = *name_lenght + 1;
	}

	// Check error
	if (path[*name_lenght] != '/' && path[*name_lenght] != '\0')
		return (-1);

	// Add null char
	name[*name_lenght] = '\0';
	return (0);
}

//TODO: update ?
struct dentry *vfs_dentry_resolve(const char *path, int mode)
{
	extern struct process *process_current;
	extern struct dentry *vfs_root_node;
	struct dentry *file;
	struct dentry *next;
	struct dentry *tmp;
	int name_lenght;
	char name[24];

	// Check VFS validity
	if (vfs_root_node == NULL)
	{
		earlyterm_write("VFS root error !\n");
		DBG_WAIT;
		return (NULL);
	}

	// Get start inode
	if (path[0] != '/')
	{
		// Check if current process validity
		if (process_current == NULL)
			return (NULL);

		// Get current working directory
		file = process_current->working_dir;
		next = process_current->working_dir->child;
	} else {
		// Get root inode
		file = vfs_root_node;
		next = vfs_root_node->child;

		// Update path (skip '/')
		path = &path[1];
	}

	// VFS walk entry !
	while (path[0] != '\0')
	{
		// Get next file name
		if (get_name(&name_lenght, path, name) != 0)
			return (NULL);

		// Check '..' file (parent directory)
		if (name[0] == '.' && name[1] == '.' && name[2] == '\0')
		{
			if (file->parent != NULL)
			{
				next = file->parent->child;
				file = file->parent->child;
			}
			path = &path[3];
			continue;
		}

		// Check '.' or '//' file (current directory)
		if (name[0] == '\0' || (name[0] == '.' && name[1] == '\0'))
		{
			path = (path[name_lenght] == '/') ? &path[name_lenght + 1] : &path[name_lenght];
			continue;
		}

		// Check parthname or path mode
		if (path[name_lenght] == '\0' && 
				mode == VFS_DENTRY_RESOLVE_FLAG_PATHNAME)
			break;

		// Check next file entry
		if (next == NULL)
			return (NULL);

		// Try to find file in the current VFS level.
		while (strncmp(next->name, name, name_lenght) != 0)
		{
			tmp = next;
			next = vfs_dentry_find_next_sibling(next);
			if (next == NULL)
			{
				earlyterm_write("vfs_resolve(): sibling error !\n");
				earlyterm_write("* old next: %s$\n", tmp->name);
				earlyterm_write("* file: %s$\n", file->name);
				earlyterm_write("* name: %s$\n", name);
				DBG_WAIT;
				return (NULL);
			}
		}

		// Update current file
		file = next;

		// Directory check
		if (path[name_lenght] == '/')
		{
			// Check type
			if ((file->mode & __S_IFDIR) == 0)
			{
				// Debug !
				earlyterm_write("vfs_resolve(): dir error !\n");
				DBG_WAIT;
				return (NULL);
			}

			// Try to find first child
			next = vfs_dentry_find_first_child(next);

			// Update name lenght to skip '/' char
			name_lenght = name_lenght + 1; 
		}
	
		// Update path
		path = &path[name_lenght];
	}
	return (file);
}
