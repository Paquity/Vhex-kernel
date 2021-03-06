#include <kernel/memory.h>
#include <kernel/devices/earlyterm.h>

void pm_heap_debug(struct pm_heap_page *page)
{
	extern struct memory_info pmemory;
	struct pm_heap_block *block;

	page = pmemory.kheap;
	while (page != NULL)
	{
		earlyterm_write("page = %p, %do\n", page, page->size);
		DBG_WAIT;
		block = &page->heap;
		while ((void*)block < page->brk) {
			earlyterm_write("* [%d] - %p, %do\n", block->status, block, block->size);
			DBG_WAIT;
			block = (void*)&block[1] + block->size;
		}
		page = page->next;
	}
}
