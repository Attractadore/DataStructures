#include "DummyCache.h"

#include <assert.h>
#include <stdlib.h>

struct DummyCache_T {
  size_t key_size;
};

DummyCache *dummyCacheAlloc(const size_t capacity, const size_t key_size) {
  assert(capacity);

  DummyCache *const cache =
      calloc(sizeof(*cache) + key_size, sizeof(unsigned char));

  return cache;
}

void dummyCacheFree(DummyCache *const cache) { free(cache); }

bool dummyCacheContains(DummyCache const *const cache, void const *const key) {
  assert(cache && key);

  return false;
}

CachePolicyAddResult dummyCacheAdd(DummyCache *const cache,
                                   void const *const key, void *const replace) {
  assert(cache && key && replace);

  return CACHE_POLICY_ADD_NO_REPLACE;
}
