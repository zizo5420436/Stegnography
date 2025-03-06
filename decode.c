#include<stdio.h>
#include<string.h>
#include "common.h"
#include "types.h"
#include <stdlib.h>
#include "decode.h"
//open files
char decoded_magic_string[MAX_SECRET_BUF_SIZE];
char user_decoded_magic_string[MAX_IMAGE_BUF_SIZE];

Status open_files_decode(DecodeInfo *decInfo)
{
    // Stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    {
        if (decInfo->fptr_stego_image == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
            return e_failure;
        }
    }
    return e_success;
}
// read and validate decode args
Status read_and_validate_decode_args(int argc, char *argv[],DecodeInfo *decinfo)
{
    //argv 4 is optional
    if(argc<3)
    {
        printf("Usage: %s -d <stego_image.bmp>\n", argv[0]);
        return e_failure;
    }
    const char *dot1 = strrchr(argv[2], '.');  // Find the last dot
    if (dot1 != NULL && strchr(argv[2], '.') != dot1)  
    {
        printf("ERROR: Destination file should have only one extension\n");
        return e_failure;
    }
    if (argc == 3)
    {
        decinfo->stego_image_fname = argv[2];
        printf("INFO: Stego image file name = %s\n", decinfo->stego_image_fname);
    }
    else if(argc == 4)
    {
        decinfo->stego_image_fname = argv[2];
        printf("INFO: Stego image file name = %s\n", decinfo->stego_image_fname);
    }
    else
    {
        printf("Usage: %s -d <stego_image.bmp>\n", argv[0]);
        return e_failure;
    }
    //if file is not bmp
    char *dot = strchr(argv[2], '.');
    if (!dot || strcmp(dot + 1, "bmp") != 0)
    {
        printf("ERROR: Stego image extension is not bmp\n");
        return e_failure;
    }
    //if destination file is beautiful.bmp
    if (strcmp(argv[2], "beautiful.bmp") == 0)
    {
        printf("ERROR: Source file and Destination file could not be same.\n");
        return e_failure;
    }
    return e_success;
}
//do decoding
Status do_decoding(DecodeInfo *decInfo,char *argv[])
{
    //open files
    if(open_files_decode(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files_decode" );
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "open_files_decode" );
    }
    //skip header
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    //decode magic string
    if(decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_magic_string" );
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "decode_magic_string" );
    }
    if(decode_secret_data_extn(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_data_extn" );
        return e_failure;
    }else
    {
        printf("SUCCESS: %s function completed\n", "decode_secret_data_extn" );
    }
    if(decode_secret_data(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_data" );
        return e_failure;
    }else
    {
        printf("SUCCESS: %s function completed\n", "decode_secret_data" );
    }
    if(copy_secret_file_data(decInfo,argv) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_secret_file_data" );
        return e_failure;
    }else
    {
        printf("SUCCESS: %s function completed\n", "copy_secret_file_data" );
    }
    return e_success;


}
//copy secret file data
Status copy_secret_file_data(DecodeInfo *decInfo,char *argv[])
{
    if(argv[3]!=NULL)
    {
        //if file name dont have extension,set as in .txt
        //user strcat
        if(strstr(argv[3],".")==NULL)
        {
            strcat(argv[3],".txt");
        }
        FILE* fptr_secret = fopen(argv[3], "w");

        if (fptr_secret == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", argv[3]);
            return e_failure;
        }
        //write data to file
        fwrite(decInfo->secret_data, 1, strlen(decInfo->secret_data), fptr_secret);
        fclose(fptr_secret);
    }
    else
    {
        FILE* fptr_secret = fopen("Output.txt", "w");
        if (fptr_secret == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", "Output.txt");
            return e_failure;
        }
        //write data to file
        fwrite(decInfo->secret_data, 1, strlen(decInfo->secret_data), fptr_secret);
        fclose(fptr_secret);
    }

    return e_success;
}
//decode secret data
Status decode_secret_data(DecodeInfo *decInfo)
{
    if((decode_string(decInfo,'S'))!=e_success)
    {
        return e_failure;
    }
    //printf("INFO: Decoded Data in the string : %s\n",decInfo->secret_data);
    return e_success;

}
//decode secret data extn
Status decode_secret_data_extn(DecodeInfo *decInfo)
{
    if((decode_string(decInfo,'E'))!=e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoded Data in the string : %s\n",decInfo->extn_secret_file);
    return e_success;
}
// Decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    if((decode_string(decInfo,'M'))!=e_success)
    {
        return e_failure;
    }
    printf("USER: Enter the magic string : ");
    scanf(" %s",user_decoded_magic_string);
    if (strcmp(decoded_magic_string, user_decoded_magic_string) != 0)  
    {
        printf("ERROR: Magic string is not matching\n");
        return e_failure;
    }
    return e_success;
}

//decode  length
uint decode_length_of_string(DecodeInfo *decInfo)
{
    uint len = 0;
    char data;

    // Read the 32 least significant bits from the image
    for (int i = 0; i < 32; i++)
    {
        // Read one byte from the stego image
        fread(&data, 1, 1, decInfo->fptr_stego_image);

        // Extract the LSB of the byte (the secret bit) and shift it to the appropriate position
        len |= ((data & 0x01) << i);
    }

    // Return the decoded length
    //printf("Decoded length of string: %u\n", len);
    return len;
}
Status decode_string(DecodeInfo *decInfo,char Type)
{
    uint size = decode_length_of_string(decInfo);  // Get the length of the magic string
    char data;
    char String[MAX_SECRET_BUF_SIZE];
    uint bit_count=0,byte_index=0,decoded_byte=0;//intialisation  of needed variables
    for(int i=0;i<size*8;i++)
    {
        fread(&data,1,1,decInfo->fptr_stego_image);
        decoded_byte=decoded_byte|(data&0x01)<<bit_count;
        bit_count++;
    
        if(bit_count==8)
        {
            String[byte_index]=(char)decoded_byte;
            byte_index++;
            decoded_byte=0;
            bit_count=0;
        }
    }
    String[byte_index]=0;
    if(Type=='S')
    {
        strcpy(decInfo->secret_data,String);
    }
    else if(Type=='E')
    {
        strcpy(decInfo->extn_secret_file,String);
    }
    else
    {
        strcpy(decoded_magic_string,String);
        //printf("%s",decoded_magic_string);
    }
    return e_success;

}