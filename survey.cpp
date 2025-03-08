#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <windows.h>
#include <stdlib.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

using namespace std;

// Structure to represent a question
struct Question
{
    string text;
    int type = 0;
    vector<string> options;
    // Overloading the << operator to write a Question object to the file
    friend ostream &operator<<(ostream &os, const Question &q)
    {
        os << q.text << endl;
        os << q.type << endl;
        for (const auto &option : q.options)
        {
            os << option << endl;
        }
        return os;
    }

    // Overloading the >> operator to read a Question object from the file
    friend istream &operator>>(istream &is, Question &q)
    {
        getline(is, q.text);

        is >> q.type;
        q.options.clear();
        is.ignore();
        // Ignore the newline character after reading the type
        if (q.type)
        {
            string option;
            int j = 0;
            while (j < 4)
            {
                getline(is, option);
                q.options.push_back(option);
                j++;
            }
        }
        return is;
    }
};

// Function to add a new survey title to the survey list file
void addSurveyTitle(const string &surveyTitle, const string &surveyFileName)
{
    ofstream surveyListFile("survey_list.txt", ios::app);
    if (surveyListFile.is_open())
    {
        surveyListFile << surveyTitle << " " << surveyFileName << endl;
        surveyListFile.close();
    }
    else
    {
        cout << "Unable to open survey list file!" << endl;
    }
}

// Function to create a new survey
void createSurvey()
{
    string surveyTitle, surveyFileName;
    cout << "Enter survey title: ";
    getline(cin, surveyTitle);

    // Generate a unique filename for the survey
    surveyFileName = surveyTitle + ".txt";

    // Add the survey title to the survey list file
    addSurveyTitle(surveyTitle, surveyFileName);

    // Prompt for questions until the user stops
    vector<Question> questions;
    char addMoreQuestions;
    do
    {

        Question question;
        cout << "Enter question " << questions.size() + 1 << ": ";
        getline(cin, question.text);
        char questionType;
        cout << "Is this a multiple-choice question (y/n)? ";
        cin >> questionType;
        cin.ignore();
        // Ignore newline character left in buffer

        if (questionType == 'y' || questionType == 'Y')
        {
            cout << "Enter options (separated by newline, enter 'done' to finish):" << endl;
            string option;
            for (int i = 0; i < 4; ++i)
            {
                getline(cin, option);
                if (option == "done")
                {
                    break;
                }
                question.options.push_back(option);
                question.type = 1;
            }
        }

        questions.push_back(question);

        cout << "Add another question (y/n)? ";
        cin >> addMoreQuestions;
        cin.ignore();
        // Ignore newline character left in buffer

    } while (addMoreQuestions == 'y' || addMoreQuestions == 'Y');

    // Write questions to survey file
    ofstream surveyFile(surveyFileName, ios::binary);

    if (surveyFile.is_open())
    {
        for (const auto &q : questions)
        {
            surveyFile << q;
            // Serialize each Question object to the file
        }
        surveyFile.close();
        cout << "Survey created successfully." << endl;
    }
    else
    {
        cout << "Unable to create survey file!" << endl;
    }
}

// Function to display all available surveys
void displaySurveys()
{
    ifstream surveyListFile("survey_list.txt");
    if (surveyListFile.is_open())
    {
        string surveyTitle, surveyFileName;
        while (surveyListFile >> surveyTitle >> surveyFileName)
        {
            cout << surveyTitle << endl;
        }
        surveyListFile.close();
    }
    else
    {
        cout << "No surveys available." << endl;
    }
}
char validateMCQInput()
{
    char userInput;
    cout << "Enter your choice (a/b/c/d): ";
    cin >> userInput;
    cin.ignore();
    // Ignore newline character left in buffer
    userInput = tolower(userInput); // Convert to lowercase
    if (userInput < 'a' || userInput > 'd')
    {
        cout << "Invalid choice. Please enter a valid option (a/b/c/d)." << endl;
        return validateMCQInput();
        // Recursive call until valid input is provided
    }
    return userInput;
}

double getRatingInput()
{
    double rating;
    while (true)
    {
        cout << "Please rate your survey experience out of 5: ";
        if (!(cin >> rating))
        {
            // Check if the input is not an integer
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Discard invalid input
        }
        else if (rating < 1 || rating > 5)
        {
            // Check if the input is out of range
            cout << "Invalid rating. Please enter a number between 1 and 5." << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Clear input buffer
            return rating;
        }
    }
}

