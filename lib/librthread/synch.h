/*	$OpenBSD: synch.h,v 1.9 2023/11/08 15:51:28 cheloha Exp $ */
/*
 * Copyright (c) 2017 Martin Pieuchot
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/atomic.h>
#include <sys/time.h>
#include <sys/futex.h>

static inline int
_wake(volatile uint32_t *p, int n)
{
	return futex(p, FUTEX_WAKE, n, NULL, NULL);
}

static inline int
_twait(volatile uint32_t *p, int val, clockid_t clockid, const struct timespec *abs)
{
	struct timespec rel;
	int saved_errno = errno;
	int error;

	if (abs == NULL) {
		error = futex(p, FUTEX_WAIT, val, NULL, NULL);
		if (error == -1) {
			error = errno;
			errno = saved_errno;
		}
		return error;
	}

	if (!timespecisvalid(abs) || clock_gettime(clockid, &rel))
		return EINVAL;

	rel.tv_sec = abs->tv_sec - rel.tv_sec;
	if ((rel.tv_nsec = abs->tv_nsec - rel.tv_nsec) < 0) {
		rel.tv_sec--;
		rel.tv_nsec += 1000000000;
	}
	if (rel.tv_sec < 0)
		return ETIMEDOUT;

	error = futex(p, FUTEX_WAIT, val, &rel, NULL);
	if (error == -1) {
		error = errno;
		errno = saved_errno;
	}
	return error;
}

static inline int
_requeue(volatile uint32_t *p, int n, int m, volatile uint32_t *q)
{
	return futex(p, FUTEX_REQUEUE, n, (void *)(long)m, q);
}
