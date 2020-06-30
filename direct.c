#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int print_size_all(char* address);
int sizefile(char* addfile);
int sizedir(char* addressdir);

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        argv[1] = "./";
    }

    if (print_size_all(argv[1]) == -1)
    {
        return 1;
    }

return 0;
}

int print_size_all(char* address) // Ф-ия выводит объём всех фалов и директроий в данной директории
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
        // printf("%s\n", entry->d_name);
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".git"))
        {
            continue;
        }

        char* addfile = calloc(strlen(address), sizeof(char));
        if (addfile == NULL)
        {
            return -1;
        }
        addfile = strcpy(addfile, address); // Чтобы не испортить адрес до директрии.

        addfile = (char*)realloc(addfile, strlen(addfile) + strlen(entry->d_name));
        if (addfile == NULL)
        {
            return -1;
        }
        strcat(addfile, entry->d_name);

        if (entry->d_type == DT_REG)
        {
            size = sizefile(addfile);
            if (size == -1)
            {  
                free(addfile);
                return -1;
            }
            printf("%f k.b  %s\n", (double)((double)size/8)/1024, entry->d_name);
            free(addfile);
        }


        if (entry->d_type == DT_DIR) 
        {
            addfile = (char*)realloc(addfile, 1);
            if (addfile == NULL)
            {
                return -1;
            }
            strcat(addfile, "/"); // Формирую корректный адрес
            
            size = sizedir(addfile);
            if (size == -1)
            {  
                free(addfile);
                return -1;
            }

            printf("%f k.b  %s\n", (double)((double)size/8)/1024, entry->d_name);
            free(addfile);
        }

    }
    closedir(direct);
return 0;
}

int sizefile(char* addfile) // Ф-ия считает объём файла
{
    FILE* file = fopen(addfile, "r"); 
    if (file == NULL)
    {
        perror("Ошибка доступа\n");
        free(addfile);
    return -1;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);
return size;
}

int sizedir(char* addressdir) // Эта ф-ия очень похожа на первую, но тут считается объм всех файлов директории
{
    DIR *direct = opendir(addressdir); 
    if(direct == NULL) 
    {
        return -1;
    }
    struct dirent *entry;
    int size = 0;

    while((entry = readdir(direct)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".git"))
        {
            continue;
        }

        char* addfile = calloc(strlen(addressdir), sizeof(char));
        if (addfile == NULL)
        {
            return -1;
        }
        addfile = strcpy(addfile, addressdir); // Чтобы не испортить адрес до директрии.

        addfile = (char*)realloc(addfile, strlen(addfile) + strlen(entry->d_name));
        if (addfile == NULL)
        {
            return -1;
        }
        strcat(addfile, entry->d_name);

        int sizef = 0;
        if (entry->d_type == DT_REG)
        {
            int sizef = sizefile(addfile);
            if (sizef == -1)
            {  
                free(addfile);
                return -1;
            }
            size += sizef;
            free(addfile);
        }

        if (entry->d_type == DT_DIR) 
        {
            addfile = (char*)realloc(addfile, 1);
            if (addfile == NULL)
            {
                return -1;
            }
            strcat(addfile, "/"); // Формирую корректный адрес

            sizef = sizedir(addfile);
            if (sizef == -1)
            {  
                free(addfile);
                return -1;
            }

            size += sizef;
            free(addfile);
        }

    }
    closedir(direct);
return size;
}