// Function to handle client's survey filling
void fillSurvey(const string &surveyFileName)
{
    ifstream surveyFile(surveyFileName);
    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }

    vector<Question> questions;
    Question tempQuestion;
    // Temporary variable to read each question from the file
    ofstream userResponsesFile("2" + surveyFileName, ios::app); // Open or create user responses file
    if (!userResponsesFile.is_open())
    {
        cout << "Unable to open user responses file for writing." << endl;
        return;
    }

    while (surveyFile >> tempQuestion)
    {
        questions.push_back(tempQuestion);
    }

    for (size_t i = 0; i < questions.size(); ++i)
    {

        cout << "Q" << i + 1 << " " << questions[i].text << endl;
        if (questions[i].type)
        {
            int j = 0;

            for (const auto &option : questions[i].options)
            {
                cout << " " << char('A' + j) << ". " << option << endl;
                j++;
            }
            char answer = validateMCQInput();
            string ans;
            ans = questions[i].options[answer - 97];
            userResponsesFile << ans << endl;
        }
        else
        {
            string answer;
            cout << "Enter your answer: ";
            getline(cin, answer);
            userResponsesFile << answer << endl;
        }
    }
    userResponsesFile << "," << endl;
    userResponsesFile.close();
    surveyFile.close();
}
void readUserResponses(const string &surveyFileName)
{
    ifstream surveyFile(surveyFileName);
    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }
    ifstream userResponsesFile("2" + surveyFileName);
    if (!userResponsesFile.is_open())
    {
        cout << "No responses available for this survey." << endl;
        return;
    }
    vector<Question> questions;
    Question tempQuestion;
    // Temporary variable to read each question from the file

    string response;
    vector<string> responses;
    while (getline(userResponsesFile, response))
    {
        responses.push_back(response);
    }
    userResponsesFile.close();
    int count = 0;
    for (int i = 0; i < responses.size(); i++)
    {
        if (responses[i] == ",")
        {
            count += 1;
        }
    }

    while (surveyFile >> tempQuestion)
    {
        questions.push_back(tempQuestion);
    }
    int l = 1;
    int m = 0;
    cout << "There are " << count << " responses" << endl;

    while (l <= count)
    {
        cout << "\n--Response " << l << "--" << endl;

        for (size_t i = 0; i < questions.size(); ++i)
        {

            cout << "Q" << i + 1 << " " << questions[i].text << endl;
            if (questions[i].type)
            {
                int j = 0;
                for (const auto &option : questions[i].options)
                {
                    cout << " " << char('A' + j) << ". " << option << endl;
                    j++;
                }
            }
            if (responses[m] == ",")
            {
                m += 1;
            }
            cout << "-->" << responses[m] << endl;

            m++;
        }
        l++;
    }
}
// Function to add a new question to an existing survey
void addQuestionToSurvey(const string &surveyFileName)
{
    vector<Question> questions;
    ifstream surveyFile(surveyFileName);
    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }
    Question tempQuestion;
    while (surveyFile >> tempQuestion)
    {
        questions.push_back(tempQuestion);
    }
    surveyFile.close();

    // Prompt admin for new question details
    Question newQuestion;
    cout << "Enter the new question text: ";
    getline(cin, newQuestion.text);
    // Prompt for question type
    cout << "Is it a multiple-choice question? (yes/no): ";
    string choice;
    getline(cin, choice);
    newQuestion.type = (choice == "yes") ? 1 : 0;

    if (newQuestion.type)
    {
        cout << "Enter the choices for the question (type 'done' when finished):\n";
        while (true)
        {
            cout << "Choice: ";
            getline(cin, choice);
            if (choice == "done")
                break;
            newQuestion.options.push_back(choice);
        }
    }

    // Add the new question to the survey
    questions.push_back(newQuestion);

    // Write updated questions to survey file
    ofstream updatedSurveyFile(surveyFileName);
    if (!updatedSurveyFile.is_open())
    {
        cout << "Unable to open survey file for writing." << endl;
        return;
    }
    for (const auto &q : questions)
    {
        updatedSurveyFile << q;
    }
    updatedSurveyFile.close();
    cout << "Question added successfully to the survey." << endl;
}
// Function to delete a question from an existing survey
void deleteQuestionFromSurvey(const string &surveyFileName)
{
    // Load questions from the survey file
    vector<Question> questions;
    ifstream surveyFile(surveyFileName);
    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }
    Question tempQuestion;
    while (surveyFile >> tempQuestion)
    {
        questions.push_back(tempQuestion);
    }
    surveyFile.close();

    // Display existing questions for deletion
    cout << "Existing Questions:" << endl;
    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << i + 1 << ". " << questions[i].text << endl;
    }

    // Prompt admin to select a question to delete
    cout << "Enter the number of the question you want to delete: ";
    size_t selectedQuestion;

    cin >> selectedQuestion;
    cin.ignore(); // Ignore newline character

    // Remove the selected question from the vector
    if (selectedQuestion > 0 && selectedQuestion <= questions.size())
    {
        questions.erase(questions.begin() + selectedQuestion - 1);

        cout << "Question deleted successfully." << endl;
    }
    else
    {
        cout << "Invalid question number." << endl;
        return;
    }

    // Write updated questions to survey file
    ofstream updatedSurveyFile(surveyFileName);
    if (!updatedSurveyFile.is_open())
    {
        cout << "Unable to open survey file for writing." << endl;
        return;
    }
    for (const auto &q : questions)
    {
        updatedSurveyFile << q;
    }
    updatedSurveyFile.close();
}

