//
// Created by MD. Zahed on 07/05/2022.
//

int contains_colon(char *str);
int contains_arrow(char *str);
void init_commands(char *source, char *dist, int first_n, int sec_n);
int move_card_to_another_block(Card *dist, Card *source, char card_name[], char *msg);
Card *find_block_name(Game_board *board, char *str);
int convert_char_to_int(char str);
char *find_foundation_name(Game_board *board, char *str);
int move_card_to_foundation(Game_board *board, Card *block_from, char *move_to, char *msg);
int is_move_to_foundation_possible(Card *temp, char *foundation);


int MOVE_NOT_POSSIBLE = 0;
int COMMAND_NOT_FOUND = -1;
int SUCCEEDED = 1;
int EMPTY_CARD = 3;




int moves_commands(Game_board *board, char *commands, char *msg){

    char move_from[3];
    char move_to[3];


    if (contains_colon(commands) == 1 && contains_arrow(commands) == 1){

        char card_name[3];
        init_commands(commands, move_from, 0, 1);
        init_commands(commands, card_name, 3, 4);
        init_commands(commands, move_to, 7, 8);

        Card *block_from = find_block_name(board, move_from);
        Card *block_to = find_block_name(board, move_to);

        if (block_from == NULL || block_to == NULL) {
            strcpy(msg, "Move is not possible");
            return MOVE_NOT_POSSIBLE;
        }



        return move_card_to_another_block(block_to, block_from, card_name, msg);



    } else if (contains_arrow(commands) == 1) {
        init_commands(commands, move_from, 0, 1);
        init_commands(commands, move_to, 4, 5);
        Card *block_from = find_block_name(board, move_from);

        if (block_from == NULL) {
            strcpy(msg, "Move is not possible");
            return MOVE_NOT_POSSIBLE;
        }


        if (move_to[0] == 'F') {
            return move_card_to_foundation(board, block_from, move_to, msg);

        } else {

            Card *block_to = find_block_name(board, move_to);
            return move_card_to_another_block(block_to, block_from, NULL, msg);
        }

    }


    return COMMAND_NOT_FOUND;


}

int move_card_to_foundation(Game_board *board, Card *block_from, char *move_to, char *msg){



    Card *temp_s = block_from;


    if (temp_s != NULL) {

        char *foundation = find_foundation_name(board, move_to);
        if (temp_s->next != NULL) {
            while (temp_s->next->next != NULL) {
                temp_s = temp_s->next;
            }

            int result = is_move_to_foundation_possible(temp_s->next, foundation);
            if (result == 1) {
                temp_s->next = NULL;
                if (temp_s->suites_value[0] != '\0') {
                    temp_s->face_up = VISIBLE;
                }

                return SUCCEEDED;

            } else {
                strcpy(msg, "Move is not possible");
                return MOVE_NOT_POSSIBLE;
            }

        } else {


            int result = is_move_to_foundation_possible(temp_s, foundation);
            if (result == 1) {

                strncpy(temp_s->suites_value, "", 1);
                temp_s->face_up = EMPTY_CARD;
                return SUCCEEDED;

            } else {
                strcpy(msg, "Move is not possible");
                return MOVE_NOT_POSSIBLE;
            }
        }



    }






    return COMMAND_NOT_FOUND;
}




int is_move_to_foundation_possible(Card *temp, char *foundation) {


    if (foundation[0] == temp->suites_value[0]) {
        int int_foundation_card_val = convert_char_to_int(foundation[1]);
        int int_block_card_val = convert_char_to_int(temp->suites_value[1]);


        if (int_block_card_val-1 == int_foundation_card_val) {
            foundation[0] = temp->suites_value[0];
            foundation[1] = temp->suites_value[1];

            return 1;
        } else {
            return 0;
        }

    } else if (foundation[0] == '[' && foundation[1] == ']') {

        if (temp->suites_value[1] == 'A') {
            foundation[0] = temp->suites_value[0];
            foundation[1] = temp->suites_value[1];

            return 1;
        }

    }

    return 0;
}




