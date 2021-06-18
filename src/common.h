#ifndef COMMON_H
# define COMMON_H

# include <assert.h>
# include <stdint.h>

# ifdef DEBUG
#  define ASSERT(X) assert(X)
# else
#  define ASSERT(X)
# endif

typedef uint32_t u32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;

typedef volatile u16 uv16;
typedef volatile u8 uv8;

# define always_inline	__attribute__((always_inline))
# define naked		__attribute__((naked))
# define pure		__attribute__((pure))

# define progmem8_t const u8 __attribute__((__progmem__))
# define progmem16_t const u16 __attribute__((__progmem__))

# define SREG_READ() (*(volatile u8 *)((0x3F) + 0x20))
# define SREG_WRITE(val) (*(volatile u8 *)((0x3F) + 0x20)) = val

# define lpm_z(dst, ptr)                                                     \
	__asm__ __volatile__(                                                \
		"lpm %0, Z" "\n\t"                                           \
		: "=r" (dst)                                                 \
		: "z" (ptr)                                                  \
	);

# define lpm_zpz(dst, ptr)                                                   \
	__asm__ __volatile__(                                                \
		"lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t"                     \
		: "=r" (dst), "=z" (ptr)                                     \
		: "1" (ptr)                                                  \
	);

# define cli_mem() __asm__ __volatile__ ("cli" ::: "memory")
# define sei_mem() __asm__ __volatile__ ("sei" ::: "memory")
# define cli() __asm__ __volatile__ ("cli" ::)
# define sei() __asm__ __volatile__ ("sei" ::)

# define mod(x, N) ((x % N + N) %N )

#endif
