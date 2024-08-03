#include "vec.h"

#define _sysalloc malloc;
#define nullptr NULL

static void ZeroMem(void *m, size_t sz) {
    memset(m, sz, 0);
}

/**
 * 
 * v_ialloc
 * 
 * Initial vector allocation. Allocates
 * one chunk for the given vector
 * 
 */
void v_ialloc(__basic_vec *v) {
    if (v == NULL || v->mem != NULL) return;
    const size_t i = v->cSz;
    v->allocSz += v->chunkSz;
    v->mem = malloc(v->allocSz * i);
    ZeroMem(v->mem, v->sz * i);
}

void v_dalloc_chunk(__basic_vec *v) {
    if (v == NULL) return;

    if (v->mem == nullptr)
        return (void) v_ialloc(v);

    //dealloc chunk
    v->allocSz -= v->chunkSz;
    const size_t n_alloc = v->allocSz * v->cSz;
    void *temp = malloc(n_alloc);
    ZeroMem(temp, n_alloc);
    memcpy(temp, v->mem, v->sz * v->cSz);
    free(v->mem);
    v->mem = temp; //swap pointers
}

void v_alloc_chunk(__basic_vec *v) {
    if (v == NULL) return;

    //ialloc vector if not
    if (v->mem == nullptr)
        v_ialloc(v);

    v->allocSz += v->chunkSz;

    //allocate more memory and copy stuff over
    const size_t n_alloc = v->allocSz * v->cSz;
    void *temp = malloc(n_alloc);
    ZeroMem(temp, n_alloc);
    memcpy(temp, v->mem, v->sz * v->cSz);
    free(v->mem);
    v->mem = temp; //swap pointers
}

void _vec_push(__basic_vec *v, void *val, size_t val_size) {
    if (val_size <= 0 || val_size != v->cSz || val == nullptr || v == nullptr)
        return;

    if (v->mem == nullptr)
        v_ialloc(v);

    byte *cur = (byte*) (v->mem + v->sz), *v_ptr = (byte*)val;

    v->sz ++;
    while (v->sz > v->allocSz)
        v_alloc_chunk(v);

    //kinda like memcpy
    while (val_size--)
        *cur = *v_ptr++;
}

__basic_vec _c_vec(size_t sz) {
    if (sz <= 0) {
        //aw
        return (__basic_vec) {};
    }

    return (__basic_vec) {
        .cSz = sz
    };
}

//free and clears a vector
void vec_free(__basic_vec *v) {
    free(v->mem);
    v->mem = NULL;
    v->allocSz = v->sz = 0;
}

void vec_clean(__basic_vec *v) {
    if (v == nullptr)
        return;

    const size_t lastEle = (v->sz * v->cSz);
    ZeroMem(v->mem + lastEle, v->cSz);

    //dealloc chunk if necessary
    if (v->sz < v->allocSz - v->chunkSz)
        v_dalloc_chunk(v);
};

void *_vec_pop(__basic_vec *v) {
    if (v == nullptr)
        return;

    v->sz--;

    //remove element
    /*const size_t lastEle = (v->sz * v->cSz);
    void *res = malloc(v->cSz);
    memcpy(res, v->mem + lastEle, v->cSz);
    ZeroMem(v->mem + lastEle, v->cSz);

    //dealloc chunk if necessary
    if (v->sz < v->allocSz - v->chunkSz)
        v_dalloc_chunk(v);*/

    return (void*) (v->mem + (v->sz * v->cSz)); //just return a pointer to the last element and "techinically" remove it
}