// Function to edit an existing question in a survey
void editQuestionInSurvey(const string &surveyFileName)
{
    // Load questions from the survey file
    vector<Question> questions;
    ifstream surveyFile(surveyFileName);

    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }
    Question tempQuestion;
    while (surveyFile >> tempQuestion)
    {

        questions.push_back(tempQuestion);
    }
    surveyFile.close();

    // Display existing questions for editing
    cout << "Existing Questions:" << endl;
    for (size_t i = 0; i < questions.size(); ++i)
    {
        cout << i + 1 << ". " << questions[i].text << endl;
    }

    // Prompt admin to select a question to edit
    cout << "Enter the number of the question you want to edit: ";
    size_t selectedQuestion;
    cin >> selectedQuestion;
    cin.ignore();
    // Ignore newline character

    // Prompt admin for new question details

    Question &selected = questions[selectedQuestion - 1];
    cout << "Enter the new question text: ";
    getline(cin, selected.text);
    cout << "Is it a multiple-choice question? (yes/no): ";
    string choice;
    getline(cin, choice);
    selected.type = (choice == "yes") ? 1 : 0;

    // If it's a multiple-choice question, prompt for options

    if (selected.type)
    {
        selected.options.clear();
        cout << "Enter the choices for the question (type 'done' when finished):\n";
        while (true)
        {
            cout << "Choice: ";
            getline(cin, choice);
            if (choice == "done")
                break;
            selected.options.push_back(choice);
        }
    }

    // Write updated questions to survey file

    ofstream updatedSurveyFile(surveyFileName);
    if (!updatedSurveyFile.is_open())
    {
        cout << "Unable to open survey file for writing." << endl;
        return;
    }
    for (const auto &q : questions)
    {
        updatedSurveyFile << q;
    }
    updatedSurveyFile.close();
    cout << "Question edited successfully." << endl;
}

// Function to validate admin login credentials

bool validateAdminLogin()
{
    string username, password;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    // Check if the provided username and password match the admin credentials
    if (username == "admin" && password == "password")
    {
        cout << "Login successful." << endl;
        return true;
    }
    else
    {
        cout << "Invalid username or password. Please try again." << endl;
        return false;
    }
}

void drawHistogram(double percentages[], int size)
{
    const char *barChars = "■□";
    // Two characters representing filled and empty bars

    int barCount = strlen(barChars);

    // Find the maximum percentage to scale the bars

    double maxPercentage = 0;
    for (int i = 0; i < size; ++i)
    {
        if (percentages[i] > maxPercentage)
        {
            maxPercentage = percentages[i];
        }
    }

    // Draw histogram
    for (int i = 0; i < size; ++i)
    {
        int barLength = round(percentages[i] / maxPercentage * 50);

        // Scale the bar length

        cout << setw(10) << percentages[i] << "% | ";
        for (int j = 0; j < 50; ++j)
        {
            if (j < barLength)
            {
                cout << barChars[0];
                // Filled bar
            }
            else
            {
                cout << barChars[1];
                // Empty bar
            }
        }
        cout << endl;
    }
}

