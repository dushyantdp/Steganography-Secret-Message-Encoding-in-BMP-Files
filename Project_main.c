/*
    Name: Dushyant Patil
    Date: 19/09/2024
    Description: Stegnography Project
*/
#include<stdio.h> 
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"


int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
	

    if(argc < 3)
    {
        printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file]\n");
        printf("./a.out: Decoding: ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }
    else if (strcmp(argv[1], "-e") == 0) 
    {
        /* Read and validate Encode args from argv */
        if( !(read_and_validate_encode_args(argc, argv, &encInfo)) )
        {
            return e_failure;
        }

        // call encoding function to encode the secret data into image file along with magic string and secret file extension
        do_encoding(&encInfo);
        
    }
    else if( strcmp(argv[1], "-d") == 0)
    {
        /* Read and validate Decode args from argv */
        if( !(read_and_validate_decode_args(argc, argv, &decInfo)) )
        {
            return e_failure;
        }

        // call decoding function to decode the encoded data(magic string length, magic string data, file extension length, file extension, secret data size, secret data) from image file
        do_decoding(&decInfo);

    }
    else
    {
        printf("INFO: %s is Unsupported Command\n",argv[1]);
        return e_failure;
    }


}


/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    encInfo->src_image_fname = strstr(argv[2], ".bmp");
            
    if( (encInfo->src_image_fname == NULL) || (strcmp(encInfo->src_image_fname, ".bmp") != 0))
    {
        printf("./a.out: Encoding: ./a.out -e <.bmp file> <secret file> [output file]\n");
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
    }

    if ((argc < 4) )
    {
        printf("./a.out: Encoding: ./a.out -e <.bmp file> <secret file> [output file]\n");
        return e_failure;
    }
    else if (argc > 4)
    {
        encInfo->stego_image_fname = strstr(argv[4], ".bmp");
        if( (encInfo->stego_image_fname == NULL) || (strcmp(encInfo->stego_image_fname, ".bmp") != 0))
        {
            printf("INFO: Output file extension should be <.bmp file>\n");
            return e_failure;
        }
        else
        {
            encInfo->stego_image_fname = argv[4];
        }
    }
    else
    {
        printf("INFO: Output File not mentioned. Creating output.bmp as default\n");
        encInfo->stego_image_fname = "output.bmp";
    }
    printf("Here\n");

    encInfo->secret_fname = argv[3];

    return e_success;
}

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{

    decInfo->stego_image_fname = strstr(argv[2], ".bmp");
            // decInfo->stego_image_fname = "output.bmp";
    if( (decInfo->stego_image_fname == NULL) || (strcmp(decInfo->stego_image_fname, ".bmp") != 0) )
    {
        printf("./a.out: Decoding: ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }
    else
    {
        decInfo->stego_image_fname = argv[2];
    }

    if (argc >3)
    {
        char *token = strtok(argv[3], ".\n");
        strcpy(decInfo->msg_fname, token);
    }
    else 
    {    
        printf("INFO: Output File not mentioned. Creating 'msg.txt' as default\n");
 
        strcpy(decInfo->msg_fname, "msg");
        printf("INFO: Done\n");
    }

    return e_success;
}