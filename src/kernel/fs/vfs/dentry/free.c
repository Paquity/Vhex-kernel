#include <kernel/fs/vfs.h>
#include <kernel/util/kmem.h>

/* vfs_dentry_free() - Free'd allocated dentry */
/* @note: *WARNING* no verification will be done, so do not use this primitive */
void vfs_dentry_free(struct dentry *dentry)
{
	// Check error
	if (dentry == NULL)
		return;

	// Free'd allocated space
	kmem_free(dentry);
}
