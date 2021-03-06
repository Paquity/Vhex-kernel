#ifndef __KERNEL_UTIL_TIMER_H__
# define __KERNEL_UTIL_TIMER_H__

#include <stddef.h>
#include <stdint.h>

// Define internal timer informations
#define TIMER_NUMBER	3
#define TIMER_UNUSED	0xff

// Structure used by the timer abstraction
struct timer_cache_s
{
	void *callback;
	volatile void *arg;
	uint8_t status;
};

// Internal enum used to setup the timer
typedef enum {
	TIMER_START,
	TIMER_STOP
} tmode_t;

// Prototypes
extern int timer_uninstall(int timerID);
extern int timer_install(void *callback, void *arg, int hertz, tmode_t mode);
extern int timer_set_callback(int timerID, void *callback, void *arg);
extern int timer_set_ticks(int timerID, uint32_t ticks);
extern int timer_start(int timerID);
extern int timer_stop(int timerID);

#endif /*__KERNEL_UTIL_TIMER_H__*/
