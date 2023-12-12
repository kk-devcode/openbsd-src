/*	$OpenBSD: i82093reg.h,v 1.8 2022/12/01 00:26:15 guenther Exp $	*/
/* 	$NetBSD: i82093reg.h,v 1.1 2003/02/26 21:26:10 fvdl Exp $ */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by RedBack Networks Inc.
 *
 * Author: Bill Sommerfeld
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Typically, the first apic lives here.
 */
#define IOAPIC_BASE_DEFAULT	0xfec00000

/*
 * Memory-space registers.
 */

/*
 * The externally visible registers are all 32 bits wide;
 * store the register number of interest in IOAPIC_REG, and store/fetch
 * the real value in IOAPIC_DATA.
 */
#define	IOAPIC_REG		0x0000
#define IOAPIC_DATA		0x0010

/*
 * Internal I/O APIC registers.
 */

#define IOAPIC_ID		0x00

#define 	IOAPIC_ID_SHIFT		24
#define		IOAPIC_ID_MASK		0x0f000000

/* Version, and maximum interrupt pin number. */
  
#define IOAPIC_VER		0x01

#define		IOAPIC_VER_SHIFT		0
#define		IOAPIC_VER_MASK			0x000000ff

#define		IOAPIC_MAX_SHIFT	       	16
#define		IOAPIC_MAX_MASK	       	0x00ff0000

/*
 * Arbitration ID.  Same format as IOAPIC_ID register.
 */
#define IOAPIC_ARB		0x02

/*
 * Redirection table registers.
 */

#define IOAPIC_REDHI(pin)	(0x11 + ((pin)<<1))
#define IOAPIC_REDLO(pin)	(0x10 + ((pin)<<1))

#define IOAPIC_REDHI_DEST_SHIFT		24	   /* destination. */
#define IOAPIC_REDHI_DEST_MASK		0xff000000

#define IOAPIC_REDLO_MASK		0x00010000 /* 0=enabled; 1=masked */

#define IOAPIC_REDLO_LEVEL		0x00008000 /* 0=edge, 1=level */
#define IOAPIC_REDLO_RIRR		0x00004000 /* remote IRR; read only */
#define IOAPIC_REDLO_ACTLO		0x00002000 /* 0=act. hi; 1=act. lo */
#define IOAPIC_REDLO_DELSTS		0x00001000 /* 0=idle; 1=send pending */
#define IOAPIC_REDLO_DSTMOD		0x00000800 /* 0=physical; 1=logical */

#define IOAPIC_REDLO_DEL_MASK		0x00000700 /* del. mode mask */
#define IOAPIC_REDLO_DEL_SHIFT		8

#define IOAPIC_REDLO_DEL_FIXED		0
#define IOAPIC_REDLO_DEL_LOPRI		1
#define IOAPIC_REDLO_DEL_SMI		2
#define IOAPIC_REDLO_DEL_NMI		4
#define IOAPIC_REDLO_DEL_INIT		5
#define IOAPIC_REDLO_DEL_EXTINT		7

#define IOAPIC_REDLO_VECTOR_MASK	0x000000ff /* delivery vector */

#define IMCR_ADDR		0x22
#define IMCR_DATA		0x23

#define IMCR_REGISTER		0x70
#define		IMCR_PIC	0x00
#define 	IMCR_APIC	0x01

#ifdef _KERNEL

#include <machine/codepatch.h>

#define ioapic_asm_ack(num) 					 \
	CODEPATCH_START						;\
	movl	$0,(local_apic+LAPIC_EOI)(%rip)			;\
	CODEPATCH_END(CPTAG_EOI)


#ifdef MULTIPROCESSOR

#ifdef notyet
#define ioapic_asm_lock(num) \
	movl	$1,%esi						;\
77:								\
	xchgl	%esi,PIC_LOCK(%rdi)				;\
	testl	%esi,%esi					;\
	jne	77b

#define ioapic_asm_unlock(num) \
	movl	$0,PIC_LOCK(%rdi)
#else
#define ioapic_asm_lock(num)
#define ioapic_asm_unlock(num)
#endif
	
#else

#define ioapic_asm_lock(num)
#define ioapic_asm_unlock(num)

#endif	/* MULTIPROCESSOR */


#define ioapic_mask(num) \
	movq	IS_PIC(%r14),%rdi				;\
	ioapic_asm_lock(num)					;\
	movl	IS_PIN(%r14),%esi				;\
	leaq	0x10(%rsi,%rsi,1),%rsi				;\
	movq	IOAPIC_SC_REG(%rdi),%r15			;\
	movl	%esi, (%r15)					;\
	movq	IOAPIC_SC_DATA(%rdi),%r15			;\
	movl	(%r15),%esi					;\
	orl	$IOAPIC_REDLO_MASK,%esi				;\
	andl	$~IOAPIC_REDLO_RIRR,%esi			;\
	movl	%esi,(%r15)					;\
	ioapic_asm_unlock(num)

#define ioapic_unmask(num) \
	cmpq	$IREENT_MAGIC,IF_ERR(%rsp)			;\
	jne	79f						;\
	movq	IS_PIC(%r14),%rdi				;\
	ioapic_asm_lock(num)					;\
	movl	IS_PIN(%r14),%esi				;\
	leaq	0x10(%rsi,%rsi,1),%rsi				;\
	movq	IOAPIC_SC_REG(%rdi),%r15			;\
	movq	IOAPIC_SC_DATA(%rdi),%r13			;\
	movl	%esi, (%r15)					;\
	movl	(%r13),%r12d					;\
	andl	$~IOAPIC_REDLO_MASK,%r12d			;\
	andl	$~IOAPIC_REDLO_RIRR,%r12d			;\
	movl	%esi,(%r15)					;\
	movl	%r12d,(%r13)					;\
	ioapic_asm_unlock(num)					;\
79:

#endif
