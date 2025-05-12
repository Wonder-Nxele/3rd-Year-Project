#include "QuestionBank.h"

//NEW
void QuestionBank::addQuestion(
    const std::string& id,
    const std::string& title,
    const std::string& narrative,
    const std::string& questionText,
    const std::vector<std::string>& options,
    int correctIndex,
    int level,
    const std::string& hint,
    const std::vector<std::string>& nextIDs
) {
    Question q(id, questionText, level, options, correctIndex, hint, title, narrative, nextIDs);
    questionMap[id] = q;

}

//Constructor for initialising the questions 
QuestionBank::Question::Question(string question, int question_level, vector<string> options, int correct_index, string hint)
{
    this->question = question;
    this->options = options;
    this->correct_index = correct_index;
    this->hint = hint;
    this->question_level = question_level;
}

//NEW
QuestionBank::Question::Question(
    std::string id,
    std::string question,
    int question_level,
    std::vector<std::string> options,
    int correct_index,
    std::string hint,
    std::string title,
    std::string narrative,
    std::vector<std::string> nextIDs
) : id(id), question(question), question_level(question_level), options(options),
correct_index(correct_index), hint(hint), title(title), narrative(narrative), nextQuestionIDs(nextIDs) {
}

QuestionBank::Question QuestionBank::getQuestionByID(const std::string& id) const {
    auto it = questionMap.find(id);
    if (it != questionMap.end()) {
        return it->second;
    }
    std::cerr << "Question with ID " << id << " not found!" << std::endl;
    return Question("Invalid", 0, {}, -1, ""); // Return an invalid question if not found
}

//UPDATED
// Check if there are questions in the queue
bool QuestionBank::hasQuestions(const std::string& currentID) const {
    if (currentID == "END") {
        return false; // Reached the end of the question path
    }

    auto it = questionMap.find(currentID);
    return it != questionMap.end(); // Question exists and isn't END
}

//Clear the question bank
void QuestionBank::clear() {
    questionMap.clear();
}
//END NEW

// Get the question text
string QuestionBank::Question::getQuestionText() const {
    return question;
}

// Get the options for the question
vector<string> QuestionBank::Question::getOptions() const {
    return options;
}

string QuestionBank::Question::get_correct_answer() const
{
    return options[correct_index];
}

string QuestionBank::Question::get_hint(Player& p) const {
    if (p.hints > 0) {
        p.hints--;
        return hint;
    }
    return "No hints left!";
}
