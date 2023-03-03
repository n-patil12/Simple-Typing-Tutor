/*CS 141 Program III --> Typing Tutor

Author(s): Niharika Patil, starter code provided by Professor George Maratos and Professor David Hayes

Description: 
  This project is a simple typing tutor. Its purpose of this project is for the user to improve their ability to spell words of a certain difficulty. 
  The program gives the user options to be able to type in 5 words correctly from either the dictionary or mispelled data files. Furthermore, the program will also give the user the option to search a certain range of words from the dictionary, or even be able to target a specific word through the implementation of a binary search. */


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void
display_header(const int ndict, const int nmiss)
{
    /* This is the header, which will be printed out at the beginning of the execution of the code. The output statements contain two parameters: the number of words dictionary.txt file (extracted as a vector from the extract_dict function) and with the misspelled.txt file (extracted from the extract_misspelled function) 
    */

  cout << "*** Typing Tutor" << endl;
  cout << "There are " << ndict << " words in the dictionary." << endl;
  cout << "There are " << nmiss << " commonly misspelled words." << endl;
}

void
display_menu_options()
{
    /* This function prints out all the menu options, it is
    given as part of the starter code that the user will also see along with the header.*/

    cout << endl << "*** Select a menu option:" << endl
         << "1. Spell commonly misspelled words" << endl
         << "2. Type random words from the full dictionary" << endl
         << "3. Display some words from one of the lists" << endl
         << "4. Use binary search to lookup a word in the dictionary" << endl
         << "5. Exit the program." << endl
         << "Your choice: ";
}

vector<int>
randn(const int n, const int max)
{
    /*  This function will return a vector that generates a random set of values a range from 0 to the max value that will contain two parameters: the number of elements that is being tested and checked within the menu option 1, and the total indices within the msplled vector

  This function is called later in the misspelled function (starts on line 298)*/
  
    vector<int> temp;

    for (int num = 1; num <= n; ++num){
      int rand_int = rand() % (max + 1);
      temp.push_back(rand_int); 
      
    }
    return temp;
}

/* This is a function provided by the professors that will read in and open both the a text file as the parameter, which are dictionary.txt and misspelled.txt, used in the extract_dict and extract_mispelled functions */

ifstream
open(const string& src_file)
{
    ifstream src{src_file};
    if (!src.is_open()) {
        cout << "Could not find file " << src_file << "." << endl;
        exit(1);
    }
    return src;
}

/* This function will pass in two parameters: a vector named dict and the dictionary.txt file

      The function will first call the open function above to stream through the dictonary.txt file

      Then, a new string named txtStr and the src will be used in the getline function to push all of the elements within the text file into the dst vector (a parameter for the dict vector)*/

void
extract_dict(vector<string>& dst, const string& src_file)
{
  string txtStr; 
  ifstream src = open(src_file);

  while (getline(src, txtStr)){ 
    dst.push_back(txtStr);
  }
}

/* Similar to the function above, his function will pass in three parameters: a vector named misspelled, a vector named correct, and the mispelled.txt file

      The function will first call the open function above to stream through the mispelled.txt file
      Then, three new strings named newStr, mispelled_Word, and correct_Word, and the src will be used in the getline function to push all of the elements within the text file into the mispelled and correct vectors

However, it needed to be taken into account that each line as a space between the two words. so, a variable for the cpace character was created in order to collected the first word, which was the mispelled word, that occurred before the space, and will be later pushed back into the mispelled vector

  A similar process is done for the correct word spelling, which will occur after the space, and will be pushed back into the correct vector*/

void
extract_misspelled(vector<string>& misspelled, vector<string>& correct,
                    const string& src_file)
{
  string newStr; 
  string misspelled_Word;
  string correct_Word;
  ifstream src = open(src_file);
  
  while (getline(src, newStr)){
    int space_Index = newStr.find(' ');
    misspelled_Word = newStr.substr(0, space_Index);

    correct_Word = newStr.substr(space_Index + 1);
    
    misspelled.push_back(misspelled_Word);
    
    correct.push_back(correct_Word);
  } 
}

