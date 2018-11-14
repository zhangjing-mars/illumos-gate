/*
 * Implement fast SHA-256 with AVX1 instructions. (x86_64)
 *
 * Copyright (C) 2013 Intel Corporation.
 *
 * Authors:
 *     James Guilford <james.guilford@intel.com>
 *     Kirk Yap <kirk.s.yap@intel.com>
 *     Tim Chen <tim.c.chen@linux.intel.com>
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************
 * This code is described in an Intel White-Paper:
 * "Fast SHA-256 Implementations on Intel Architecture Processors"
 *
 * To find it, surf to http://www.intel.com/p/en_US/embedded
 * and search for that title.
 ********************************************************************
 * This code schedules 1 block at a time, with 4 lanes per block
 */

/*
 * Modifications:
 *
 * Copyright (C) 2014 Beijing Asia Creattien Technology Co. Ltd
 * Authors:
 *     John Huang <huang.jiang@marstor.com>
 *
 * 1. This file is modification of Linux 3.14.5 kernel source arch\x86\crypto\sha256-avx-asm.S
 *
 * 2. Added comments
 *
 * 3. Translate Intel/yasm/nasm syntax to ATT/Solaris as(1) syntax
 *
 *
 * 4. Added Solaris ENTRY_NP/SET_SIZE macros from
 * /usr/include/sys/asm_linkage.h, lint(1B) guards, and dummy C function
 * definitions for lint.
 *
 */

#if defined(lint) || defined(__lint)
#include <sys/stdint.h>
#include <sys/sha2.h>

/* ARGSUSED */
void
sha256_transform_avx2(SHA2_CTX *ctx, const void *in, size_t num)
{
}

#else
#include <sys/asm_linkage.h>
#include <sys/controlregs.h>
#ifdef _KERNEL
#include <sys/machprivregs.h>
#endif

// assume buffers not aligned
#define	VMOVDQ vmovdqu

#ifndef YMM_SIZE
#define YMM_SIZE 32
#endif

#ifdef _KERNEL
#ifdef __xpv
#define	PROTECTED_CLTS	\
	push	%rsi;	\
	CLTS;		\
	pop	%rsi
#else
#define	PROTECTED_CLTS \
	CLTS
#endif	/* __xpv */

#define CLEAR_TS_OR_PUSH_YMM_REGISTERS \
	movq	%cr0, %rcx; \
	testq	$CR0_TS, %rcx; \
	jnz	1f; \
	sub	$[YMM_SIZE * 14], %rsp; \
	vmovaps	%ymm0, (%rsp); \
	vmovaps	%ymm1, 32(%rsp); \
	vmovaps	%ymm2, 64(%rsp); \
	vmovaps	%ymm3, 96(%rsp); \
	vmovaps	%ymm4, 128(%rsp); \
	vmovaps	%ymm5, 160(%rsp); \
	vmovaps	%ymm6, 192(%rsp); \
	vmovaps	%ymm7, 224(%rsp); \
	vmovaps	%ymm8, 256(%rsp); \
	vmovaps	%ymm9, 288(%rsp); \
	vmovaps	%ymm10, 320(%rsp); \
	vmovaps	%ymm11, 352(%rsp); \
	vmovaps	%ymm12, 384(%rsp); \
	vmovaps	%ymm13, 416(%rsp); \
	jmp	2f; \
1: \
	PROTECTED_CLTS; \
2: \
	sub	$[YMM_SIZE], %rsp; \
	mov	%rcx, (%rsp);
	

