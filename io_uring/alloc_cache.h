#ifndef IOU_ALLOC_CACHE_H
#define IOU_ALLOC_CACHE_H

#include <linux/io_uring_types.h>

/*
 * Don't allow the cache to grow beyond this size.
 */
#define IO_ALLOC_CACHE_MAX	128

/**
 * Frees all entries in the allocation cache and deallocates the cache itself.
 */
void io_alloc_cache_free(struct io_alloc_cache *cache,
			 void (*free)(const void *));

/**
 * Initializes the allocation cache with the specified parameters.
 * Returns false if the cache was initialized properly, true otherwise.
 */
bool io_alloc_cache_init(struct io_alloc_cache *cache,
			 unsigned max_nr, unsigned int size,
			 unsigned int init_bytes);

/**
 * Allocates a new object for the allocation cache, optionally clearing
 * a portion of the memory if specified.
 */
void *io_cache_alloc_new(struct io_alloc_cache *cache, gfp_t gfp);

/**
 * Attempts to add an entry to the allocation cache. Returns true if the entry
 * was successfully added, false otherwise.
 */
static inline bool io_alloc_cache_put(struct io_alloc_cache *cache,
				      void *entry)
{
	if (cache->nr_cached < cache->max_cached) {
		if (!kasan_mempool_poison_object(entry))
			return false;
		cache->entries[cache->nr_cached++] = entry;
		return true;
	}
	return false;
}

/**
 * Retrieves an entry from the allocation cache. Returns the entry if available,
 * or NULL if the cache is empty.
 */
static inline void *io_alloc_cache_get(struct io_alloc_cache *cache)
{
	if (cache->nr_cached) {
		void *entry = cache->entries[--cache->nr_cached];

		/*
		 * If KASAN is enabled, always clear the initial bytes that
		 * must be zeroed post alloc, in case any of them overlap
		 * with KASAN storage.
		 */
#if defined(CONFIG_KASAN)
		kasan_mempool_unpoison_object(entry, cache->elem_size);
		if (cache->init_clear)
			memset(entry, 0, cache->init_clear);
#endif
		return entry;
	}

	return NULL;
}

/**
 * Allocates an object from the allocation cache, or creates a new one if the
 * cache is empty.
 */
static inline void *io_cache_alloc(struct io_alloc_cache *cache, gfp_t gfp)
{
	void *obj;

	obj = io_alloc_cache_get(cache);
	if (obj)
		return obj;
	return io_cache_alloc_new(cache, gfp);
}

/**
 * Frees an object, either returning it to the allocation cache or deallocating
 * it if the cache is full.
 */
static inline void io_cache_free(struct io_alloc_cache *cache, void *obj)
{
	if (!io_alloc_cache_put(cache, obj))
		kfree(obj);
}

#endif
