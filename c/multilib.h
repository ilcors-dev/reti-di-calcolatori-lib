#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define STR_LEN 512

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