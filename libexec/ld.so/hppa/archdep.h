/*	$OpenBSD: archdep.h,v 1.19 2023/11/18 16:26:17 deraadt Exp $	*/

/*
 * Copyright (c) 2004 Michael Shalayeff
 * Copyright (c) 1998 Per Fogelstrom, Opsycon AB
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

#ifndef _HPPA_ARCHDEP_H_
#define _HPPA_ARCHDEP_H_

#define	RELOC_TAG	DT_RELA
#define	MACHID		EM_PARISC	/* ELF e_machine ID value checked */

#include <sys/exec_elf.h>

Elf_Addr _dl_md_plabel(Elf_Addr, Elf_Addr *);


/* Only used in lib/csu/boot.h */
#ifdef RCRT0

static inline void
RELOC_JMPREL(const Elf_RelA *r, const Elf_Sym *s, Elf_Addr *p, unsigned long v,
    Elf_Addr pltgot)
{
	if (ELF_R_TYPE(r->r_info) == RELOC_IPLT) {
		p[0] = v + s->st_value + r->r_addend;
		p[1] = pltgot;
	} else {
		_csu_abort();
	}
}

static inline void
RELOC_DYN(const Elf_RelA *r, const Elf_Sym *s, Elf_Addr *p, unsigned long v)
{
	if (ELF_R_TYPE(r->r_info) == RELOC_DIR32) {
		if (ELF_R_SYM(r->r_info) != 0)
			*p = v + s->st_value + r->r_addend;
		else
			*p = v + r->r_addend;
	} else if (ELF_R_TYPE(r->r_info) == RELOC_PLABEL32) {
		*p = v + s->st_value + r->r_addend;
	} else {
		_csu_abort();
	}
}

#endif /* RCRT0 */
#endif /* _HPPA_ARCHDEP_H_ */
