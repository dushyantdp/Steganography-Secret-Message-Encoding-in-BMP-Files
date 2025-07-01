#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char msg_fname[25];
    FILE *fptr_msg;

} DecodeInfo;


OperationType check_operation_type(char *argv[]);

Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status dopen_files(DecodeInfo *decInfo);

// Status check_capacity(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);


#endif
