#include <stdio.h>
#include <stdlib.h>

#define DIAL_START 50
#define ERROR_RETURN -1
#define SUCCESS_RETURN 0


/**
 * @brief Represents an instruction.
 * 
 * @param direction 'L' or 'R' representing left and right dial turns
 * @param distance Integer amount to turn the dial
 */
typedef struct instruction_t {
    char direction;
    int distance;
} instruction_t;

/**
 * @brief reads file of inputs and converts to an array of instruction_t.
 * 
 * @param filename Name (path) of the input file
 * @param arr Pointer to the resulting array to be filled
 * @param arr_len Integer pointer to the length of the resulting array (set by read_inputs)
 * 
 * @return Integer, -1 if error, 0 if successful
 * */
int read_inputs(char *filename, instruction_t **arr, int *arr_len){

    int num_lines = 0;
    int arr_index = 0;
    char line[8] = {0};

    if (!filename || !arr || !arr_len){return ERROR_RETURN;}
    FILE *file = fopen(filename, "r");
    if (!file){return ERROR_RETURN;}

    while (NULL != fgets(line, sizeof(line), file)) {
        num_lines ++;
    }
    rewind(file);

    *arr = calloc(num_lines, sizeof(instruction_t));
    *arr_len = num_lines;

    while (NULL != fgets(line, sizeof(line), file)) {
        int line_len = sizeof(line) - 1, distance = 0;
        
        (*arr)[arr_index].direction = line[0];

        for (int i = 1; i < line_len; i++){
            if (line[i] == '\n' || line[i] == '\0'){break;}
            distance *= 10;
            distance += line[i] - 48;
        }

        (*arr)[arr_index].distance = distance;

        arr_index += 1;
    }

    fclose(file);

    return SUCCESS_RETURN;
}

/**
 * @brief calculates dial movement, returning end position.
 * 
 * @param instruction dial-turn instruction, including L/R and distance
 * @param current_position Where the dial is currently at
 * @param touched_zeroes Counter for when the dial passes a 0. Increment when passing a 0.
 * 
 * @return Integer representing the end-state of the dial
 * */
int move_dial(instruction_t *instruction, int *current_position, int *touched_zeroes){
    int distance = instruction->distance;
    int next_position = *current_position;
    
    if (instruction->direction == 'R'){
        next_position += distance;
    }
    else if (instruction->direction == 'L'){
        next_position -= distance;
    }
    
    int next_position_cycled = ((next_position % 100) + 100) % 100;
    
    // Case 1: both current and next are zero
    if (*current_position == 0 && next_position_cycled == 0){
        *touched_zeroes += distance / 100;
    }
    // Case 2: current is non-zero, next is zero (landing on zero)
    else if (*current_position != 0 && next_position_cycled == 0){
        *touched_zeroes += (distance / 100) + 1;  // +1 for landing on zero
    }
    // Case 3: current is zero, next is non-zero
    else if (*current_position == 0 && next_position_cycled != 0){
        *touched_zeroes += distance / 100;
    }
    // Case 4: both non-zero
    else {
        int rotation = distance / 100;
        int left_over = (instruction->direction == 'R') ? (distance % 100) : -(distance % 100);
        int remaining_dist = *current_position + left_over;
        int extra = (remaining_dist < 0 || remaining_dist >= 100) ? 1 : 0;
        
        if ((next_position < 0 || next_position >= 100) && distance < 100){
            *touched_zeroes += 1;
        }
        else if (distance >= 100){
            *touched_zeroes += rotation + extra;
        }
    }
    
    *current_position = next_position_cycled;
    return *current_position;
}

int main(int argc, char *argv[]) {

    instruction_t *arr = NULL;
    int array_length = 0;
    int dial_stopped_zeroes = 0;
    int dial_touched_zeroes = 0;
    int dial_position = DIAL_START;

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
        if (move_dial(&arr[i], &dial_position, &dial_touched_zeroes) == 0){
            dial_stopped_zeroes ++;
        }
    }

    printf("Times the dial stopped on 0: %d\n", dial_stopped_zeroes);
    printf("Times the dial touched 0: %d\n", dial_touched_zeroes);

    free(arr);

    return SUCCESS_RETURN;
}