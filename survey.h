#pragma once
#include <string>
#include <vector>
#include <fstream>

struct Question {
    std::string text;
    int type = 0;
    std::vector<std::string> options;
    friend std::ostream& operator<<(std::ostream& os, const Question& q);
    friend std::istream& operator>>(std::istream& is, Question& q);
};

class Survey {
public:
    static void addSurveyTitle(const std::string& surveyTitle, const std::string& surveyFileName);
    static void createSurvey();
    static void displaySurveys();
    static void fillSurvey(const std::string& surveyFileName);
    static void readUserResponses(const std::string& surveyFileName);
    static void addQuestionToSurvey(const std::string& surveyFileName);
    static void deleteQuestionFromSurvey(const std::string& surveyFileName);
    static void editQuestionInSurvey(const std::string& surveyFileName);
    static void showStatistics(const std::string& surveyFileName);
    static void submitFeedback(const std::string& surveyTitle, double rating);
    static double calculateAverageRating(const std::string& surveyTitle);
    static char validateMCQInput();
    static double getRatingInput();
};
