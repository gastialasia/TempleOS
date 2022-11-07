#include <stdlib.h>
#include <testUtil.h>
#include <programs.h>

static int testRead(int arc, char arv[6][21]){
    fd * fileDescriptor = createFd();
    if (openPipe(fileDescriptor, 101, 1)==-1) {
        printf("Error opening pipe");
        putchar('\n');
        exit();
    }
    printf("Read process blocked waiting for something to be written in the pipe\n\n");
    char buffer[150];
    int read = pipeRead(fileDescriptor, buffer);
    closeFd(fileDescriptor);
    if (read>0){
        strcat(buffer, "and I'm reading from read process\n");
        printf(buffer);
        putchar('\n');
    }
    exit();
    return 0;
}

static int testWrite(int arc, char arv[6][21]){
    fd * fileDescriptor = createFd();
    openPipe(fileDescriptor, 101, 0);
    pipeWrite(fileDescriptor, "I'm writing on the pipe from write process ");
    closeFd(fileDescriptor);
    exit();
    return 0;
}

void testPipe(int arc, char arv[6][21]){
    char args[6][21];
    printf("CREATING READ PROCESS...\n\n");
    strcpy(args[0],"_readPipe");
    createProcess((uint64_t)testRead, 2, 1, args, NULL, NULL);
    sleep(1000);
    ps();
    printf("\nSee pipe list below:\n\n");
    pipeListProgram(0, NULL);
    putchar('\n');
    sleep(2000);
    printf("CREATING WRITE PROCESS...\n\n");
    strcpy(args[0],"_writePipe");
    createProcess((uint64_t)testWrite, 2, 1, args, NULL, NULL);
    exit();
}