#include <kernel/devices/tty.h>
#include <kernel/util/atomic.h>
#include <kernel/util/debug.h>
#include <kernel/util/string.h>

/* tty_vertical_update() - Check / update TTY vertical cursor */
static void tty_vertical_update(struct tty_s *tty)
{
	// Get next line.
	tty->cursor.y = 0;
	if (tty->cursor.y + 1 <= tty->cursor.max.y)
		tty->cursor.y = tty->cursor.y + 1;

	// Wipe new line.
	memset(tty->buffer[tty->cursor.y], '\0', tty->cursor.max.x);
}

/* tty_horizontal_update() - Check / update TTY horizotal cursor */
static int tty_horizontal_update(struct tty_s *tty)
{
	tty->cursor.x = tty->cursor.x + 1;
	if (tty->cursor.x >= tty->cursor.max.x)
	{
		tty_vertical_update(tty);
		tty->cursor.x = 0;
		return (1);
	}
	return (0);
}

/* tty_buffer_update() - Line discipline */
static ssize_t tty_buffer_update(struct tty_s *tty, const uint8_t *buffer, size_t count)
{
	int16_t offset;
	ssize_t i;

	i = -1;
	while (++i < (ssize_t)count)
	{
		// Check bell char.
		if (buffer[i] == '\a')
		{
			// TODO
			continue;
		}
		// Check backspace.
		if (buffer[i] == '\b')
		{
			if (tty->cursor.x > 0)
				tty->cursor.x = tty->cursor.x - 1;
			tty->buffer[tty->cursor.y][tty->cursor.x] = '\0';
			continue;
		}

		// Check horizotal tab.
		if (buffer[i] == '\t')
		{
			// Check if we need a new line or not.
			offset = 5 - (tty->cursor.x - ((tty->cursor.x / 5) * 5));
			if (tty->cursor.x + offset < tty->cursor.max.x)
			{
				memset(&tty->buffer[tty->cursor.y][tty->cursor.x], ' ', offset);
				tty->cursor.x = tty->cursor.x + offset;
				continue;
			}

			// If a new line is required char.
			// Generate a new line.
			tty->cursor.x = 0;
			tty_vertical_update(tty);
			continue;
		}

		// Check new line char.
		if (buffer[i] == '\n')
		{
			tty->cursor.x = 0;
			tty_vertical_update(tty);
			continue;
		}

		
		// Check 'form feed' and 'vertical tab' char.
		// @note: for now this character is interpreted like '\v'
		if (buffer[i] == '\f' || buffer[i] == '\v')
		{
			tty_vertical_update(tty);
			memset(tty->buffer[tty->cursor.y], ' ', tty->cursor.x);
			continue;
		}

		// Check carriage return.
		if (buffer[i] == '\r') { tty->cursor.x = 0; continue;}

		// Update TTY buffer char.
		tty->buffer[tty->cursor.y][tty->cursor.x] = buffer[i];
		tty_horizontal_update(tty);
	}
	return (i);
}

// FIXME: this function is device-specific !
// TODO: Update me ?
static void tty_display(struct tty_s *tty)
{
	int saved_start;
	int line_len;
	int line;
	int start;
	int y;

	// Start atomic operation.
	atomic_start();

	// Get the "first" line and number of line.
	// @note: circular buffer.
	line = 0;
	start = tty->cursor.y;
	while (++line < DISPLAY_VCHAR_MAX)
	{
		// Update check line.
		saved_start = start;
		start = (start - 1 < 0) ? tty->cursor.max.y : start - 1;

		// Check if the line existe.
		if (tty->buffer[start][0] == '\0')
		{
			start = saved_start;
			break;
		}
	}

	// Display "on-screen" string lines.
	y = -1;
	while (++y < line)
	{
		// Get / check line lenght.
		line_len = strnlen(tty->buffer[start], tty->cursor.max.x);
		if (line_len == 0)
			continue;

		// Display line.
		kvram_print(0, y, tty->buffer[start], line_len);

		// Get "next" line.
		start = (start + 1 > tty->cursor.max.y) ? 0 : start + 1;
	}

	// Display on screen.
	kvram_display();

	// Stop atomic operation
	atomic_stop();
}

/* tty_write() - TTY write primitive ! */
ssize_t tty_write(void *inode, const void *buffer, size_t count)
{
	ssize_t written;

	// Update internal buffer and display
	// TTY on the screen.
	written = tty_buffer_update(inode, buffer, count);

	// TODO: Monotonic display ?
	tty_display(inode);

	// Return the number of char written into
	// TTY's internal buffer.
	return (written);
}
