#include <stdio.h>
#include<string.h>
#include "header.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;
/*
    // Fill with sample filenames
    encInfo.src_image_fname = "beautiful.bmp";
    encInfo.secret_fname = "secret.txt";
    encInfo.stego_image_fname = "stego_img.bmp";

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }
*/
    if(argc<=2)
    {
        printf("❌ Please enter a valid number of arguments.\n");
        error();
        return 0;
    }
    else
    {
        int ret=check_operation_type(argv);
        if(ret==e_encode)
        {
            int res=read_and_validate_encode_args(argv,&encInfo);
            if(res==e_success)
            {
                printf("⏳ Encoding in progress...\n");
                do_encoding(&encInfo);
            }
            else
            {
                printf("💡 Error: Encoding could not be completed. Please check the input.\n");
                return 0;

            }
        }
        else if(ret==e_decode)
        {
        
            int res=read_and_validate_decode_args(argv,&encInfo);
            if(res==d_success)
            {
                printf("⏳ Decoding in progress...\n");

                do_decoding(&encInfo);
                return 0;
                //printf("decoding done\n");
                
            }
            else
            {
                   printf("⚠️ Failed to decode the data.\n");
                   return 0;
            }
        }
        else
        {
             printf("❌ Invalid option.\n");
             return 1;
        }
        
    }

   

    // Test get_image_size_for_bmp
    //img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    //printf("INFO: Image size = %u\n", img_size);

    return 0;
}
void error()
{
    printf("\n======================================================================\n");
printf("💡 USAGE INSTRUCTIONS:\n");
printf("----------------------------------------------------------------------\n");

// Encoding
printf("📝 To encode   👉 ./a.out -e inputimage.bmp secret.txt output.bmp  (output.bmp is optional)\n");

// Decoding
printf("🔍 To decode   👉 ./a.out -d output.bmp output.txt  (output.txt is optional)\n");

printf("======================================================================\n");

}