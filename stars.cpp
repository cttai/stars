/*
Christine Tai

The purpose of this program is to allow the computer to pick a secret word of four to six letters
and the player has to try to guess the word. The computer will tell the player how many letters 
are in the word. The player tries to determine the secret word by presenting the computer 
with a series of probe words, which are four to six letter words. If the probe word is the 
secret word, the player wins. Otherwise, the computer responds to the probe word with two 
integers: the number of stars and the number of planets. Stars and planets are pairings between 
a letter in the probe word and the same letter in the secret word. A star is a pairing of a 
letter in the probe word and the same letter in the secret word in the same position. A planet is
a pairing between a letter in the probe word and the same letter in the secret word, but not in 
the same position as in the probe word, provided that neither of the two letters are involved 
in a star or another planet. The player's score for each round is the number of probe words 
needed to get the correct word (counting the probe word that matched the secret word).
*/

#define _CRT_SECURE_NO_DEPRECATE

#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;


const char WORDFILENAME[] = "C:/temp/CS 31/Project5/words.txt";

const int MAXWORDS = 9000;
const int PROBESIZE = 100;

int runOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
bool isWord(const char words[][MAXWORDLEN + 1], int nWords, const char probe[]);


int main()
{
	char wordList[MAXWORDS][MAXWORDLEN + 1];
	int nWords = getWords(wordList, 10000, WORDFILENAME);
	
	
	if (nWords < 1) {
		cout << "No words were loaded, so I can't play the game." << endl;
		return -1;
	}

	int rounds;
	int tries;

	cout << "How many rounds do you want to play? ";
	cin >> rounds;
	cin.ignore(1000, '\n');

	if (rounds < 1) {
		cout << "The number of rounds must be positive." << endl;
		return -1;
	}

	cout.setf(ios::fixed);
	cout.precision(2);

	double average = 0.00;
	int scoreSum = 0;
	int min;
	int max;

	if (nWords > 0 && nWords < MAXWORDS) {
		for (int roundNumber = 1; roundNumber <= rounds; roundNumber++) {
			cout << endl << "Round " << roundNumber << endl;

			int wordnum = randInt(0, nWords - 1);
			int length = strlen(wordList[wordnum]);
			
			cout << "The secret word is " << length << " letters long." << endl;

			tries = runOneRound(wordList, nWords, wordnum);
			scoreSum += tries;
			average = static_cast<double>(scoreSum) / roundNumber;

			if (roundNumber == 1) {
				min = tries;
				max = tries;
			}
			if (tries > max)
				max = tries;
			if (tries < min)
				min = tries;

			if (tries == 1) {
				cout << "You got it in 1 try." << endl;
			}
			else {
				cout << "You got it in " << tries << " tries." << endl;
			}
			
			cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << endl;
		}
		
	}

}


int runOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	if (nWords < 1 || wordnum < 0 || wordnum >= nWords)
		return -1;
	
	int numTries = 1;
	int length = strlen(words[wordnum]);

	char probe[PROBESIZE + 1];
	do {
		
		cout << "Probe word: ";
		cin.getline(probe, PROBESIZE + 1);
		
		int lengthProbe = 0;
		while (probe[lengthProbe] != '\0') {
			if (! islower(probe[lengthProbe])) {
				cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
				lengthProbe = -1;     // indicates that there were chars that were not lower case letters
				break;
			}

			lengthProbe++;
		}

		if (lengthProbe == -1)      // There were chars that were not lower case letters, so continue to the beginning of the do-while loop
			continue;


		if (!(lengthProbe >= 4) || !(lengthProbe <= 6)) {
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
			continue;
		}


		
		if (!isWord(words, nWords, probe)) {    // checking if probe is within the file
			cout << "I don't know that word." << endl;
			continue;
		}
		

		if (strcmp(words[wordnum], probe) == 0)
			return numTries;

		int stars = 0;  //A star is a pairing of a letter in the probe word and the same letter in the secret word in the same position. 
		int planets = 0; //A planet is a pairing between a letter in the probe word and the same letter in the secret word, but not in the same position as in the probe word, provided the two letters are not involved in a star or another planet.

		char temp[MAXWORDLEN + 1];                    // a copy of words[wordnum] (at least it will be soon)
		strcpy(temp, words[wordnum]);

		for (int index = 0; (index < length) || (index < lengthProbe); index++) {
			if ((index < length) && (index < lengthProbe)) {                        //only for stars does the position of the probe and target have to match, so need to make sure we're not accessing the array out of bounds
				if (probe[index] == words[wordnum][index]) {
					stars++;
					continue;
				}
			}
			if(index < lengthProbe){
				for (int j = 0; j < length; j++) {
					if (probe[index] == temp[j]) {
						if (j < lengthProbe && probe[j] == temp[j])       // making sure a star is not counted for a planet
							continue;

						temp[j] = 'A';						// to make sure a letter in the target word is not counted as a planet multiple times
						planets++;
						break;
					}
				}
			}
		}
		


		cout << "Stars: " << stars << ", Planets: " << planets << endl;
		numTries++;

	} while (probe != words[wordnum]);
	
	return numTries;
}


//Checking if the probe word is within the file
bool isWord(const char words[][MAXWORDLEN + 1], int nWords, const char probe[])
{
	for (int i = 0; i < nWords; i++) {
		if (strcmp(words[i], probe) == 0)
			return true;
	}
	return false;
}
