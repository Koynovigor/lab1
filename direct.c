#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sizedir(char* address);
int sizedir_in_dir(char* address); // Я сделал новую функцию. Она почти не отличается от придыдущей,
                                   // но она не выводит размер каждого файла, а суммирует его
                                   // и выводи размер всей дирректории

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        argv[1] = "./";
    }
    char* adddir = strdup(argv[1]);

    if (sizedir(adddir) == -1)
    {
        perror("Error");
        return 1;
    }
    free(adddir);
return 0;
}

int sizedir(char* address)
{
    DIR *direct = opendir(address); 
    if(direct == NULL) 
    {
        return -1;
    }
    struct dirent *entry;
    long size = 0;
    
    while((entry = readdir(direct)) != NULL)
    {
        size = 0;
        char* addfile = strdup(address); // Чтобы не испортить адрес до директрии.
        char* noopen1 = ".";
        char* noopen2 = "..";
        char* namefile = strdup(entry->d_name); 
        if (!strcmp(namefile, noopen1) || !strcmp(namefile, noopen2))
        {
            continue;
        }

        strcat(addfile, namefile);
        if (entry->d_type == DT_DIR) // Так я проверяю директроия это или файл.
        {
            strcat(addfile, "/"); // Формирую корректный адрес
            int sizedirect = sizedir_in_dir(addfile);
            if (sizedirect == -1)
            {
                // free(namefile);  
                free(addfile);
                return -1;
            }
            printf("%f k.b  %s\n", (double)((double)sizedirect/8)/1024, namefile);

            continue;
        }

        
        FILE* file = fopen(addfile, "r"); 
        if (file == NULL)
        {
            perror("Ошибка доступа\n");
            // free(namefile);
            free(addfile);
            continue;
        }

        fseek(file, 0, SEEK_END);
        size = ftell(file);
        printf("%f k.b  %s\n", (double)((double)size/8)/1024, namefile);
        fclose(file);
        free(addfile);
        // free(namefile);
    }
    closedir(direct);
return 0;
}

int sizedir_in_dir(char* address)
{
    DIR *direct = opendir(address); 
    if(direct == NULL) 
    {
        return -1;
    }
    struct dirent *entry;
    int size = 0;
    
    while((entry = readdir(direct)) != NULL)
    {
        char* addfile = strdup(address); // Чтобы не испортить адрес до директрии.
        char* noopen1 = ".";
        char* noopen2 = "..";
        char* namefile = strdup(entry->d_name); 
        if (!strcmp(namefile, noopen1) || !strcmp(namefile, noopen2))
        {
            continue;
        }

        strcat(addfile, namefile);

        if (entry->d_type == DT_DIR) // Так я проверяю директроия это или файл.
        {
            strcat(addfile, "/"); // Формирую корректный адрес
            int sizedirect = sizedir_in_dir(addfile);
            if (sizedirect == -1)
            {
                // free(namefile);
                free(addfile);
                return -1;
            }
            size =+ sizedirect;
            continue;
        }

        
        FILE* file = fopen(addfile, "r"); 
        if (file == NULL)
        {
            perror("Ошибка доступа\n");
            // free(namefile);
            free(addfile);
            continue;
        }

        fseek(file, 0, SEEK_END);
        size += ftell(file);
        fclose(file);
        free(addfile);
        // free(namefile);
    }
    closedir(direct);
return size;
}