#define SET_TS_OR_POP_YMM_REGISTERS \
	mov	(%rsp), %rcx; \
	add	$[YMM_SIZE], %rsp; \
	testq	$CR0_TS, %rcx; \
	jnz	1f; \
	vmovaps	(%rsp), %ymm0; \
	vmovaps	32(%rsp), %ymm1; \
	vmovaps	64(%rsp), %ymm2; \
	vmovaps	96(%rsp), %ymm3; \
	vmovaps	128(%rsp), %ymm4; \
	vmovaps	160(%rsp), %ymm5; \
	vmovaps	192(%rsp), %ymm6; \
	vmovaps	224(%rsp), %ymm7; \
	vmovaps	256(%rsp), %ymm8; \
	vmovaps	288(%rsp), %ymm9; \
	vmovaps	320(%rsp), %ymm10; \
	vmovaps	352(%rsp), %ymm11; \
	vmovaps	384(%rsp), %ymm12; \
	vmovaps	416(%rsp), %ymm13; \
	jmp	2f; \
1: \
	STTS(%rcx); \
2:

#else
#define PROTECTED_CLTS
#define CLEAR_TS_OR_PUSH_YMM_REGISTERS
#define SET_TS_OR_POP_YMM_REGISTERS
#endif	/* _KERNEL */

// Define Macros

// addm [mem], reg
// Add reg to mem using reg-mem add and store
.macro addm p1 p2
	add	\p1, \p2
	mov	\p2, \p1
.endm

X0 = %ymm4
X1 = %ymm5
X2 = %ymm6
X3 = %ymm7

// XMM versions of above
XWORD0 = %xmm4
XWORD1 = %xmm5
XWORD2 = %xmm6
XWORD3 = %xmm7

XTMP0 = %ymm0
XTMP1 = %ymm1
XTMP2 = %ymm2
XTMP3 = %ymm3
XTMP4 = %ymm8
XFER  = %ymm9
XTMP5 = %ymm11

SHUF_00BA =	%ymm10 // shuffle xBxA -> 00BA
SHUF_DC00 =	%ymm12 // shuffle xDxC -> DC00
BYTE_FLIP_MASK = %ymm13

X_BYTE_FLIP_MASK = %xmm13 // XMM version of BYTE_FLIP_MASK

CTX = %rdi        // 1st arg
INP = %rsi        // 2nd arg
NUM_BLKS = %rdx   // 3rd arg

c	= %ecx
d	= %r8d
e       = %edx	// clobbers NUM_BLKS
y3	= %esi	// clobbers INP


TBL	= %rbp
SRND	= CTX	// SRND is same register as CTX

a = %eax
b = %ebx
f = %r9d
g = %r10d
h = %r11d
old_h = %r11d

T1 = %r12d
y0 = %r13d
y1 = %r14d
y2 = %r15d


_XFER_SIZE	= 2*64*4	// 2 blocks, 64 rounds, 4 bytes/round
_INP_END_SIZE	= 8
_INP_SIZE	= 8
_CTX_SIZE	= 8
_RSP_SIZE	= 8

_XFER		= 0
_INP_END	= _XFER     + _XFER_SIZE
_INP		= _INP_END  + _INP_END_SIZE
_CTX		= _INP      + _INP_SIZE
_RSP		= _CTX      + _CTX_SIZE
STACK_SIZE	= _RSP      + _RSP_SIZE

// rotate_Xs
// Rotate values of symbols X0...X3
.macro rotate_Xs
	X_ = X0
	X0 = X1
	X1 = X2
	X2 = X3
	X3 = X_
.endm

// ROTATE_ARGS
// Rotate values of symbols a...h
.macro ROTATE_ARGS
	old_h = h
	TMP_ = h
	h = g
	g = f
	f = e
	e = d
	d = c
	c = b
	b = a
	a = TMP_
.endm

