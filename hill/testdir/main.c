#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

char * getFile(char[]);
bool I = false, L = false, t = false, p = false, i = false, u = false, g = false, s = false, d = false, l = false;
int level = 0, indent = 3;
char finalPath[100] = "";

void display(char file[], char currentPath[], struct stat str){

    char fileWithSpacing[100] = "" , tempFile[100];

    if(I == true){
        int x;
        for(x = indent; x != 0; x--){
            int tempLevel;
            for(tempLevel = level; tempLevel != 0; tempLevel--) {
                strcat(fileWithSpacing, " ");
            }
        }
        strcpy(tempFile, file);
        strcat(fileWithSpacing, tempFile);
        printf("%-20s", fileWithSpacing);
    }
    else{
        int tempLevel;
        for(tempLevel = level; tempLevel != 0; tempLevel--) {
            strcat(fileWithSpacing, "   ");
        }
        strcpy(tempFile, file);
        strcat(fileWithSpacing, tempFile);
        printf("%-20s", fileWithSpacing);
    }
    if(l == true){
        t = true;
        p = true;
        i = true;
        u = true;
        g = true;
        s = true;
    }
    if(L == true){
        if(str.st_mode & S_IFLNK){
            char *sympath;
            if(lstat(currentPath, &str) == 0) {
                sympath = realpath(currentPath, NULL);
                printf("Path of Link is %s\n", sympath);
            }
        }
    }
    if(d == true){
        if(stat(currentPath, &str) == 0){
            char time[100];
            strcpy(time, ctime(&str.st_mtime));
            int timeLen = strlen(time);
            if(time[timeLen-1] == '\n'){
                time[timeLen-1] = 0;
            }
            printf("%-26s ", time);
        }
    }
    if(g == true){
        printf("%-5ld ", getgid());
    }
    if(i == true){
        if(stat(currentPath, &str) == 0) {
            printf("%-3ld ", (long) str.st_nlink);
        }
    }
    if(p == true){
        char mode[10] = "";

        if(stat(currentPath, &str) == 0){
            mode_t permissions = str.st_mode;
            mode[0] = (permissions & S_IRUSR) ? 'r' : '-';
            mode[1] = (permissions & S_IWUSR) ? 'w' : '-';
            mode[2] = (permissions & S_IXUSR) ? 'x' : '-';
            mode[3] = (permissions & S_IRGRP) ? 'r' : '-';
            mode[4] = (permissions & S_IWGRP) ? 'w' : '-';
            mode[5] = (permissions & S_IXGRP) ? 'x' : '-';
            mode[6] = (permissions & S_IROTH) ? 'r' : '-';
            mode[7] = (permissions & S_IWOTH) ? 'w' : '-';
            mode[8] = (permissions & S_IXOTH) ? 'x' : '-';
            mode[9] = '\0';
        }
        printf("%-11s", mode);
    }
    if(s == true){
        char type[2] = "";
        char fullSize[10];
        long size;
        if(stat(currentPath, &str) == 0) {
            size = (long) str.st_size;
            if(size >= 1000){
                size = size / 1000;
                if (size >= 1000){
                    size = size / 1000;
                    if(size >= 1000){
                        size = size / 1000;
                        strcpy(type, "G");
                    }
                    else{
                        strcpy(type, "M");
                    }
                }
                else{
                    strcpy(type, "K");
                }
            }
        }
        sprintf(fullSize, "%d", size);
        strcat(fullSize, type);
        printf("%-6s", fullSize);
    }
    if(t == true){
        char fileType[15];

        switch (str.st_mode & S_IFMT) {
            case S_IFBLK:  strcpy(fileType, "block");                   break;
            case S_IFCHR:  strcpy(fileType, "character device");        break;
            case S_IFDIR:  strcpy(fileType, "directory");               break;
            case S_IFIFO:  strcpy(fileType, "FIFO/pipe");               break;
            case S_IFLNK:  strcpy(fileType, "symlink");                 break;
            case S_IFREG:  strcpy(fileType, "regular file");            break;
            case S_IFSOCK: strcpy(fileType, "socket");                  break;
            default:       strcpy(fileType, "unknown");                 break;
        }

        printf("%-20s", fileType);
    }
    if(u == true){
        printf("%-6ld ", getuid());
    }

    printf("\n");

}

char * getPath(){
    char buffer[100];
    char path[100];
    system("pwd >> temp.txt");
    FILE *pathPtr = fopen("temp.txt", "r");
    while(fgets(buffer, sizeof(buffer), pathPtr)){
        strcpy(path, buffer);
    }
    fclose(pathPtr);
    remove("temp.txt");
    char *fpath = path;
    return fpath;
}

void makeLsFile(){
    char commandTemp[100] = "";
    sprintf(commandTemp, "%d", level);
    char command[100] = "ls >> ";
    strcat(command, commandTemp);
    system(command);
    char levelName[100];
    sprintf(levelName, "%d",level);
    getFile(levelName);
}

