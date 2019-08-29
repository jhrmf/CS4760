#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
//#include "tree.h"

void optFunc(){
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

}

int main( int argc, const char* argv[] )
{
    printf("%s \n", argv[argc]);

}