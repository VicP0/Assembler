#include "pre_assembler.h"
#include "encoder.h"


int main(int argc, char *argv[]) {
    FILE **readFiles, **writeFiles, **postDeployReadFiles, **preScanFiles, **preScanDataFiles, **finalFiles;
    hashTable **macrosTables;
    hashTableInt **labelsTables, **entriesTables, **externsTables;
    int i, j, lastFileIndex = 0;
    int IC = 100, DC = 0, lines = 0;
    int longestMacroBodyLength = -1;

    char **preDeployFileNames, **postDeployFileNames, **preScanFileNames, **preScanDataFileNames, **finalFileNames;
    char **inputFileNames;
    int currentFileNameLength;

    const char filePostfix[] = ".as";
    const char fileWritePostfix[] = ".am";
    const char preScanFilePostfix[] = ".temp_ob";
    const char preScanDataFilePostfix[] = ".temp_data_ob";
    const char finalFilePostfix[] = ".ob";

    int numOfFiles = argc - 1;

    /* no files specified */
    if (argc == 1) return 0;

    /* files received from program arguments */
    readFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    writeFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    postDeployReadFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    preScanFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    preScanDataFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    finalFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);

    /* tables for each file name */
    macrosTables = (hashTable **) malloc(sizeof(hashTable *) * numOfFiles);
    labelsTables = (hashTableInt **) malloc(sizeof(hashTable *) * numOfFiles);

    /* file names for each step */
    preDeployFileNames = (char **) calloc(numOfFiles, sizeof(char));
    postDeployFileNames = (char **) calloc(numOfFiles, sizeof(char));
    preScanFileNames = (char **) calloc(numOfFiles, sizeof(char));
    preScanDataFileNames = (char **) calloc(numOfFiles, sizeof(char));
    finalFileNames = (char **) calloc(numOfFiles, sizeof(char));
    inputFileNames = (char **) calloc(numOfFiles, sizeof(char));

    for (i = 0; i < numOfFiles; i++) {
        currentFileNameLength = strlen(argv[i + 1]);
        inputFileNames[i] = calloc(currentFileNameLength, sizeof(char));
        strcpy(inputFileNames[i], argv[i + 1]);

        preDeployFileNames[i] = (char *) calloc(currentFileNameLength + strlen(filePostfix),
                                                sizeof(char)); /* adding 4 for .as postfix */
        postDeployFileNames[i] = (char *) calloc(currentFileNameLength + strlen(fileWritePostfix),
                                                 sizeof(char)); /* adding 4 as adding .am postfix */
        preScanFileNames[i] = (char *) calloc(currentFileNameLength + strlen(preScanFilePostfix),
                                              sizeof(char)); /* adding 8 as adding .temp_ob postfix */
        preScanDataFileNames[i] = (char *) calloc(currentFileNameLength + strlen(preScanDataFilePostfix),
                                                  sizeof(char)); /* adding 12 as adding .temp_data_ob postfix */
        finalFileNames[i] = (char *) calloc(currentFileNameLength + strlen(finalFilePostfix),
                                            sizeof(char)); /* adding 4 as adding .ob postfix */

        if (preDeployFileNames[i] == NULL || postDeployFileNames[i] == NULL
            || preScanFileNames[i] == NULL || finalFileNames[i] == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

        /* setting every file name */
        strcpy(preDeployFileNames[i], inputFileNames[i]);
        strcpy(postDeployFileNames[i], inputFileNames[i]);
        strcpy(preScanFileNames[i], inputFileNames[i]);
        strcpy(preScanDataFileNames[i], inputFileNames[i]);
        strcpy(finalFileNames[i], inputFileNames[i]);

        /* adding ".as" postfix */
        strcat(preDeployFileNames[i], filePostfix);
        /* adding ".am" postfix */
        strcat(postDeployFileNames[i], fileWritePostfix);
        /* adding ".temp_ob" postfix */
        strcat(preScanFileNames[i], preScanFilePostfix);
        /*adding ".temp_data_ob" postfix */
        strcat(preScanDataFileNames[i], preScanDataFilePostfix);
        /* adding ".ob" postfix */
        strcat(finalFileNames[i], finalFilePostfix);

        /* trying to open file to read macros from */
        if ((readFiles[i] = fopen(preDeployFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Error trying to open file %s\n", preDeployFileNames[i]);
            free(preDeployFileNames[i]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(readFiles[j]);
            free(readFiles);
            return MAIN_ERROR_CODE;
        }
        /* trying to open file to write to with all macros */
        if ((writeFiles[i] = fopen(postDeployFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", postDeployFileNames[i]);
            free(postDeployFileNames[i]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            return MAIN_ERROR_CODE;
        }

        /* initializing table in memory for current file */
        macrosTables[i] = (hashTable *) malloc(sizeof(hashTable));

        if (!init_hash_table(macrosTables[i], HASH_TABLE_SIZE)) {
            fprintf(stderr, "Unsuccessful initialization of hash table.\n");
            return MAIN_ERROR_CODE;
        }
    }

    entriesTables = (hashTableInt **) malloc(sizeof(hashTableInt *));
    externsTables = (hashTableInt **) malloc(sizeof(hashTableInt *));

    for (i = 0; i < numOfFiles; i++) {
        printf("Processing file %s...\n", inputFileNames[i]);
        if (!deploy_macros(readFiles[i], writeFiles[i], macrosTables[i], &longestMacroBodyLength)) {
            IC = 100;
            DC = 0;
            /* remove .am file */
            fclose(writeFiles[i]);
            remove(postDeployFileNames[i]);
            continue;
        }

        entriesTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));
        externsTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));
        labelsTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));

        if (!entriesTables[i] || !externsTables[i] || !labelsTables[i]) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

        if (!init_hash_table_int(entriesTables[i], HASH_TABLE_SIZE) ||
            !init_hash_table_int(externsTables[i], HASH_TABLE_SIZE) ||
            !init_hash_table_int(labelsTables[i], HASH_TABLE_SIZE)) {
            fprintf(stderr, "Unsuccessful initialization of hash table.\n");
            break;
        }

        if ((preScanFiles[i] = fopen(preScanFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanFileNames[i]);
            break;
        }
        if ((preScanDataFiles[i] = fopen(preScanDataFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanDataFileNames[i]);
            break;
        }

        if ((postDeployReadFiles[i] = fopen(postDeployFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", postDeployFileNames[i]);
            free(postDeployFileNames[i]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            break;
        }
        fflush(stdout);
        if ((!first_scan(preScanFileNames[i], postDeployReadFiles[i], preScanFiles[i], preScanDataFiles[i],
                         labelsTables[i], &IC, &DC, entriesTables[i], externsTables[i], &lines)) || lines > MAX_LINES) {
            if (lines > MAX_LINES) {
                fprintf(stderr, "%s", NUMBER_OF_LINE_EXCEED_MAX_LINES);
            }
            IC = 100;
            DC = 0;
            lines = 0;
            /* remove .am, .temp_ob,.temp_data_ob files */
            fclose(preScanFiles[i]);
            fclose(postDeployReadFiles[i]);
            fclose(preScanDataFiles[i]);
            remove(preScanFileNames[i]);
            remove(postDeployFileNames[i]);
            remove(preScanDataFileNames[i]);

            continue;
        }
        fflush(stdout);
        fclose(preScanDataFiles[i]);
        if ((preScanDataFiles[i] = fopen(preScanDataFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanDataFileNames[i]);
            break;
        }
        combine_files(preScanFiles[i], preScanDataFiles[i]);

        fclose(preScanFiles[i]);
        fclose(preScanDataFiles[i]);
        remove(preScanDataFileNames[i]);
        if ((finalFiles[i] = fopen(finalFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", finalFileNames[i]);
            break;
        }

        if ((preScanFiles[i] = fopen(preScanFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanFileNames[i]);
            break;
        }

        if (!second_scan(preScanFileNames[i], preScanFiles[i], finalFiles[i], labelsTables[i],
                         entriesTables[i], externsTables[i], &IC, &DC)) {
            IC = 100;
            DC = 0;

            /* remove .am, .temp_ob, .ob files */
            fclose(preScanFiles[i]);
            fclose(postDeployReadFiles[i]);
            fclose(finalFiles[i]);
            remove(preScanFileNames[i]);
            remove(postDeployFileNames[i]);
            remove(finalFileNames[i]);

            continue;
        }
        fflush(stdout);
        lastFileIndex++;
        /* remove .am, .temp_ob */
        remove(postDeployFileNames[i]);
        remove(preScanFileNames[i]);
        remove(preScanDataFileNames[i]);

        printf("Finished processing file %s!\n\n", inputFileNames[i]);
    }

    return 0;
/*
    for (i = 0; i < numOfFiles; i++) {
        if (readFiles[i]) fclose(readFiles[i]);
        if (writeFiles[i]) fclose(writeFiles[i]);
        if (postDeployReadFiles[i]) fclose(postDeployReadFiles[i]);
        if (preScanFiles[i]) fclose(preScanFiles[i]);
        if (preScanDataFiles[i]) fclose(preScanDataFiles[i]);
        if (finalFiles[i]) fclose(finalFiles[i]);

        if (inputFileNames[i]) free(inputFileNames[i]);
        if (preDeployFileNames[i]) free(preDeployFileNames[i]);
        if (postDeployFileNames[i]) free(postDeployFileNames[i]);
        if (preScanFileNames[i]) free(preScanFileNames[i]);
        if (preScanDataFileNames[i]) free(preScanDataFileNames[i]);
        if (finalFileNames[i]) free(finalFileNames[i]);

       // free_hash_table(macrosTables[i]);
       // free_hash_table_int(labelsTables[i]);
       // free_hash_table_int(entriesTables[i]);
       // free_hash_table_int(externsTables[i]);
    }

    free(readFiles);
    free(writeFiles);
    free(postDeployReadFiles);
    free(preScanFiles);
    free(preScanDataFiles);
    free(finalFiles);

    free(macrosTables);
    free(labelsTables);
    free(entriesTables);
    free(externsTables);

    free(preDeployFileNames);
    free(postDeployFileNames);
    free(preScanFileNames);
    free(preScanDataFileNames);
    free(finalFileNames);
    free(inputFileNames);
*/

}