/* This function will print out the elements in the vector src only at those indices With the vector of indices

          This function takes in two vectors: the msplled and the temp vector. This function will go through each of the indices in the temop vector, and using the elements of the positions vector based on the current loop index, it will find the mispelled version of the word at that specfic index. 

          In other words, the elements of the positions vector acts as an index to find the word in the mispelled vecotr at that position. This is also called in the mispelled function later on*/

void
display_elems(const vector<string>& src, const vector<int>& positions)
{
  for (int pos_Idx = 0; pos_Idx < positions.size(); ++pos_Idx){
    string element = src.at(positions.at(pos_Idx));
    cout << element;
    if (pos_Idx != positions.size() - 1){
      cout << " ";
    }
  }
  cout << endl;
}

/* Similar to display_elems, this function will copy the elements at the indices, in the positions vector, from the correct vector, into the vector dst. 
      For this, the function has three arguements that are passed in: an empty vector named corrAns, the correct vector, and the positions vector

      The function will use a for loop to loop through the positions vector, and then push them back into the empty dst vector

      This function will be called within the display_elems function*/

void
extract_spelling_answers(vector<string>& dst, const vector<string>& correct, const vector<int> positions)
{
string elem;
for (int correct_index = 0; correct_index < positions.size(); ++correct_index){
  dst.push_back(correct.at(positions.at(correct_index)));
  }
}

/* Also called within the display_elems function, this function will populate the vector dst with all the spelling errors in the user's answer (to_grade).

  This function will pass in three arguements: a empty vector named mistakes, the user's input under the a string type, and the corrAns vector previously used within the extract_spelling_answers function above
  
  The input, passed into the parameter to_grade, is a series of characters. In order to convert from chracters to strings, to_grade will be ran through a loop

  The input contains five words, each containing a space, so each single word's characters will be put into a string before the space is reached

  Then, another loop is created to run through the vector of words, where it will check if the word is matching to the correct version, if it is not, thn*/

void
extract_spelling_mistakes(vector<string>& dst, const string& to_grade,
                const vector<string>& correct)
{
  string to_gradeStr;
  vector <string> input_Str;
  for (int c = 0; c < to_grade.size(); ++c){
    if (to_grade[c] == ' '){
          input_Str.push_back(to_gradeStr);
          to_gradeStr = "";
       }
       else{
          to_gradeStr += to_grade[c];
       }
    }
    input_Str.push_back(to_gradeStr);

  for (int vect_Idx = 0; vect_Idx < input_Str.size(); ++vect_Idx){
    if (input_Str[vect_Idx] != correct[vect_Idx]){
      string error_Str = input_Str[vect_Idx] + " -> " + correct[vect_Idx];
      dst.push_back(error_Str);
    }
  }   
}



void
report_misspell_mistakes(const vector<string>& mistakes,
                            const int num_words)
{
    /*  This function will pass in two arguments: the mistakes vector, and the number of words within the user input

    The purpose of this function is to calculate what the user's score is if they either get a word right or wrong. Based on how they have typed in the word.

    if the user gets all the words correct, that score will be multiplied by 3, so the max score that the user can get for all correct words is 15.

    However, if the user gets all the words wrong, then that number will be multiplied by -6, which will give max score for incorrect words as -30.

The score will be adjusted depending on which words the user spells correctly or incorrectly */
  
  int correct = 0;
  int incorrect = 0;

  if (mistakes.size() != 0){
    cout << "Misspelled words:" << endl;
    for (int index = 0; index < mistakes.size(); ++index){
      cout << "    " << mistakes[index] << endl;
      incorrect += 1;
      correct = num_words - incorrect;
    }
  }
  else {
    cout << "No misspelled words!" << endl;
    incorrect = 0;
    correct = num_words - incorrect;
  }
  int corr_Score = correct * 3;
  int incorr_Score = -(incorrect * -6);
  int total_Score = corr_Score - incorr_Score;

  cout << corr_Score << " points: ";
  cout << correct << " spelled correctly x 3 points for each.";
  cout << endl;

  cout << incorr_Score << " point penalty: ";
  cout << incorrect << " spelled incorrectly x -6 points for each.";
  cout << endl;
  
  cout << "Score: " << total_Score;

}