void restoreStack(char levelName[]){
    char cloneBuffer[100];
    FILE *clonePtr = fopen("tempStack.txt", "r");     //opens the temporary file
    FILE *clonePtr2 =fopen(levelName, "w");
    while(fgets(cloneBuffer, sizeof(cloneBuffer), clonePtr)){  //seeks through file until end
        fputs(cloneBuffer, clonePtr2);
    }
    fclose(clonePtr);
    remove("tempStack.txt");
    fclose(clonePtr2);
}

char * getFile(char levelName[]){
    char *returnedFile;
    char popBuffer[100], holdFile[100];
    FILE *popPtr = fopen(levelName, "r");     //opens the temporary file
    FILE *popPtr2 =fopen("tempStack.txt", "w");
    int i = 0;
    while(fgets(popBuffer, sizeof(popBuffer), popPtr)){  //seeks through file until end
        if(i==0){
            int buffLen = strlen(popBuffer);
            if(popBuffer[buffLen-1] == '\n'){
                popBuffer[buffLen-1] = 0;
            }
            strcpy(holdFile, popBuffer);

        }
        else if(strcmp(popBuffer, levelName) != 0) {
            fputs(popBuffer, popPtr2);
        }
        i++;
    }
    fclose(popPtr);
    remove(levelName);
    fclose(popPtr2);
    restoreStack(levelName);
    returnedFile = holdFile;
    return returnedFile;
}

int fileEmpty(char levelName[]){
    FILE *ptr = fopen(levelName, "r");
    fseek(ptr, 0, SEEK_END);
    if(ftell(ptr) == 0){
        fclose(ptr);
        return 0;
    }
    fclose(ptr);
    return 1;
}

void checkStat(char file[]){

    char currentPath[100] = "";
    strcpy(currentPath, getPath());
    int pathLen = strlen(currentPath);
    if(currentPath[pathLen-1] == '\n'){
        currentPath[pathLen-1] = 0;
    }
    strcat(currentPath, "/");
    strcat(currentPath, file);
    struct stat s;

    if(stat(currentPath, &s) == 0){

        int tempLevel = level;
        while(tempLevel != 0){
            tempLevel--;
        }
        if(s.st_mode & S_IFDIR) {
            char levelName[100], newfile[100];
            display(file, currentPath, s);
            chdir(currentPath);
            level++;
            sprintf(levelName, "%d", level);
            makeLsFile();
            strcpy(newfile, getFile(levelName));
            checkStat(newfile);
            chdir("..");
            level--;
        }
        else if(s.st_mode & S_IFREG){
            display(file, currentPath, s);
        }
        else{
            printf("We don't know wtf this is \n");
        }
    }
    char checkLevel[100];
    sprintf(checkLevel, "%d", level);
    if(fileEmpty(checkLevel) == 0){
        remove(checkLevel);
        return;
    }
    else{
        char newFile[100];
        strcpy(newFile, getFile(checkLevel));
        checkStat(newFile);
    }

}


void control(){
    int i, bufferLen;
    char buffer[100];
    char levelName[100];
    char path[100];

    strcpy(path, getPath());

    sprintf(levelName, "%d", level);
    makeLsFile();
    strcpy(buffer, getFile(levelName));
    checkStat(buffer);
    remove("0");

}

int main( int argc, char *argv[] )
{

    int opt = 0;
    while ((opt = getopt(argc, argv,"hI:Ldgipstul")) != -1) {
        switch (opt) {
            case 'h' : printf("Display help \n");
                printf("List of arguments:\n");
                printf("-h -> Print a help message and exit.\n");
                printf("I n -> Change indentation to n spaces for each level.\n");
                printf("L -> Follow symbolic links, if any. Default will be to not follow symbolic links.\n");
                printf("t -> Print information on file type.\n");
                printf("p -> Print permission bits as rwxrwxrwx.\n");
                printf("i -> Print the number of links to file in inode table.\n");
                printf("u -> Print the uid associated with the file.\n");
                printf("g -> Print the gid associated with the file.\n");
                break;
            case 'I' :
                I = true;
                indent = atoi(optarg);
                break;
            case ':' :
                break;
            case 'L' :
                L = true;
                break;
            case 'd' :
                d = true;
                break;
            case 'g' :
                g = true;
                break;
            case 'i' :
                i = true;
                break;
            case 'p' :
                p = true;
                break;
            case 's' :
                s = true;
                break;
            case 't' :
                t = true;
                break;
            case 'u' :
                u = true;
                break;
            case 'l' :
                l = true;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    /*
    if(argc > 1) {
        struct stat testStat;
        char testPath[1000];
        int testLen;
        strcpy(testPath, getPath());
        testLen = strlen(testPath);
        if(testPath[testLen-1] == '\n'){
            testPath[testLen-1] = 0;
        }
        strcat(testPath, "/");
        strcat(testPath, argv[optind]);
        if(stat(testPath, &testStat) == 0){
            if(testStat.st_mode & S_IFDIR){
                chdir(testPath);
            }
        }
    }
     */
    control();

    remove("finalPath.txt");

}