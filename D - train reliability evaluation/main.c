/*
 *  Train Reliability Evaluator
 *
 *  trainReliability - Evaluate the reliability of the overall train given
 *  the unreliability of its coaches
 *  Copyright (C) 2025 by Gloria Gori <gloria.gori@unifi.it>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbd.h"

#define STRING_SIZE     1000

#define MIN_COACHES     3
#define MAX_COACHES     6


static FILE *pInputFile[MAX_COACHES];
static FILE *pOutputFile;
static double *unreliabilities;
static double *reliabilities;
static double *output;
static char strLine[STRING_SIZE];


static void cleanup() {
    for (int idx = 0; idx < MAX_COACHES; idx++) {
        if (pInputFile[idx] != NULL) {
            fclose(pInputFile[idx]);
        }
    }
    if (pOutputFile != NULL) {
        fclose(pOutputFile);
    }
    if (unreliabilities != NULL) {
        free(unreliabilities);
    }
    if (reliabilities != NULL) {
        free(reliabilities);
    }
    if (output != NULL) {
        free(output);
    }
}


static void analyzeThreeCoaches(double *reliabilities, double *output, unsigned int numTimes) {
    rbdKooNGeneric(reliabilities, output, 3, 2, numTimes);
}

static void analyzeFourCoaches(double *reliabilities, double *unreliabilities, double *output, unsigned int numTimes) {
    double *tmpInput;
    double *tmpOutput;
    unsigned int numBytes = numTimes * sizeof(double);

    tmpOutput = (double *)malloc(4 * numBytes);

    rbdKooNGeneric(reliabilities, &tmpOutput[0 * numTimes], 4, 3, numTimes);

    tmpInput = (double *)malloc(4 * numBytes);
    memcpy(&tmpInput[0 * numTimes], &unreliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &reliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[2 * numTimes], &unreliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[1 * numTimes], 4, numTimes);

    memcpy(&tmpInput[2 * numTimes], &reliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[2 * numTimes], 4, numTimes);

    memcpy(&tmpInput[0 * numTimes], &reliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &unreliabilities[1 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[3 * numTimes], 4, numTimes);

    rbdParallelGeneric(tmpOutput, output, 4, numTimes);

    free(tmpInput);
    free(tmpOutput);
}

static void analyzeFiveCoaches(double *reliabilities, double *unreliabilities, double *output, unsigned int numTimes) {
    double *tmpInput;
    double *tmpOutput;
    unsigned int numBytes = numTimes * sizeof(double);

    tmpOutput = (double *)malloc(7 * numBytes);
    rbdKooNGeneric(reliabilities, &tmpOutput[0 * numTimes], 5, 4, numTimes);

    tmpInput = (double *)malloc(5 * numBytes);
    memcpy(&tmpInput[0 * numTimes], &unreliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &reliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[2 * numTimes], &unreliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[1 * numTimes], 5, numTimes);

    memcpy(&tmpInput[2 * numTimes], &reliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[2 * numTimes], 5, numTimes);

    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &unreliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[3 * numTimes], 5, numTimes);

    memcpy(&tmpInput[0 * numTimes], &reliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &unreliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[4 * numTimes], 5, numTimes);

    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &unreliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[5 * numTimes], 5, numTimes);

    memcpy(&tmpInput[1 * numTimes], &reliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[2 * numTimes], &unreliabilities[2 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[6 * numTimes], 5, numTimes);

    rbdParallelGeneric(tmpOutput, output, 7, numTimes);

    free(tmpInput);
    free(tmpOutput);
}

static void analyzeSixCoaches(double *reliabilities, double *unreliabilities, double *output, unsigned int numTimes) {
    double *tmpInput;
    double *tmpOutput;
    unsigned int numBytes = numTimes * sizeof(double);

    tmpOutput = (double *)malloc(11 * numBytes);
    rbdKooNGeneric(reliabilities, &tmpOutput[0 * numTimes], 6, 5, numTimes);

    tmpInput = (double *)malloc(6 * numBytes);
    memcpy(&tmpInput[0 * numTimes], &unreliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &reliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[2 * numTimes], &unreliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &reliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[1 * numTimes], 6, numTimes);

    memcpy(&tmpInput[2 * numTimes], &reliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[2 * numTimes], 6, numTimes);

    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &unreliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[3 * numTimes], 6, numTimes);

    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &unreliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[4 * numTimes], 6, numTimes);

    memcpy(&tmpInput[0 * numTimes], &reliabilities[0 * numTimes], numBytes);
    memcpy(&tmpInput[1 * numTimes], &unreliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &reliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[5 * numTimes], 6, numTimes);

    memcpy(&tmpInput[3 * numTimes], &reliabilities[3 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &unreliabilities[4 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[6 * numTimes], 6, numTimes);

    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &unreliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[7 * numTimes], 6, numTimes);

    memcpy(&tmpInput[1 * numTimes], &reliabilities[1 * numTimes], numBytes);
    memcpy(&tmpInput[2 * numTimes], &unreliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[4 * numTimes], &unreliabilities[4 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &reliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[8 * numTimes], 6, numTimes);

    memcpy(&tmpInput[4 * numTimes], &reliabilities[4 * numTimes], numBytes);
    memcpy(&tmpInput[5 * numTimes], &unreliabilities[5 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[9 * numTimes], 6, numTimes);

    memcpy(&tmpInput[2 * numTimes], &reliabilities[2 * numTimes], numBytes);
    memcpy(&tmpInput[3 * numTimes], &unreliabilities[3 * numTimes], numBytes);
    rbdSeriesGeneric(tmpInput, &tmpOutput[10 * numTimes], 6, numTimes);

    rbdParallelGeneric(tmpOutput, output, 11, numTimes);

    free(tmpInput);
    free(tmpOutput);
}


int main(int argc, char **argv) {
    long numCoaches;
    long coachIdx;
    double time[MAX_COACHES];
    double step[MAX_COACHES];
    double unreliability;
    unsigned int numTimes;
    unsigned int timeIdx;
    char *strTmp;

    if ((argc < 6) || (argc > 9)) {
        printf("Invalid number of arguments\n");
        return -1;
    }

    numCoaches = strtol(argv[1], NULL, 10);
    if ((numCoaches < MIN_COACHES) || (numCoaches > MAX_COACHES) || (numCoaches != (argc - 3))) {
        printf("Invalid number of coaches\n");
        return -1;
    }

    for (coachIdx = 0; coachIdx < numCoaches; coachIdx++) {
        pInputFile[coachIdx] = fopen(argv[coachIdx + 2], "r");
        if (pInputFile[coachIdx] == NULL) {
            printf("Input file %s does not exist\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
    }
    pOutputFile = fopen(argv[numCoaches + 2], "w");
    if (pOutputFile == NULL) {
        printf("Cannot create output file %s\n", argv[numCoaches + 2]);
        cleanup();
        return -1;
    }

    for (coachIdx = 0; coachIdx < numCoaches; coachIdx++) {
        if (fgets(strLine, STRING_SIZE, pInputFile[coachIdx]) == NULL) {
            printf("Empty input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
        if (strstr(strLine, "Analysis Result of failure ") == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
    }

    for (coachIdx = 0; coachIdx < numCoaches; coachIdx++) {
        if (fgets(strLine, STRING_SIZE, pInputFile[coachIdx]) == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
        if (strstr(strLine, "Time: ") == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
        if (strstr(strLine, "Step: ") == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
        strTmp = strstr(strLine, "Time: ") + strlen("Time: ");
        time[coachIdx] = strtod(strTmp, NULL);
        strTmp = strstr(strLine, "Step: ") + strlen("Step: ");
        step[coachIdx] = strtod(strTmp, NULL);
    }

    for (coachIdx = 0; coachIdx < numCoaches; coachIdx++) {
        if (fgets(strLine, STRING_SIZE, pInputFile[coachIdx]) == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
        if (strstr(strLine, "Values: ") == NULL) {
            printf("Invalid input file %s\n", argv[coachIdx + 2]);
            cleanup();
            return -1;
        }
    }

    numTimes = (unsigned int)(time[0] / step[0]) + 1;
    for (coachIdx = 1; coachIdx < numCoaches; coachIdx++) {
        if (numTimes != ((unsigned int)(time[coachIdx] / step[coachIdx]) + 1)) {
            printf("Invalid input files, number of time instants is not constant\n");
            cleanup();
            return -1;
        }
    }

    unreliabilities = (double *)malloc(numTimes * numCoaches * sizeof(double));
    if (unreliabilities == NULL) {
        printf("Unable to allocate memory\n");
        cleanup();
        return -1;
    }
    reliabilities = (double *)malloc(numTimes * numCoaches * sizeof(double));
    if (reliabilities == NULL) {
        printf("Unable to allocate memory\n");
        cleanup();
        return -1;
    }
    output = (double *)malloc(numTimes * sizeof(double));
    if (output == NULL) {
        printf("Unable to allocate memory\n");
        cleanup();
        return -1;
    }

    for (timeIdx = 0; timeIdx < numTimes; timeIdx++) {
        for (coachIdx = 0; coachIdx < numCoaches; coachIdx++) {
            if (fgets(strLine, STRING_SIZE, pInputFile[coachIdx]) == NULL) {
                printf("Unexpected end of file %s reached\n", argv[coachIdx + 2]);
                cleanup();
                return -1;
            }
            unreliability = strtod(strLine, NULL);
            unreliabilities[timeIdx + coachIdx * numTimes] = unreliability;
            reliabilities[timeIdx + coachIdx * numTimes] = 1.0 - unreliability;
        }
    }

    if (numCoaches == 3) {
        analyzeThreeCoaches(reliabilities, output, numTimes);
    }
    else if (numCoaches == 4) {
        analyzeFourCoaches(reliabilities, unreliabilities, output, numTimes);
    }
    else if (numCoaches == 5) {
        analyzeFiveCoaches(reliabilities, unreliabilities, output, numTimes);
    }
    else {
        analyzeSixCoaches(reliabilities, unreliabilities, output, numTimes);
    }

    for (time[0] = 0.0, timeIdx = 0; timeIdx < numTimes; timeIdx++) {
        fprintf(pOutputFile, "%.4f,%.20e\n", time[0], output[timeIdx]);
        time[0] += step[0];
    }

    cleanup();

    return 0;
}

