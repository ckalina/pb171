#include <string.h>

#include "buffer.h"

void buf_clean(struct buf *buf)
{
	memset(buf, 0, sizeof(struct buf));
}

/* can write as long as the next item isn't
 * trasmitted yet */
int buf_can_push(struct buf *buf)
{
	return mod((buf->in + 1),64) != mod(buf->out,64);
}

/* can transmit as long as the next item isn't
 * written to yet */
int buf_can_pop(struct buf *buf)
{
	return mod(buf->out,64) != mod(buf->in,64);
}

int buf_push(struct buf *buf, uv8 byte)
{
	if (!buf_can_push(buf))
		return 0;
	buf->buf[buf->in] = byte;
	buf->in = mod((buf->in + 1), 64);
	return 1;
}

int buf_pop(struct buf *buf, uv8 *data)
{
	if (!buf_can_pop(buf))
		return 0;
	*data = buf->buf[buf->out];
	buf->out = mod((buf->out + 1), 64);
	return 1;
}

int buf_empty(struct buf *buf)
{
	return !(buf_sz(buf) - 63);
}

buf_idx_t buf_sz(struct buf *buf)
{
	return mod((buf->out - buf->in - 1),64);
}

int buf_push_string(struct buf *buf, uv8 *data, int8_t len)
{
	if (buf_sz(buf) < len)
		return 0;
	while (len--)
		buf_push(buf, *(data++));
	return 1;
}
u8 buf_peek(struct buf *buf, u8 *data)
{
	if (!buf_can_pop(buf))
		return 0;
	*data = buf->buf[buf->out];
	return 1;
}
