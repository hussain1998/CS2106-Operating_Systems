/*************************************
* Lab 4
* Name:
* Student No:
* Lab Group:
*************************************/

#include "shmheap.h"

shmheap_memory_handle shmheap_create(const char *name, size_t len) {
    /* TODO */

    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRWXU);
    ftruncate(fd, len);
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    //Bookkeeping. Adding header to the start of shared memory
    shmheap_header header = {len, 1, NULL};
    memcpy(ptr, &header, sizeof(shmheap_header));

    shmheap_memory_handle mem;// = {ptr,len};
    mem.ptr = ptr;
    mem.len = len;
    return mem;

}

shmheap_memory_handle shmheap_connect(const char *name) {
    /* TODO */
    int fd = shm_open(name, O_RDWR,S_IRWXU);

    //Using fstat to get length
    // struct stat statbuff;
    // fstat(fd, &statbuff);
    // size_t len = statbuff.st_size;

    shmheap_header *header_ptr = mmap(NULL, sizeof(shmheap_header), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    size_t len = header_ptr->len;
    void *ptr = mremap(header_ptr, sizeof(shmheap_header), len, MREMAP_MAYMOVE);
    
    shmheap_memory_handle mem;// = {ptr, statbuff.st_size};
    mem.ptr = ptr;
    mem.len = len;
    return mem;
}

void shmheap_disconnect(shmheap_memory_handle mem) {
    /* TODO */
    munmap(mem.ptr, mem.len);
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    /* TODO */
    shmheap_disconnect(mem);
    //munmap(mem.ptr, mem.len);
    shm_unlink(name);
}

void *shmheap_underlying(shmheap_memory_handle mem) {
    /* TODO */
}

void *shmheap_alloc(shmheap_memory_handle mem, size_t sz) {
    /* TODO */
    shmheap_header *header_ptr = mem.ptr;
    return header_ptr + 1; //After the header
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
    shmheap_object_handle handle;// = {ptr - mem.ptr};
    handle.offset = ptr - mem.ptr;
    return handle;
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    /* TODO */
    return mem.ptr + hdl.offset;
}
