#include <stdio.h>
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define SECRET 'S'
#define MAGIC_STRING 'M'

/* 
 * Structure to store information required for
 * decoding secret file from source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 200
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;


/* Decoding function prototype */
//open files
Status open_files_decode(DecodeInfo *decInfo);

/* Read and validate Decode args from argv */

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */

Status do_decoding(DecodeInfo *decInfo,char *argv[]);

/* Get File pointers for i/p and o/p files */

Status open_files(DecodeInfo *decInfo);

//decode magic string
Status decode_magic_string(DecodeInfo *decInfo);

//decode length of string
Status decode_length_of_string(DecodeInfo *decInfo);

//decode string data
Status decode_string(DecodeInfo *decInfo,char type);

//decode Secret extn
Status decode_secret_data_extn(DecodeInfo *decInfo);

//decode  Secret data 
Status decode_secret_data(DecodeInfo *decInfo);

//copy secret file data

Status copy_secret_file_data(DecodeInfo *decInfo,char *argv[]);




#endif

