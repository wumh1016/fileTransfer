#include <stdio.h>

#define FILE_NAME_SIZE  48

typedef struct transfer{
    char    fileName[FILE_NAME_SIZE];
    int     fileSize;
    int     filePieces;
    
}transfer_t;