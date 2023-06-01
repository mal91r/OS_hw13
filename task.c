#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_DEPTH 100 // максимальная глубина рекурсии

int main() {
    char *dir = "./files"; // директория для создания файлов
    char filename[256];
    char linkname[256];
    int fd, depth = 0;

    mkdir(dir, 0777); // создаем директорию
    chdir(dir); // переходим в директорию

    // создаем регулярный файл с именем "a"
    fd = creat("a", 0644);

    // создаём символьную связь с именем "aa" на файл "a"
    snprintf(filename, sizeof(filename), "a");
    snprintf(linkname, sizeof(linkname), "aa");
    symlink(filename, linkname);

    // проверяем, удалось ли открыть файл "aa"
    while((fd = open(linkname, O_RDONLY)) != -1 && depth < MAX_DEPTH) {
        depth++;
        close(fd);
        // создаем символьную связь с именем "ab" на файл "aa"
        snprintf(filename, sizeof(filename), "aa");
        snprintf(linkname, sizeof(linkname), "ab");
        symlink(filename, linkname);
    }

    // выводим глубину рекурсии на экран
    printf("Depth of recursion: %d\n", depth);

    // удаляем созданные файлы и символьные связи
    unlink("a");
    unlink("aa");
    for(int i = 0; i < depth; i++) {
        snprintf(linkname, sizeof(linkname), "%s/%c%c", dir, 'a' + i, 'a' + i);
        unlink(linkname);
    }
    rmdir(dir); // удаляем директорию

    return 0;
}
