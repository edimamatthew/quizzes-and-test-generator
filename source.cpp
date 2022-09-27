/*
* This program helps teachers to create quizzes and tests. It allows them to add multiple choice questions to the question bank.
* Then they have the option of creating tests from the questions added to the bank.
* To create test from the question bank, they have the options of either manually selecting the questions or letting the system pick for them
* It has 3 files - bank.txt, randomanswers.txt and test.txt
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void generateTest();
void addQuestion();
int getLastQuestionFromBank(fstream&);
vector<int> extractChosenQuestions(string, char);
int getTotalNumberOfQuestions(fstream&);
void listQuestionsFromBank(fstream&);
void addTest(fstream&, vector<int>);
int addRandomAnswers(fstream&, int, vector<int>);
bool isNumeric(string str);
int getTotalNumberOfRandomAnswers(fstream&);

int main()
{
	// Declare constants representing the 3 operations that can be performed on this system
	const int GENERATE_TEST = 1;
	const int ADD_QUESTION = 2;
	const int EXIT = 3;

	int operationCode;

	cout << "This program helps teachers create quizzes and tests.\n\n";

	do {
		// show available operation options and take input
		cout << "List of available operations\n------------\n"
			<< GENERATE_TEST << ": Generate a multiple-choice test\n"
			<< ADD_QUESTION << ": Add a question to the bank of questions\n"
			<< EXIT << ": End the program\n\n";

		do {
			cout << "Enter a code from the list above: ";
			cin >> operationCode;
			if (cin.fail() || (operationCode < GENERATE_TEST || operationCode > EXIT))
			{
				cin.clear(); cin.ignore();
				cout << "Operation code must be an integer between \"" << GENERATE_TEST << "\" and \"" << EXIT << "\"\n\n";
			}
			else break;
		} while (!cin.fail());

		// Check the selected operation and call the corresponding functions to handle the request
		if (operationCode == GENERATE_TEST || operationCode <= ADD_QUESTION) {

			switch (operationCode)
			{
			case GENERATE_TEST:
				generateTest();
				break;

			case ADD_QUESTION:
				addQuestion();
				break;

			default:
				break;
			}

			// Display a success message when everything has been completed
			cout << "\n=====================================================================" << endl;
			cout << "Operation completed. Use the options below to either continue or exit." << endl;
			cout << "=====================================================================" << endl << endl;
		}
		else {
			// Exit the program if user selected the EXIT code
			cout << "Thank you for using our program. See you next time :)\n";
			break;
		}

	} while (true);

	return 0;
}

void generateTest()
{
	srand((unsigned int)time(NULL)); // Initialize the random number generator

	const int CHOOSE_QUESTION = 1;
	const int RANDOM_PICK = 2;
	int chooseOrRandom;
	int numberOfQuestions;
	int totalNumberOfQuestionsInBank = 0;
	int randomItem;

	string question;
	string chosenQuestions;
	vector<int> questionNumbers;
	char chosenDelimiter = '\0'; // Declare delimiter variable for question selection(s) and initialize to a null char

	fstream bank;
	bank.open("bank.txt", ios::in);

	if (!bank)
	{
		cout << "We could not open bank.txt file, so you cannot proceed." << endl;
	}
	else {
		do {
			cout << "Do you want to choose the question or get it randomly? "
				<< CHOOSE_QUESTION << " = > Choose | " << RANDOM_PICK << " = > Random: ";
			cin >> chooseOrRandom;

			if (cin.fail() || (chooseOrRandom != CHOOSE_QUESTION && chooseOrRandom != RANDOM_PICK))
			{
				cin.clear(); cin.ignore();
				cout << "Invalid input. Enter either " << CHOOSE_QUESTION << " or " << RANDOM_PICK << "\n\n";
			}
			else break;
		} while (!cin.fail());

		totalNumberOfQuestionsInBank = getTotalNumberOfQuestions(bank); // Get number of questions from the bank

		do {
			cout << "How many questions should be on the test? ";
			cin >> numberOfQuestions;

			if (cin.fail() || numberOfQuestions < 1)
			{
				cin.clear(); cin.ignore();
				cout << "Invalid input. The number of questions must be a number and at least 1. " << "\n\n";
			}
			else if (numberOfQuestions > totalNumberOfQuestionsInBank)
			{
				cout << "Your selected number of questions is more than the number of questions in the bank, which is => " << totalNumberOfQuestionsInBank << "\n\n";
			}
			else break;
		} while (!cin.fail());

		cin.clear(); cin.ignore();

		if (chooseOrRandom == CHOOSE_QUESTION)
		{
			// Run this section if user wants to choose the questions
			cout << "See all questions below and make your selection(s);\n==========\n";

			listQuestionsFromBank(bank); // List all the questions from the bank with their corresponding numbers

			do {
				do {
					cout << "\nEnter the number(s) that corresponds with your choice question(s) below, seperated by commas(,). Eg. 1,5,8.\nYou can also enter a range; Eg. 2-6:" << endl;
					getline(cin, chosenQuestions);

					// Check if user tries to combine comma separated selector with range-based, which is illegal
					if (chosenQuestions.find(',') != string::npos && chosenQuestions.find('-') != string::npos)
					{
						cout << "You cannot use comma separators and range based options at the same time." << endl;
						continue;
					}
					break;
				} while (true);

				// Set the delimiter value so it can be passed to the function to work on the selected choices
				if (chosenQuestions.find(',') != string::npos)
				{
					chosenDelimiter = ',';
				}
				else if (chosenQuestions.find('-') != string::npos)
				{
					chosenDelimiter = '-';
				}

				questionNumbers = extractChosenQuestions(chosenQuestions, chosenDelimiter); // Returns a vector containing the selected options

				int numberOfSelectedQuestions = static_cast<int>(questionNumbers.size());

				if (numberOfQuestions != numberOfSelectedQuestions) // Check if user tries to add more questions what what was initially defined
				{
					cout << "It appears you wanted to create \"" << numberOfQuestions << "\" question(s). But your selection is \"" << numberOfSelectedQuestions << "\" question(s). Select again." << endl;
				}
				else break;

			} while (true);
		}
		else {
			// Run this section if user wants to load random questions
			vector<int> randomQuestions;

			for (int i = 0; i < numberOfQuestions; i++) // Loop through the number of random questions to be picked
			{
				// On each loop, loop again and apply some logics to ensure random questions are not picked more than once
				while (true)
				{
					randomItem = (rand() % totalNumberOfQuestionsInBank) + 1; // Generate random number within the range of questions available

					// Go through the vector to see if the randomly generated number above has already added.
					// Continue the loop if it has alread been added. Else, add it.
					bool itemAlreadyAdded = find(begin(randomQuestions), end(randomQuestions), randomItem) != end(randomQuestions);

					if (!itemAlreadyAdded)
					{
						randomQuestions.push_back(randomItem);
						break;
					}

				}
			}

			questionNumbers = randomQuestions;
		}

		addTest(bank, questionNumbers); // Call the function to add the test sending a vector of the question numbers picked
	}

	bank.close();
}

vector<int> extractChosenQuestions(string questions, char chosenDelimiter) {
	vector<int> data; // Vector variable
	vector<int> tmpData;
	stringstream ssQuestions(questions); // convert the date to a stream so we can perform extraction operations on it
	string questionsString;

	// In the while loop below, additional data (delimiter) is passed to the getline function
	// This reads the date stream up to the delimiter, stores it in the dateString function and adds to the vector, then continue reading
	while (getline(ssQuestions, questionsString, chosenDelimiter)) {
		data.push_back(stoi(questionsString)); // Add new element to the data vector
	}

	// If user selected the range-based selection, then loop through the data in the range, save to a temporary variable, and assign to data
	if (chosenDelimiter == '-')
	{
		for (int i = data[0]; i <= data[1]; i++)
		{
			tmpData.push_back(i);
		}
		data = tmpData;
	}

	return data;
}

int getTotalNumberOfQuestions(fstream& bank)
{
	string question;
	int counter = 0;

	// Clear the opened file and start from the beginning

	bank.clear();
	bank.seekp(0, ios::beg);

	// Go through all the lines in the file. Check, if a line starts with "Question", then increase the counter
	// The counter here represents the number of questions in the bank
	while (getline(bank, question))
	{
		if (question.rfind("Question", 0) == 0) {
			counter++;
		}
	}
	return counter;
}

void listQuestionsFromBank(fstream& bank)
{
	string question;

	bank.clear();
	bank.seekp(0, ios::beg);

	// Go through all the lines in the file. Check, if a line starts with "Question", then list it, as it is a question.
	while (getline(bank, question))
	{
		if (question.rfind("Question", 0) == 0) {
			cout << question << endl;
		}
	}
}

void addTest(fstream& bank, vector<int> questionNumbers)
{
	string question;
	string questionWithNumber;
	string formattedQuestion;
	string formattedOption;
	string newQuestionWithNumber;
	int questionCounter = 1;
	int options;
	int optionsCounter = 0;
	int optionNumber;
	bool readNextLine = false;
	bool readNextOption = false;
	bool hasOptionsCounter = false;
	bool isOptions = false;

	fstream test;
	test.open("test.txt", ios::out);

	if (!test)
	{
		cout << "We could not open test.txt file, so you cannot proceed." << endl;
	}
	else
	{
		// Go through the number of selected or randomly picked questions
		for (int x = 0; x < static_cast<int>(questionNumbers.size()); x++)
		{
			questionWithNumber = "Question " + to_string(questionNumbers[x]) + "."; // Format the question numbers to use later in this function

			bank.clear();
			bank.seekp(0, ios::beg);

			optionNumber = 1;

			vector<int> addedRandomAnswers;

			// Go through all the lines in the software
			while (getline(bank, question))
			{
				// The immediate line after the question is the number of answer options. Save it to optionsCounter and iterate.
				// On each iteration, decrement the options until it finishes
				// Before it finishes, on each loop, those are the answer options. Add them under the questions and properly format the numbering
				if (readNextLine || readNextOption) {
					readNextOption = true;
					stringstream(question) >> options;
					if (!hasOptionsCounter)
					{
						optionsCounter = options;
						hasOptionsCounter = true;
						isOptions = true;
					}
						
					if (optionsCounter == 0)
					{
						readNextOption = false;
						readNextLine = false;
						hasOptionsCounter = false;
					}
					else {
						optionsCounter--;
					}
					
					if (!isOptions) // So the options counter will not be stored
					{
						// If the answer option is "Random", then call the random function to pick from the randomanswers.txt options
						if (question.substr(3, 6) == "Random") // Eg. 1. Random. So we remove first 3 characters and check the next 6 if it is "Random"
						{
							// Get the last added random numbers, save it to a vector and send back to the function so it checks before adding
							// This is to stop adding more than one random answer to one question
							addedRandomAnswers.push_back(addRandomAnswers(test, optionNumber, addedRandomAnswers));
						}
						else {
							// If it is not random, then format the numbering and add it to test.
							formattedOption = question;
							formattedOption.erase(0, 3);
							test << optionNumber << ". " << formattedOption << endl;
						}
						optionNumber++;
					}
					isOptions = false;
				}

				// Look for questions with their numbering, replace the default numbering with new numbering according to the order of adding it to test
				if (question.rfind(questionWithNumber) == 0) {
					formattedQuestion = question;
					newQuestionWithNumber = "Question " + to_string(questionCounter) + ": ";
					formattedQuestion.replace(formattedQuestion.find(questionWithNumber), questionWithNumber.length(), newQuestionWithNumber);

					test << formattedQuestion << endl;
					questionCounter++;

					readNextLine = true;
				}
				
			}
		}
	}
}

int addRandomAnswers(fstream& test, int optionNumber, vector<int> addedRandomAnswers) {
	string answer;
	int counter = 1;
	int totalRandomAnswers;
	int answerToFetch;
	int lastAddedOption;

	fstream random;
	random.open("randomanswers.txt", ios::in);

	if (!random)
	{
		cout << "We could not open randomanswers.txt file, so you cannot proceed." << endl;
	}
	else {
		totalRandomAnswers = getTotalNumberOfRandomAnswers(random);

		// Create a vector and store the random answers. It can then easily be used and/or checked for duplicate
		vector<string> answers;

		random.clear();
		random.seekp(0, ios::beg);

		while (getline(random, answer))
		{
			if (isNumeric(answer))
			{
				continue;
			}
			answers.push_back(answer);
			counter++;
		}

		while (true)
		{
			answerToFetch = (rand() % totalRandomAnswers);
			if (count(addedRandomAnswers.begin(), addedRandomAnswers.end(), answerToFetch)) {
				continue;
			}
			break;
		}

		lastAddedOption = answerToFetch;
		test << optionNumber << ". " << answers[answerToFetch] << endl; // Format the random answers option numbering
	}
	return lastAddedOption;
}

int getTotalNumberOfRandomAnswers(fstream& random)
{
	string answer;
	int counter = 0;

	random.clear();
	random.seekp(0, ios::beg);

	// Go through all the lines in the randomanswers.txt and increment the counter
	// However, skip the first line. This line has a number, eg. 4
	while (getline(random, answer))
	{
		if (isNumeric(answer))
		{
			continue;
		}
		counter++;
	}
	return counter;
}

void addQuestion()
{
	fstream bank;
	bank.open("bank.txt", ios::out | ios::app | ios::in);

	string question;
	string answer;
	string answerOptions;
	string line;
	string questionName;
	int answersOptionsNumber;
	int questionNumber;

	if (!bank)
	{
		cout << "We could not open bank.txt file, so you cannot proceed." << endl;
	}
	else {
		cin.ignore();

		do {
			cout << "\nEnter a question below:" << endl;
			getline(cin, question);

			if (question.length() >= 10)
			{
				break;
			}
			cout << "Your question must be atleast 10 characters long." << endl;
		} while (true);

		questionNumber = getLastQuestionFromBank(bank);

		bank.clear();
		bank.seekp(0, ios::beg);

		questionName = "Question " + to_string(questionNumber + 1) + ". " + question;

		bank << questionName << endl;

		do
		{
			do {
				cout << "Enter the number of answer options below." << endl;
				getline(cin, answerOptions);
				if (isNumeric(answerOptions))
				{
					stringstream(answerOptions) >> answersOptionsNumber;
					if (answersOptionsNumber >= 2 && answersOptionsNumber <= 5)
					{
						break;
					}
				}
				cout << "Your question should have atleast 2 answer options, and not more than 5. Try again." << endl;
				continue;
			} while (true);

			bank << answersOptionsNumber << endl;

			cout << "Type the options below. Typing \"Random\" will randomly generate options:" << endl;

			for (int i = 1; i <= answersOptionsNumber; i++)
			{
				do {
					cout << "Answer option " << i << ": ";
					getline(cin, answer);

					if (answer.length() > 0)
					{
						break;
					}
					cout << "You should atleast write something to give an answer option. Try again." << endl;
				} while (true);
				bank << i << ". " << answer << endl;
			}
			break;

		} while (true);
	}

	bank.close();
}

int getLastQuestionFromBank(fstream& bank)
{
	int questionNumber = 0;
	string line;

	bank.clear();
	bank.seekp(0, ios::beg);

	while (bank >> line)
	{
		if (line.rfind("Question", 0) == 0) {
			questionNumber++;
		}
	}

	return questionNumber;
}

bool isNumeric(string str)
{
	stringstream stream;
	double number;

	stream << str;
	stream >> number;

	return stream.eof();
}
