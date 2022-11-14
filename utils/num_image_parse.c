#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c

/* This is just a sample code, modify it to meet your need */
int main(int argc, char **argv)
{
    DIR* FD;
    struct dirent* in_file;
//    FILE    *common_file;
    FILE    *entry_file;
    FILE    *text_file;
    char    buffer[BUFSIZ];

    /* Openiing common file for writing */
    text_file = fopen("image_name.txt", "w");
    if (text_file == NULL)
    {
        fprintf(stderr, "Error : Failed to open common_file - %s\n", strerror(errno));

        return 1;
    }

    /* Scanning the in directory */
    if (NULL == (FD = opendir ("assets/"))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        fclose(text_file);

        return 1;
    }

    while ((in_file = readdir(FD))) 
    {
        /* On linux/Unix we don't want current and parent directories
         * On windows machine too, thanks Greg Hewgill
         */
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        char file_name[20];
        strcpy(file_name, in_file->d_name);
        if (file_name[0] == '5' || file_name[0] == '6' || file_name[0] == '7'){
            fprintf(text_file, file_name);
            int num_dice = 0; 
            if (file_name[0] == '5') num_dice = 5; //12
            if (file_name[0] == '6') num_dice = 6; //14
            if (file_name[0] == '7') num_dice = 7; //16
            

            if (file_name[(num_dice+1)*2] == '.'){
                int i = 0; 
                for (i = 0; i < num_dice; i++){
                fprintf(text_file, " "); 
                fprintf(text_file, "%c", file_name[i*2 + 3]);
                }
            }
            else {
                fprintf(text_file, " error");
            }
            

            
            fprintf(text_file, "\n");
        }
        


        //entry_file = fopen(in_file->d_name, "rw");
        //if (entry_file == NULL)
        //{
        //    fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
        //    fclose(common_file);
//
        //    return 1;
        //}
//
        ///* Doing some struf with entry_file : */
        ///* For example use fgets */
        //while (fgets(buffer, BUFSIZ, entry_file) != NULL)
        //{
        //    /* Use fprintf or fwrite to write some stuff into common_file*/
        //}
//
        ///* When you finish with the file, close it */
        //fclose(entry_file);
    }

    /* Don't forget to close common file before leaving */
    fclose(text_file);

    return 0;
}

