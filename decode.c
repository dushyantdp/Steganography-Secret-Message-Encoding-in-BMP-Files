#include<stdio.h>
#include "common.h"
#include "decode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>
// long int *len=0;
int decode_int_to_msb(int , char *);
char decode_byte_to_msb(char , char *);
char  ch,buf[32];
char pass[15], user[15];
int len=0;

Status dopen_files(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started\n");
    // printf("%s\n",decInfo->stego_image_fname);
 decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
 if(decInfo->fptr_stego_image == NULL)
 {
    printf("Stego Image is NOT Opened\n");
    return e_failure;
 }   
 printf("INFO: Opened steged_beautiful.bmp\n");
 decInfo->fptr_msg = fopen(decInfo->msg_fname, "w");
 {
    if(decInfo->fptr_msg == NULL)
    {
        printf("Output Message File is NOT Opened\n");
        return e_failure;
    }
 }
 return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if(dopen_files(decInfo)== e_success)
    {
        printf("INFO: Opened all required files\n");
    }
    else
    {
        printf("INFO: Files are NOT opened\n");
    }
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    printf("INFO: Decoding Magic string Signature\n");
    decode_magic_string(decInfo);
    printf("INFO: Done\n");
    printf("INFO: Decoding Output File Extenstion\n");
    decode_secret_file_extn(decInfo);
    if(strcmp(pass,user)!=0)
    {
        printf("INFO: Mismatch of Magic string\n Exiting.....!\n");
        fseek(decInfo->fptr_msg, 0,SEEK_SET);
        fprintf(decInfo->fptr_msg,"%s","We caught YOU!\nJust Close the Laptop and RUN AWAY\n");
        return e_failure;
    }
    printf("INFO: Done\n");
    decode_secret_file_data(decInfo);
    printf("INFO: ## Decoding Done Successfully ##\n");

}
Status decode_magic_string(DecodeInfo *decInfo)
{
    {
        int llen = strlen(MAGIC_STRING);
        // printf("MAGIC_len: %d\n",llen);
        // char buf[32];
        // int len=0;
        fread(buf, 32, 1, decInfo->fptr_stego_image);
        // decode_int_to_msb(&len, buf);
        len = decode_int_to_msb(len, buf);
        if(len != llen)
        {
            printf("ERROR: Give Data Encoded file as Input\n");
            exit(0);
        }
        // fprintf(decInfo->fptr_msg,"%d\n",len); 
        printf("Magic string length DECODED\n");
        int i=0;
        for( i=0; i<len; i++)
        {
            // char ch=0;
            fread(buf, 8, 1, decInfo->fptr_stego_image);
            ch = decode_byte_to_msb(ch, buf);
            pass[i]=ch;
            // fprintf(decInfo->fptr_msg,"%c",ch);
        }
        pass[i]='\0';
        // printf("%s\n",pass);
        printf("Enter MAGIC string:\n");
        scanf(" %[^\n]",user);
    }
}
int decode_int_to_msb( int length, char *buf)
{
    for(int i=0; i<32; i++)
    {
        length = length << 1;
        length = length | (buf[i] & 1);
    }
    return length;
}
char decode_byte_to_msb(char ch, char *buf)
{
    for(int i=0; i<8; i++)
    {
        ch = ch << 1;
        ch = ch | (buf[i] & 1);
    }
    return ch;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    // char buf[32];
    // int len=0;
    // char ch;
    fread(buf,32, 1, decInfo->fptr_stego_image);
    len = decode_int_to_msb(len, buf);
    // printf("Extn -> %d\n",len);
    // fprintf(decInfo->fptr_msg,"%d\n",len);
    int i=0;
    char ext[6];
    for(i=0; i<len; i++)
    {
        fread(buf, 8, 1,decInfo->fptr_stego_image);
        ext[i]= decode_byte_to_msb(ch, buf);
        // fprintf(decInfo->fptr_msg,"%",ch);
    }
     ext[i]='\0';
     printf("%s\n",ext);
     strcat(decInfo->msg_fname,ext);
     printf("%s\n",decInfo->msg_fname);
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    len =0;
    fread(buf,32,1,decInfo->fptr_stego_image);
    printf("INFO: Decoding decoded.txt File size\n");
    len = decode_int_to_msb(len, buf);
    printf("INFO: Done");
    printf("INFO: Decoding decoded.txt File Data\n");
    // fprintf(decInfo->fptr_msg,"%d\n",len);
    for(int i=0; i<len; i++)
    {
        fread(buf, 8, 1,decInfo->fptr_stego_image);
        ch = decode_byte_to_msb(ch, buf);
        fprintf(decInfo->fptr_msg,"%c",ch);
    }
    printf("INFO: Done\n");
}