/* This function will pass in two parameters: the correct vector an the temp vector that holds all of the positions of the correct words within misspelled.txt

      This function runs through for menu option 1. This function will just take in the user input, and it will go through three different functions that has its own set of functionalities in the comments above, where it will evaluate whether or not the user has typed in the word correctly. */

void
evaluate_user_misspells(const vector<string>& src, const vector<int>& positions)
{
  
  string userInput;
  getline(cin, userInput);

  vector<string> corrAns;
  vector<string> mistakes;

  extract_spelling_answers(corrAns, src, positions); 

  extract_spelling_mistakes(mistakes, userInput, corrAns);

  report_misspell_mistakes(mistakes, corrAns.size());
}

/*  This functipn is included within menu option 1, which takes in two parameters: a vector that has all of the misspelled words, and a vector that has all of the correct words. 

        The purpose of this function is to direct the user to the first menu option, where they will type in a series of words that will be compared with the correct set of words */

void
misspelled(const vector<string>& mspelled, const vector<string>& correct)
{

  cout << endl;
  cout << "*** Using commonly misspelled words." << endl;
  
  vector<int> temp = randn(5, mspelled.size() - 1);
  display_elems(mspelled, temp);

  cout << "Type the words above with the correct spelling:";
  cout << endl;
  
  evaluate_user_misspells(correct, temp);
}

/* This function will take in one parameter, which is the dictionary vector from the dictionary.txt file.

      The implementation of this function is very similar to the display_elems function used for Menu Option 1. However, although similar, the only change within this function is that it will compare the words that the user inputs from the words from the full dictionary */

void
full_dict(const vector<string>& dict)
{
  
  cout << endl;
  cout << "*** Using full dictionary." << endl;
  
  vector<int> temp = randn(5, dict.size() - 1);
  display_elems(dict, temp);
  
  cout << "Correctly type the words above:";
  cout << endl;
  evaluate_user_misspells(dict, temp);
}

/* This function will just diplay the prompt for the user for Menu Option 3*/

void
display_words_prompt()
{
    cout << endl << "*** Displaying word list entries." << endl
         << "Which words do you want to display?" << endl
         << "A. Dictionary of all words" << endl
         << "B. Commonly misspelled words (wrongly spelled)" << endl
         << "C. Commonly misspelled words (correctly spelled)" << endl
         << "Your choice: ";
}

/* This function will pass in the vector of the dictionary. Its purpose as for Menu Option 3 is to check that based on the user input, if the numbers/range if within the range of 0 and either the dictionary, mispelled, or correct vectors (depending on what th user decides to choose), as well as whether the ranges that the user inputs are in numerical order.

      if the user's input passes those checks, then the program will print out the words within those ranges in numerical order. If the user does not pass those checks, then an error message will be printed out to the user.*/

void
print_range(const vector<string>& data)
{
    
  cout << "*** Enter the start and end indices between 0 and ";
  cout << data.size() - 1 << ": ";
  
  int range_Choice1;
  int range_Choice2;
  
  cin >> range_Choice1;
  cin >> range_Choice2;

  if ((((range_Choice1 >= 0) && (range_Choice1 < data.size()))&& ((range_Choice1 >= 0) && (range_Choice1 < data.size()))) && (range_Choice2 > range_Choice1)){
    for (int idx = range_Choice1; idx < range_Choice2 + 1; ++idx){
      cout << idx << ". ";
      cout << data.at(idx) << endl;
    }
  }
  else {
    cout << "The end index must come after the start, and the indices must be in range.";
  }
}

