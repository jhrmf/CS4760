#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>

char * getFile(char[]);

int level = 0;
char finalPath[100] = "";

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
    char command[100] = "ls -r >> ";
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
        else{

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
    printf("%s is current path \n", currentPath);
    struct stat s;

    if(stat(currentPath, &s) == 0){
        char checkFile[100];
        sprintf(checkFile, "%d", level);
        if(s.st_mode & S_IFDIR) {
            //display(file);
            printf("%s is directory \n", file);
            chdir(currentPath);
            level++;
            makeLsFile();
            char levelName[100];
            sprintf(levelName, "%d", level);
            char newFile[100];
            strcpy(newFile, getFile(levelName));
            //printf("passing file %s\n ", newFile);
            checkStat(newFile);
            level--;
            chdir("..");
            return;
        }
        else if(s.st_mode & S_IFREG){
            printf("%s is file \n", file);
            char levelName[100];
            sprintf(levelName, "%d", level);
            char newFile[100];
            if(fileEmpty(levelName) != 0) {
                strcpy(newFile, getFile(levelName));
                checkStat(newFile);
            }
            else{
                chdir("..");
                level--;
                sprintf(levelName, "%d", level);
                if(level == 0 && fileEmpty(levelName) != 0){
                    strcpy(newFile, getFile(levelName));
                    checkStat(newFile);
                }
                else{
                    return;
                }

            }


        }
        else{
            printf("We don't know wtf this is \n");
        }
    }
}


void control(){
    int i, bufferLen;
    char buffer[100];
    char levelName[100];
    char path[100];

    strcpy(path, getPath());
    printf("%s", path); //lists current path

    sprintf(levelName, "%d", level);
    makeLsFile();
    strcpy(buffer, getFile(levelName));
    checkStat(buffer);



    /*for(i = level; i != -1; i--){
        char this[100];
        sprintf(this, "%d", i);
        remove(this);
    }*/
}

int main( int argc, const char* argv[] )
{
    int i;
    for( i = 0; i < argc; i++) {
        printf("%s \n", argv[i]);   //lists commandline arguments
    }

    control();

    remove("finalPath.txt");

}