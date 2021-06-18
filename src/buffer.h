#ifndef BUFFER_H
# define BUFFER_H

#include "common.h"

typedef int8_t buf_idx_t;

/**
 * in	points to a place that we can write to
 * out	points to a place that we will transmit
 *
 * BUFFER WRITE
 *
 * If in + 1 == out mod 64, we would overwrite data
 * that are yet to be transmitted.
 *
 * BUFFER FLUSH
 *
 * We can trasmit as long as out % 64 != in % 64.
 */
__attribute__((packed))
struct buf {
	uv8 buf[64];
	volatile buf_idx_t in;
	volatile buf_idx_t out;
	uv8 flag : 1;
	uv8 overflow : 1;
};

int buf_can_push(struct buf *buf);
int buf_can_pop(struct buf *buf);
int buf_push(struct buf *buf, uv8 byte);
int buf_pop(struct buf *buf, uv8 *data);
int buf_empty(struct buf *buf);
buf_idx_t buf_sz(struct buf *buf);
int buf_push_string(struct buf *buf, uv8 *data, int8_t len);
u8 buf_peek(struct buf *buf, u8 *data);
void buf_clean(struct buf *buf);
#endif
