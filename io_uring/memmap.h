#ifndef IO_URING_MEMMAP_H
#define IO_URING_MEMMAP_H

#define IORING_MAP_OFF_PARAM_REGION		0x20000000ULL
#define IORING_MAP_OFF_ZCRX_REGION		0x30000000ULL

/**
 * Pins user pages in memory for the given user buffer and length.
 * Returns an array of pinned pages.
 */
struct page **io_pin_pages(unsigned long ubuf, unsigned long len, int *npages);

#ifndef CONFIG_MMU
/**
 * Returns the mmap capabilities for io_uring when MMU is not configured.
 */
unsigned int io_uring_nommu_mmap_capabilities(struct file *file);
#endif

/**
 * Retrieves an unmapped area for io_uring, ensuring proper alignment and safety.
 */
unsigned long io_uring_get_unmapped_area(struct file *file, unsigned long addr,
                     unsigned long len, unsigned long pgoff,
                     unsigned long flags);

/**
 * Handles mmap requests for io_uring, mapping regions into user space.
 */
int io_uring_mmap(struct file *file, struct vm_area_struct *vma);

/**
 * Frees a mapped memory region, unpins or releases pages, and cleans up resources.
 */
void io_free_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr);

/**
 * Creates a mapped memory region, either user-backed or kernel-backed.
 */
int io_create_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr,
             struct io_uring_region_desc *reg,
             unsigned long mmap_offset);

/**
 * Safely creates a mapped memory region, ensuring atomicity with a temporary region.
 */
int io_create_region_mmap_safe(struct io_ring_ctx *ctx,
                struct io_mapped_region *mr,
                struct io_uring_region_desc *reg,
                unsigned long mmap_offset);

/**
 * Retrieves the pointer for a mapped region.
 */
static inline void *io_region_get_ptr(struct io_mapped_region *mr)
{
    return mr->ptr;
}

/**
 * Checks if a mapped region is set (i.e., has pages allocated).
 */
static inline bool io_region_is_set(struct io_mapped_region *mr)
{
    return !!mr->nr_pages;
}

#endif
