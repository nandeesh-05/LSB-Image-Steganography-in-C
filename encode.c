#include <stdio.h>
#include "header.h"
#include "types.h"
#include<string.h>
#include"common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
// function to check whether the operation is encoding or decoding
 OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))
    {
        return e_encode;
    }
    else if(!strcmp(argv[1],"-d"))
    {
         return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        printf("Error: The input image must be in BMP format.\n");

        return e_failure;
    }
    if(strstr(argv[3],".txt")!=NULL)
    {
        encInfo->secret_fname=argv[3];
         char *arr=strstr(argv[3],".txt");
         strcpy(encInfo->extn_secret_file_size,arr);
    }
    else
    {
        printf("Please ensure the secret file has a .txt extension.\n");

        return e_failure;
    }
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL)
        {
            encInfo->stego_image_fname=argv[4];
            return e_success;
        }
        else
        {
            printf("✅ The output image must have a .bmp extension.\n");
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname="stego.bmp";
        return e_success;
    }
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen"); // print the reasonable errors 
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
// function definition to check the capacity
Status check_capacity(EncodeInfo *encInfo)
{
    // to get the size of beautiful.bmp;
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    // to get the secret file size
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity >= (54+(2 + 4 + 4 + 4 + encInfo->size_secret_file)*8))
    {
        printf("✅ Size is sufficient to encode.\n");
        return e_success;
        
    }
    printf("⚠️ Size is not sufficient to encode.\n"); 
    return e_failure;

}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);
    // rewind the fptr
    fseek(fptr_image, 0, SEEK_SET);
    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

// definition of secret file size function
uint get_file_size(FILE *fptr)
{
    // move the offset to end
    fseek(fptr,0,SEEK_END);
    // size of file using ftell and store
    uint size=ftell(fptr);
    //printf("The size of secret file : %u\n",size);
    rewind(fptr);
    return size;

}



Status do_encoding(EncodeInfo *encInfo)
{

    if(open_files(encInfo)==e_success)
    {
            printf("✅ All files have been opened successfully.\n");

        
            if(check_capacity(encInfo)==e_success)
            {
               // printf("📊 Capacity has been checked successfully.\n");

                if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                {
                   // printf("📄 The header has been copied successfully.\n");

                    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                    {
                    printf("✨ Magic string has been successfully encoded into output.bmp.\n");

                     if(encode_secret_file_extn_size(encInfo->extn_secret_file_size,encInfo)==e_success)
                     {

                        if(encode_secret_file_extn(encInfo->extn_secret_file_size,encInfo)==e_success)
                        {
        
                            if(encode_secret_file_size(encInfo->size_secret_file, encInfo)==e_success)
                            {
                                printf("✅ Secret file size, extension, and extension size encoded successfully into output.bmp.\n");
                                if( encode_secret_file_data(encInfo->secret_fname,encInfo)==e_success)
                                {
                                    printf("✅ Secret file data encoded successfully into output.bmp.\n");

                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image )==e_success)
                                    {
                                       // printf("📄 All content from the input image is being copied to the output image.\n");

                                        if( close_files(encInfo)==e_success)
                                        {
                                            printf("📂 All files are being closed.\n");
                                            return e_success;
                                        }


                                    }
                                }

                            }

                        }

                     }
                   
                   }

                }
               
            }
        
    }
    else
    {
        printf("⚠️ Encoding failed due to an error.\n");
        return e_failure;
    }
}
// function to find size of secret file
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char head[54];
    fread(head,1,54,fptr_src_image);
    fwrite(head,1,54,fptr_dest_image);
    return e_success;
    
}

// definition of encode magic string -> data copy separate function ()
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
   encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image);
   return e_success;

}
// function definition encode_secret_file_extn_size inside this function call of encode_size_to_lsb
Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo)
{
  char image_buffer[32];
  // read 32 bytes from beautiful.bmp
  fread(image_buffer,1,32,encInfo->fptr_src_image);
  encode_size_to_lsb(strlen(file_extn), image_buffer);
  fwrite(image_buffer,1,32,encInfo->fptr_stego_image);
  return e_success;

}
// function definition to encode secret file extrn
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;

}

// function to encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
   char image_buffer[32];
  // read 32 bytes from beautiful.bmp
  fread(image_buffer,1,32,encInfo->fptr_src_image);
  encode_size_to_lsb(file_size, image_buffer);
  fwrite(image_buffer,1,32,encInfo->fptr_stego_image);
  return e_success;

}

// function defnition to encode secret file data
Status encode_secret_file_data(const char *file_data,EncodeInfo *encInfo)
{
    // fread(encInfo->secret_data,1,1,encInfo->fptr_secret);
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
         fread(encInfo->secret_data,1,1,encInfo->fptr_secret);
        encode_data_to_image(encInfo->secret_data, 1,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    }
    return e_success;

}

// function defnition to encoding the input to image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(image_buffer,1,8,fptr_src_image); // Readed 8 bytes from input file for encoding
        encode_byte_to_lsb(data[i] ,image_buffer);//bit manipulation 1byte to 8bytes
        fwrite(image_buffer,1,8,fptr_stego_image);//writing encoded 8 bytes to output.bmp
    }
    return e_success;

}
// function defnition to encode secret.txt to output.bmp image
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> (7 - i)) & 1);
    }
     return e_success;

}

// function defnition of encode_size_to_lsb
Status encode_size_to_lsb(int size, char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
       // image_buffer[i] = (image_buffer[i] & ~1) | (size & (31 - i));
       image_buffer[i] = (image_buffer[i] & ~1) | ((size >> (31 - i)) & 1);

    }
    return e_success;
}



/*  function definition toCopy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch[1];
    while(fread(ch,1,1,fptr_src))
    {
        fwrite(ch,1,1,fptr_dest);
    }
    return e_success;
}
// function def to close all files
Status close_files(EncodeInfo *encInfo)
{
    fclose( encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret );
    fclose(encInfo->fptr_stego_image);
    return e_success;
}