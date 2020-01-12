#ifndef __KERNEL_LOADER_H__
# define __KERNEL_LOADER_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/fs/file.h>
#include <kernel/process.h>
#include <kernel/elf.h>

// Function
extern void *loader(const char *path, struct process *process);
extern int loader_get_header(FILE *file, Elf32_Ehdr *header);
extern void *loader_load_image(FILE *file, Elf32_Ehdr *header, struct process *process);

#endif /*__KERNEL_LOADER_H__*/
