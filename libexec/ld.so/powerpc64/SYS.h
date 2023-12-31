/*	$OpenBSD: SYS.h,v 1.4 2023/12/11 22:29:24 deraadt Exp $ */

/*
 * Copyright (c) 1999 Dale Rahn
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/syscall.h>
#include <machine/asm.h>

#define PINSYSCALL(sysno, label)				\
	.pushsection .openbsd.syscalls,"",@progbits		;\
	.p2align 2						;\
	.long label						;\
	.long sysno						;\
	.popsection

#define	DL_SYSCALL(n)						\
ENTRY(_dl_##n)							\
	RETGUARD_SETUP(_dl_##n, %r11)				;\
	li	%r0, SYS_##n					;\
99:	sc							;\
	PINSYSCALL(SYS_##n, 99b)				;\
	cmpdi	%r0, 0						;\
	beq .L_end##n						;\
	neg	%r3, %r3					;\
.L_end##n:							;\
	RETGUARD_CHECK(_dl_##n, %r11)				;\
	blr
