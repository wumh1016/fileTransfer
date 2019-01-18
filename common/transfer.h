#ifndef _TRANSFER_H_
#define _TRANSFER_H_

#include <stdio.h>

#define     HEAD_POS_SYNC_WORD      0       // 同步字
#define     HEAD_POS_TOTAL_SIZE 	2       // 所有分片数据的大小（不包括HEAD） //total data length of all pieces
#define     HEAD_POS_TOTAL_PIECES 	4       // 所有分片的数量
#define     HEAD_POS_P_INDEX 		6       // 分片序号，从0开始
#define     HEAD_POS_P_LENGTH 		8       // 当前分片数据的大小
#define     CLIENT_PTHREAD_SIZE     10      // 客户端线程数
#define     HAND_TOTAL_SIZE         12      // 头部大小
#define     PIECE_DATA_SIZE         1448 	//每个分片数据最大字节数
//#define     FILE_NAME_SIZE          48      //文件名字节数
#define     ALL_BUF_SIZE            1460    //总大小


/* typedef struct buff{
    char    hand[HAND_TOTAL_SIZE];
//    char    fileName[FILE_NAME_SIZE];
    char    buffer[PIECE_DATA_SIZE];
}buff_t; */

typedef struct transfer{
    int     fileSize;                   //总文件数据大小
    int     filePieces;                 //总分片个数
    int     pieceNo;                    //分片编号
    int     pieceSize;                  //单片大小
    int     clientPthreads;             //客户端线程数
//    buff_t* buffer;
}transfer_t;

/* transfer_t* piece_init();
void piece_destory(transfer_t* piece_);
void piece_cut(transfer_t* piece_, void* fullFileName); */
int get_piece_num(int fileSize);
int get_single_piece_size(int pieceNo, int filePieces, int fileSize);

#endif