.macro FOUR_ROUNDS_AND_SCHED disp
// RND N + 0

	mov	a, y3				// y3 = a
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11

	addl	\disp(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c
	vpalignr $4, X2, X3, XTMP0 		// XTMP0 = W[-7]
	mov	f, y2				// y2 = f
	rorx	$13, a, T1			// T1 = a >> 1

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	xor	g, y2				// y2 = f^g
	vpaddd	X0, XTMP0, XTMP0 		// XTMP0 = W[-7] + W[-16]
	rorx	$6, e, y1			// y1 = (e >> 6)

	and	e, y2				// y2 = (f^g)&e
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$22, a, y1			// y1 = a >> 22
	add	h, d				// d = k + w + h + d

	and	b, y3				// y3 = (a|c)&b
	vpalignr $4, X0, X1, XTMP1		// XTMP1 = W[-15]
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a, T1			// T1 = (a >> 2)

	xor	g, y2				// y2 = CH = ((f^g)&e)^g
	vpsrld	$7, XTMP1, XTMP2
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	c, T1				// T1 = a&c

	add	y0, y2				// y2 = S1 + CH
	vpslld	$(32-7), XTMP1, XTMP3
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0

	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1
	vpor	XTMP2, XTMP3, XTMP3		// XTMP3 = W[-15] ror 7

	vpsrld	$18, XTMP1, XTMP2
	add	y2, h				// h = k + w + h + S0 + S1 + CH = t1 + S0
	add	y3, h				// h = t1 + S0 + MAJ


	ROTATE_ARGS

// RND N + 1

	mov	a, y3				// y3 = a
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	offset = \disp + 1*4
	addl	offset(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c


	vpsrld	$3, XTMP1, XTMP4 		// XTMP4 = W[-15] >> 3
	mov	f, y2				// y2 = f
	rorx	$13, a, T1			// T1 = a >> 13
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	xor	g, y2				// y2 = f^g


	rorx	$6, e, y1			// y1 = (e >> 6)
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$22, a, y1			// y1 = a >> 22
	and	e, y2				// y2 = (f^g)&e 
	add	h, d				// d = k + w + h + d

	vpslld	$(32-18), XTMP1, XTMP1
	and	b, y3				// y3 = (a|c)&b
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)

	vpxor	XTMP1, XTMP3, XTMP3
	rorx	$2, a, T1			// T1 = (a >> 2)
	xor	g, y2				// y2 = CH = ((f^g)&e)^g

	vpxor	XTMP2, XTMP3, XTMP3		// XTMP3 = W[-15] ror 7 ^ W[-15] ror 18
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH

	vpxor	XTMP4, XTMP3, XTMP1		// XTMP1 = s0
	vpshufd	$0b11111010, X3, XTMP2		// XTMP2 = W[-2] {BBAA}
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0

	vpaddd	XTMP1, XTMP0, XTMP0		// XTMP0 = W[-16] + W[-7] + s0
	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1
	add	y2, h				// h = k + w + h + S0 + S1 + CH = t1 + S0
	add	y3, h				// h = t1 + S0 + MAJ

	vpsrld	$10, XTMP2, XTMP4 		// XTMP4 = W[-2] >> 10 {BBAA}


	ROTATE_ARGS

// RND N + 2

	mov	a, y3				// y3 = a
	rorx	$25, e, y0			// y0 = e >> 25
	offset = \disp + 2*4
	addl	offset(%rsp, SRND), h		// h = k + w + h

	vpsrlq	$19, XTMP2, XTMP3 		// XTMP3 = W[-2] ror 19 {xBxA}
	rorx	$11, e, y1			// y1 = e >> 11
	or	c, y3				// y3 = a|c
	mov	f, y2				// y2 = f
	xor	g, y2				// y2 = f^g

	rorx	$13, a, T1			// T1 = a >> 13
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	vpsrlq	$17, XTMP2, XTMP2		// XTMP2 = W[-2] ror 17 {xBxA}
	and	e, y2				// y2 = (f^g)&e

	rorx	$6, e, y1			// y1 = (e >> 6)
	vpxor	XTMP3, XTMP2, XTMP2
	add	h, d				// d = k + w + h + d
	and	b, y3				// y3 = (a|c)&b

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$22, a, y1			// y1 = a >> 22
	vpxor	XTMP2, XTMP4, XTMP4		// XTMP4 = s1 {xBxA}
	xor	g, y2				// y2 = CH = ((f^g)&e)^g

	vpshufb	SHUF_00BA, XTMP4, XTMP4		// XTMP4 = s1 {00BA}
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a ,T1			// T1 = (a >> 2)
	vpaddd	XTMP4, XTMP0, XTMP0		// XTMP0 = {..., ..., W[1], W[0]}

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH
	vpshufd	$0b01010000, XTMP0, XTMP2	// XTMP2 = W[-2] {DDCC}

	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1,h				// h = k + w + h + S0
	add	y2,d				// d = k + w + h + d + S1 + CH = d + t1
	add	y2,h				// h = k + w + h + S0 + S1 + CH = t1 + S0

	add	y3,h				// h = t1 + S0 + MAJ


	ROTATE_ARGS

// RND N + 3

	mov	a, y3				// y3 = a
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	offset = \disp + 3*4
	addl	offset(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c


	vpsrld	$10, XTMP2, XTMP5		// XTMP5 = W[-2] >> 10 {DDCC}
	mov	f, y2				// y2 = f
	rorx	$13, a, T1			// T1 = a >> 13
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	xor	g, y2				// y2 = f^g


	vpsrlq	$19, XTMP2, XTMP3		// XTMP3 = W[-2] ror 19 {xDxC}
	rorx	$6, e, y1			// y1 = (e >> 6)	
	and	e, y2				// y2 = (f^g)&e
	add	h, d				// d = k + w + h + d
	and	b, y3				// y3 = (a|c)&b

	vpsrlq	$17, XTMP2, XTMP2		// XTMP2 = W[-2] ror 17 {xDxC}
	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	xor	g, y2				// y2 = CH = ((f^g)&e)^g

	vpxor	XTMP3, XTMP2, XTMP2
	rorx	$22, a, y1			// y1 = a >> 22
	add	y0, y2				// y2 = S1 + CH

	vpxor	XTMP2, XTMP5, XTMP5		// XTMP5 = s1 {xDxC}
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1

	rorx	$2, a, T1			// T1 = (a >> 2)
	vpshufb	SHUF_DC00, XTMP5, XTMP5		// XTMP5 = s1 {DC00}

	vpaddd	XTMP0, XTMP5, X0		// X0 = {W[3], W[2], W[1], W[0]}
	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	c, T1				// T1 = a&c
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)

	add	y1, h				// h = k + w + h + S0
	add	y2, h				// h = k + w + h + S0 + S1 + CH = t1 + S0
	add	y3, h				// h = t1 + S0 + MAJ

	ROTATE_ARGS
	rotate_Xs
.endm

.macro DO_4ROUNDS disp
// RND N + 0

	mov	f, y2				// y2 = f
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	xor	g, y2				// y2 = f^g

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	rorx	$6, e, y1			// y1 = (e >> 6)
	and	e, y2				// y2 = (f^g)&e

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$13, a, T1			// T1 = a >> 13
	xor	g, y2				// y2 = CH = ((f^g)&e)^g
	rorx	$22, a, y1			// y1 = a >> 22
	mov	a, y3				// y3 = a

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a, T1			// T1 = (a >> 2)
	addl	\disp(%rsp, SRND), h		// h = k + w + h 
	or	c, y3				// y3 = a|c

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	b, y3				// y3 = (a|c)&b
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH


	add	h, d				// d = k + w + h + d
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0
	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1

	ROTATE_ARGS

// RND N + 1

	add	y2, old_h			// h = k + w + h + S0 + S1 + CH = t1 + S0
	mov	f, y2				// y2 = f
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	xor	g, y2				// y2 = f^g

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	rorx	$6, e, y1			// y1 = (e >> 6)
	and	e, y2				// y2 = (f^g)&e
	add	y3, old_h			// h = t1 + S0 + MAJ

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$13, a, T1			// T1 = a >> 13
	xor	g, y2				// y2 = CH = ((f^g)&e)^g
	rorx	$22, a, y1			// y1 = a >> 22
	mov	a, y3				// y3 = a

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a, T1			// T1 = (a >> 2)
	offset = 4*1 + \disp
	addl	offset(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	b, y3				// y3 = (a|c)&b
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH


	add	h, d				// d = k + w + h + d
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0

	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1

	ROTATE_ARGS

// RND N + 2

	add	y2, old_h			// h = k + w + h + S0 + S1 + CH = t1 + S0
	mov	f, y2				// y2 = f
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	xor	g, y2				// y2 = f^g

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	rorx	$6, e, y1			// y1 = (e >> 6)
	and	e, y2				// y2 = (f^g)&e
	add	y3, old_h			// h = t1 + S0 + MAJ

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$13, a, T1			// T1 = a >> 13
	xor	g, y2				// y2 = CH = ((f^g)&e)^g
	rorx	$22, a, y1			// y1 = a >> 22
	mov	a, y3				// y3 = a

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a, T1			// T1 = (a >> 2)
	offset = 4*2 + \disp
	addl	offset(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	b, y3				// y3 = (a|c)&b
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH


	add	h, d				// d = k + w + h + d
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0

	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1

	ROTATE_ARGS

// RND N + 3

	add	y2, old_h			// h = k + w + h + S0 + S1 + CH = t1 + S0
	mov	f, y2				// y2 = f 
	rorx	$25, e, y0			// y0 = e >> 25
	rorx	$11, e, y1			// y1 = e >> 11
	xor	g, y2				// y2 = f^g

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11)
	rorx	$6, e, y1			// y1 = (e >> 6)
	and	e, y2				// y2 = (f^g)&e
	add	y3, old_h			// h = t1 + S0 + MAJ

	xor	y1, y0				// y0 = (e>>25) ^ (e>>11) ^ (e>>6)
	rorx	$13, a, T1			// T1 = a >> 13
	xor	g, y2				// y2 = CH = ((f^g)&e)^g
	rorx	$22, a, y1			// y1 = a >> 22
	mov	a, y3				// y3 = a

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13)
	rorx	$2, a, T1			// T1 = (a >> 2)
	offset = 4*3 + \disp
	addl	offset(%rsp, SRND), h		// h = k + w + h
	or	c, y3				// y3 = a|c

	xor	T1, y1				// y1 = (a>>22) ^ (a>>13) ^ (a>>2)
	mov	a, T1				// T1 = a
	and	b, y3				// y3 = (a|c)&b
	and	c, T1				// T1 = a&c
	add	y0, y2				// y2 = S1 + CH


	add	h, d				// d = k + w + h + d
	or	T1, y3				// y3 = MAJ = (a|c)&b)|(a&c)
	add	y1, h				// h = k + w + h + S0

	add	y2, d				// d = k + w + h + d + S1 + CH = d + t1


	add	y2, h				// h = k + w + h + S0 + S1 + CH = t1 + S0

	add	y3, h				// h = t1 + S0 + MAJ

	ROTATE_ARGS

.endm

/*
 * void sha256_transform_avx(SHA2_CTX *ctx, const void *in, size_t nums)
 */
.text
ENTRY(sha256_transform_avx2)
.align 32
	pushq	%rbx
	pushq	%rbp
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	mov	%rsp, %rax
	and	$-YMM_SIZE, %rsp
	CLEAR_TS_OR_PUSH_YMM_REGISTERS
	subq	$STACK_SIZE, %rsp
	mov	%rax, _RSP(%rsp)


	shl	$6, NUM_BLKS			// convert to bytes
	jz	done_hash
	add	$8, CTX				// Skip OpenSolaris field, "algotype"
	lea	-64(INP, NUM_BLKS), NUM_BLKS	// pointer to end of data
	mov	NUM_BLKS, _INP_END(%rsp)

	cmp	NUM_BLKS, INP
	je	only_one_block

	// load initial digest
	mov	(CTX), a
	mov	4*1(CTX), b
	mov	4*2(CTX), c
	mov	4*3(CTX), d
	mov	4*4(CTX), e
	mov	4*5(CTX), f
	mov	4*6(CTX), g
	mov	4*7(CTX), h

	vmovdqa  PSHUFFLE_BYTE_FLIP_MASK(%rip), BYTE_FLIP_MASK
	vmovdqa  _SHUF_00BA(%rip), SHUF_00BA
	vmovdqa  _SHUF_DC00(%rip), SHUF_DC00

	mov	CTX, _CTX(%rsp)

loop0:
	lea     K256(%rip), TBL

	// Load first 16 dwords from two blocks
	VMOVDQ	0*32(INP),XTMP0
	VMOVDQ	1*32(INP),XTMP1
	VMOVDQ	2*32(INP),XTMP2
	VMOVDQ	3*32(INP),XTMP3

	// byte swap data
	vpshufb	BYTE_FLIP_MASK, XTMP0, XTMP0
	vpshufb	BYTE_FLIP_MASK, XTMP1, XTMP1
	vpshufb	BYTE_FLIP_MASK, XTMP2, XTMP2
	vpshufb	BYTE_FLIP_MASK, XTMP3, XTMP3

	// transpose data into high/low halves
	vperm2i128	$0x20, XTMP2, XTMP0, X0
	vperm2i128	$0x31, XTMP2, XTMP0, X1
	vperm2i128	$0x20, XTMP3, XTMP1, X2
	vperm2i128	$0x31, XTMP3, XTMP1, X3

last_block_enter:
	add	$64, INP
	mov	INP, _INP(%rsp)

	// schedule 48 input dwords, by doing 3 rounds of 12 each
	xor	SRND, SRND

.align 16
loop1:
	vpaddd	0*32(TBL, SRND), X0, XFER
	vmovdqa XFER, 0*32+_XFER(%rsp, SRND)
	FOUR_ROUNDS_AND_SCHED	_XFER + 0*32

	vpaddd	1*32(TBL, SRND), X0, XFER
	vmovdqa XFER, 1*32+_XFER(%rsp, SRND)
	FOUR_ROUNDS_AND_SCHED	_XFER + 1*32

	vpaddd	2*32(TBL, SRND), X0, XFER
	vmovdqa XFER, 2*32+_XFER(%rsp, SRND)
	FOUR_ROUNDS_AND_SCHED	_XFER + 2*32

	vpaddd	3*32(TBL, SRND), X0, XFER
	vmovdqa XFER, 3*32+_XFER(%rsp, SRND)
	FOUR_ROUNDS_AND_SCHED	_XFER + 3*32

	add	$4*32, SRND
	cmp	$3*4*32, SRND
	jb	loop1

loop2:
	// Do last 16 rounds with no scheduling
	vpaddd	0*32(TBL, SRND), X0, XFER
	vmovdqa XFER, 0*32+_XFER(%rsp, SRND)
	DO_4ROUNDS	_XFER + 0*32
	vpaddd	1*32(TBL, SRND), X1, XFER
	vmovdqa XFER, 1*32+_XFER(%rsp, SRND)
	DO_4ROUNDS	_XFER + 1*32
	add	$2*32, SRND

	vmovdqa	X2, X0
	vmovdqa	X3, X1

	cmp	$4*4*32, SRND
	jb	loop2

	mov	_CTX(%rsp), CTX
	mov	_INP(%rsp), INP

	addm    (4*0)(CTX),a
	addm    (4*1)(CTX),b
	addm    (4*2)(CTX),c
	addm    (4*3)(CTX),d
	addm    (4*4)(CTX),e
	addm    (4*5)(CTX),f
	addm    (4*6)(CTX),g
	addm    (4*7)(CTX),h

	cmp	_INP_END(%rsp), INP
	ja	done_hash

	// Do second block using previously scheduled results
	xor	SRND, SRND
.align 16
loop3:
	DO_4ROUNDS	 _XFER + 0*32 + 16
	DO_4ROUNDS	 _XFER + 1*32 + 16
	add	$2*32, SRND
	cmp	$4*4*32, SRND
	jb	loop3

	mov	_CTX(%rsp), CTX
	mov	_INP(%rsp), INP
	add	$64, INP

	addm    (4*0)(CTX),a
	addm    (4*1)(CTX),b
	addm    (4*2)(CTX),c
	addm    (4*3)(CTX),d
	addm    (4*4)(CTX),e
	addm    (4*5)(CTX),f
	addm    (4*6)(CTX),g
	addm    (4*7)(CTX),h

	cmp	_INP_END(%rsp), INP
	jb	loop0
	ja	done_hash

do_last_block:
	// do last block
	lea	K256(%rip), TBL

	VMOVDQ	0*16(INP),XWORD0
	VMOVDQ	1*16(INP),XWORD1
	VMOVDQ	2*16(INP),XWORD2
	VMOVDQ	3*16(INP),XWORD3

	vpshufb	X_BYTE_FLIP_MASK, XWORD0, XWORD0
	vpshufb	X_BYTE_FLIP_MASK, XWORD1, XWORD1
	vpshufb	X_BYTE_FLIP_MASK, XWORD2, XWORD2
	vpshufb	X_BYTE_FLIP_MASK, XWORD3, XWORD3

	jmp	last_block_enter

only_one_block:

	// load initial digest
	mov	(4*0)(CTX),a
	mov	(4*1)(CTX),b
	mov	(4*2)(CTX),c
	mov	(4*3)(CTX),d
	mov	(4*4)(CTX),e
	mov	(4*5)(CTX),f
	mov	(4*6)(CTX),g
	mov	(4*7)(CTX),h

	vmovdqa	PSHUFFLE_BYTE_FLIP_MASK(%rip), BYTE_FLIP_MASK
	vmovdqa	_SHUF_00BA(%rip), SHUF_00BA
	vmovdqa	_SHUF_DC00(%rip), SHUF_DC00

	mov	CTX, _CTX(%rsp)
	jmp	do_last_block

done_hash:
	mov	_RSP(%rsp), %rax
	addq	$STACK_SIZE, %rsp
	SET_TS_OR_POP_YMM_REGISTERS
	mov	%rax, %rsp

	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbp
	popq	%rbx
	ret
SET_SIZE(sha256_transform_avx2)

.text
.align 64
K256:
	.long	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5
	.long	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5
	.long	0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5
	.long	0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5
	.long	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3
	.long	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3
	.long	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174
	.long	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174
	.long	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc
	.long	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc
	.long	0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da
	.long	0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da
	.long	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7
	.long	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7
	.long	0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967
	.long	0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967
	.long	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13
	.long	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13
	.long	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85
	.long	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85
	.long	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3
	.long	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3
	.long	0xd192e819,0xd6990624,0xf40e3585,0x106aa070
	.long	0xd192e819,0xd6990624,0xf40e3585,0x106aa070
	.long	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5
	.long	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5
	.long	0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3
	.long	0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3
	.long	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208
	.long	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208
	.long	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	.long	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2

PSHUFFLE_BYTE_FLIP_MASK:
	.octa 0x0c0d0e0f08090a0b0405060700010203,0x0c0d0e0f08090a0b0405060700010203

// shuffle xBxA -> 00BA
_SHUF_00BA:
	.octa 0xFFFFFFFFFFFFFFFF0b0a090803020100,0xFFFFFFFFFFFFFFFF0b0a090803020100

// shuffle xDxC -> DC00
_SHUF_DC00:
	.octa 0x0b0a090803020100FFFFFFFFFFFFFFFF,0x0b0a090803020100FFFFFFFFFFFFFFFF
#endif
