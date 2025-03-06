#include <stdio.h>
#include <string.h>
#include "common.h"
#include "decode.h"
#include "encode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    OperationType op_type = check_operation_type(argc,argv);
    if(op_type == e_encode)
    {
        //validaate
        if(read_and_validate_encode_args(argc,argv, &encInfo) == e_failure)
        {
            printf("ERROR: %s function failed\n", "read_and_validate_encode_args" );
            return e_failure;
        }
        else
        {
            printf("SUCCESS: %s function completed\n", "read_and_validate_encode_args" );
        }
        printf("INFO: Encoding operation begins\n");
        //do encoding
        if(do_encoding(&encInfo) == e_success)
        {
            printf("SUCCESS: Encoding operation completed\n");
        }
        else
        {
            printf("ERROR: Encoding operation failed\n");
        }
        
    }
    else if(op_type == e_decode)
    {
        printf("INFO: Decoding operation\n");
        //validate
        if(read_and_validate_decode_args(argc,argv, &decInfo) == e_failure)
        {
            printf("ERROR: %s function failed\n", "read_and_validate_decode_args" );
            return e_failure;
        }
        else
        {
            printf("SUCCESS: %s function completed\n", "read_and_validate_decode_args" );
        }
        //do decoding
        if(do_decoding(&decInfo,argv)==e_success)
        {
            printf("SUCCESS: Decoding is successful\n");
        }
        else
        {
            printf("ERROR: Decoding is not successful\n");
        } 
    }
    else
    {
        printf("ERROR: Unsupported operation\n");
        printf("TRY: ./a.out -e <source.bmp> <secret.txt> [stego.bmp]\n");
        printf("TRY: ./a.out -d <stego.bmp>\n");
        
        return e_failure;
    }
    // Test get_image_size_for_bmp
   /* img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    encInfo.size_secret_file = get_file_size(encInfo.fptr_secret);
    printf("INFO: Image size = %u\n", img_size);*/
    return 0;
}
