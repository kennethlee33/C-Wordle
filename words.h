// Created by Kenneth Lee (klee33@uw.edu)

#ifndef WORDS
#define WORDS

// number of guessable 5 letter words
#define GUESSABLE_WORDS_COUNT 10657

// number of 5 letter words in answer list
#define ANSWER_WORDS_COUNT 2315

// returns list of all 5 letter words that can be guessed
// identical to Wordle guessable list (10657 words)
char** guessable_words_list();

// returns list of all 5 letter words from Wordle answer 
// list (2315 words). This is used because these words are
// more common than words from the guessable list.
// These two lists are exclusive to each other
char** answer_words_list();

#endif