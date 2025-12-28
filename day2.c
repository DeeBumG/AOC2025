#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERROR_RETURN -1
#define SUCCESS_RETURN 0

/**
 * @brief Represents a range of IDs.
 * 
 * @param direction Long starting ID.
 * @param distance Long ending ID.
 */
typedef struct range_t {
    long start;
    long end;
} range_t;

/**
 * @brief reads file of inputs and converts to an array of range_t.
 * 
 * @param filename Name (path) of the input file.
 * @param arr Pointer to the resulting array to be filled.
 * @param arr_len Integer pointer to the length of the resulting array (set by read_inputs).
 * 
 * @return Integer, -1 if error, 0 if successful.
 * */
int read_inputs(char *filename, range_t **arr, int *arr_len){

    int i;
    int arr_index = 0;
    int array_len = 1;
    long fileSize;
    char *line;

    if (!filename || !arr || !arr_len){return ERROR_RETURN;}
    FILE *file = fopen(filename, "r");
    if (!file){return ERROR_RETURN;}

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    line = malloc(fileSize + 1);

    if (!line) {
        fclose(file);
        return ERROR_RETURN;
    }
    fgets(line, fileSize + 1, file);

    for (i = 0; i < fileSize; i++){
        if(line[i] == ','){
            array_len ++;
        }
    }

    *arr = calloc(array_len, sizeof(range_t));
    *arr_len = array_len;
 
    long current_value = 0;

    for (i = 0; i < fileSize + 1; i++){

        if(line[i] == '-'){
            (*arr)[arr_index].start = current_value;
            current_value = 0;
        }
        else if(line[i] == ',' || i == fileSize){
            (*arr)[arr_index].end = current_value;
            current_value = 0;
            arr_index ++;
        }
        else{
            current_value *= 10;
            current_value += line[i] - 48;
        }
    }

    free(line);
    fclose(file);

    return SUCCESS_RETURN;
}

/**
 * @brief finds length (number of digits) of input number.
 * 
 * @param number long input number.
 * 
 * @return integer length of input number.
 * */
int num_length(long number){
    return (floor(log10(number)) + 1);
}

/**
 * @brief Finds IDs made of 2 repeating parts in the given range.
 * Methodology: Generates all possible repeating IDs with 2 parts
 * that are the same starting and ending length as the range given.
 * Adds IDs to total if in the given range.
 * 
 * @param range range_t range to find invalid IDs in.
 * @param total Pointer to the total to add invalid IDs to.
 * 
 * @return 0 if successful.
 * */
int find_invalid_part_1(range_t range, long *total){
    long start_length, end_length, half_num, invalid_test_num;
    long start_num = range.start, end_num = range.end;
    int num_invalid_in_range;
    
    start_length = num_length(start_num);
    end_length = num_length(end_num);

    for (int n = start_length; n <= end_length; n++){
        
        if (n % 2 == 0){
            half_num = pow(10, ((n / 2) - 1));
            invalid_test_num = (pow(10, num_length(half_num)) * half_num) + half_num;

            while (invalid_test_num <= end_num){
                if (invalid_test_num >= start_num){
                    *total += invalid_test_num;
                }
                half_num ++;
                invalid_test_num = (pow(10, num_length(half_num)) * half_num) + half_num;
            }
        }
    }
    return SUCCESS_RETURN;
}

/**
 * @brief Finds IDs made of any (n) repeating parts in the given range.
 * Methodology: Generates all possible repeating IDs with (n) parts
 * that are the same starting and ending length as the range given.
 * Starts at 1 repeating, then 2 repeating, up to the first half of the digits in range.end.
 * Adds IDs to total if in the given range.
 * 
 * Example: range.end = 159827 (length 6). Would start at 1 repeating -> 111111 (length 6).
 * Then, moves to 2 repeating etc. until 159 repeating -> 159159. Does not move to 160 since this
 * would never be possible.
 * 
 * Additionally, excludes already calculated IDs, such as 1 vs 11 both producing 1111. 
 * This is found by determining if the starting number is made of 2 to (x) repeating parts,
 * with (x) being 1/2 (starting_number.length). 
 * Example: Starting num: 11 -> made of 2 repeating 1s, exclude. 
 * Example: Starting num: 343434 -> made of 3 repeating parts (34), exclude as any IDs created
 * would already have been added with starting num = 34. 
 * 
 * @param range range_t range to find invalid IDs in.
 * @param total Pointer to the total to add invalid IDs to.
 * 
 * @return 0 if successful.
 * */
int find_invalid_part_2(range_t range, long *total){
    long start_length, end_length;
    long start_num = range.start, end_num = range.end;
    int num_tests;
    long comparison_num;

    start_length = num_length(start_num);
    end_length = num_length(end_num);


    if (end_length % 2 == 0){
        num_tests = (range.end / pow(10, (end_length / 2)));
        //first half of the number
    }
    else{
        num_tests = (range.end / pow(10, ((end_length - 1) / 2)));
        //first half + 1 if odd number of digits
    }

    for (int i = 1; i <= num_tests; i++){
        long test_num = (long) i;

        for (int j = 0; j < end_length; j++){
            
            if (test_num > end_num){
                break;
            }
            else if (test_num >= start_num){
                int add_test_num = 1;

                for(int k = 1; k < num_length(i); k++){
                    int first_n = test_num / pow(10, (num_length(test_num) - k));
                    long comparison_num = first_n;

                    for (int l = 1; l < num_length(test_num); l++){
                        comparison_num *= pow(10, (num_length(first_n)));
                        comparison_num += (long)first_n;

                        if (comparison_num == test_num){
                            add_test_num = 0;
                        }
                    }
                }
                if (add_test_num){
                    *total += test_num;
                }
            }
            test_num *= pow(10, num_length(i));
            test_num += i;
        }
    }  
    return SUCCESS_RETURN;
}

int main(int argc, char *argv[]) {

    range_t *arr = NULL;
    int array_length = 0;
    long part_1_total = 0, part_2_total = 0;

    if (argc != 2){
        printf("Error. Usage: ./day2 <path_to_input_file>\n");
        free(arr);
        return ERROR_RETURN;
    }

    if (read_inputs(argv[1], &arr, &array_length) == ERROR_RETURN){
        printf("Error. Usage: ./day2 <path_to_input_file>\n");
        free(arr);
        return ERROR_RETURN;
    }

    for (int i = 0; i < array_length; i++){
        find_invalid_part_1(arr[i], &part_1_total);
        find_invalid_part_2(arr[i], &part_2_total);
    }

    printf("Part 1: %lu\n", part_1_total);
    printf("Part 2: %lu\n", part_2_total);

    free(arr);

    return SUCCESS_RETURN;
}