/*
 * Copyright(c) 2006 to 2018 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef NN_FREELIST_H
#define NN_FREELIST_H

#include "dds/ddsrt/atomics.h"
#include "dds/ddsrt/sync.h"

#define FREELIST_SIMPLE 1
#define FREELIST_ATOMIC_LIFO 2
#define FREELIST_DOUBLE 3

#define FREELIST_TYPE FREELIST_DOUBLE

#if 0
#if HAVE_ATOMIC_LIFO
#define FREELIST_TYPE FREELIST_ATOMIC_LIFO
#else
#define FREELIST_TYPE FREELIST_SIMPLE
#endif
#endif

#if FREELIST_TYPE == FREELIST_ATOMIC_LIFO

struct nn_freelist {
  ddsrt_atomic_lifo_t x;
  ddsrt_atomic_uint32_t count;
  uint32_t max;
  off_t linkoff;
};

#elif FREELIST_TYPE == FREELIST_DOUBLE

#define NN_FREELIST_NPAR 4
#define NN_FREELIST_NPAR_LG2 2
#define NN_FREELIST_MAGSIZE 256

struct nn_freelistM {
  void *x[NN_FREELIST_MAGSIZE];
  void *next;
};

struct nn_freelist1 {
  ddsrt_mutex_t lock;
  uint32_t count;
  struct nn_freelistM *m;
};

struct nn_freelist {
  struct nn_freelist1 inner[NN_FREELIST_NPAR];
  ddsrt_atomic_uint32_t cc;
  ddsrt_mutex_t lock;
  struct nn_freelistM *mlist;
  struct nn_freelistM *emlist;
  uint32_t count;
  uint32_t max;
  off_t linkoff;
};

#endif

void nn_freelist_init (struct nn_freelist *fl, uint32_t max, off_t linkoff);
void nn_freelist_fini (struct nn_freelist *fl, void (*free) (void *elem));
bool nn_freelist_push (struct nn_freelist *fl, void *elem);
void *nn_freelist_pushmany (struct nn_freelist *fl, void *first, void *last, uint32_t n);
void *nn_freelist_pop (struct nn_freelist *fl);

#endif /* NN_FREELIST_H */