int move_card_to_another_block(Card *dist, Card *source, char card_name[], char *msg){

    Card *temp_s = source;

    int card_found = 0;
    int is_source_last_card = 0;
    int is_dest_last_card = 1;


    if (card_name != NULL) {
        while (temp_s->next != NULL) {
            if (temp_s->next->face_up == VISIBLE) {

                if (strncmp(temp_s->next->suites_value, card_name, 2) == 0) {
                    card_found = 1;
                    break;
                }
            }

            if (temp_s->suites_value[0] == '\0') {
                break;
            }

            temp_s = temp_s->next;
        }

        if (card_found == 0) {
            strcpy(msg, "Card x does not exist in the block");
            return MOVE_NOT_POSSIBLE;
        }


    } else {

        if (temp_s->next != NULL) {
            while (temp_s->next->next != NULL) {
                if (temp_s->suites_value[0] == '\0') {
                    break;
                }

                temp_s = temp_s->next;

            }
        } else is_source_last_card = 1;

    }



    int card_to_move_val;

    Card *temp_d = dist;


    while (temp_d->next != NULL) {
        temp_d = temp_d->next;
        is_dest_last_card = 0;
    }

    if (is_source_last_card != 1) {

        card_to_move_val = convert_char_to_int(temp_s->next->suites_value[1]);
        if (temp_s->next->suites_value[0] == temp_d->suites_value[0]) {
            strcpy(msg, "Move is not possible!");
            return MOVE_NOT_POSSIBLE;
        }

    } else {

        card_to_move_val = convert_char_to_int(temp_s->suites_value[1]);
        if (temp_s->suites_value[0] == temp_d->suites_value[0]) {
            strcpy(msg, "Move is not possible!");
            return MOVE_NOT_POSSIBLE;
        }
    }





    int card_dist_val = convert_char_to_int(temp_d->suites_value[1]);

    if (card_to_move_val == (card_dist_val-1) || (is_dest_last_card == 1 && card_to_move_val == 13)) {

        if (is_source_last_card == 0) {
            temp_d->next = temp_s->next;
            temp_s->next = NULL;
            temp_s->face_up = 1;


        } else {


            if (is_dest_last_card == 1) {
                //temp_d = temp_s->next;

                strncpy(temp_d->suites_value, temp_s->suites_value, 2);
                temp_d->face_up = VISIBLE;
                temp_d->next = temp_s->next;

            } else {

                Card *to_add = malloc(sizeof(Card));
                strncpy(to_add->suites_value, temp_s->suites_value, 2);
                to_add->face_up = 1;
                temp_d->next = to_add;


                strncpy(temp_s->suites_value, "", 1);
                temp_s->face_up = EMPTY_CARD;
            }


        }
        return SUCCEEDED;

    } else if (is_dest_last_card == 1) {



    }



    strcpy(msg, "Move is not possible!");
    return MOVE_NOT_POSSIBLE;

}




int convert_char_to_int(char str){
    int int_val = atoi(&str);

    if (int_val == 0) {

        if (str == 'A') return 1;
        else if (str == 'T') return 10;
        else if (str == 'J') return 11;
        else if (str == 'Q') return 12;
        else if (str == 'K') return 13;

    }
    return int_val;

}


char *find_foundation_name(Game_board *board, char *str){

    if (strncmp(str, "F1", 2) == 0) {return board->foundation1;}
    else if (strncmp(str, "F2", 2) == 0) {return board->foundation2;}
    else if (strncmp(str, "F3", 2) == 0) {return board->foundation3;}
    else if (strncmp(str, "F4", 2) == 0) {return board->foundation4;}
    else return NULL;

}




Card *find_block_name(Game_board *board, char *str){

    if (strncmp(str, "B1", 2) == 0) return board->block1;
    else if (strncmp(str, "B2", 2) == 0) return board->block2;
    else if (strncmp(str, "B3", 2) == 0) return board->block3;
    else if (strncmp(str, "B4", 2) == 0) return board->block4;
    else if (strncmp(str, "B5", 2) == 0) return board->block5;
    else if (strncmp(str, "B6", 2) == 0) return board->block6;
    else if (strncmp(str, "B7", 2) == 0) return board->block7;
    else return NULL;
}







int contains_colon(char *str) {

    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ':') {
            return 1;
        }
        i++;
    }

    return 0;
}



int contains_arrow(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '-' && str[i+1] == '>') {
            return 1;
        }
        i++;
    }

    return 0;
}

void init_commands(char *source, char *dist, int first_n, int sec_n) {
    dist[0] = source[first_n];
    dist[1] = source[sec_n];
    dist[2] = '\0';
}