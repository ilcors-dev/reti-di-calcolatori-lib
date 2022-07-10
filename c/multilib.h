#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define STR_LEN 512
#define BUFF_READ_LEN 20

/**
 * Checks whether the port is valid or not.
 *
 * @return 0 if the port is not valid, 1 otherwise
 */
int is_port_valid(int port)
{
    return port < 1024 || port > 65535 ? 0 : 1;
}

/**
 * Controlla se la sequenza di caratteri passati è un numero valid
 */
int is_numeric_string(char *str)
{
    int nread = 0;
    while (str[nread] != '\0')
    {
        if ((str[nread] < '0') || (str[nread] > '9'))
        {
            return 0;
        }

        nread++;
    }

    return 1;
}

/**
 * Finds the longest word in a file and returns its length.
 * The longest word gets copied to the `longestWordResult` parameter.
 */
int find_longest_word_in_file(char *file, char *longestWordResult)
{
    int rdFile, nread = 0, currentDim = 0, ris = 0;
    char c;
    char word[STR_LEN], tmpword[STR_LEN];

    if ((rdFile = open(file, O_RDONLY)) < 0)
    {
        perror("open file sorgente");
        return -1;
    }

    while ((nread = read(rdFile, &c, sizeof(c))) > 0)
    {
        if (c != ' ' && c != '\n' && c != '\r')
        {
            tmpword[currentDim] = c;
            currentDim++;
        }
        else
        {
            if (currentDim > ris)
            {
                ris = currentDim;
                tmpword[currentDim] = '\0';
                strcpy(word, tmpword);
            }
            // reset tmpword
            memset(tmpword, 0, sizeof(tmpword));

            currentDim = 0;
        }
    }

    strcpy(longestWordResult, word);

    close(rdFile);

    return ris;
}

/**
 * Deletes the matching `word` from a file
 */
int delete_words_from_file(char *file, char *wordToRemove)
{
    char __tmp[10] = "__tmp_out";
    char c;
    char tmpword[STR_LEN];
    int rdFile, wrFile;
    int nread = 0, removed = 0, currentDim = 0;

    if ((rdFile = open(file, O_RDONLY)) < 0)
    {
        perror("open file sorgente");
        return -1;
    }

    if ((wrFile = open(__tmp, O_CREAT | O_WRONLY, 0644)) < 0)
    {
        perror("open file destinazione");
        return -1;
    }

    while ((nread = read(rdFile, &c, sizeof(c))) > 0)
    {
        // we consider words the block of chars delimited by spaces, new lines or carriage returns
        if (c != ' ' && c != '\n' && c != '\r')
        {
            tmpword[currentDim] = c;
            currentDim++;
        }
        else
        {
            tmpword[currentDim] = '\0';

            // if the wordToRemove matches with the read word, we simply do not write it to the file otherwise
            // we write what we read to the file including the word delimiter char
            if (strcmp(wordToRemove, tmpword) == 0)
            {
                removed++;
            }
            else
            {
                write(wrFile, tmpword, strlen(tmpword));

                // write the end of line chars or space
                write(wrFile, &c, 1);
            }

            currentDim = 0;

            // reset tmpword
            memset(tmpword, 0, sizeof(tmpword));
        }
    }

    // remove the current file & replace it with the tmp one that will get renominated to the original file
    if (remove(file) != 0)
    {
        perror("remove file");
        return -1;
    }

    if (rename(__tmp, file) != 0)
    {
        perror("rename file");
        return -1;
    }

    close(rdFile);
    close(wrFile);

    return removed;
}

/**
 * Lists file in a directory with the subdirs as well.
 */
void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[STR_LEN * 2];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);

            listdir(path, indent + 2);
        }
        else
        {
            printf("%*s- %s\n", indent, "", entry->d_name);
        }
    }
    closedir(dir);
}

/**
 * Conta i file in un direttorio
 */
int count_files_in_dir(char *dirname)
{
    DIR *dir;
    struct dirent *dd;
    int count = 0;
    dir = opendir(dirname);

    if (dir == NULL)
    {
        return -1;
    }

    while ((dd = readdir(dir)) != NULL)
    {
        printf("Trovato il file %s\n", dd->d_name);
        count++;
    }

    // Conta anche direttorio stesso e padre
    printf("Numero totale di file %d\n", count);
    closedir(dir);
    return count;
}

struct FileScanOutput
{
    int nChars;
    int nWords;
    int nLines;
    int errorCode;
};
typedef struct FileScanOutput FileScanOutput;

