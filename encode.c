#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>
#include<stdlib.h>
void encode_int_to_lsb(int , char *);
int check_capacity1(EncodeInfo *encInfo);
void close_file(EncodeInfo *);
long unsigned int file_size;
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	printf("Image file is not opened\n");
    	return e_failure;
    }
    printf("INFO: Opened beautiful.bmp\n");
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        printf("Secret file is not opened\n");
    	return e_failure;
    }
    printf("INFO: Opened secret.txt\n");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        printf("Output file is not opened\n");
    	return e_failure;
    }
    printf("INFO: Opened steged_img.bmp\n");
    // printf("Done\n");
    // No failure return e_success
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("INFO: Done\n");
    }
    else
    {
        printf("INFO: File NOt opened \n");
    }

    printf("INFO: ## Encoding Procedure Started ##\n");
    printf("INFO: Checking for secret.txt size\n");
    if(check_capacity(encInfo) == e_success)
    {
        printf("INFO: Done. Not Empty\n");
    }
    else
    {
        printf("INFO: secret.txt is Empty\n");
        exit(0);
    }
    printf("INFO: Checking for beatiful.bmp capacity to handle secret.txt\n");
    if(check_capacity1(encInfo) == e_success)
    {
        printf("INFO: Done. Found OK\n");
    }
    //copy header from beautiful.bmp to destination output.bmp which is 54 bytes
    printf("INFO: Copying Image Header\n");
    copy_bmp_header(encInfo->fptr_src_image, encInfo-> fptr_stego_image);
    printf("INFO: Done\n");
    //encoding the length of magic string and data
    printf("INFO: Encoding Magic String Signature\n");
    encode_magic_string(MAGIC_STRING, encInfo);
    printf("INFO: Done\n");

    //encoding length secret file extantion and extention
    printf("INFO: Encoding secret.txt File Extasion\n");
    encode_secret_file_extn(encInfo->secret_fname, encInfo);
    printf("INFO: Done!\n");

    //getting the secret file length or size
    printf("INFO: Encoding secret.txt File size\n");
    file_size = get_file_size(encInfo->fptr_secret);
    // printf("Secret file length is : %ld\n",file_size);
    
    //encoding secret file length or size
    encode_secret_file_size(file_size, encInfo);
    printf("INFO: Done!\n");

    //Encoding secret file data
    printf("INFO: Encoding secret.txt File Data\n");
    encode_secret_file_data(encInfo);
    printf("INFO: Done!\n");

    //Copying the remaining data of src_image to dest_image
    printf("INFO: Copying Left Over Data\n");
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO: Done!\n");
    printf("## Encoding Done Successfully ##\n");

    //END OF ENCODING.......!
}
Status check_capacity(EncodeInfo *encInfo)
{
    long int ch = fgetc(encInfo->fptr_secret);
    // fread(buf, 8, 1,encInfo->fptr_secret);
    if(ch != EOF)
    {
        fseek(encInfo->fptr_secret , 0, SEEK_SET);
    return e_success;
    }
    else
    return e_failure;
}
int check_capacity1(EncodeInfo *encInfo)
{
    int src_len = get_file_size(encInfo->fptr_src_image);
    // printf("src len: %d\n",src_len);
    int secr_len = get_file_size(encInfo->fptr_secret);
    // printf("stego len : %d\n",secr_len);
    if(src_len < secr_len)
    {
        printf("INFO: Size of beautiful.bmp is less than size of secret.txt\n");
        exit(0);
    }
    else
    return e_success;
}
uint get_file_size(FILE *fptr) //This is to get the length of .txt file 
{
    fseek(fptr, 0, SEEK_END);
    long unsigned int Secret_file_len = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return Secret_file_len;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buf[54];
    fread(buf,54,1,fptr_src_image);
    fwrite(buf,54,1,fptr_stego_image);
    return e_success;
}

void encode_int_to_lsb(int len, char *buffer)    //this function is to encode the any length 
{
    int ind=0;
    for(int i=31; i>=0; i--)
    {
        if((len & (1 << i)))
        {
            buffer[ind] = buffer[ind] | 1;
        }
        else
        buffer[ind] = (buffer[ind] & ~(1));
        ind++;
    }
}

Status encode_byte_to_lsb(char ch, char *buf) //This function is to Encode the data of any file character by character
{
    char index=0;
    for(int i=7; i>=0; i--)
    {
        if((ch & (1<<i)))
        {
            buf[index] = buf[index] | 1;
        }
        else
        {
            buf[index] = buf[index] & (~(1));
        }
        index++;
    }
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len=strlen(magic_string);
    char buf[32];
    //read from next 32 bytes of beautiful.bmp and store into Buffer 
    fread(buf,32,1,encInfo->fptr_src_image);
    encode_int_to_lsb(len, buf);
    //write to fptr_dest_image form encoded buffer
    fwrite(buf,32,1,encInfo->fptr_stego_image);
    // printf("length Encoded\n");
    for(int i=0; i<len; i++)
    {
    fread(buf,8,1,encInfo->fptr_src_image);
    encode_byte_to_lsb(magic_string[i], buf);
    fwrite(buf,8,1,encInfo->fptr_stego_image);
    }
    // fclose(encInfo->fptr_stego_image);
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char secret_ext[10], buffer[32];
    // secret_ext[0]= '\0';
    // printf("Length of Extn: %ld\n",strlen(file_extn));
    get_file_extn((char *)file_extn,secret_ext);
    // printf("%s\n",secret_ext);
    int len = strlen(secret_ext);
    // printf("LEngth: %d\n",len);
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_int_to_lsb(len,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    // printf("secret file extentation length Encoded\n");
    for(int i=0; i<len; i++)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(secret_ext[i],buffer);
        fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }    
}
 void get_file_extn(char* file_name,char *secret_ext)
{
    int i=0;
    // printf(" %s\n",file_name);
    while( file_name[i]!= '\0')
    {
        if(file_name[i]=='.')
        {
            int j=0;
            while(file_name[i]!='\0')
            {
                secret_ext[j]=file_name[i];
                i++;
                j++;
            }
            secret_ext[j]='\0';
            // printf("%s\n",secret_ext);
            // return secret_ext;
        }
        i++;
    }
    // printf("%s\n",secret_ext);
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buf[32];
    fread(buf, 32, 1, encInfo->fptr_src_image);
    encode_int_to_lsb(file_size, buf);
    fwrite(buf, 32, 1, encInfo->fptr_stego_image);
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // printf("S_file_ size : %ld\n",file_size);
    char buf[32]; //32 bytes taken for safe side
    char ch;
    ch= fgetc(encInfo->fptr_secret);
    while(ch != EOF)
    {

        // printf("%c",ch);
        fread(buf, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch,buf);
        fwrite(buf, 8, 1, encInfo->fptr_stego_image);
        ch=fgetc(encInfo->fptr_secret);        
    }
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego)
{
    char buf[32]; //32 for safe side
    // char ch;
    while(!feof(fptr_src))
    {
        fread(buf, 8, 1, fptr_src);
        fwrite(buf, 8, 1, fptr_stego);
    }
}
void close_file(EncodeInfo *encInfo)
{
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
}
int check_char(char *str)
{
	int i=0;
	while(str[i]='\0')
	{
		if(str[i]=='.')
		{
			printf("%c\n",str[i]);
			return 1;
			// break;
		}
	}
	return  0;
}