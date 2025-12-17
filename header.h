#ifndef HEADER_H
#define HEADER_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; // to store the beautiful.bmp name
    FILE *fptr_src_image; // to store the beautiful.bmp offset
    uint image_capacity;  // to store the size of image
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE]; // to store the readed 8 bytes

    /* Secret File Info */
    char *secret_fname;   // to store secret file name
    FILE *fptr_secret;    // to store secret file offset
    char extn_secret_file[MAX_FILE_SUFFIX]; // to store the extension of secret file
    char extn_secret_file_size[MAX_FILE_SUFFIX]; // to store extension size of secret file
    char secret_data[MAX_SECRET_BUF_SIZE];  // to store the readed 1 byte data
    long size_secret_file; // store the size of secret file

    /* Stego Image Info */
    char *stego_image_fname;  // to store the output filename
    FILE *fptr_stego_image; // to store the output file offset
        //decoding
    FILE *fptr_output_image;// to store the output
    char decoded_magic_string[10];
    char output_file_name[30];// to store output file extern name
    uint extn_output_file_size; // to store extension size of output file
    char extn_output_file[6]; // to store extension of output file
    char output_data[100];  // to store the readed 1 byte data
    uint size_output_file; // store the size of output file

    

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file extenstion size */
Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(const char *file_data,EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(const char *data, int size,FILE *fptr_src_image, FILE *fptr_dest_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Encode a byte into LSB of image data array */
Status encode_size_to_lsb(int size, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/* Get File pointers  to close for i/p and o/p files */
Status close_files(EncodeInfo *encInfo);



/* decoding function prototype */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_decoding(EncodeInfo *encInfo);

/* Store Magic String */
Status decode_magic_string(char * arr,EncodeInfo *encInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(char *arr, EncodeInfo *encInfo);

/* Decode secret file extenstion size */
Status decode_secret_file_extn_size( EncodeInfo *encInfo);

/* Decode secret file size */
Status decode_secret_file_size(EncodeInfo *encInfo);

/* Decode secret file data*/
Status decode_secret_file_data(char *arr,EncodeInfo *encInfo);

/* Decode function, which does the real magic string decoding */
Status decode_data_from_image( char * arr,int size, FILE *fptr_stego_image);


/* Decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode a byte into LSB of image data array */
Status decode_size_from_lsb(int *size,char*image_buffer);

void error();


#endif
