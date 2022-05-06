#include <stdio.h>
#include <stdlib.h>
#include "game_board.c"
#include <string.h>
#include "utils.c"
#define MAX_BUFFER 60
Game_board *game_board;


int main() {

    char input[MAX_BUFFER];
    char *command = "";
    char *optional_command;
    char last_command[60] = "";
    char message_output[70] = "";


    // Initialize game board
    starting_point();




    // Main game loop
    while(strcmp(command, "stop") != 0) {

        printf("LAST Command: ");
        printf("%s \n", last_command);
        printf("Message: ");
        printf("%s \n", message_output);
        printf("INPUT > \t");
        fgets(input, MAX_BUFFER, stdin);
        printf("\n");

        command = strtok(input, " ");
        optional_command = strtok(NULL, " ");




        if(strncmp(command, "LD", 2) == 0) {
            strcpy(last_command, command);
            strcpy(message_output, "OK");
            initialize_card_deck(0);
            game_board = initialize_game_board(card_deck);
            display_game_board(game_board);


            //printf("You entered %*.*s\n", length, length, input);
        } else if(strncmp(command, "SW", 2) == 0) {

            if (game_board == NULL){
                strcpy(message_output, "Initialize a game board by typing LD");
                //puts("Initialize a game board by typing LD");

            } else {
                strcpy(last_command, command);
                strcpy(message_output, "OK");
                set_cards_visible(&game_board);
                display_game_board(game_board);
            }

        } else if  (strncmp(command, "SD", 2) == 0) {

            if (game_board == NULL) {
                strcpy(message_output, "Initialize a game board by typing LD");

            } else {
                if (optional_command == NULL) {
                    strcpy(message_output, "Board name");



                } else {
                    save_game_board_on_pc( game_board, optional_command);
                    strcpy(last_command, command);
                    strcpy(message_output, "OK");
                }

            }


        }

        else {
            strcpy(message_output, "Unknown command");
            strcpy(last_command, "INVALID COMMAND EXECUTED");
        }



    }


}


