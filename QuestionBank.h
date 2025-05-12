#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "Player.h"
using namespace std;
// foward declare the class Question
class Question;
class QuestionBank {  // Outer class
public:
    // Constructors
    //QuestionBank();

    //NEW
    void addQuestion(
        const std::string& id,
        const std::string& title,
        const std::string& narrative,
        const std::string& questionText,
        const std::vector<std::string>& options,
        int correctIndex,
        int level,
        const std::string& hint,
        const std::vector<std::string>& nextIDs
    );

private:
    class Question {  // Inner class
    public:
        Question() = default; // Default constructor
        Question(string question, int question_level, vector<string> options, int correct_index, string hint);

        //NEW
        Question(
            std::string id,
            std::string question,
            int question_level,
            std::vector<std::string> options,
            int correct_index,
            std::string hint,
            std::string title,
            std::string narrative,
            std::vector<std::string> nextIDs
        );
        //END_NEW

        string get_correct_answer() const;
        string get_hint(Player& player) const;
        string getQuestionText() const;
        vector<string> getOptions() const;

        //NEW
        std::string getId() const { return id; };
        std::vector<std::string> getNextQuestionIDs() const { return nextQuestionIDs; }
        std::string getTitle() const { return title; }
        std::string getNarrative() const { return narrative; }
        std::string getHint() const { return hint; }
        int getQuestionLevel() const { return question_level; }
        int getCorrectIndex() const { return correct_index; }
        //END_NEW

    private:
        string question, hint, title, narrative;
        vector<string> options;
        int correct_index;
        int question_level;

        //NEW
        std::vector<std::string> nextQuestionIDs;
        string id;
        //END_NEW
    };

public:
    void clear();
    bool hasQuestions(const std::string& currentID) const;
    std::string answer()
    {
        Question q;
        return q.get_correct_answer();
    }

    std::string getCorrectAnswer(std::string selected)
    {
        char character = ' ';
        switch (selected[0]) {
        case 'A':
            character = 'A';
            break;
        case 'B':
            character = 'B';
            break;
        case 'C':
            character = 'C';
            break;
        case 'D':
            character = 'D';
            break;
        }
        return std::string(1, character);
    }

    //NEW
    Question getQuestionByID(const std::string& id) const;
    std::unordered_map<std::string, Question> questionMap;
    //END_NEW
};
