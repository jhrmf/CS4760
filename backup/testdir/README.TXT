Assignment 1
CS4760 Fall Semester
Joseph Hill

Invocation of project:
- dt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirname]

Project Description
Create a new version of bash's ls command to display certain details of files, while traversing a directory that is
given, or the current working directory if not. The invocation is above, and their functions are respectively listed
below.

h Print a help message and exit.
I n Change indentation to n spaces for each level.
L Follow symbolic links, if any. Default will be to not follow symbolic links.
t Print information on file type.
p Print permission bits as rwxrwxrwx.
i Print the number of links to file in inode table.
u Print the uid associated with the file.
g Print the gid associated with the file.
s Print the size of file in bytes. If the size is larger than 1K, indicate the size in KB with a suffix K; if the size is
larger than 1M, indicate the size in MB with a suffix M; if the size is larger than 1G, indicate the size in GB
with a suffix G.
d Show the time of last modification.
l This option will be used to print information on the file as if the options tpiugs are all specified.

Note: As said in class, for whatever reason my file system thinks that all files have links except for directories, and
will list the path of the link when L is a commandline argument. I have code that should list correctly if the files do
not have links, however, my filesystem is pretty sure there are. I would be more than happy to troubleshoot this if
needed, but I cannot find any reason or documentation on this being an issue for anyone else.

Any code that is not my own was taken from Linux Man pages, specfically a case function for stat(), and I do not believe
anything else is not my own.