void
display_in_range(const vector<string>& dict, const vector<string>& mspelled,
                const vector<string>& correct)
{
    /* (Menu Option 3) This function will take in three parameters: the dictionary vector, the mispelled vector, and the correct vector

      The purpose of this function is for the user to be able to check a range of words within one of those three vectors with the range that they give to the program */
  
  char a = 'a';
  char b = 'b';
  char c = 'c';
  char upperA = 'A';
  char upperB = 'B';
  char upperC = 'C';
  
  display_words_prompt();
  char menu3Choice;
  cin >> menu3Choice;

  if ((menu3Choice == a) || (menu3Choice == upperA)){
    print_range(dict);
  }
  else if ((menu3Choice == b) || (menu3Choice == upperB)){
    print_range(mspelled);
  }
  else if ((menu3Choice == c) || (menu3Choice == upperC)){
    print_range(correct);
  }
  
}

/* (Menu Option 4) This function is a helper function that will conduct a binary based on the dictionary vector. Binary search will divide the range of indices by 2 in order to get to the target word faster. This method of searching is typically used within larger ranges of vectors or files */

bool
binary_helper(const vector<string>& data, int size, string target){
  int counter = 0;
  int start = 0, mid;
  
  do{ 
    string elem;
    mid = (start + size) / 2;

    elem = data[mid];

    if (elem < target){
      start = mid + 1;
      counter += 1;
      if (counter >= 10){
        cout << "   " << counter << ". Comparing to: " << elem << endl;
      }
      else {
        cout << "    " << counter << ". Comparing to: " << elem << endl;
      }
    }
    else if (elem > target){
      size = mid;
      counter += 1;
      if (counter >= 10){
        cout << "   " << counter << ". Comparing to: " << elem << endl;
      }
      else {
        cout << "    " << counter << ". Comparing to: " << elem << endl;
      }
    }
    else {
      counter += 1;
      if (counter >= 10){
        cout << "   " << counter << ". Comparing to: " << elem << endl;
      }
      else {
        cout << "    " << counter << ". Comparing to: " << elem << endl;
      }
      cout << target << " was found.";
      return true;
      
    }  
  }while(start < size);
  return false;
}

/* (Menu Option 4) This function will conduct the user to input a word that they want to search within the dictionary, using the binary_helper function above to clean the code. It will print out whether the word that the user types exists or not and can be located within the dictionary. 

  Since this function is a larger vector or file, linear search will take more time than binary search will*/
void
bsearch(const vector<string>& data)
{
  cout << "*** Enter word to search: ";

  string word_to_search;
  cin >> word_to_search;

  const int SIZE = data.size();

  bool validity = binary_helper(data, SIZE, word_to_search);

  if (validity == false){
    cout << word_to_search << " was NOT found.";
  }
}

/* This function will just extract the data from both the dictionary.txt and misspelled.txt files*/

void
extract_all_data(vector<string>& dict, vector<string>& mspelled,
                    vector<string>& correct)
{
    extract_dict(dict, "dictionary.txt");
    extract_misspelled(mspelled, correct, "misspelled.txt");
}

/* The main is responsible for just ccalling the function based on which option the user wants*/
int
main()
{
    srand(1);
    vector<string> dict, mspelled, correct;
    extract_all_data(dict, mspelled, correct);
    display_header(dict.size(), correct.size());
    unsigned int choice;
    do {
        display_menu_options();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        case 1:
            misspelled(mspelled, correct);
            break;
        case 2:
            full_dict(dict);
            break;
        case 3:
            display_in_range(dict, mspelled, correct);
            break;
        case 4:
            bsearch(dict);
            break;
        case 5:
            cout << "Exiting." << endl;
            break;
        default:
            cout << "No menu option " << choice << ". "
                 << "Please try again." << endl;
        }
    } while ((choice != 5) && !cin.eof());

    return 0;
}