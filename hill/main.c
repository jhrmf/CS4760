#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
//#include "tree.h"


/*void optFunc(){
    //dt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirname]
    int opt = 0;
    while ((opt = getopt(argc, argv,"-h-I:-L-d-g-i-p-s-t-u-l")) != -1) {
        switch (opt) {
            case '-h' : printf("Display help \n");
                break;
            case '-I' : printf("Set indentation \n");
                break;
            case ':' : printf("Indentation Value \n");
                break;
            case '-L' : printf("Follow symbolic links \n");
                break;
            case '-d' : printf("Show last modified date \n");
                break;
            case '-g' : printf("Show GID \n");
                break;
            case '-i' : printf("number of links in inode table \n");
                break;
            case '-p' : printf("Show last modified date \n");
                break;
            case '-s' : printf("size of bytes in corresponding size \n");
                break;
            case '-t' : printf("Show last modified date \n");
                break;
            case '-u' : printf("Print UID \n");
                break;
            case '-l' : printf("TPIUGS displayed together \n");
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }

}*/

void checkStat(char path[]){

    struct stat s;

    if(stat(path, &s) == 0){
        if(s.st_mode & S_IFDIR){
            printf("%s is a directory \n", path);
        }
        else if(s.st_mode & S_IFREG){
            printf("%s is a file \n", path);
        }
        else{
            printf("We don't know wtf this is \n");
        }
    }
}

void loadDirectoryStack(char *path){
    char buffer[100];
    char buffStor[100] = "";
    int pathLen = strlen(path);
    char tempPath[100] = "";

    if(path[pathLen-1] == '\n'){
        path[pathLen-1] = 0;
    }

    FILE *loadPtr = fopen("stack.txt", "r");
    while(fgets(buffer, sizeof(buffer), loadPtr)){
        strcpy(tempPath, path);
        strcat(tempPath, "/");
        strcpy(buffStor, buffer);
        int len = strlen(buffStor);
        if( buffStor[len-1] == '\n') {
            buffStor[len - 1] = 0;
        }
        strcat(tempPath, buffStor);
        checkStat(tempPath);
    }

    fclose(loadPtr);

}

void makeLsFile(){
    system("ls >> stack.txt");
}

void restoreStack(){
    char cloneBuffer[100];
    FILE *clonePtr = fopen("tempStack.txt", "r");     //opens the temporary file
    FILE *clonePtr2 =fopen("stack.txt", "w");
    while(fgets(cloneBuffer, sizeof(cloneBuffer), clonePtr)){  //seeks through file until end
        fputs(cloneBuffer, clonePtr2);
    }
    fclose(clonePtr);
    remove("tempStack.txt");
    fclose(clonePtr2);
}

void stackPop(){
    char popBuffer[100];
    FILE *popPtr = fopen("stack.txt", "r");     //opens the temporary file
    FILE *popPtr2 =fopen("tempStack.txt", "w");
    int i = 0;
    while(fgets(popBuffer, sizeof(popBuffer), popPtr)){  //seeks through file until end
        if(i>0){
            fputs(popBuffer, popPtr2);
        }
        i++;
    }
    fclose(popPtr);
    remove("stack.txt");
    fclose(popPtr2);
    restoreStack();
}
void stackPush(char str[]){
    char pushBuffer[100];
    FILE *pushPtr = fopen("stack.txt", "r");
    FILE *pushPtr2 = fopen("tempStack.txt", "w");
    int i = 0;
    while(fgets(pushBuffer, sizeof(pushBuffer), pushPtr)){  //seeks through file until end
        if(i==0){
            fputs(str, pushPtr2);
            fputs("\n", pushPtr2);
        }
        fputs(pushBuffer, pushPtr2);
        i++;
    }
    fclose(pushPtr);
    remove("stack.txt");
    fclose(pushPtr2);
    restoreStack();
}
void stackDeletePos(int n){
    int i = 0;
    char deleteBuffer[100];
    FILE *deletePtr = fopen("stack.txt", "r");
    FILE *deletePtr2 = fopen("tempStack.txt", "w");

    while(fgets(deleteBuffer, sizeof(deleteBuffer), deletePtr)){
        if(n != i){
            fputs(deleteBuffer, deletePtr2);
        }
        i++;
    }
    fclose(deletePtr);
    remove("stack.txt");
    fclose(deletePtr2);
    restoreStack();
}

int getPosInStack(char *str){ //returns -1 if doesn't exist in stack
    int i = 0, exists = -1;
    char temp[] = "\n";
    strcat(str, temp);
    char posBuffer[100];
    FILE *posPtr = fopen("stack.txt", "r");
    while(fgets(posBuffer, sizeof(posBuffer), posPtr)){
        if(strcmp(str, posBuffer) == 0){
            printf("here");
            exists = i;
        }
        i++;
    }
    fclose(posPtr);
    return exists;

}

int main( int argc, const char* argv[] )
{
    remove("stack.txt");
    int i;
    char buffer[100];
    char path[100];
    for( i = 0; i < argc; i++) {
        printf("%s \n", argv[i]);   //lists commandline arguments
    }
    system("pwd >> temp.txt");      //reads path into temporary file
    FILE *ptr = fopen("temp.txt", "r");     //opens the temporary file

    while(fgets(buffer, sizeof(buffer), ptr)){  //seeks through file until end
        strcpy(path, buffer);                   //stores contents (current path) into cstring
    }

    fclose(ptr);
    remove("temp.txt");

    makeLsFile();
    loadDirectoryStack(path);


}