#include <stdlib.h>
#include "util.h"

typedef struct {
    void *mem = nullptr;
    size_t sz = 0;
    size_t allocSz = 0;
    const size_t chunkSz = 0xffff;
    size_t cSz = 0;
} __basic_vec;

typedef __basic_vec vector;

#define _get_vptr(v) (v).__basic_vec
#define vec_push(v, val) _vec_push((v), (void*)&(val), sizeof((val)))
#define vec_pop(v, ty) *(ty*)_vec_pop((v)); vec_clean((v))
#define vec(ty) _c_vec(sizeof((ty)))

extern __basic_vec _c_vec(size_t sz);
extern void _vec_push(__basic_vec *v, void *val, size_t val_size);
extern void vec_free(__basic_vec *v);
extern void vec_remove(__basic_vec *v, size_t idx);
extern void *_vec_pop(__basic_vec *v);
extern void vec_clean(__basic_vec *v);

#define vec_push(v, val) _vec_push(v, (void*)&(val), sizeof((val)))