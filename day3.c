#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_RETURN -1
#define SUCCESS_RETURN 0
#define LINE_LENGTH 100
#define NUM_ELEMS 12


/**
 * @brief reads file of inputs and converts to an array of char arrays.
 * Line length constant at 100.
 * 
 * @param filename Name (path) of the input file.
 * @param arr Pointer to the resulting array of arrays to be filled.
 * @param arr_len Integer pointer to the length of the resulting array (set by read_inputs).
 * 
 * @return Integer, -1 if error, 0 if successful.
 * */
int read_inputs(char *filename, char ***arr, int *arr_len){

    int num_lines = 0;
    int arr_index = 0;
    char line[LINE_LENGTH + 2] = {0};

    if (!filename || !arr || !arr_len){return ERROR_RETURN;}
    
    FILE *file = fopen(filename, "r");
    if (!file){return ERROR_RETURN;}

    while (NULL != fgets(line, sizeof(line), file)) {
        num_lines++;
    }
    rewind(file);

    *arr = calloc(num_lines, sizeof(char*));
    if (!*arr) {
        fclose(file);
        return ERROR_RETURN;
    }
    *arr_len = num_lines;

    while (NULL != fgets(line, sizeof(line), file)) {
    
        (*arr)[arr_index] = calloc(LINE_LENGTH + 1, sizeof(char));
        
        strncpy((*arr)[arr_index], line, LINE_LENGTH);
        (*arr)[arr_index][LINE_LENGTH] = '\0';
        
        int len = strlen((*arr)[arr_index]);
        if (len > 0 && (*arr)[arr_index][len - 1] == '\n') {
            (*arr)[arr_index][len - 1] = '\0';
        }

        arr_index++;
    }
    
    fclose(file);

    return SUCCESS_RETURN;
}

/**
 * @brief Finds largest combination of 2 numbers from input line (without changing order).
 * Methodology: Always looks to increase value of most significant digit until the (line.length -1) digit.
 * This is done by moving right along the line. Then, if no greater digit can be found,
 * moves left to find the greatest least significant digit.
 * Converts these digits to an integer and adds to total.
 * 
 * @param line char array of an input line.
 * @param total integer pointer to the total to add greatest joltage from line to.
 * 
 * @return 0 if successful.
 * */
int find_largest_part1(char *line, int *total){
    int largest_left = -1, largest_right = -1;
    int left_index = 0, right_index = LINE_LENGTH - 1;

    for (int i = 0; i < LINE_LENGTH; i++){
        if (line[i] >= '0' && line[i] <= '9') {
            if ((line[i] - '0') > largest_left){
                largest_left = (line[i] - '0');
                left_index = i;
            }
        }
    }
    for (int i = LINE_LENGTH - 1; i > left_index; i--){
        if (line[i] >= '0' && line[i] <= '9') {
            if ((line[i] - '0') > largest_right){
                largest_right = (line[i] - '0');
                right_index = i;
            }
        }
    }

    *total += (largest_left * 10) + largest_right;

    return SUCCESS_RETURN;
}

/**
 * @brief Finds largest combination of 12 numbers from input line (without changing order).
 * Methodology: Always looks to increase value of most significant digits until the (line.length -n) digit.
 * (n) Starts at 12, then decreases as most significant digits are found.
 * Similar methodology to part 1, but includes an additional loop to find all 12 numbers. 
 * Converts these digits to an integer and adds to total.
 * 
 * @param line char array of an input line.
 * @param total long pointer to the total to add greatest joltage from line to.
 * 
 * @return 0 if successful.
 * */
int find_largest_part2(char *line, long *total){
    int largest_elems[NUM_ELEMS] = {0};
    int left_index = -1;
    long total_sum = 0;

    for (int elem = 0; elem < NUM_ELEMS; elem++){

        for (int i = left_index + 1; i < ((LINE_LENGTH - (NUM_ELEMS - 1)) + elem); i++){
            if (line[i] >= '0' && line[i] <= '9') {
                if ((line[i] - '0') > largest_elems[elem]){
                    largest_elems[elem] = (line[i] - '0');
                    left_index = i;
                }
            }
        }

    }

    for (int i = 0; i < NUM_ELEMS; i++){
        total_sum *= 10;
        total_sum += (long)largest_elems[i];
    }

    *total += total_sum;

    return SUCCESS_RETURN;
}

int main(int argc, char *argv[]) {

    char **arr = NULL;
    int array_length = 0;
    int part1 = 0;
    long part2 = 0;

    if (argc != 2){
        printf("Error. Usage: ./day1 <path_to_input_file>\n");
        free(arr);
        return ERROR_RETURN;
    }

    if (read_inputs(argv[1], &arr, &array_length) == ERROR_RETURN){
        printf("Error. Usage: ./day1 <path_to_input_file>\n");
        free(arr);
        return ERROR_RETURN;
    }

    for (int i = 0; i < array_length; i++){
        find_largest_part1(arr[i], &part1);
        find_largest_part2(arr[i], &part2);
    }

    printf("Part 1: %d\n", part1);
    printf("Part 2: %lu\n", part2);

    for (int i = 0; i < array_length; i++){
        free(arr[i]);
    }

    free(arr);

    return SUCCESS_RETURN;
}