/**
 * Counts the chars, words and lines of a target file.
 */
FileScanOutput filescan(char *filepath)
{
    static FileScanOutput result;
    result.nChars = 0;
    result.nLines = 0;
    result.nWords = 0;
    result.errorCode = 0;

    int fd, nread = 0;
    char c, prevChar = ' ';
    char word[STR_LEN];

    if ((fd = open(filepath, O_RDONLY)) < 0)
    {
        perror("open file sorgente");
        result.errorCode = 1;
        return result;
    }

    while ((nread = read(fd, &c, sizeof(c))) > 0)
    {
        result.nChars++;

        if (c == ' ' && prevChar == ' ')
        {
            continue;
        }

        if (c == '\n' || c == '\r')
        {
            result.nLines++;
        }
        if (c == ' ')
        {
            result.nWords++;
        }

        prevChar = c;
    }

    printf("nChars: %d\n", result.nChars);
    printf("nLines: %d\n", result.nLines);
    printf("nWords: %d\n", result.nWords);

    return result;
}

/**
 * Scans the files in a given directory by ignoring the ones which size < threshold
 */
int dirscan_threshold(char *targetDir, int threshold)
{
    static int result = 0;
    DIR *dir;
    struct dirent *dd;
    int i, fd_file;
    off_t fileSize;

    result = -1;

    printf("Richiesto direttorio %s\n", targetDir);
    if ((dir = opendir(targetDir)) == NULL)
    {
        return result;
    }

    result = 0;
    while ((dd = readdir(dir)) != NULL)
    {
        fd_file = open(dd->d_name, O_RDONLY);

        if (fd_file < 0)
        {
            printf("File inesistente\n");
            return result;
        }

        fileSize = lseek(fd_file, 0L, SEEK_END);
        if (fileSize >= threshold)
        {
            printf("DEBUG: candidate file size %lld\n", fileSize);
            result++;
        }
    }

    printf("Numero totale di file nel direttorio richiesto %d\n", result);

    return result;
}

/**
 * Sends the file corresponding to the given path through the given socket.
 *
 * @return the number of bytes sent or -1 in case of error.
 */
int send_file_via_socket(int socket_d, char *filename, char *path)
{
    int file_fd, nread, size = 0;
    char fbuff[BUFF_READ_LEN];

    printf("transferring %s to client\n", filename);

    // need to pass the raw STR_LEN size, sizeof(filename) would be wrong for some reason
    // !CHECK!
    write(socket_d, filename, STR_LEN);

    if ((file_fd = open(path, O_RDONLY)) < 0)
    {
        perror("file open");
        return -1;
    }

    // get the file size bytes and send it to the recipient
    size = lseek(file_fd, 0L, SEEK_END);
    lseek(file_fd, 0L, SEEK_SET);
    write(socket_d, &size, sizeof(size));

    printf("sending file size of %s: %d\n", filename, size);

    // buffered read of the file in chunks of BUFF_READ_LEN bytes
    while ((nread = read(file_fd, &fbuff, sizeof(fbuff))) > 0)
    {
        // write number of read bytes from the file
        if (write(socket_d, fbuff, nread) < 0)
        {
            perror("writing to socket_d");
            return -1;
        }
    }

    close(file_fd);

    printf("file transfer completed, sent %d bytes\n", size);

    return size;
}

/**
 * Saves a file received from a socket to the current directory.
 *
 * @return the number of bytes received or -1 in case of error.
 */
int save_file_from_socket(int socket_d)
{
    int nread, file_fd, size = 0;
    char filename[STR_LEN];
    char c;

    // read the filename from the socket
    if (read(socket_d, &filename, sizeof(filename)) < 0)
    {
        perror("reading from socket_d");
        exit(1);
    }

    printf("Downloading %s\n", filename);

    // receive the file size
    if (read(socket_d, &size, sizeof(size)) < 0)
    {
        perror("reading from socket_d");
        return -1;
    }

    printf("received file size: %d\n", size);

    // open the file for writing
    if ((file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
    {
        perror("file open");
        return -1;
    }

    printf("waiting for file..\n");

    // receive byte-a-byte and write them to the file
    for (int i = 0; i < size; i++)
    {
        if (read(socket_d, &c, sizeof(c)) <= 0)
            break;

        if (write(file_fd, &c, sizeof(c)) < 0)
            break;
    }

    close(file_fd);

    printf("wrote %d bytes to disk\n", size);

    return size;
}