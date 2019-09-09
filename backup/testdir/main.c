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
//above is the boolean variables corresponding with their respective argument, marked true when entered
int level = 0, indent = 4;
//level holds the global level the program is at in the directory tree, and indent stores the indentation specified or default of 4
char finalPath[100] = "";
//final path holds the values of the path to the given file

void display(char file[], char currentPath[], struct stat str){ //function to display file information

    char fileWithSpacing[100] = "" , tempFile[100]; //char for file with indentation and temp char

    if(I == true){      //entered if I was specified
        int x;
        for(x = indent; x != 0; x--){   //set x to indent value specified or default
            int tempLevel;
            for(tempLevel = level; tempLevel != 0; tempLevel--) {   //set tempLevel to current level
                strcat(fileWithSpacing, " ");       //add spacing prior to file for required format indentation
            }
        }
        strcpy(tempFile, file);     //set tempFile equal to file
        strcat(fileWithSpacing, tempFile);  //add file name to the spacing required
        printf("%-20s", fileWithSpacing);   //print out the file with indentation
    }
    else{       //if indentation was not specified
        int tempLevel;
        for(tempLevel = level; tempLevel != 0; tempLevel--) {
            strcat(fileWithSpacing, "   ");     //default of 4 spaces is used
        }
        strcpy(tempFile, file);
        strcat(fileWithSpacing, tempFile);
        printf("%-20s", fileWithSpacing);       //print file with default
    }
    if(l == true){  //if -l is entered by user, set t p u i g s to true as specified
        t = true;
        p = true;
        i = true;
        u = true;
        g = true;
        s = true;
    }
    if(L == true){  //if -L is entered by user, get path of the link to said file
        if(str.st_mode & S_IFLNK){
            char *sympath;
            if(lstat(currentPath, &str) == 0) {
                sympath = realpath(currentPath, NULL);
                printf("Path of Link is %s\n", sympath);
            }
        }
    }
    if(d == true){  //if -d is entered by user, display last modified date of file
        if(stat(currentPath, &str) == 0){
            char time[100];
            strcpy(time, ctime(&str.st_mtime)); //store the time in a string
            int timeLen = strlen(time);
            if(time[timeLen-1] == '\n'){
                time[timeLen-1] = 0;        //remove newline
            }
            printf("%-26s ", time); //display
        }
    }
    if(g == true){  //if -g is entered by user, display the GID of file
        printf("%-5ld ", str.st_uid);
    }
    if(i == true){  //if -i is entered by user, display the number of links to file
        if(stat(currentPath, &str) == 0) {
            printf("%-3ld ", (long) str.st_nlink);
        }
    }
    if(p == true){ //if -p is entered by user, display permissions of file
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
    if(s == true){  //if -s is entered by user, display size with corresponding size formatting for KB, MB, GB, or bytes
        char type[2] = "";
        char fullSize[10];
        long size;
        if(stat(currentPath, &str) == 0) {
            size = (long) str.st_size;  //get size of file in bytes
            if(size >= 1000){
                size = size / 1000;
                if (size >= 1000){
                    size = size / 1000;
                    if(size >= 1000){
                        size = size / 1000;
                        strcpy(type, "G"); //store G as type if file is large enough to be 1 or more Gigabytes
                    }
                    else{
                        strcpy(type, "M"); //store G as type if file is large enough to be 1 or more Megabytes
                    }
                }
                else{
                    strcpy(type, "K"); //store G as type if file is large enough to be 1 or more Kilobytes
                }
            }
        }
        sprintf(fullSize, "%d", size);  //convert the size into a string
        strcat(fullSize, type);     //concatenate the type with the size string
        printf("%-6s", fullSize);   //display the size
    }
    if(t == true){  //if -t is entered by user, display file information
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

        printf("%-20s", fileType);  //display file information
    }
    if(u == true){
        printf("%-6ld ", str.st_uid);     //if -u is true, display uid
    }

    printf("\n");   //end with a newline.

}

char * getPath(){   //function for retrieving the current path to the directory the program is in
    char buffer[100];
    char path[100];
    system("pwd >> temp.txt"); //pipe the path to the working directory into a temp file.
    FILE *pathPtr = fopen("temp.txt", "r");     //open the temp file containing the path
    while(fgets(buffer, sizeof(buffer), pathPtr)){      //traverse through the file by line
        strcpy(path, buffer);   //copy the path into the buffer
    }
    fclose(pathPtr);    //close the temp file
    remove("temp.txt"); //delete the temp file
    char *fpath = path; //store the path into a string pointer
    return fpath; //return the string pointer to the path
}

void makeLsFile(){  //function for creating a file that stores the value of LS and uses the file as a stack
    char commandTemp[100] = "";
    sprintf(commandTemp, "%d", level);      //convert the current level into a string
    char command[100] = "ls >> ";   //string containing the ls argument needed for building the file
    strcat(command, commandTemp);   //concatenate the command and the file
    system(command);    //run the command on the system, piping the ls into a file named after the level
    char levelName[100];
    sprintf(levelName, "%d",level);     //make another string of just the level name
    getFile(levelName); //return the level name
}

void restoreStack(char levelName[]){
    char cloneBuffer[100];
    FILE *clonePtr = fopen("tempStack.txt", "r");     //opens the temporary file
    FILE *clonePtr2 =fopen(levelName, "w");     //remakes a file with the name of the level
    while(fgets(cloneBuffer, sizeof(cloneBuffer), clonePtr)){  //seeks through file until end
        fputs(cloneBuffer, clonePtr2); //copy each line into the new file with the name of the level
    }
    fclose(clonePtr);   //close the temp file
    remove("tempStack.txt"); //remove the temp file
    fclose(clonePtr2);  //close the new level file
}

char * getFile(char levelName[]){
    char *returnedFile;
    char popBuffer[100], holdFile[100];
    FILE *popPtr = fopen(levelName, "r");     //opens the temporary file
    FILE *popPtr2 =fopen("tempStack.txt", "w");
    int i = 0;
    while(fgets(popBuffer, sizeof(popBuffer), popPtr)){  //seeks through file until end
        if(i==0){   //ensure that the first instance is not coppied over in some cases
            int buffLen = strlen(popBuffer);    //set buffLen integer equal to she string length of the buffer
            if(popBuffer[buffLen-1] == '\n'){       //remove the new line character at the end of the buffer
                popBuffer[buffLen-1] = 0;
            }
            strcpy(holdFile, popBuffer);    //copy the value into the holdFile variable

        }
        else if(strcmp(popBuffer, levelName) != 0) {        //store all files other than the level file into newfile
            fputs(popBuffer, popPtr2);
        }
        i++;    //increment i
    }
    fclose(popPtr); //close the level file
    remove(levelName);  //remove the level file
    fclose(popPtr2);    //close the temp file
    restoreStack(levelName);    //rebuild the level file
    returnedFile = holdFile;    //point to the name of the level file
    return returnedFile;    //return the level file
}

int fileEmpty(char levelName[]){    //function to check if file is empty
    FILE *ptr = fopen(levelName, "r");  //open the level file
    fseek(ptr, 0, SEEK_END);    //seek to the end
    if(ftell(ptr) == 0){    //if the file is empty
        fclose(ptr);    //close the file and return 0
        return 0;
    }
    fclose(ptr);    //otherwise it must not be empty so return 1
    return 1;
}

void checkStat(char file[]){    // function for evaluation the stat of each file

    char currentPath[100] = "";     //set current path to null for later use
    strcpy(currentPath, getPath()); //copy the current working path to current path
    int pathLen = strlen(currentPath);  //set pathLen to the length of the current path
    if(currentPath[pathLen-1] == '\n'){     //remove the newline from the current path
        currentPath[pathLen-1] = 0;
    }
    strcat(currentPath, "/");   //insert the slash before adding the file at the end of the path
    strcat(currentPath, file);  //insert the file to the end of the path
    struct stat s;

    if(stat(currentPath, &s) == 0){

        int tempLevel = level;  //set a temporary level holder
        while(tempLevel != 0){  //as long the level is not 0 reset it
            tempLevel--;
        }
        if(s.st_mode & S_IFDIR) {   //check if the file is a directory
            char levelName[100], newfile[100];
            display(file, currentPath, s);  //send it to the display function
            chdir(currentPath); //change the directory to said directory
            level++;    //increase the level
            sprintf(levelName, "%d", level);        //convert the level to a string
            makeLsFile();   //create a new ls file for the new working directory
            strcpy(newfile, getFile(levelName));    //copy the name of the new file into a holder
            checkStat(newfile); //recall this function
            chdir("..");    //go back after we have traversed the new directory
            level--;    //decrease the level
        }
        else if(s.st_mode & S_IFREG){   //check if the file is a regular file
            display(file, currentPath, s);  //display the file
        }
        else{
            perror("Unknown File\n");       //display the file as unknown that is neither directory nor file
        }
    }
    char checkLevel[100];
    sprintf(checkLevel, "%d", level);   //convert the level into a string
    if(fileEmpty(checkLevel) == 0){     //if the file is empty
        remove(checkLevel);     //remove the file
        return; //return to last call
    }
    else{   //otherwise file is not empty
        char newFile[100];  //store the name of the current file over the next two lines
        strcpy(newFile, getFile(checkLevel));
        checkStat(newFile); //recall the function
    }

}


void control(){ //function that controls the program
    int i, bufferLen;
    char buffer[100];
    char levelName[100];
    char path[100];

    strcpy(path, getPath());    //get the current working directory path

    sprintf(levelName, "%d", level);    //get the current level
    makeLsFile();   //make the current ls file
    strcpy(buffer, getFile(levelName)); //copy the name of the file to the buffer string
    checkStat(buffer);  //call checkStat
    remove("0");    //at the end, remove the first ls file named after its level, 0

}

int main( int argc, char *argv[] )
{

    int opt = 0;
    while ((opt = getopt(argc, argv,"hI:Ldgipstul")) != -1) {   //GET OPT WOOOOO
        switch (opt) {  //start a switch statement for the commandline arguments
            case 'h' : printf("Display help \n");   //display a detailed help message
                printf("Invocation: dt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirname]\n\n");
                printf("List of arguments:\n");
                printf("-h -> Print a help message and exit.\n");
                printf("-I n -> Change indentation to n spaces for each level.\n");
                printf("-L -> Follow symbolic links, if any. Default will be to not follow symbolic links.\n");
                printf("-t -> Print information on file type.\n");
                printf("-p -> Print permission bits as rwxrwxrwx.\n");
                printf("-i -> Print the number of links to file in inode table.\n");
                printf("-u -> Print the uid associated with the file.\n");
                printf("-g -> Print the gid associated with the file.\n");
                printf("-s -> Print size of the file in bytes, megabytes, kilobytes, or gigabytes, depending on size.\n");
                printf("-d -> Show the last time of file modification\n");
                printf("-l -> This option acts as if arguemnts tpiugs are all given\n");
                exit(0);
                break;
            case 'I' :      //set I equal to true if -I is listed
                I = true;
                indent = atoi(optarg);
                break;
            case ':' :      //require a variable after I because it needs an indent value
                break;
            case 'L' :      //set L equal to true if -L is listed
                L = true;
                break;
            case 'd' :      //set d equal to true if -d is listed
                d = true;
                break;
            case 'g' :      //set g equal to true if -g is listed
                g = true;
                break;
            case 'i' :      //set i equal to true if -i is listed
                i = true;
                break;
            case 'p' :      //set p equal to true if -p is listed
                p = true;
                break;
            case 's' :      //set s equal to true if -s is listed
                s = true;
                break;
            case 't' :      //set t equal to true if -t is listed
                t = true;
                break;
            case 'u' :      //set u equal to true if -u is listed
                u = true;
                break;
            case 'l' :      //set l equal to true if -l is listed
                l = true;
                break;
            default:        //if nothing is listed print an error
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }
    }

    if(argc >= 2) {     //if there are more than 1 arguements (so not just the execution)
        struct stat testStat;
        char testPath[1000];
        int testLen;
        strcpy(testPath, getPath());    //get the path and store it
        testLen = strlen(testPath);     //get the length of the path
        if(testPath[testLen-1] == '\n'){        //remove the new line
            testPath[testLen-1] = 0;
        }
        strcat(testPath, "/");      //add a slash at the end of the path before the file
        if(argc == 2){      //if there are just 2 arguments
            strcat(testPath, argv[1]);  //add the value to the end of the path
        }
        else {
            strcat(testPath, argv[optind]);     //add the last argument to the end of the path
        }
        if(stat(testPath, &testStat) == 0){
            if(testStat.st_mode & S_IFDIR){ //check if the path is a directory
                chdir(testPath);        //change directory if so
            }
        }
    }

    control();      //call the control function


}