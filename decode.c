#include <stdio.h>
#include<stdlib.h>
#include "header.h"
#include "types.h"
#include<string.h>
#include"common.h"	
// function defnition to check valiadations of decode file
Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        encInfo->stego_image_fname=argv[2];
        encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "r");
    }
    else
    {
        printf("⚠️ Error: Output should have a .bmp extension.\n");
        return d_failure;

    }
    if(argv[3]!=NULL)
    {
        
    
       strcpy(encInfo->output_file_name,strtok(argv[3],"."));
       return d_success;

    }
    else
    {
        strcpy(encInfo->output_file_name,"output");
        return d_success;
    }

}

/*  function defnition to Perform the decoding */
Status do_decoding(EncodeInfo *encInfo)
{
    
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
    char *arr=malloc(1*sizeof(char));
    
    if(decode_magic_string(arr,encInfo)==d_success)
    {
        //printf("✨ Magic string has been successfully decoded.\n");

           if(decode_secret_file_extn_size(encInfo)==d_success)
           {
              if(decode_secret_file_extn(arr,encInfo)==d_success)
              {
                
                if(decode_secret_file_size(encInfo)==d_success)
                {
                    if(decode_secret_file_data(arr,encInfo)==d_success)
                    {

                        encInfo->fptr_output_image = fopen(encInfo->output_file_name, "w");
                        
                        if(encInfo->fptr_output_image==NULL)
                        {
                           printf("Failed to open the file\n");
                        }
                        
                        for(int i=0;i<encInfo->size_output_file;i++)
                        {
                            
                            fprintf(encInfo->fptr_output_image,"%c",encInfo->output_data[i]);
                        }
                        fclose(encInfo->fptr_output_image);
                        fclose(encInfo->fptr_stego_image);
                        printf("💡 Decoding complete — file data saved successfully to the output file.\n");


                       return d_success;

                    }
                    
                }


              }
               
           }
            

    }
    
}




/*  function denition to Store Magic String */
Status decode_magic_string(char *arr,EncodeInfo *encInfo)
{
    arr=realloc(arr,((strlen(MAGIC_STRING))*sizeof(char)));
    
    decode_data_from_image(arr,strlen(MAGIC_STRING),encInfo->fptr_stego_image);
    
    strcpy(encInfo->decoded_magic_string,arr);
    if(!strcmp(MAGIC_STRING,encInfo->decoded_magic_string))
    {
        printf("✅ Decoded magic string matches the original magic string.\n");
        return d_success;
    }
    else
    {
       printf("❌ Decoded magic string does not match the original magic string.\n");
        return d_failure;

    }
}

/*  function defnition to Decode secret file extenstion size */
Status decode_secret_file_extn_size(EncodeInfo *encInfo)
{
     char image_buffer[32];
     int size=0;
     
  // read 32 bytes from beautiful.bmp
   fread(image_buffer,1,32,encInfo->fptr_stego_image);
   decode_size_from_lsb(&size,image_buffer);
   encInfo->extn_output_file_size=size;
   //printf("%u\n",encInfo->extn_output_file_size);
   return d_success;

}


/*  function defnition to Decode secret file extenstion */
Status decode_secret_file_extn(char *arr, EncodeInfo *encInfo)
{
    arr = realloc(arr, (encInfo->extn_output_file_size) * sizeof(char));
    decode_data_from_image(arr,encInfo->extn_output_file_size,encInfo->fptr_stego_image);
    strcat(encInfo->output_file_name,arr);
    //printf("%s\n",encInfo->output_file_name);
    return d_success;

}


/*  function defnition to Decode secret file size */
Status decode_secret_file_size(EncodeInfo *encInfo)
{
    char image_buffer[32];
     int size=0;
     
  // read 32 bytes from beautiful.bmp
   fread(image_buffer,1,32,encInfo->fptr_stego_image);
   decode_size_from_lsb(&size,image_buffer);
   encInfo->size_output_file=size;
   //printf("%u\n",encInfo->size_output_file);
   return d_success;

}

/* function defnition to Decode secret file data*/
Status decode_secret_file_data(char *arr,EncodeInfo *encInfo)
{
    arr = realloc(arr, (encInfo->size_output_file ) * sizeof(char));
    
    decode_data_from_image(arr,encInfo->size_output_file,encInfo->fptr_stego_image);
    strcpy(encInfo->output_data,arr);
    //printf("%s\n",encInfo->output_data);
    return d_success;
    
    

}

/*  function defnition to Decode function, which does the real decoding */
Status decode_data_from_image(char * arr,int size,FILE *fptr_stego_image)
{
    char image_buffer[8];
    
    unsigned char ch2=0;
    char *cptr=&ch2;
    
    for(int i=0;i<size;i++)
    {
        
        fread(image_buffer,1,8,fptr_stego_image); // Readed 8 bytes from input file for decoding
        
        decode_byte_from_lsb(cptr,image_buffer);//bit manipulation 1byte to 8bytes
    
           arr[i] = *cptr;
    }

    //printf("%c\n",ch2);
    return d_success;
    
   
}

/* function defnition Decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *ch2, char *image_buffer)
{
    unsigned char ch=0;
    for(int i=0;i<8;i++)
    {
        ch= ch | ((image_buffer[i] & 1) << (7-i));
    }
    *ch2=ch;
   return d_success;
}



/* function definition a byte into LSB of image data array */
Status decode_size_from_lsb(int *size, char *image_buffer)
{
    uint s=0;
    for(int i=0;i<32;i++)
    {
        s= s | ((image_buffer[i] & 1) << (31-i));
    }
    *size=s;
   return d_success;
}



