#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;
typedef struct{
    char * buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef struct {
  StatementType type;
} Statement;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;



//headers
InputBuffer * new_input_buffer();
MetaCommandResult do_meta_command(InputBuffer * input_buffer);
PrepareResult prepare_statement(InputBuffer * input_buffer, Statement * statement);

//void functions
void print_prompt(){printf("db > ");};
void read_input(InputBuffer * input_buffer){
    //getline takes in a char address, a string length, and returns a signed number representing the # of bytes read
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if(bytes_read <= 0){
        printf("Error reading input");
        exit(EXIT_FAILURE);
    }
    
    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
};

void close_input_buffer(InputBuffer* input_buffer){
    //free memory
    free(input_buffer->buffer);
    free(input_buffer);
}


void execute_statement(Statement * statement){
    switch(statement->type){
        case(STATEMENT_INSERT):
            printf("This is where insert belongs \n");
            break;
        
        case(STATEMENT_SELECT):
            printf("This is where select belongs \n");
            break;
    }
}

int main(int argc, char * argv[]){
    InputBuffer * input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        //check if command is meta
        if(input_buffer->buffer[0] == '.'){
           switch(do_meta_command(input_buffer)){
            case(META_COMMAND_SUCCESS):
                continue;
            case(META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized Command '%s' \n", input_buffer->buffer);
                continue;
           }
        }

        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            //skip over to execute_statement step
            case(PREPARE_SUCCESS):
                break;
            //print out unrecognized statement
            case(PREPARE_UNRECOGNIZED_STATEMENT):

                printf("Unrecognized Keyword at start of '%s'. \n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed.\n");
    }
    return 0;
}

//Initialize Input buffer
InputBuffer * new_input_buffer() {
    InputBuffer * input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
};

//Checks for meta commands like exit
MetaCommandResult do_meta_command(InputBuffer * input_buffer){
    if(strcmp(input_buffer->buffer, ".exit") == 0){
        exit(EXIT_SUCCESS);
    }
    else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
};

//Handles database commands and returns either a successful or unsuccessful execution
PrepareResult prepare_statement(InputBuffer * input_buffer, Statement * statement){
    //checks if the first 6 characters spell insert, since there can be other characters following the insert command
    if(strncmp(input_buffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
};