void showStatistics(const string &surveyFileName)
{
    ifstream surveyFile(surveyFileName);
    if (!surveyFile.is_open())
    {
        cout << "Unable to open survey file: " << surveyFileName << endl;
        return;
    }
    ifstream userResponsesFile("2" + surveyFileName);
    if (!userResponsesFile.is_open())
    {
        cout << "No responses available for this survey." << endl;
        return;
    }
    vector<Question> questions;
    Question tempQuestion;

    // Temporary variable to read each question from the file

    string response;
    vector<string> responses;
    while (getline(userResponsesFile, response))
    {
        responses.push_back(response);
    }
    userResponsesFile.close();
    int count = 0;
    for (int i = 0; i < responses.size(); i++)
    {
        if (responses[i] == ",")
        {
            count += 1;
        }
    }

    while (surveyFile >> tempQuestion)
    {
        questions.push_back(tempQuestion);
    }
    int j = 0;
    int m = 0;
    responses.erase(remove_if(responses.begin(), responses.end(), [](const string &str)
                              {
                                  return str == ",";
                                  // Check if string contains only commas
                              }),
                    responses.end());
    cout << "There are " << count << " responses" << endl;

    cout << "There are " << questions.size() << " number of questions in this Survey." << endl;

    for (size_t i = 0; i < questions.size(); ++i)
    {

        cout << "\n\nQ" << i + 1 << " " << questions[i].text << endl;

        if (questions[i].type)
        {
            int j = 0;
            int percentages[4];
            int z = 0;

            for (const auto &option : questions[i].options)
            {
                cout << " " << char('A' + j) << ". " << option << endl;
                int n = 0;
                int t = 0;

                while (t < responses.size())
                {
                    // cout<<(responses[j]==option)<<" "<<responses[j];
                    if (t % questions.size() == i && responses[t] == option)
                    {
                        n += 1;
                    }
                    t++;
                }
                int percent = ((n * 100) / count);
                percentages[z] = percent;
                z++;
                // cout << percent << n<<option;
                int numBars = percent;

                // Print the bars
                cout << "[";
                for (int i = 0; i < 100; ++i)
                {
                    if (i < numBars)
                    {
                        cout << "|";
                    }
                }
                cout << "] " << percent << "%" << endl;

                j++;
            }
            double doubleArray[4];
            for (int h = 0; h < 4; ++h)
            {
                doubleArray[h] = static_cast<double>(percentages[h]);
            }
            // drawHistogram(doubleArray,4);
        }

        else
        {
            int j = 0;
            int n = 0;
            while (j < responses.size())
            {
                if (j % questions.size() == i && responses[j] != "")
                {
                    n += 1;
                }
                j++;
            }
            cout << "-->This question was answered : " << n << "/" << count << " times" << endl;
        }
    }
}

void tittle()
{

    system("Color 7C");

    cout << "-------------------------------------------" << endl;

    cout << " +++++++++++++++++++++++++++++++++++++++++++++" << endl;

    cout << "  _____                                          " << endl
         << " / ____|                              _     _   " << endl
         << "| (___  _   _ _ ____   _____ _   _   _| |_ _| |_ " << endl
         << " \\___ \\| | | | '__\\ \\ / / _ \\ | | | |_   _|_   _|" << endl
         << " ____) | |_| | |   \\ V /  __/ |_| |   |_|   |_|  " << endl
         << "|_____/ \\__,_|_|    \\_/ \\___|\\__, |              " << endl
         << "                              __/ |              " << endl
         << "                             |___/               " << endl;

    cout << "|           SDF MINI PROJECT               |" << endl;

    cout << "|  Made by Backbenchers of B10, JIIT Noida |" << endl;

    cout << "-------------------------------------------" << endl;

    cout << "|    Anjaneya Sharma - 23103301            |" << endl;

    cout << "-------------------------------------------" << endl;

    cout << "|    Shubham Kumar - 23103295              |" << endl;

    cout << "-------------------------------------------" << endl;

    cout << "|    Annant Kumar Singh - 23103291         |" << endl;

    cout << "-------------------------------------------" << endl;

    cout << "|    Arjun Kumar - 23103284                |" << endl;

    cout << " +++++++++++++++++++++++++++++++++++++++++++++" << endl;

    cout << "--------------------------------------------" << endl;

    cout << endl;
}

// Function to handle client signup
void clientSignup()
{
    string username, password;
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    // Open the clients file in append mode and store the new username and password
    ofstream clientsFile("clients.txt", ios::app);
    if (clientsFile.is_open())
    {
        clientsFile << username << " " << password << endl;

        cout << "Signup successful. You can now login with your credentials." << endl;
        clientsFile.close();
    }
    else
    {
        cout << "Error: Unable to open clients file." << endl;
    }
}

