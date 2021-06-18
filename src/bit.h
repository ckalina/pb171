#ifndef BIT_H
# define BIT_H
# include "common.h"

/*
 * Compute the n-th bit, starting with zero.
 * Note that this will overflow for large values.
 */
always_inline pure static inline uint16_t bit(const u8 bit)
{
	ASSERT(1 << bit != 0);
	return 1 << bit;
}

/*
 * Clear the n-th bit, starting from zero.
 * The function x => x & ~(1 << n) can be memoized.
 */
always_inline pure static inline uint16_t bitClear(const u16 val, const u8 n)
{
	return val & ~(1 << n);
}

/*
 * Return n-th bit, starting from zero.
 * This would require some benchmarking. We can do either of:
 * 	(val >> n) & 1
 * 	(val & (1 << n)) >> n
 */
always_inline pure static inline uint8_t bitRead(const u16 val, const u8 n)
{
	return (val >> n) & 1;
}

always_inline pure static inline uint16_t bitSet(const u16 val, const u8 n)
{
	// TODO profile this, maybe order of OR operands matter w.r.t to perf
	// read might block the pipeline
	return val | (1 << n);
}

always_inline pure static inline uint16_t bitWrite(const u16 val, const u8 n,
						   const u8 bit)
{
	return bit ? bitSet(val, n) : bitClear(val, n);
}

always_inline pure static inline uint8_t lowByte(const u16 val)
{
	return (uint8_t) val;
}

always_inline pure static inline uint8_t highByte(const u16 val)
{
	return (uint8_t) (val >> 8);
}

#endif
