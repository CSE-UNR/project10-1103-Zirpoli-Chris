// Author: Chris Zirpoli
// Date: 5/1/2026
// Purpose: To make the word game Wordle using File IO and 2D Arrays.

#include <stdio.h>
#define ROW_LIMIT 12
#define COLUMN_LIMIT 6
#define WORD_LIMIT 99

// Checks the file:
void file_check(FILE* loaded_file);

// Checks whether the user made a valid guess or not
// (the word must be 5 letters long and not contain anything other than alphabetical letters):
int word_checker(int row_limit, char checked_word[]);

// Generates a line after the user has made a valid guess:
void end_of_guess();

// Checks whether the user's guess is fully correct or not:
int user_guess(int num_of_guesses, int row_limit, int column_limit, char guessed_word[], char word_directory[][column_limit], const char correct_word[]);

// Displays the user's guess history:
void guess_history(int num_of_user_guesses, int row_limit, int column_limit, char word_directory[][column_limit]);

// Final Result:
void user_outcome(int num_of_guesses, int final_outcome, const char user_word[]);


int main(){
	// Used for the mystery.txt file:
	FILE* mystery_word_file;
	mystery_word_file = fopen("word.txt", "r");
	file_check(mystery_word_file);
	// Used for storing the mystery word:
	char mystery_word[COLUMN_LIMIT];
	// Used for storing all of the user's guesses:
	char word_list[ROW_LIMIT][COLUMN_LIMIT];
	// Used for storing the user's most recent guess:
	char one_word[WORD_LIMIT];
	// Used for keeping track of how many guesses the user has made:
	int guess_count = 0;
	// Used to determine whether the user has made a valid guess or not:
	int first_word_check_outcome;
	// Used to determine whether the user's guess fully matches the mystery word or not:
	int second_word_check_outcome;
	
	// Receives the "mystery word"
	fscanf(mystery_word_file, "%s", mystery_word);
	
	for (int main_index_one = 0; main_index_one < 6; main_index_one++){
		
		// Runs if the user has more than one guess left:
		if (main_index_one < 5){
			printf("GUESS %d! Enter your guess: ", main_index_one + 1);
			scanf("%s", one_word);
		}
		
		// Runs if the user has one guess remaining:
		else{
			printf("FINAL GUESS: ");
			scanf("%s", one_word);
		}
		first_word_check_outcome = word_checker(ROW_LIMIT, one_word);
		
		// Runs if the user's guess is invalid or not:
		if (first_word_check_outcome == 0){
			do{
				printf("Please try again: ");
				scanf("%s", one_word);
				first_word_check_outcome = word_checker(ROW_LIMIT, one_word);
			}while(first_word_check_outcome == 0);
		}
		
		// Runs once the user's guess is valid:
		
		if (first_word_check_outcome == 1){
			end_of_guess();
			guess_count++;
			second_word_check_outcome = user_guess(guess_count, ROW_LIMIT, COLUMN_LIMIT, one_word, word_list, mystery_word);
			// Runs if the user's guess is incorrect:
			if (second_word_check_outcome == 0){
				guess_history(guess_count, ROW_LIMIT, COLUMN_LIMIT, word_list);
			}
			// Runs if the user's guess is correct:
			else if (second_word_check_outcome == 1){
				break;
			}
		}
		
	}
	user_outcome(guess_count, second_word_check_outcome, one_word);
	fclose(mystery_word_file);
	return 0;
}

void file_check(FILE* loaded_file){
	if (loaded_file == NULL){
		printf("The mystery word's file could not be opened.\n");
	}
}

int word_checker(int row_limit, char checked_word[]){
	int check_outcome;
	int letter_count = 0;
	int non_letter_count = 0;
	
	for (int index_a = 0; checked_word[index_a] != '\0'; index_a++){
		if (checked_word[index_a] >= 'A' && checked_word[index_a] <= 'Z'){
			letter_count++;
			checked_word[index_a] = checked_word[index_a] + 32;
		}
		else if (checked_word[index_a] >= 'a' && checked_word[index_a] <= 'z'){
			letter_count++;
		}
		else{
			// Even though the invalid character shouldn't be counted as part of the letter counter,
			// I still included it in case the user's guess is 5 characters long.
			letter_count++;
			non_letter_count++;
		}
	}
	
	if (letter_count == 5 && non_letter_count == 0){
		check_outcome = 1;
	}
	else if(letter_count == 5 && non_letter_count > 0){
		printf("Your guess must contain alphabetical letters only.\n");
		check_outcome = 0;
	}
	
	else if (letter_count != 5 && non_letter_count == 0){
		printf("Your guess must be 5 letters long.\n");
		check_outcome = 0;
	}
	
	else if (letter_count != 5 && non_letter_count > 0){
		printf("Your guess must be 5 letters long, and only contain alphabetical letters.\n");
		check_outcome = 0;
	}
	return check_outcome;
}

