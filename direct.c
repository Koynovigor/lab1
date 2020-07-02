#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int size_all(char* address, int mode_of_operation);
int sizefile(char* addfile);
int print_size(char* address);


int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        argv[1] = "./";
    }

    if (print_size(argv[1]))
    {
        return 1;
    }

return 0;
}

int print_size(char* address)
{
    if (size_all(address, 1) == -1 )
    {
        return 1;
    }
return 0;
}

int size_all(char* address, int mode_of_operation)
{
    DIR *direct = opendir(address); 
    if(direct == NULL) 
    {
        perror("Не удалось открыть директорию");
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

        char* addfile = calloc(strlen(address) + strlen(entry->d_name), sizeof(char));
        if (addfile == NULL)
        {
            perror("Не удалось выделить память");
            return -1;
        }
        addfile = strcpy(addfile, address); // Чтобы не испортить адрес до директрии.
        strcat(addfile, entry->d_name);

        int sizef = 0;
        if (entry->d_type == DT_REG)
        {
            sizef = sizefile(addfile);
            if (sizef == -1)
            {  
                free(addfile);
                return -1;
            }
            if (mode_of_operation == 1)
            {
                printf("%f k.b  %s\n", (double)((double)sizef/8)/1024, entry->d_name);
                free(addfile);
                continue;
            }
            else
            {
                size += sizef;
                free(addfile);
                continue;
            }
        }


        if (entry->d_type == DT_DIR) 
        {
            addfile = (char*)realloc(addfile, strlen(addfile) + 1);
            if (addfile == NULL)
            {
                perror("Не удалось выделить память");
                return -1;
            }
            strcat(addfile, "/"); // Формирую корректный адрес
            
            sizef = size_all(addfile, 0);
            if (sizef == -1)
            {
                free(addfile);
                return -1;
            }
            

            if (mode_of_operation == 1)
            {
                printf("%f k.b  %s\n", (double)((double)sizef/8)/1024, entry->d_name);
                free(addfile);
                continue;
            }
            else
            {
                size += sizef;
                free(addfile);
                continue;
            }
        }

    }
    closedir(direct);
return size;
}

int sizefile(char* addfile)
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