// Function to handle client login
bool clientLogin()
{
    string username, password;
    cout << "Enter your username: ";

    cin >> username;

    cout << "Enter your password: ";
    cin >> password;

    // Open the clients file and check if the provided username and password match any entries

    ifstream clientsFile("clients.txt");
    if (clientsFile.is_open())
    {

        string storedUsername, storedPassword;

        while (clientsFile >> storedUsername >> storedPassword)
        {

            if (storedUsername == username && storedPassword == password)
            {
                cout << "Login successful. Welcome, " << username << "!" << endl;
                clientsFile.close();
                return true;
            }
        }
        cout << "Invalid username or password. Please try again." << endl;
        clientsFile.close();
        return false;
    }
    else
    {
        cout << "Error: Unable to open clients file." << endl;
        return false;
    }
}

void submitFeedback(const string &surveyTitle, double rating)
{
    ofstream feedbackFile("f" + surveyTitle, ios::app);

    if (feedbackFile.is_open())
    {

        feedbackFile << rating << endl;
        feedbackFile.close();

        cout << "Thank you for your feedback!" << endl;
    }
    else
    {
        cout << "Unable to open feedback file!" << endl;
    }
}

double calculateAverageRating(const string &surveyTitle)
{
    string feedbackFileName = "f" + surveyTitle;

    ifstream feedbackFile(feedbackFileName);

    if (!feedbackFile.is_open())
    {

        cout << "No feedback available for this survey." << endl;
        return 0.0;
    }

    double rating;
    double totalRatings = 0;
    int numRatings = 0;

    while (feedbackFile >> rating)
    {

        totalRatings += rating;

        numRatings++;
    }

    feedbackFile.close();

    if (numRatings == 0)
    {
        cout << "No ratings available for this survey." << endl;
        return 0.0;
    }

    return static_cast<double>(totalRatings) / numRatings;
}

void displayRatingVisual(double avgRating)
{
    // Convert average rating to an integer
    cout << "Average Rating: " << endl;

    vector<double> values = {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

    double nearest = values[0];

    for (double val : values)
    {

        if (abs(avgRating - val) < abs(avgRating - nearest))
        {
            nearest = val;
        }
    }

    // Use if-else statements to handle each possible rounded value
    if (nearest == 0)
    {
        cout << endl;
    }
    else if (nearest == 0.5)
    {

        cout << "_/|" << endl;
        cout << "\\ |" << endl;
        cout << "|/" << endl;
    }
    else if (nearest == 1)
    {

        cout << "_/\\_" << endl;
        cout << "\\  /" << endl;
        cout << "|/\\|" << endl;
    }
    else if (nearest == 1.5)
    {

        cout << "_/\\_ _/|" << endl;
        cout << "\\  / \\ |" << endl;
        cout << "|/\\| |/" << endl;
    }
    else if (nearest == 2)
    {

        cout << "_/\\_ " << "_/\\_ " << endl;
        cout << "\\  / " << "\\  / " << endl;
        cout << "|/\\| " << "|/\\| " << endl;
    }
    else if (nearest == 2.5)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/|" << endl;
        cout << "\\  / " << "\\  / " << "\\ |" << endl;
        cout << "|/\\| " << "|/\\| " << "|/" << endl;
    }
    else if (nearest == 3)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/\\_" << endl;
        cout << "\\  / " << "\\  / " << "\\  /" << endl;
        cout << "|/\\| " << "|/\\| " << "|/\\|" << endl;
    }
    else if (nearest == 3.5)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/|" << endl;
        cout << "\\  / " << "\\  / " << "\\  / " << "\\ |" << endl;
        cout << "|/\\| " << "|/\\| " << "|/\\| " << "|/" << endl;
    }
    else if (nearest == 4)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/\\_ " << endl;
        cout << "\\  / " << "\\  / " << "\\  / " << "\\  / " << endl;
        cout << "|/\\| " << "|/\\| " << "|/\\| " << "|/\\| " << endl;
    }
    else if (nearest == 4.5)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/|" << endl;
        cout << "\\  / " << "\\  / " << "\\  / " << "\\  / " << "\\ |" << endl;
        cout << "|/\\| " << "|/\\| " << "|/\\| " << "|/\\| " << "|/" << endl;
    }
    else if (nearest == 5)
    {

        cout << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/\\_ " << "_/\\_" << endl;
        cout << "\\  / " << "\\  / " << "\\  / " << "\\  / " << "\\  /" << endl;
        cout << "|/\\| " << "|/\\| " << "|/\\| " << "|/\\| " << "|/\\|" << endl;
    }
    else
    {
        cout << "Invalid value." << endl;
    }

    cout << " (" << fixed << setprecision(2) << nearest << "/5)" << endl;
}

