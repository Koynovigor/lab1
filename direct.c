#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

long sizedir(char* address);

int main(int a, char* add[])
{
    if (add[1] == NULL)
    {
        add[1] = "./";
    }
    char* adddir = strdup(add[1]);
    long size = sizedir(adddir);
    if (size == -1)
    {
        perror("Error");
        return 1;
    }
    double sizekb = (double)((double)size/8)/1024; // Перевожу в килобайты
    printf("directory size is %f k.b.\n", sizekb);
return 0;
}

long sizedir(char* address)
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
        // printf("%s --- %d --- %ld --- %ld --- %d\n", entry->d_name, entry->d_type, entry->d_ino, entry->d_off, entry->d_reclen);
        // printf("%s\n", entry -> d_name);
        char* addfile = strdup(address); // Чтобы не испортить адрес до директрии.
        char* noopen1 = "."; // Я не знаю, что это за файлы. 
        char* noopen2 = ".."; // И когда я их открывал, проискодило что-то странное с размером.
        int typedir = 4; // В структуре dirent есть поле d_type, у директорий он равен 4.
        char* namefile = strdup(entry->d_name); 
        if (!strcmp(namefile, noopen1) || !strcmp(namefile, noopen2))
        {
            continue;
        }

        strcat(addfile, namefile);
        if (entry->d_type == typedir) // Так я проверяю директроия это или файл.
        {
            strcat(addfile, "/"); // Формирую корректный адрес
            long sizedirect = sizedir(addfile);
            if (sizedirect == -1)
            {
                return -1;
            }
            size += sizedirect;
            continue;
        }

        
        FILE* file = fopen(addfile, "r"); 
        if (file == NULL)
        {
            return -1;
        }
        fseek(file, 0, SEEK_END);
        size += ftell(file);
        fclose(file);
    }
    closedir(direct);
return size;
}