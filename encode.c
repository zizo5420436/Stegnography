#include <stdio.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "types.h"
uint size_file;
uint size_magic_string;
OperationType check_operation_type(int argc,char *argv[])
{
    if(argc<2)
    {
        return e_unsupported;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
char MAGIC_STRING[100];
Status open_files_encode(EncodeInfo *encInfo)
{
    
    // Src image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    {
        if (encInfo->fptr_src_image == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
            return e_failure;
        }
    }
    printf("INFO: Opened %s\n", encInfo->src_image_fname);
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    {
        if (encInfo->fptr_secret == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
            return e_failure;
        }
    }
    printf("INFO: Opened %s\n", encInfo->secret_fname);
    // Stego image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    {
        if (encInfo->fptr_stego_image == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
            return e_failure;
        }
    }
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);
    return e_success;
}
// do encoding
Status do_encoding(EncodeInfo *encInfo)
{
    // Open files
    // Test open_files
    printf("USER: Enter the magic string : ");
    scanf("%s", MAGIC_STRING);
    if (open_files_encode(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "open_files");
    }
    // check_capacity
    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "check_capacity");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "check_capacity");
    }
    // copy_bmp_header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_bmp_header");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "copy_bmp_header");
    }
    // encode_magic_string
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_magic_string");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "encode_magic_string");
    }
    // encode_secret_file_extn
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_extn");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "encode_secret_file_extn");
    }
    // encode_secret_file_size
    if (encode_secret_file_size(size_file, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_size");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "encode_secret_file_size");
    }
    // encode_secret_file_data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_data");
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "encode_secret_file_data");
    }
    // copy_remaining_img_data
     if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
     {
         printf("ERROR: %s function failed\n", "copy_remaining_img_data" );
         return e_failure;
     }
     else
     {
         printf("SUCCESS: %s function completed\n", "copy_remaining_img_data" );
     }
    return e_success;
}
// validation for encoding
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    printf("INFO: Validation for encoding\n");
    // check for number of arguments
    if (argc < 4)
    {
        printf("Usage: %s -e <source.bmp> <secret.txt> [stego.bmp]\n", argv[0]);
        return e_failure;
    }
    // check source image extension
    char *dot = strchr(argv[2], '.');
    if (!dot || strcmp(dot + 1, "bmp") != 0)
    {
        printf("ERROR: Source image extension is not bmp\n");
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];
    printf("INFO: Source image extension = .bmp\n");

    // check secret file extension
    char *dot1 = strchr(argv[3], '.');
    if (!dot1)
    {
        printf("ERROR: Secret file extension is not txt\n");
        return e_failure;
    }
    encInfo->secret_fname = argv[3];
    // get extension secret file
    get_file_extension(encInfo->secret_fname, encInfo->extn_secret_file);
    printf("INFO: Secret file extension = %s\n", encInfo->extn_secret_file);
    // printf("INFO: Secret file extension is txt\n");
    //scret file extension is not in bmp
    if (strcmp(encInfo->extn_secret_file, ".bmp") == 0)
    {
        printf("ERROR: Secret file extension is bmp\n");
        return e_failure;
    }
    // check stego image extension

    if (argv[4])
    {
        char *dot2 = strchr(argv[4], '.');
        if(!dot2)
        {
            printf("ERROR: Stego image extension is not bmp\n");
            return e_failure;
        }
        if (strcmp(dot2 + 1, "bmp") != 0)
        {
            printf("ERROR: Stego image extension is not bmp\n");
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
        printf("INFO: Stego image extension = .bmp\n");
    }
    else
    {
        // rename steog image file
        printf("INFO: Stego name is not given \n");
        printf("INFO: Stego image file is taken as 24028A.bmp \n");
        encInfo->stego_image_fname = "24028A.bmp";
    }
    return e_success;
}
// copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char data = 0;
    while (fread(&data, 1, 1, fptr_src) == 1)
    {
        fwrite(&data, 1, 1, fptr_dest);
    }
    return e_success;
}
// encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // encode length of secret file size
    for (int i = 0; i < 32; i++)
    {
        char data;
        fread(&data, 1, 1, encInfo->fptr_src_image);
        data = (data & 0xFE) | ((file_size >> i) & 1);
        fwrite(&data, 1, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
// encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // encode data to image
    encode_data_to_image(encInfo->secret_data, size_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
// encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    // length of file extension
    uint len = strlen(file_extn);
    // encode length of file extension
    encode_length_of_string(file_extn, encInfo);
    // encode file extension
    encode_data_to_image(file_extn, len, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
// check capacity
Status check_capacity(EncodeInfo *encInfo)
{
    // get size of magic string
    size_magic_string = strlen(MAGIC_STRING);
    //printf("size_magic_string = %u\n", size_magic_string);
    // get size of extension
    uint size_extn = strlen(encInfo->extn_secret_file);
    //printf("size_extn = %u\n", size_extn);
    // get size of image
    uint size_image = get_image_size_for_bmp(encInfo->fptr_src_image);
    //printf("size_image = %u\n", size_image);
    // get size of file
    size_file = get_file_size(encInfo->fptr_secret);
    //printf("size_file = %u\n", size_file);
    // store the secret file data to structure variable
    fseek(encInfo->fptr_secret, 0, SEEK_SET);


    fread(encInfo->secret_data, 1, size_file, encInfo->fptr_secret);

    // get size of image capacity

    uint image_capacity = size_file * 8 + size_magic_string * 8 + size_extn * 8 + 96;
    //printf("image_capacity = %u\n", image_capacity);
    if (size_image < image_capacity)
    {
        return e_failure;
    }
    else
    {

        return e_success;
    }
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{

    char data[54];
    // Seek to 0th byte
    fseek(fptr_src_image, 0, SEEK_SET);
    // Read the header
    fread(&data, 1, 54, fptr_src_image);
    fwrite(&data, 1, 54, fptr_stego_image);

    return e_success;
}
// get file extension
uint get_file_extension(const char *file_name, char *file_extn)
{
    const char *dot = strchr(file_name, '.');
    if (!dot || dot == file_name)
    {
        return 0;
    }
    else
    {
        strcpy(file_extn, dot);
        return 1;
    }
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);
    // Read the width (an int)
    fread(&width, sizeof(uint), 1, fptr_image);
    //printf("width = %u\n", width);
    // read the height
    fread(&height, sizeof(uint), 1, fptr_image);
    //printf("height = %u\n", height);
    // Return image capacity
    return width * height * 3;
}
// Get file size
uint get_file_size(FILE *fptr)
{
    uint size;
    // Seek to 0th byte
    fseek(fptr, 0, SEEK_END);
    // Get the file size
    size = ftell(fptr);
    // Return file size
    return size;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // length of magic string
    uint len = strlen(MAGIC_STRING);
    // encode length of magic string
    encode_length_of_string(MAGIC_STRING, encInfo);
    // encode magic string
    encode_data_to_image(MAGIC_STRING, len, encInfo->fptr_src_image, encInfo->fptr_stego_image);

    return e_success;
}
// encode data to image(string)
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {

        if (fread(image_buffer, 1, 8, fptr_src_image) != 8)
        {
            return e_failure;
        }
        // printf("Encoding character %d: '%c' (0x%02x)\n", i, data[i], data[i]);
        encode_byte_to_lsb(data[i], image_buffer);
        // printf("Encoded character %d: '%c' (0x%02x)\n", i, data[i], data[i]);
        // write image buffer to stego image
        if (fwrite(image_buffer, 1, 8, fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
}
// encode byte to lsb(string)
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> i) & 1);
    }
    return e_success;
}
// encode length of string
Status encode_length_of_string(const char *string, EncodeInfo *encInfo)
{
    uint len = strlen(string);
    for (int i = 0; i < 32; i++)
    {
        char data;
        fread(&data, 1, 1, encInfo->fptr_src_image);
        data = (data & 0xFE) | ((len >> i) & 1);
        fwrite(&data, 1, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}