void end_of_guess(){
	for(int index_b = 0; index_b < 32; index_b++){
		printf("=");
	}
	printf("\n");
}

int user_guess(int num_of_guesses, int row_limit, int column_limit, char guessed_word[], char word_directory[][column_limit], const char correct_word[]){
	
	// Since the program needs one row to display the user's guess,
	// and another row to show if there were any partial matches,
	// the first_row and second_row variables are made
	// for that purpose.
	// (and will change, depending on how many guesses the user has made)
	int first_row = (num_of_guesses * 2) - 2;
	int second_row = (num_of_guesses * 2) - 1;
	// Used to calculate any full matches and "partial matches"
	int partial_matches = 0;
	int full_matches = 0;
	// Used to help with finding any duplicate "partial matches"
	// for the same letter:
	int match_found = 0;
	char similar_letter;
	
	for (int index_d = 0; index_d < 5; index_d++){
		// Runs if a letter in the user's guessed word is in the same position as the mystery word:
		if(guessed_word[index_d] == correct_word[index_d]){
			word_directory[first_row][index_d] = guessed_word[index_d] - 32;
			word_directory[second_row][index_d] = ' ';
			full_matches++;
		}
		// Runs if a letter is in the mystery word, but isn't in the correct position of that word:
		else if (guessed_word[index_d] != correct_word[index_d]){
			// Checks each letter in the mystery word to find a "partial match":
			// (a match where the letter from each word is located in different positions)
			for (int index_e = 0; index_e < 5; index_e++){
				if (guessed_word[index_d] == correct_word[index_e]){
					word_directory[first_row][index_d] = guessed_word[index_d];
					word_directory[second_row][index_d] = '^';
					partial_matches++;
					break;
				}
			}
			if (partial_matches == 0){
				word_directory[first_row][index_d] = guessed_word[index_d];
				word_directory[second_row][index_d] = ' ';
			}
			partial_matches = 0;
			for (int index_f = 0 ; index_f < 5; index_f++){
				for (int index_g = 0; index_g < 5; index_g++){
					// Making sure the indices are different:
					if (index_f != index_g){
						// Making sure that a given letter can be found in more than
						// one position within the user's guessed word:
						if (guessed_word[index_f] == guessed_word[index_g]){
							// Runs if the letters both have a partial match:
							if (word_directory[second_row][index_f] == '^' && word_directory[second_row][index_g] == '^'){
								if (index_f > index_g && guessed_word[index_f] != similar_letter && guessed_word[index_g] != similar_letter){
									word_directory[second_row][index_f] = ' ';
									match_found++;
									similar_letter = guessed_word[index_f];
								}
							}
						}
					}
				}
			}
		}
	}
	// Transfers the user's correctly guessed word to the guessed word array
	// so that the word will be displayed in the final message:
	if (full_matches == 5){
		for (int index_h = 0; index_h < 5; index_h++){
			guessed_word[index_h] = word_directory[first_row][index_h];
		}
		return 1;
	}
	else{
		// Removes any "partial matches" for letters with a full match:
		for (int index_i = 0; index_i < 5; index_i++){
			if (guessed_word[index_i] == correct_word[index_i]){
				for (int index_j = 0; index_j < 5; index_j++){
					if (index_i != index_j && guessed_word[index_i] == guessed_word[index_j]){
						word_directory[second_row][index_j] = ' ';
					}
				}
			}
		}
		return 0;
	}
}

void guess_history(int num_of_user_guesses, int row_limit, int column_limit, char word_directory[][column_limit]){
	int loop_limit = num_of_user_guesses * 2;
	for (int index_k = 0; index_k < loop_limit; index_k++){
		printf("%s\n", word_directory[index_k]);
	}
}

void user_outcome(int num_of_guesses, int final_outcome, const char user_word[]){
	// Runs if the user did not correctly guess the mystery word:
	if (final_outcome == 0){
		printf("You lost, better luck next time!\n");
	}
	// Runs if the user correctly guessed the mystery word:
	else if (final_outcome == 1){
		printf("		%s\n", user_word);
		if(num_of_guesses == 1){
			printf("	You won in %d guess!\n", num_of_guesses);
			printf("		GOATED!\n");
		}
		else if (num_of_guesses == 2 || num_of_guesses == 3){
			printf("	You won in %d guesses!\n", num_of_guesses);
			printf("		Amazing!\n");
		}
		else if (num_of_guesses == 4 || num_of_guesses == 5){
			printf("	You won in %d guesses!\n", num_of_guesses);
			printf("		Nice!\n");
		}
		else if (num_of_guesses == 6){
			printf("	You won in %d guesses!\n", num_of_guesses);
		}
	}
}
