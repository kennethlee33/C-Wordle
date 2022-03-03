// Created by Kenneth Lee (klee33@uw.edu)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "words.h"

#define WORD_LENGTH 5
#define GUESS_LIMIT 6

// prints error message in red, then
// switch back to default color
void print_error(char* err_msg);

// prints wordle hint for current guess
// (grey = not in word, yellow = elsewhere, green = correct position).
// Returns true if correct word is guessed, and false otherwise
bool print_hints(char* answer, char* guess);

// private helper for print_hints()
// (hint_color is 2 for green,
//  1 for yellow,
//  0 for default)
void set_hint_color(int hint_color);

int main(int argc, char** argv) {

  // load all guessable five-letter words from list into memory
  char** guessable_words = guessable_words_list();

  // load all possible answer words from list into memory
  char** answer_words = answer_words_list();

  // pick random word from answer words list
  // NOTE: not completely uniform distribution
  srand(time(NULL));
  int w = rand() % ANSWER_WORDS_COUNT;
  char* word = answer_words[w];

  // clear terminal for new game
  system("clear");

  // explain game
  printf("You have 6 tries to guess a randomly chosen 5-letter English word!\n");

  // for debugging    
  // printf("Actual word: %s", word);

  // user guess loop
  int guesses_used = 0;
  while (guesses_used < GUESS_LIMIT) {
    printf("\n");
    printf("Guess %d:\n", guesses_used + 1);
        
    // get user guess
    char curr_guess[100];
    if (fgets(curr_guess, sizeof(curr_guess), stdin) == NULL) {
      print_error("Invalid input, please retry\n");
      continue;
    }

    // remove new line character
    size_t len = strlen(curr_guess);
    if (len > 0 && curr_guess[len - 1] == '\n') {
      curr_guess[len - 1] = '\0';
    }

    // check validity of word
    // (5 letters long, alphabetic)
    int letter_count = 0;
    bool alphabetic = true;
    for (char* i = curr_guess; *i != '\0'; i++) {
      if (!(*i >= 65 && *i <= 90) && !(*i >= 97 && *i <= 122)) {
        // not alphabetic ASCII
        alphabetic = false;
        break;
      }

      letter_count++;
    }
    if (!alphabetic) {
      print_error("Non-alphabetic characters entered\n");
      continue;
    } else if (letter_count > WORD_LENGTH) {
      print_error("Word too long, must enter 5 letters exactly\n");
      continue;
    } else if (letter_count < WORD_LENGTH) {
      print_error("Word too short, must enter 5 letter exactly\n");
      continue;
    }

    // convert guess to all lowercase
    for (int i = 0; i < 5; i++) {
      curr_guess[i] = tolower(curr_guess[i]);
    }

    // check if word is a real English word from word lists
    bool real_word = false;
    for (int i = 0; i < ANSWER_WORDS_COUNT; i++) {
      if (strcmp(answer_words[i], curr_guess) == 0) {
        real_word = true;
        break;
      }
    }
    if (!real_word) {
      for (int i = 0; i < GUESSABLE_WORDS_COUNT; i++) {
        if (strcmp(guessable_words[i], curr_guess) == 0) {
          real_word = true;
          break;
        }
      }
    }
    if (!real_word) {
      print_error("Not a real English word\n");
      continue;
    }

    // print out hints from guess
    bool won = print_hints(word, curr_guess);

    // check if correct word is guessed
    if (won) {
      printf("\n");
      printf("Congratulations, you have guessed the correct word in ");
      if (guesses_used + 1 == 1) {
        printf("%d guess!\n", guesses_used + 1);
      } else {
        printf("%d guesses!\n", guesses_used + 1);
      }

      break;
    }

    guesses_used++;
  }

  if (guesses_used == 6) {
    printf("\n");
    print_error("Unfortunately, you have run out of guesses! Better luck next time.\n");
  }

  // free all dynamically allocated data before exiting
  free(guessable_words);
  free(answer_words);

  return EXIT_SUCCESS;
}

void print_error(char* err_msg) {
  printf("\033[0;31m");
  printf("%s", err_msg);
  printf("\033[0m");
}

bool print_hints(char* answer, char* guess) {
  printf("\n");

  // for printing out hint
  // (0 = grey, 1 = yellow, 2 = green)
  int hint[WORD_LENGTH];

  // count letters in answer
  int ans_letter_count[26] = {0};
  for (int i = 0; i < WORD_LENGTH; i++) {
    ans_letter_count[answer[i] - 'a']++;
  }

  // prioritize checking all correct letters before anything else
  int correct_letters = 0;
  for (int i = 0; i < WORD_LENGTH; i++) {
    if (answer[i] == guess[i]) {
      correct_letters++;
      hint[i] = 2;
      ans_letter_count[guess[i] - 'a']--;
    }
  }

  // logic for designating hints
  for (int i = 0; i < WORD_LENGTH; i++) {
    if (answer[i] == guess[i]) {
      continue;
    }
    if (ans_letter_count[guess[i] - 'a'] != 0) {
      // letter is in answer somewhere else, mark as yellow
      hint[i] = 1;
      ans_letter_count[guess[i] - 'a']--;
    } else {
      hint[i] = 0;
    }
  }

  // print in nice style the generated hint from guess now
  for (int i = 0; i < WORD_LENGTH; i++) {
    set_hint_color(hint[i]);

    printf("+---+");
  }
  printf("\n");
  for (int i = 0; i < WORD_LENGTH; i++) {
    set_hint_color(hint[i]);

    printf("| %c |", guess[i]);
  }
  printf("\n");
  for (int i = 0; i < WORD_LENGTH; i++) {
    set_hint_color(hint[i]);

    printf("+---+");
  }

  printf("\033[0m");
  printf("\n");

  // check if answer was guessed
  return (correct_letters == 5);
}

void set_hint_color(int hint_color) {
  if (hint_color == 2) {
      // set green
      printf("\033[0;32m");
    } else if (hint_color == 1) {
      // set yellow
      printf("\033[0;33m");
    } else {
      // set default
      printf("\033[0m");
    }
}
