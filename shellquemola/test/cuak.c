#include <elf.h>
#include <elf.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef void (*ptr_t) (int, char**);

void* alloc_ex_memory(int fd) {
    struct stat s;
    fstat(fd, &s);
    void * ptr = mmap(0, s.st_size, PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_PRIVATE | MAP_ANONYMOUS, fd, 0);
    if (ptr == (void *)-1) {
        perror("mmap");
        return NULL;
    }
    return ptr;
}

void* load_ex_file(const char* elfFile) {
    Elf64_Ehdr header;
    void * ptr;
    FILE* file = fopen(elfFile, "rb");
    if(file) {
        fread(&header, 1, sizeof(header), file);

        if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
            ptr = alloc_ex_memory(fileno(file));
            printf("PTR AT -> %p\n", ptr);
            printf("Entry at -> %lx\n", header.e_entry+256);
            ptr = (header.e_entry + 256);
        } else {
            ptr = NULL;
            }

        fclose(file);
            return ptr;
    }
    return NULL;
}

int main(int argc, char** argv) {
    ptr_t func = load_ex_file(argv[1]);
    printf("POINTER AT: %p\n", func);
    getchar();
    func(argc, argv);
    return 0;
}
