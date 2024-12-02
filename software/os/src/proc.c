#include <debug.h>
#include <proc.h>
#include <fs.h>

#define MAX_NR_PROC 4


PCB *current = NULL;
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};



Context* schedule(Context *prev) {
  return NULL;
}


void init_proc() {
  Log("Initializing processes...");
  //load program here
  user_naive_load("/bin/menu");
}


int execve(const char *filename, char *const argv[], char *const envp[]) {
    // check if the file exists
    int fd = fs_open(filename, 0, 0);
    if(fd == -1) {
        return -2;
    }
    fs_close(fd);
    // load the file
    user_ctx_load(current, filename, argv, envp);
    current = &pcb[0];
    yield();
    return 0;
}