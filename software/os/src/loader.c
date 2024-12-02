#include <elf.h>
#include <fs.h>
#include <proc.h>
#include <context.h>
#include <common.h>

#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr

static uintptr_t elf_load(const char *filename) {

    Elf_Ehdr elf_h;
    
    // Open the file
    int fd = fs_open(filename, 0, 0);
    assert(fd != -1);

    // Read the ELF header
    fs_read(fd, &elf_h, sizeof(Elf_Ehdr));
    assert(*(uint32_t*)elf_h.e_ident == 0x464C457F);
    assert(elf_h.e_machine == EM_RISCV);

    Elf_Phdr elf_ph;

    // Read the program headers
    for (int i = 0; i < elf_h.e_phnum; i++) {
        fs_lseek(fd, elf_h.e_phoff + i * elf_h.e_phentsize, SEEK_SET);
        fs_read(fd, &elf_ph, sizeof(Elf_Phdr));
        if (elf_ph.p_type == PT_LOAD) {
            fs_lseek(fd, elf_ph.p_offset, SEEK_SET);
            fs_read(fd, (void*)elf_ph.p_vaddr, elf_ph.p_filesz);
            if (elf_ph.p_filesz < elf_ph.p_memsz) {
                memset((void*)(elf_ph.p_vaddr + elf_ph.p_filesz), 0, elf_ph.p_memsz - elf_ph.p_filesz);
            }
        }
    }

    fs_close(fd);

    asm volatile("fence.i");

    return elf_h.e_entry;
}

void user_naive_load(const char *filename) {
    Log("Loading %s...", filename);
    uintptr_t entry = elf_load(filename);
    Log("Jump to entry = %p", (void*)entry);
    ((void(*)())entry) ();
}


void kernel_ctx_load(PCB *pcb, void (*entry)(void *), void *arg) {
    Area karea;
    karea.start = &(pcb->cp);
    karea.end = karea.start + STACK_SIZE;
    pcb->cp = kcontext(karea, entry, arg);
}

void user_ctx_load(PCB *pcb, const char *filename, char *const argv[], char *const envp[]){
    Area karea;
    int argv_num = 0;
    int envp_num = 0;
    if(argv){
        for(; argv[argv_num] != NULL; argv_num++);
    }
    if(envp){
        for(; envp[envp_num] != NULL; envp_num++);
    }
    
    // alloc user stack for program
    uintptr_t ustack = (uintptr_t)new_page(8);
    char ** argv_ptr = (char**)ustack;
    argv_ptr[argv_num] = NULL;
    char ** envp_ptr = (char**)ustack + (argv_num + 1);
    envp_ptr[envp_num] = NULL;
    char* sp = (char*)ustack + STACK_SIZE;
          
    // load envp and argv
    for(int i = 0; i < envp_num; i++){
        size_t len = strlen(envp[i]) + 1;
        sp -= len;
        strcpy(sp, envp[i]);
        envp_ptr[i] = sp;
    }
    
    for(int i = 0; i < argv_num; i++){
        size_t len = strlen(argv[i]) + 1;
        sp -= len;
        strcpy(sp, argv[i]);
        argv_ptr[i] = sp;
    }
    uintptr_t* p = (uintptr_t *)ustack;
    p--;
    *p = argv_num;
    Log("Loading %s...", filename);
    uintptr_t entry = elf_load(filename);
    Log("Load finish. Entry = %lx", filename, entry);
    karea.start = &(pcb->cp);
    karea.end = karea.start + STACK_SIZE;
    pcb->cp = ucontext(NULL, karea, (void*)entry);

    // set the stack pointer of the user program by return value 
    pcb->cp->SYS_RET = (uintptr_t)p;
}


