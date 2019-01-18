#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../common/mmapFile.h"
#include "../common/transfer.h"

//0xAF 0xAE -- 传送文件信息
//0xAF 0xAF -- 传送文件数据

#define BUF_SIZE 1024
#define FILENAME "file.txt"

void *send_fileinfo(void *arg);
void *send_filedata(void *arg);
int socket_and_connect();

static int pthreads = 1; //线程数

int main(int argc, char const *argv[])
{
    pthread_t pthread_;
    pthread_create(&pthread_, NULL, send_fileinfo, NULL);
    pthread_join(pthread_, NULL);
    int pthreadNo = 0;
    pthread_create(&pthread_, NULL, send_filedata, (void*)&pthreadNo);
    pthread_join(pthread_, NULL);

    /*     memset(&transfer_, 0, sizeof(transfer_t));

    int pthreads = 1;
    int i = 0;
    int arr[4] = {0};
    for (i = 0; i < pthreads; i++)
    {
        arr[i] = i;
    }
    pthread_t pthread_id[4];
    for (i = 0; i < pthreads; i++)
    {
        pthread_create(&pthread_id[i], NULL, send_filedata, (void *)&arr[i]);
    }
    for (i = 0; i < pthreads; i++)
    {
        pthread_join(pthread_id[i], NULL);
    } */
    return 0;
}

void *send_fileinfo(void *arg)
{
    int ret = 0;
    int fd = socket_and_connect();
    uint8_t buf[BUF_SIZE] = {0};

    int fileSize = getFileSize(FILENAME);
    /*     transfer_.fileSize = fileSize;
    transfer_.filePieces = get_piece_num(fileSize); */

    int fileNameSize = strlen(FILENAME);
    //同步字
    buf[HEAD_POS_SYNC_WORD] = 0xAF;
    buf[HEAD_POS_SYNC_WORD + 1] = 0xAE;
    //所有数据大小==fileSize
    buf[HEAD_POS_TOTAL_SIZE] = fileSize >> 8;
    buf[HEAD_POS_TOTAL_SIZE + 1] = (fileSize & 0xff);
    /* 	// 所有分片的数量
	buf[HEAD_POS_TOTAL_PIECES] = 0 >> 8;
	buf[HEAD_POS_TOTAL_PIECES + 1] = (0 & 0xff);
	// 分片编号，从0开始
	buf[HEAD_POS_P_INDEX] = 0 >> 8;
	buf[HEAD_POS_P_INDEX + 1] = (0 & 0xff); */
    // 文件名字节数
    buf[HEAD_POS_P_LENGTH] = fileNameSize >> 8;
    buf[HEAD_POS_P_LENGTH + 1] = (fileNameSize & 0xff);

    memcpy(buf + HAND_TOTAL_SIZE, FILENAME, fileNameSize);

    send(fd, buf, BUF_SIZE, 0);

    close(fd);
}

void *send_filedata(void *arg)
{
    int pthread_number = *(int *)arg; //线程编号--从0开始
    int ret = 0;
    int fd = socket_and_connect();
    uint8_t buf[HAND_TOTAL_SIZE + 1] = {0}; //12+1
    transfer_t transfer_;
    int fileSize = getFileSize(FILENAME);
    transfer_.fileSize = fileSize;
    transfer_.filePieces = get_piece_num(fileSize);
    char *begin_ = mmap_file(FILENAME, fileSize);

    int n = 0;
    int pieceNo = pthread_number; //某一线程起始编号为线程编号
    //对应本线程将要处理的编号 --> 总线程数 * n + 线程编号
    while (pieceNo < transfer_.filePieces)
    {
        //同步字
        buf[HEAD_POS_SYNC_WORD] = 0xAF;
        buf[HEAD_POS_SYNC_WORD + 1] = 0xAF; //0xAF 0xAF == 发送数据部分
        //所有数据大小==fileSize
        buf[HEAD_POS_TOTAL_SIZE] = transfer_.fileSize >> 8;
        buf[HEAD_POS_TOTAL_SIZE + 1] = (transfer_.fileSize & 0xff);
        // 所有分片的数量
        buf[HEAD_POS_TOTAL_PIECES] = transfer_.filePieces >> 8;
        buf[HEAD_POS_TOTAL_PIECES + 1] = (transfer_.filePieces & 0xff);
        // 分片编号，从0开始
        transfer_.pieceNo = pieceNo;
        buf[HEAD_POS_P_INDEX] = transfer_.pieceNo >> 8;
        buf[HEAD_POS_P_INDEX + 1] = (transfer_.pieceNo & 0xff);
        // 分片数据的大小
        transfer_.pieceSize = get_single_piece_size(transfer_.pieceNo, transfer_.filePieces, transfer_.fileSize);
        buf[HEAD_POS_P_LENGTH] = transfer_.pieceSize >> 8;
        buf[HEAD_POS_P_LENGTH + 1] = (transfer_.pieceSize & 0xff);
        //客户端线程数
        transfer_.clientPthreads = pthreads;
        buf[CLIENT_PTHREAD_SIZE] = transfer_.clientPthreads >> 8;
        buf[CLIENT_PTHREAD_SIZE + 1] = (transfer_.clientPthreads & 0xff);

        send(fd, buf, HAND_TOTAL_SIZE, 0);
        send(fd, begin_ + (transfer_.pieceNo * PIECE_DATA_SIZE), transfer_.pieceSize, 0);
        n++;
        pieceNo = pthreads * n + pthread_number;
    }
    close(fd);
}

int socket_and_connect()
{
    int fd, ret;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    if (ret < 0)
        exit(-1);

    return fd;
}