int main()
{
    int userType;
    tittle();

    cout << "Are you an admin (1) or a client (2)? ";
    cin >> userType;
    cin.ignore();
    // Ignore newline character left in buffer

    if (userType == 1)
    { // Admin
        if (!validateAdminLogin())
        {
            cout << "Exiting..." << endl;
            return 0;
        }

        // Authenticate admin here (not implemented in this code)

        int adminChoice;
        system(CLEAR_SCREEN);
        do
        {

            system("Color B0");
            cout << "                 _               _             __  __               _        " << endl
                 << "     /\\         | |             (_)           |  \\/  |             | |       " << endl
                 << "    /  \\      __| |  _ __ ___    _   _ __     | \\  / |   ___     __| |   ___ " << endl
                 << "   / /\\ \\    / _` | | '_ ` _ \\  | | | '_ \\    | |\\/| |  / _ \\   / _` |  / _ \\" << endl
                 << "  / ____ \\  | (_| | | | | | | | | | | | | |   | |  | | | (_) | | (_| | |  __/" << endl
                 << " /_/    \\_\\  \\__,_| |_| |_| |_| |_| |_| |_|   |_|  |_|  \\___/   \\__,_|  \\___|" << endl
                 << "                                                                              " << endl
                 << "                                                                              " << endl;

            cout << "1. Create new survey\n"

                 << "2. View existing surveys\n"

                 << "3. Read user responses\n"

                 << "4. View Statistics\n"

                 << "5. Add Question\n"

                 << "6. Edit Question\n"

                 << "7. Delete Question\n"

                 << "8. View Feedback Rating\n"

                 << "9. Exit\n"

                 << "Enter your choice: ";
            cin >> adminChoice;
            cin.ignore();
            // Ignore newline character left in buffer

            switch (adminChoice)
            {
            case 1:
            {
                system(CLEAR_SCREEN);
                createSurvey();
                break;
            }
            case 2:
            {
                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();

                string surveyChoice;

                cout << "Enter the survey title you want to view: ";
                getline(cin, surveyChoice);

                string surveyFileName = surveyChoice + ".txt";
                ifstream surveyFile(surveyFileName);

                if (!surveyFile.is_open())
                {
                    cout << "Unable to open survey file: " << surveyFileName << endl;
                }
                vector<Question> questions;

                Question tempQuestion;

                while (surveyFile >> tempQuestion)
                {
                    questions.push_back(tempQuestion);
                }
                for (size_t i = 0; i < questions.size(); ++i)
                {

                    cout << "Q" << i + 1 << " " << questions[i].text << endl;
                    if (questions[i].type)
                    {

                        int j = 0;

                        for (const auto &option : questions[i].options)
                        {
                            cout << " " << char('A' + j) << ". " << option << endl;
                        }
                    }
                }
                break;
            }

            case 3:
            {
                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();
                cout << "Enter the survey title to read user responses: ";

                string surveyTitle;

                getline(cin, surveyTitle);

                readUserResponses(surveyTitle + ".txt");
                break;
            }
            case 4:
            {
                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;

                displaySurveys();

                string surveyChoice;

                cout << "Enter the survey title you want to view Statistics for: ";

                getline(cin, surveyChoice);
                string surveyFileName = surveyChoice + ".txt";
                showStatistics(surveyFileName);
                adminChoice = 4;

                break;
            }
            case 5:
            {
                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;

                displaySurveys();

                cout << "Enter the survey title to add question: ";

                string surveyTitle;
                getline(cin, surveyTitle);

                addQuestionToSurvey(surveyTitle + ".txt");
                break;
            }
            case 6:
            {
                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();

                cout << "Enter the survey title to edit question: ";
                string surveyTitle;

                getline(cin, surveyTitle);

                editQuestionInSurvey(surveyTitle + ".txt");
                break;
            }
            case 7:
            {
                system(CLEAR_SCREEN);

                cout << "Available Surveys:" << endl;
                displaySurveys();

                cout << "Enter the survey title to delete question: ";
                string surveyTitle;

                getline(cin, surveyTitle);

                deleteQuestionFromSurvey(surveyTitle + ".txt");
                break;
            }

            case 8:
            {

                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();
                string surveyChoice;
                cout << "Enter the survey title you want to view the average rating for: ";
                getline(cin, surveyChoice);
                double avgRating = calculateAverageRating(surveyChoice + ".txt");
                if (avgRating != 0.0)
                {
                    displayRatingVisual(avgRating);
                }
                else
                {
                    cout << "No rating" << endl;
                }
                break;
            }

            case 9:
            {

                system(CLEAR_SCREEN);
                cout << "Exiting...";

                exit(0);
            }
            default:
            {

                system(CLEAR_SCREEN);
                cout << "Invalid choice. Please try again." << endl;
            }
            }
        } while (adminChoice != 9);
    }
    else if (userType == 2)
    { // Client

        int loginChoice;
        cout << "1. Login\n";
        cout << "2. Signup\n";
        cout << "Enter your choice: ";
        cin >> loginChoice;

        switch (loginChoice)
        {
        case 1:
            if (!clientLogin())
            {
                cout << "Exiting..." << endl;
                return 0;
            }
            break;
        case 2:
            clientSignup();
            // After signup, prompt the user to login
            if (!clientLogin())
            {
                cout << "Exiting..." << endl;
                return 0;
            }
            break;
        default:
            cout << "Invalid choice. Exiting..." << endl;
            return 0;
        }

        int clientChoice;
        system(CLEAR_SCREEN);
        do
        {

            system("Color E5");
            cout << "   _____   _   _                  _       __  __               _        " << endl
                 << "  / ____| | | (_)                | |     |  \\/  |             | |       " << endl
                 << " | |      | |  _    ___   _ __   | |_    | \\  / |   ___     __| |   ___ " << endl
                 << " | |      | | | |  / _ \\ | '_ \\  | __|   | |\\/| |  / _ \\   / _` |  / _ \\" << endl
                 << " | |____  | | | | |  __/ | | | | | |_    | |  | | | (_) | | (_| | |  __/" << endl
                 << "  \\_____| |_| |_|  \\___| |_| |_|  \\__|   |_|  |_|  \\___/   \\__,_|  \\___|" << endl
                 << "                                                                       " << endl
                 << "                                                                       " << endl;

            cout << "1. View surveys" << endl;
            cout << "2. Fill out a survey" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> clientChoice;
            cin.ignore(); // Ignore newline character left in buffer

            switch (clientChoice)
            {
            case 1:
            {

                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();
                string surveyChoice;
                cout << "Enter the survey title you want to view: ";
                getline(cin, surveyChoice);
                string surveyFileName = surveyChoice + ".txt";
                ifstream surveyFile(surveyFileName);
                if (!surveyFile.is_open())
                {
                    cout << "Unable to open survey file: " << surveyFileName << endl;
                }
                vector<Question> questions;
                Question tempQuestion;
                while (surveyFile >> tempQuestion)
                {
                    questions.push_back(tempQuestion);
                }
                for (size_t i = 0; i < questions.size(); ++i)
                {

                    cout << "Q" << i + 1 << " " << questions[i].text << endl;
                    if (questions[i].type)
                    {
                        int j = 0;
                        for (const auto &option : questions[i].options)
                        {
                            cout << " " << char('A' + j) << ". " << option << endl;
                            j++;
                        }
                    }
                }
                break;
            }
            case 2:
            {

                system(CLEAR_SCREEN);
                cout << "Available Surveys:" << endl;
                displaySurveys();
                string surveyChoice;
                cout << "Enter the survey title you want to fill: ";
                getline(cin, surveyChoice);
                string surveyFileName = surveyChoice + ".txt";
                fillSurvey(surveyFileName);

                // feedback

                double rating = getRatingInput();
                submitFeedback(surveyFileName, rating);

                break;
            }
            case 3:
            {
                system(CLEAR_SCREEN);
                cout << "Exiting...";
                break;
            }
            default:
            {
                system(CLEAR_SCREEN);
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
            }
        } while (clientChoice != 3);
    }
    else
    {
        system(CLEAR_SCREEN);
        cout << "Invalid user type. Exiting..." << endl;
    }

    return 0;
}
