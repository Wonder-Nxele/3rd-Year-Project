#include "Level.h"
#include "QuestionBank.h"
#include "Button.h"
#include <iostream>
#include <vector>
#include "ProgressBar.h"

// have player instance
Player p;
// take users name
string userInput;

sf::RectangleShape progressBar;
extern bool showProgressBar;
bool showProgressBar = false;

bool showingNarrative = false;
std::string currentNarrative;
std::string currentHint;
int currentLine = 0;
const int maxVisibleLines = 5;


std::vector<Level::QuestionOutcome> Level::outcomes;
const float PROGRESS_BAR_WIDTH_PIXELS = 100.f;
const float PROGRESS_BAR_HEIGHT_PIXELS = 10.f;

sf::RectangleShape backgroundBox;
int limit = 20;
sf::Text inputText;

//Everything that needs to implemented for LEVEL class********************************************************************************************
void Level::render()          //Level render
{


	window->clear();

	//Draw intro page for specified level
	if (p.getLevel() == 1 || p.getLevel() == 2 || p.getLevel() == 3) {
		window->draw(LevelSprite);
		window->draw(message(level, font, 550, 50, 80));
		start.drawTo(*window);
	}
	switch (p.getLevel()) {
	case 1:
		window->draw(message(intro1, font, 200, 380, 20));
		break;
	case 2:
		window->draw(message(intro2, font, 200, 380, 20));
		break;
	case 3:
		window->draw(message(intro3, font, 200, 380, 20));
		break;
	}

	// *********************************************************************
	//Draw the background

	window->draw(backgroundSprite);
	window->draw(textboxSprite);
	skullSprite.setTexture(skullTexture);

	//Set color, font and position for text.
	questionText.setFillColor(sf::Color::Red);
	questionText.setFont(font);
	//questionText.setPosition(210, 10);


	//Set the color, font, position of the options and display them
	std::vector<sf::Text*> optionTexts = { &option1Text, &option2Text, &option3Text, &option4Text };
	int optionPos = 70;


	count = currentOptions.size();     //Checking how many options are being displayed for a questions
	// Only set options that exist
	for (size_t i = 0; i < currentOptions.size() && i < optionTexts.size(); ++i) {
		optionTexts[i]->setFillColor(sf::Color::Yellow);
		optionTexts[i]->setFont(font);
		if (currentOptions.size() <= 2) {
			optionTexts[i]->setPosition(210, optionPos);
			optionPos += 30;
		}
		else {
			optionTexts[i]->setPosition(210, optionPos);
		}
		optionPos += 30;

	}

	if (!hintTexture.loadFromFile("Hint.jpg")) {
		std::cerr << "Failed to load image!" << std::endl;
	}
	hintSprite.setTexture(hintTexture);
	hintSprite.setPosition(1240, 600);


	//Set the positions of the text
	/*narrativeText.setPosition(210, 10);*/
	sf::FloatRect s = narrativeStoryText.getLocalBounds();
	questionText.setPosition(210, 150);
	float startY = questionText.getPosition().y + questionText.getLocalBounds().height + 10;
	float optionSpacing = 30;

	option1Text.setPosition(210, startY);
	option2Text.setPosition(210, startY + optionSpacing);
	option3Text.setPosition(210, startY + 2 * optionSpacing);
	option4Text.setPosition(210, startY + 3 * optionSpacing);
	

	window->draw(timerText);

	//Drawing the buttons
	/*if (!loadFile(nextFont, "arial.ttf")) {
		std::cerr << "Failed to load font!" << std::endl;
	}*/
	FileLoader<Font>::loadFile(nextFont, "arial.ttf");

	//Setting the positions for the buttons
	nextButton.setPosition(sf::Vector2f(1200, 675));   //setting the position of the next button


	//Drawing the buttons
	nextButton.drawTo(*window);
	if (needButton) {

		int a = 0;
		for (int i = 0; i < 2; i++) {
			skullSprite.setPosition(a, 0);
			window->draw(skullSprite);
			a += 1160;
		}

		

		if (count == 4) {

			buttonA->drawTo(*window);
			buttonB->drawTo(*window);
			buttonC->drawTo(*window);
			buttonD->drawTo(*window);
			window->draw(hintSprite);

			/*Setting the positions differently because they are not the same for multiple choice and true or false*/
			buttonA->setPosition(sf::Vector2f(20.f, 300.f));
			buttonB->setPosition(sf::Vector2f(20.f, 600.f));
			buttonC->setPosition(sf::Vector2f(1000.f, 300.f));
			buttonD->setPosition(sf::Vector2f(1000.f, 600.f));

		}
		else if (count == 2) {

			buttonA->drawTo(*window);
			buttonB->drawTo(*window);
			buttonA->setPosition(sf::Vector2f(100.f, 420.f));
			buttonB->setPosition(sf::Vector2f(1000.f, 420.f));
			window->draw(hintSprite);
		}

		//drawing progress
		progressBar.setProgress(progressBar.progressPercent);
		progressBar.setFillColor(sf::Color::Green);

		// Create percentage text
		sf::Text percentText;
		percentText.setFont(font); // Use your loaded font
		percentText.setCharacterSize(13);
		percentText.setFillColor(sf::Color::White);

		// Convert progressPercent (e.g., 0.3f) to percentage string (e.g., "30%")
		/*Using static_cast<int> is a safe and clear way to convert a float or double to an int.
		  static_cast<int>(...) converts the float into an integer
		*/
		int percentValue = static_cast<int>(progressBar.progressPercent * 100); 
		percentText.setString(std::to_string(percentValue) + "%");

		// Position it near or on the progress bar
		percentText.setPosition(10.f + PROGRESS_BAR_WIDTH_PIXELS / 2.f - 10.f, 10.f - 5.f);

		//Drawimg the progress bar with it's percentage
		progressBar.draw(*window);
		window->draw(percentText);




	}

	//Drawing the scrolling functionalities
	window->setView(scrollView);   // Apply the scroll view
	window->draw(narrativeText);      //Narrative
	window->draw(questionText);        // Draw the scrollable text
	window->draw(option1Text);
	window->draw(option2Text);
	window->draw(option3Text);
	window->draw(option4Text);
	window->setView(window->getDefaultView());  // Reset view to normal for buttons, etc.
	hint.drawTo(*window);

	window->display();                   //display the window
}

Level::Level(): progressBar({ 10,10 }, { PROGRESS_BAR_WIDTH_PIXELS,PROGRESS_BAR_HEIGHT_PIXELS }) {    //Initiallizing our progressBar because it doesn't have a default constructor

	//Attributes of the progress bar
	currentLevelIndex = 0;
	progressBar.setProgress(0.f);
	progressBar.setFillColor(sf::Color::Green);

	scrollView.setSize(1280, 300);           // The world portion visible at a time
	scrollView.setCenter(640, 150);          // Start centered at top portion
	scrollView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 250.f / 960.f)); // Clip to top 300px of 960px window

	buttonA = new Button("A", sf::Vector2f(100.f, 60.f), 35, font, sf::Color::Transparent, sf::Color::Yellow);
	buttonB = new Button("B", sf::Vector2f(100.f, 60.f), 35, font, sf::Color::Transparent, sf::Color::Yellow);
	buttonC = new Button("C", sf::Vector2f(100.f, 60.f), 35, font, sf::Color::Transparent, sf::Color::Yellow);
	buttonD = new Button("D", sf::Vector2f(100.f, 60.f), 35, font, sf::Color::Transparent, sf::Color::Yellow);
	buttonHint = new Button("Hint", sf::Vector2f(100.f, 60.f), 20, font, sf::Color::Red, sf::Color::Yellow);
	buttonHint->setPosition({ 500,300 });

	//loadFile(skullTexture, "virus_image.png");
}
Level::~Level()
{
	delete buttonD;
	delete buttonC;
	delete buttonB;
	delete buttonA;
	delete buttonHint;
}

//method for adding the sound effect
void Level::gameSound(const string& gameSound) {

	static sf::Music music;
	FileLoader<sf::Music> musicLoader;

	if (!musicLoader.load(music, gameSound)) {
		std::cerr << "Music loading failed.\n";
	}

	music.setLoop(true); // Loop the music
	music.setVolume(50.0f); // Set to 15% volume
	music.play();

	/*if (!music.openFromFile(gameSound)) {
		std::cout << "Failed to load sound!" << std::endl;
		return;
	}*/

	
}

//Implementing all the level functions
void Level::pollEvents() {
	
	while (window->pollEvent(event))
	{

		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)//Closes window if esc pressed
				window->close();
			break;
		}

	}

}

void Level::skipAnimation()
{
	if (isTyping) {
		// Immediately complete the text.
		displayedQuestionText = fullQuestionText;
		if (displayStory)
			narrativeStoryText.setString(fullQuestionText);
		else
			questionText.setString(fullQuestionText);

		// Stop the animation and sound.
		isTyping = false;
		typingSound.stop();
		optionsDisplayed = true;
	}
};

//Load questions from a file
void Level::loadQuestionsFromFile(const std::string& filename, int level) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Could not open question file: " << filename << std::endl;
		return;
	}

	auto trimCR = [&](std::string& s) {
		if (!s.empty() && s.back() == '\r') s.pop_back();
		};
	auto trimWS = [&](std::string& s) {
		// Trim leading whitespace
		while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
		// Trim trailing whitespace
		while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
		};

	std::string line;
	std::string id, title, narrative, hint, questionText, correctAnswer;
	std::vector<std::string> options;
	std::vector<std::string> nextIDs;

	while (std::getline(file, line)) {
		trimCR(line);

		if (line == "BEGIN_QUESTION") {
			id.clear(); title.clear(); narrative.clear();
			hint.clear(); questionText.clear(); correctAnswer.clear();
			options.clear(); nextIDs.clear();
		}
		else if (line.rfind("ID|", 0) == 0) {
			id = line.substr(3);
			trimWS(id);
		}
		else if (line.rfind("Question_title|", 0) == 0) {
			title = line.substr(15);
			trimWS(title);
		}
		else if (line.rfind("Narrative|", 0) == 0) {
			narrative = line.substr(10);
			trimWS(narrative);
		}
		else if (line.rfind("Hint|", 0) == 0) {
			hint = line.substr(5);
			trimWS(hint);
		}
		else if (line.rfind("Question|", 0) == 0) {
			questionText = line.substr(9);
			trimWS(questionText);
		}
		else if (line.rfind("Correct_answer|", 0) == 0) {
			correctAnswer = line.substr(15);
			trimWS(correctAnswer);
		}
		else if (line.size() > 2 && (line[0] == 'A' || line[0] == 'B' || line[0] == 'C' || line[0] == 'D') && line[1] == '|') {
			// Parse option label, text, and nextID
			char label = line[0];
			size_t firstPipe = 1; // position of '|'
			size_t secondPipe = line.find('|', firstPipe + 1);
			std::string optionText = line.substr(firstPipe + 1, secondPipe - firstPipe - 1);
			std::string nextID = line.substr(secondPipe + 1);

			trimWS(optionText);
			trimWS(nextID);

			options.emplace_back(std::string(1, label) + ") " + optionText);
			nextIDs.push_back(nextID);
		}
		else if (line == "END_QUESTION") {
			// Determine correctIndex by label rather than full-string match
			int correctIndex = -1;
			if (!correctAnswer.empty()) {
				char label = correctAnswer[0];            // 'A', 'B', 'C', or 'D'
				if (label >= 'A' && label <= 'D')
					correctIndex = label - 'A';           // 'A'→0, 'B'→1, etc.
			}

			if (correctIndex == -1) {
				std::cerr << "Warning: Couldn't determine correct answer index for question "
					<< id << std::endl;
			}

			// Add to the QuestionBank
			questionBank.addQuestion(
				id,
				title,
				narrative,
				questionText,
				options,
				correctIndex,
				level,
				hint,
				nextIDs
			);
		}
	}

	file.close();

	// Start at root
	currentQuestionID = "Q1";
	showCurrentQuestion();
}

//Function to show the current question
void Level::showCurrentQuestion() {

	auto q = questionBank.getQuestionByID(currentQuestionID);
	currentNarrative = q.getNarrative();
	currentHint = q.getHint();
	hint = MessageBox(currentHint, font, { 600,300 });
	hint.setPosition({ 300,300 });


	std::string visibleText;
	int panelHeight = static_cast<int>(window->getSize().y * 0.30f);
	int lineHeight = 24;
	int maxLines = panelHeight / lineHeight - 1;  // leave one line for padding

	int maxLineLength = 60;  // max chars per line

	std::vector<std::string> wrappedLines;
	const std::string& rawText = currentNarrative;

	for (size_t i = 0; i < rawText.length(); i += maxLineLength) {
		wrappedLines.push_back(rawText.substr(i, maxLineLength));
	}

	// get the visible chunk to display based on scroll
	for (int i = 0; i < maxLines; ++i) {
		if (currentLine + i < wrappedLines.size()) {
			visibleText += wrappedLines[currentLine + i] + "\n";
		}
	}

	narrativeText.setString(visibleText);
	narrativeText.setPosition(210, 10);
	
	cout << "Correct Index: " << q.getCorrectIndex() << endl;
	if (q.getCorrectIndex() >= 0) {
		QuestionOutcome ot = getQuestionOutcome(
			q.getQuestionText(),
			clickedCharAsString,
			questionBank.getCorrectAnswer(q.get_correct_answer()));

		Level::outcomes.push_back(ot);	
		
	}
	animateText(q.getQuestionText(), q.getOptions()); //Display questions and options

}

//Function to handle the next question based on the selected option
void Level::nextQuestionFromChoice(int selectedIndex) {
	auto current = questionBank.getQuestionByID(currentQuestionID);
	currentNarrative = current.getNarrative();
	currentHint = current.getHint();
	std::string visibleText;
	int panelHeight = static_cast<int>(window->getSize().y * 0.30f);
	int lineHeight = 24;
	int maxLines = panelHeight / lineHeight - 1;  // leave one line for padding
	int maxLineLength = 60;  // max chars per line
	std::vector<std::string> wrappedLines;
	const std::string& rawText = currentNarrative;

	for (size_t i = 0; i < rawText.length(); i += maxLineLength) {
		wrappedLines.push_back(rawText.substr(i, maxLineLength));
	}

	// get the visible chunk to display based on scroll
	for (int i = 0; i < maxLines; ++i) {
		if (currentLine + i < wrappedLines.size()) {
			visibleText += wrappedLines[currentLine + i] + "\n";
		}
	}

	narrativeText.setString(visibleText);
	narrativeText.setPosition(210, 10);
	//narrativeText.setString(current.getNarrative());


	if (selectedIndex >= 0 && selectedIndex < current.getNextQuestionIDs().size()) {
		currentQuestionID = current.getNextQuestionIDs()[selectedIndex];
		showCurrentQuestion();
	}
	else {
		std::cerr << "Invalid selection index or missing path!" << std::endl;
	}
}

void Level::setFont()
{
	::loadFile(font, "arial.ttf");
	narrativeText.setFont(font);
}

//END NEW
// Displays narrative messages
sf::Text Level::message(const std::string& my_message, const sf::Font& font, const double& x, const double& y)
{
	sf::Text message;
	message.setFont(font);
	message.setString(my_message);
	message.setFillColor(sf::Color::White);
	message.setCharacterSize(20);
	message.setPosition(x, y);
	return message;
}
sf::Text Level::message(const std::string& my_message) {
	sf::Text message;
	message.setFont(font);
	message.setString(my_message);
	message.setFillColor(sf::Color::Green);
	message.setCharacterSize(20);
	message.setPosition(290, 30);
	return message;
}
sf::Text Level::message(const std::string& my_message, const sf::Font& font, const double& x, const double& y, const int size)
{
	sf::Text message;
	message.setFont(font);
	message.setString(my_message);
	message.setFillColor(sf::Color::Red);
	message.setCharacterSize(size);
	message.setPosition(x, y);
	return message;
}

inline void Level::clearOptions() {
	std::vector<sf::Text*> optionTexts = { &option1Text, &option2Text, &option3Text, &option4Text };

	// Clear all option texts first
	for (sf::Text* textPtr : optionTexts) {
		textPtr->setString("");
	}
}

//Sets up for text animation.Animation is actually done in the update method 
void Level::animateText(const std::string& text, const std::vector<std::string>& options) {
	fullQuestionText = text;  // Store the full question
	displayedQuestionText = ""; // Start with an empty string
	currentOptions = options;  // Store options
	isTyping = true;  // Start animation
	optionsDisplayed = false;  // Hide options until the question finishes

	typingClock.restart();  // Reset the timer

	// Load and play the typing sound
	if (typingSoundBuffer.loadFromFile("typing_sound.wav")) {
		typingSound.setBuffer(typingSoundBuffer);
		typingSound.setLoop(true); // Keep playing while typing
		typingSound.play();
	}
	else {
		std::cerr << "Failed to load typing sound!" << std::endl;
	}
}

//Process user input
void Level::processEvents() {

	sf::Event event;

	while (window->pollEvent(event)) {
		handleEvent(event);//Calls the handle event method depending on which level is created
		//rectangle.setSize(rectangleSize);

		buttonA->handleClick(*window, event);
		buttonB->handleClick(*window, event);
		buttonC->handleClick(*window, event);
		buttonD->handleClick(*window, event);
		//Handles what happens when the hint button is clicked
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
				sf::FloatRect bounds = hintSprite.getGlobalBounds();

				if (bounds.contains(mousePos)) {
					std::cout << "Hint button clicked!" << std::endl;
					hint.open();  // your hint logic
				}
			}
		}



		if (buttonA->isClicked())
		{
			
			clickedCharAsString = clickedButtonChar(*buttonA);
			selectedIndex = 0;
			buttonA->unclick();
		}
		if (buttonB->isClicked())
		{
			clickedCharAsString = clickedButtonChar(*buttonB);
			selectedIndex = 1;
			buttonB->unclick();
		}
		if (buttonC->isClicked())
		{
			clickedCharAsString = clickedButtonChar(*buttonC);
			selectedIndex = 2;
			buttonC->unclick();
		}
		if (buttonD->isClicked())
		{
			clickedCharAsString = clickedButtonChar(*buttonD);
			selectedIndex = 3;
			buttonD->unclick();
		}

		if (buttonA->isButtonClicked(*window)) {
		
			moveToNextQuestion = true; //Now the player can move to the next question
			
		}                     //Cjecking if the question has been answered or not
		else if (buttonB->isButtonClicked(*window)) {

			moveToNextQuestion = true; //Now the player can move to the next question

		}
		else if (buttonC->isButtonClicked(*window)) {

			moveToNextQuestion = true; //Now the player can move to the next question

		}
		else if (buttonD->isButtonClicked(*window)) {

			moveToNextQuestion = true; //Now the player can move to the next question
		}

		if (event.type == sf::Event::Closed)
			window->close();

		hint.handleEvent(*window, event);//Passed the event so that the button inside the MessageBox will work

		 //Handle the event of moving to the next question         //Only allowed to see the next question when they have selected an option		
		if (moveToNextQuestion == true &&  nextButton.isButtonClicked(*window) || event.key.code == sf::Keyboard::Enter ) {     //isMouseOver() will only work if the window is minimised

			//Checking the results of the previous question to see how you answered it and give you the immediate feedback
			QuestionOutcome out = outcomes.back();

			if (out.correct_answer == clickedCharAsString) { corr = true; }
			else { corr = false; }

			moveToNextQuestion = false; //So that we can also check if the answer has been selected for the following question
			count = 0;  //Making sure we start from 0 to check the number of options displayed for every question

			scrollView.setSize(1280, 300);           // The world portion visible at a time
			scrollView.setCenter(640, 150);          // Start centered at top portion
			scrollView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 250.f / 960.f)); // Clip to top 300px of 960px window


			if (isTyping) {
				skipAnimation();
			}
			else if (questionBank.hasQuestions(currentQuestionID)) {
				displayStory = false;
				nextQuestionFromChoice(selectedIndex);
				if (!questionBank.hasQuestions(currentQuestionID)) {
					finished = true;
				}

				std::cout << "Current Question ID: " << currentQuestionID << std::endl;

			}

			// Check if timer is running 
			if (!isTimerRunning) {
				++p;
				resetTimer();
			}

		}

		// Handle the event for scrolling with buttons
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			scrollView.move(0, 2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			scrollView.move(0, -2);
		}
	}
}

//Update the game state
void Level::update()
{

	updateTimer();
	updateTexture();

	// Check if typing animation is ongoing
	if (isTyping) {
		// Clear options while typing
		clearOptions();

		// Control typing speed, add one character at a time
		if (typingClock.getElapsedTime().asSeconds() > 0.08) {

			if (displayedQuestionText.length() < fullQuestionText.length()) {

				displayedQuestionText += fullQuestionText[displayedQuestionText.length()]; // Add next character
				if (displayStory == true) {
					narrativeStoryText.setString(displayedQuestionText);
				}
				else {
					questionText.setString(displayedQuestionText);
				}
				// Update displayed text

				typingClock.restart();  // Reset clock for next character

			}

			else {
				isTyping = false; // Typing animation finished
				typingSound.stop(); // Stop the typing sound
				optionsDisplayed = true; // Show options after typing is complete
			}
		}
	}
	// Display options only after question finishes typing
	if (optionsDisplayed && !currentOptions.empty()) {

		std::vector<sf::Text*> optionTexts = { &option1Text, &option2Text, &option3Text, &option4Text };
		// Only set options that exist
		for (size_t i = 0; i < currentOptions.size() && i < optionTexts.size(); ++i) {
			optionTexts[i]->setString(currentOptions[i]);
		}
	}


}

void Level::setAnimation(bool& condition)
{
	displayStory = condition;

}

void Level::resetTimer() {
	levelTime = 5 * 60; // 8 minutes in seconds
	gameClock.restart();
	isTimerRunning = true;
}
//Used for the timer in each level
void Level::updateTimer() {
	if (isTimerRunning) {
		float elapsedTime = gameClock.getElapsedTime().asSeconds(); //Measures how much time has passed since the last frame.
		gameClock.restart();

		levelTime -= elapsedTime; //responsible for reducing the remaining time in the countdown timer.

		// Check if time has expired
		if (levelTime <= 0) {
			levelTime = 0;
			isTimerRunning = false;
			handleTimerExpired();
		}

		// Update timer display
		char timeBuffer[10];
		formatTime(levelTime, timeBuffer); // converts the seconds into the right MM:SS format
		timerText.setString(timeBuffer);

		// Change color when time is running low (less than 1 minute)
		if (levelTime < 60) {
			timerText.setFillColor(sf::Color(255, 0, 0)); // Bright red
			float pusleTime = pulseClock.getElapsedTime().asSeconds();
			float scaleFactor = 1.0f + 1.0f * std::sin(pusleTime * 5);
			timerText.setScale(scaleFactor, scaleFactor);
		}
		else {
			timerText.setFillColor(sf::Color(220, 220, 0)); // Yellow
			timerText.setScale(1.0f, 1.0f);

		}
	}
}

// Format seconds into MM:SS
void Level::formatTime(float seconds, char* buffer) {
	int minutes = static_cast<int>(seconds) / 60;
	int secs = static_cast<int>(seconds) % 60;
	sprintf_s(buffer, 10, "%02d:%02d", minutes, secs);
}

StoryPage::StoryPage(sf::RenderWindow* win)
{


	window = win;
	setSprites();
	start = Button("Continue", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	setFont();
	displayStory = true;
	std::string story = userInput +
		", It was supposed to be an ordinary night at the university computer lab.\n"
		"You, a skilled but unsuspecting IT student, stayed back to work on an important project.\n"
		"But just as you were about to leave, the lights flickered,\n"
		"the screens glowed red, and an eerie robotic voice echoed through the room.\n"
		"WARNING! SYSTEM BREACH DETECTED! VIRUS ACTIVATED!\n"
		"Before you could react, the lab doors locked, trapping you inside.\n"
		"Every screen displayed corrupted code, and a sinister AI named MalX introduced itself.\n"
		"It was an advanced cyber-virus designed to take control of the entire network.\n"
		"If it spreads beyond the lab, it could wipe out years of research\n"
		"and plunge the institution into digital chaos.\n"
		"Your only chance of escape? Find the antivirus hidden deep in the lab's systems\n"
		"before MalX evolves beyond containment.\n"
		"But it won't be easy. The virus has encrypted every terminal\n"
		"and corrupted the security protocols.\n"
		"To unlock each phase of the system,\n"
		"you must solve IT-related puzzles and programming challenges,\n"
		"each more difficult than the last.";
	/*String story2 = "Before you could react, the lab doors locked, trapping you inside. Every screen \ndisplayed corrupted code, and a sinister AI named MalX introduced itself, an advanced\ncyber-virus designed to take control of the entire university network. If it spreads \nbeyond the lab, it could wipe out years of research and plunge the institution into \ndigital chaos.\n\n";
	String story3 = "Your only chance of escape? Find the antivirus hidden deep within the labs systems \nbefore MalX evolves beyond containment. But it wont be easy. The virus has encrypted \nevery terminal and corrupted the security protocols. To unlock each phase of the \nsystem, you must solve IT-related puzzles and programming challenges, each more \ndifficult than the last.";*/
	animateText(story, { "", "", "", "" });
}

//NEW
bool StoryPage::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* StoryPage::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// now actually enter LevelTwo
	return new LevelOnePage(window);
}

void StoryPage::handleEvent(const sf::Event& event)
{
	

	bool TextAnimationSkipped = false;
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			if (isTyping) {
				skipAnimation();
			}
			else {
				finished = true;
				//nextLevelPage = true;
				std::cout << "You Clicked the button\n";
			}
		}
	}
}
void StoryPage::setSprites()

{
	narrativeStoryText.setFillColor(sf::Color::Red);

	narrativeStoryText.setFont(font);

	FileLoader<Font>::loadFile(introTexture, "IntroPage.png");
	introSprite.setTexture(introTexture);
	//animateText();
}
void StoryPage::render()
{
	window->clear();

	//Draw the background
	window->draw(introSprite);
	start.drawTo(*window);
	//Set color for text
	narrativeStoryText.setFillColor(sf::Color::Red);
	//Set the font for the text
	narrativeStoryText.setFont(font);
	//Display the message
	window->draw(narrativeStoryText);

	window->display();                    //display the window

}

//Everything that needs to implemented for Level Narratives*********************************************************
LevelOnePage::LevelOnePage(sf::RenderWindow* win)
{
	needButton = false;
	window = win;
	setSprites();
	setFont();
	start = Button("Start", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,570 });


}

//NEW
bool LevelOnePage::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelOnePage::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;

	return new LevelOne(window);
}

LevelTwoPage::LevelTwoPage(sf::RenderWindow* win)
{
	window = win;
	setSprites();
	setFont();
	start = Button("Start", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	//introLevelNum = 2;
}

//NEW
bool LevelTwoPage::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelTwoPage::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// now actually enter LevelTwo
	return new LevelTwo(window);
}

LevelThreePage::LevelThreePage(sf::RenderWindow* win)
{
	window = win;
	setSprites();
	setFont();
	start = Button("Start", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	//introLevelNum = 3;
}

//NEW
bool LevelThreePage::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelThreePage::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// now actually enter LevelThree
	return new LevelThree(window);
}

void LevelOnePage::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			
			
			
			finished = true;
			//nextLevelOnePage = true;
			std::cout << "You Clicked the button\n";
		}
	}
}
void LevelOnePage::setSprites()
{

	level = "LEVEL 1";
	intro1 = "The first layer of security is the firewall, now corrupted by MalX. \nTo restore it, you must solve basic IT trivia, logic puzzles, and simple coding riddles. \nAs you progress, MalX tries to slow you down with glitches and deceptive clues.";
	FileLoader<Texture>::loadFile(levelpageTexture, "LevelPage.png");
	LevelSprite.setTexture(levelpageTexture);


}
void LevelOnePage::render()
{
	gameSound("LastGladiator.wav");
	window->clear();
	window->draw(LevelSprite);
	window->draw(message(level, font, 550, 50, 80));
	window->draw(message(intro1, font, 200, 380, 20));
	start.drawTo(*window);
	window->display();

}

void LevelTwoPage::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			//nextLevelTwoPage = true;
			std::cout << "You Clicked the button\n";

		}
	}
};
void LevelTwoPage::setSprites()
{
	needButton = false;
	level = "LEVEL 2";
	intro2 = "With the firewall restored, you gain deeper access into the system, but \nMalX has fragmented the antivirus into encrypted pieces hidden \nacross multiple servers.You must navigate a virtual maze, solving intermediate \nprogramming problems, debugging errors, and cracking cryptographic \npuzzles to reassemble the antivirus.";
	FileLoader<Texture>::loadFile(levelpageTexture, "LevelPage.png");
	LevelSprite.setTexture(levelpageTexture);
}

void LevelThreePage::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			//nextLevelThreePage = true;
			std::cout << "You Clicked the button\n";

		}
	}
}
void LevelThreePage::setSprites()
{
	needButton = false;
	level = "LEVEL 3";
	intro3 = "The final piece of the antivirus is within reach, but MalX transforms into a \nself-learning AI, challenging you directly";
	FileLoader<Texture>::loadFile(levelpageTexture, "LevelPage.png");
	LevelSprite.setTexture(levelpageTexture);

}

//Everything that needs to implemented for Levels*********************************************************
LevelOne::LevelOne(sf::RenderWindow* win)
{
	progressBar.progressPercent = 0.10f;// whenever we enter level 1, we set the progress bar to 10% for completing the story page
	initialSkullColor = 0;  //To set the skull color to green when a level is to be started
	// increment player level
	//currentLevel = 1;
	++p;
	cout << "Player is at level one" << endl;
	needButton = true;
	//Creating the buttons
	nextButton = Button("Next", sf::Vector2f(150, 50), 24, nextFont, sf::Color::Red, sf::Color::Yellow);	window = win;
	setFont();
	setSprites();

	//Load the questions from a file
	loadQuestionsFromFile("questions1.txt", currentLevel);
}

//NEW
bool LevelOne::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelOne::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;

	if (!isTimerRunning)
		return new FeedBackPage1(window);

	// First go to any feed back page before Level Two:
	return new FeedBackPage1(window);
}
//END NEW

void LevelOne::handleEvent(const sf::Event& event)
{
}
void LevelOne::setSprites()
{
	//rectangle.setPosition(170, 5);
	//rectangle.setFillColor(sf::Color::Black);

	FileLoader<Font>::loadFile(font, "arial.ttf");

	//Load images
	FileLoader<Texture>::loadFile(backgroundTexture, "LevelOne.jpg");
	FileLoader<Texture>::loadFile(textboxTexture, "textbox2.png");
	
	//Scaling the top half of the background to fit 40%% of the screen
	sf::Vector2u windowSize = window->getSize();
	float targetHeight = windowSize.y * 0.30f;  // 40% of screen height
	textboxSprite.setTexture(textboxTexture);
	sf::Vector2u imageSize = textboxTexture.getSize();
	float scaleY = targetHeight / static_cast<float>(imageSize.y);
	float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	textboxSprite.setScale(scaleX, scaleY);
	textboxSprite.setPosition(0, 0);  // starts at 40% down


	//Scaling the bottom half of the background to fit 60% of the screen
	targetHeight = windowSize.y * 0.70f;  // 60% of screen height
	backgroundSprite.setTexture(backgroundTexture);
	imageSize = backgroundTexture.getSize();
	scaleY = targetHeight / static_cast<float>(imageSize.y);
	scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	backgroundSprite.setScale(scaleX, scaleY);
	backgroundSprite.setPosition(0.f, windowSize.y * 0.3f);  // starts at 40% down


	//Set the texture for the sprites
	textboxSprite.setTexture(textboxTexture);
	

	// Set up timer text
	timerText.setFont(font);
	timerText.setCharacterSize(30);
	timerText.setFillColor(sf::Color::Red);
	timerText.setPosition(1110, 160);

	resetTimer(); // Initialize the timer

	//Initiallise text message to be displayed at the start of the game
	clearOptions();
	animateText("Welcome To Level 1 Let The Game Begin!", { "", "", "", "" });

}

LevelTwo::LevelTwo(sf::RenderWindow* win)
{
	progressBar.progressPercent = 0.40f;// whenever we enter level 2, we set the progress bar to 40% of level 1
	initialSkullColor = 0;  //To set the skull color to green when a level is to be started
	
	// increment player level
	std::cout << "!!!!!!!!!!!!!!!! Level Two !!!!!!!!!!!!!!!!";
	++p;
	cout << "Player is at level two" << endl;
	needButton = true;
	//currentLevel = 2;
	//Creating the buttons
	nextButton = Button("Next", sf::Vector2f(150, 50), 24, nextFont, sf::Color::Red, sf::Color::White);

	window = win;
	setSprites();
	setFont();

	//Clear questions from previous level
	questionBank.clear();
	outcomes.clear();
	//Load the questions from a file
	loadQuestionsFromFile("questions2.txt", p.getLevel());
}

//NEW
bool LevelTwo::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelTwo::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;

	if (!isTimerRunning)
		return new FeedBackPage2(window);

	// First go to any feed back page before Level Three:
	return new FeedBackPage2(window);
}
//END NEW

void LevelTwo::handleEvent(const sf::Event& event)
{
}
void LevelTwo::setSprites()
{
	//rectangle.setPosition(170, 5);
	//rectangle.setFillColor(sf::Color::Black);

	FileLoader<Font>::loadFile(font, "arial.ttf");

	//Load images
	FileLoader<Texture>::loadFile(GameTexture, "virus_image3.png");
	FileLoader<Texture>::loadFile(backgroundTexture, "LevelTwo.jpg");
	FileLoader<Texture>::loadFile(textboxTexture, "textbox2.png");
	
	//Scaling the top half of the background to fit 40%% of the screen
	sf::Vector2u windowSize = window->getSize();
	float targetHeight = windowSize.y * 0.30f;  // 40% of screen height
	textboxSprite.setTexture(textboxTexture);
	sf::Vector2u imageSize = textboxTexture.getSize();
	float scaleY = targetHeight / static_cast<float>(imageSize.y);
	float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	textboxSprite.setScale(scaleX, scaleY);
	textboxSprite.setPosition(0, 0);


	//Scaling the bottom half of the background to fit 70% of the screen
	targetHeight = windowSize.y * 0.70f;  // 70% of screen height
	backgroundSprite.setTexture(backgroundTexture);
	imageSize = backgroundTexture.getSize();
	scaleY = targetHeight / static_cast<float>(imageSize.y);
	scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	backgroundSprite.setScale(scaleX, scaleY);
	backgroundSprite.setPosition(0.f, windowSize.y * 0.3f);  // starts at 30% down

	// Set up timer text
	timerText.setFont(font);
	timerText.setCharacterSize(30);
	timerText.setFillColor(sf::Color::Red);
	timerText.setPosition(1110, 160);

	resetTimer(); // Initialize the timer

	//Initiallise text message to be displayed at the start of the game
	clearOptions();
	animateText("Welcome To Level 2 Let The Game Begin!", { "", "", "", "" });

}

LevelThree::LevelThree(sf::RenderWindow* win)
{
	progressBar.progressPercent = 0.70f;// whenever we enter level 3, we set the progress bar to 73% of level 1 & 2
	initialSkullColor = 0;  //To set the skull color to green when a level is to be started
	// increment player level
	++p;
	cout << "Player is at level three" << endl;
	needButton = true;

	//Creating the buttons
	nextButton = Button("Next", sf::Vector2f(150, 50), 24, nextFont, sf::Color::Red, sf::Color::White);	window = win;
	setSprites();
	setFont();

	//Clear questions from previous level
	questionBank.clear();
	outcomes.clear();
	//Load the questions from a file
	loadQuestionsFromFile("questions3.txt", p.getLevel());
}

//NEW
bool LevelThree::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* LevelThree::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;

	if (isTimerRunning)
		return new FeedBackPage3(window);

	// First go to any feed back page for the last level:
	return new FeedBackPage3(window);
}
//END NEW

void LevelThree::handleEvent(const sf::Event& event)
{
}
void LevelThree::setSprites()
{
	FileLoader<Font>::loadFile(font, "arial.ttf");
	//Load images
	FileLoader<Texture>::loadFile(backgroundTexture, "level3_image.jpg");
	FileLoader<Texture>::loadFile(textboxTexture, "textbox2.png");
	
	//Scaling the top half of the background to fit 40%% of the screen
	sf::Vector2u windowSize = window->getSize();
	float targetHeight = windowSize.y * 0.40f;  // 40% of screen height
	textboxSprite.setTexture(textboxTexture);
	sf::Vector2u imageSize = textboxTexture.getSize();
	float scaleY = targetHeight / static_cast<float>(imageSize.y);
	float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	textboxSprite.setScale(scaleX, scaleY);
	textboxSprite.setPosition(0, 0);  // starts at 40% down


	//Scaling the bottom half of the background to fit 60% of the screen
	targetHeight = windowSize.y * 0.60f;  // 60% of screen height
	backgroundSprite.setTexture(backgroundTexture);
	imageSize = backgroundTexture.getSize();
	scaleY = targetHeight / static_cast<float>(imageSize.y);
	scaleX = static_cast<float>(windowSize.x) / static_cast<float>(imageSize.x); // optional full stretch
	backgroundSprite.setScale(scaleX, scaleY);
	backgroundSprite.setPosition(0.f, windowSize.y * 0.4f);  // starts at 40% down

	// Set up timer text
	timerText.setFont(font);
	timerText.setCharacterSize(30);
	timerText.setFillColor(sf::Color::Red);
	timerText.setPosition(1110, 160);

	resetTimer(); // Initialize the timer

	//Initiallise text message to be displayed at the start of the game
	clearOptions();
	animateText("Welcome To Level 3 Let The Game Begin!", { "", "", "", "" });

}

//Everything that needs to implemented for FeedBackPage*********************************************************
FeedBackPage1::FeedBackPage1(sf::RenderWindow* win)
{
	scrollView.setSize(1280, 300);           // The world portion visible at a time
	scrollView.setCenter(640, 150);          // Start centered at top portion
	scrollView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 700.f / 960.f)); // Clip to top 300px of 960px window

	window = win;
	start = Button("NextLevel", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	setSprites();
	setFont();
}

//NEW
bool FeedBackPage1::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* FeedBackPage1::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// First go to any feed back page before Level Two:
	else if (isTimerRunning)
		return new LevelTwoPage(window);

	return new LevelTwoPage(window);
}
//END NEW

void FeedBackPage1::render()
{
	if (p.getLevel() = 1) {
		window->clear();
		//Draw the home page
		window->draw(homeSprite);
		//for feedback1
		sf::Text feedbackText;
		feedbackText.setFont(font);  // Use the Level font
		feedbackText.setCharacterSize(18);
		feedbackText.setFillColor(sf::Color::Yellow);
		feedbackText.setPosition(250, 50); // Start position
		std::string allFeedback = "Feedback Summary !!! \n\n";// a variable that holds the overall feedback1 summary

		vector<QuestionOutcome>::iterator it = outcomes.begin();

		for (; it != outcomes.end(); ++it) {
			allFeedback += "Question: " + it->question + "\n";
			allFeedback += "Your Answer: " + it->user_answer + "\n";
			allFeedback += "Correct Answer: " + it->correct_answer + "\n";
			allFeedback += std::string("Result: ") + (it->isCorrect() ? "Correct" : "Wrong") + "\n\n";
		}

		feedbackText.setString(allFeedback);
		start.drawTo(*window);

		window->setView(scrollView);   // Apply the scroll view
		window->draw(feedbackText);
		window->setView(window->getDefaultView());  // Reset view to normal for buttons, etc.


		window->display();                         //display the window

		// Handle the event for scrolling with buttons
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			scrollView.move(0, 2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			scrollView.move(0, -2);
		}

	}
}
void FeedBackPage1::handleEvent(const sf::Event& event)
{

	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			nextLevelFeedBack1 = true;
			std::cout << "You are moving to the next LEVEL\n";
		}
	}
}
void FeedBackPage1::setSprites()
{
	FileLoader<Texture>::loadFile(homeTexture, "intropage.png");
	homeSprite.setTexture(homeTexture);
}
//for feedback page 2

FeedBackPage2::FeedBackPage2(sf::RenderWindow* win)
{
	scrollView.setSize(1280, 300);           // The world portion visible at a time
	scrollView.setCenter(640, 150);          // Start centered at top portion
	scrollView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 700.f / 960.f)); // Clip to top 300px of 960px window


	window = win;
	setSprites();
	start = Button("Continue", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	setFont();

}

//NEW
bool FeedBackPage2::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* FeedBackPage2::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// First go to any feed back page before Level Three:
	else if (isTimerRunning)
		return new LevelThreePage(window);

	return new LevelThreePage(window);
}
//END NEW

void FeedBackPage2::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			nextLevelFeedBack2 = true;
			std::cout << "You are moving to the next LEVEL\n";
		}
	}
}

void FeedBackPage2::render()
{
	if (p.getLevel() = 2) {
		window->clear();
		//Draw the home page
		window->draw(homeSprite);
		sf::Text feedbackText2;
		feedbackText2.setFont(font);  // Use the Level font
		feedbackText2.setCharacterSize(15);
		feedbackText2.setFillColor(sf::Color::Yellow);
		feedbackText2.setPosition(250, 50); // Start position
		std::string allFeedback2 = "Feedback Summary:\n\n";// a variable that holds the overall feedback1 summary

		vector<QuestionOutcome>::iterator it = outcomes.begin();

		for (; it != outcomes.end(); ++it) {
			allFeedback2 += "Question: " + it->question + "\n";
			allFeedback2 += "Your Answer: " + it->user_answer + "\n";
			allFeedback2 += "Correct Answer: " + it->correct_answer + "\n";
			allFeedback2 += std::string("Result: ") + (it->isCorrect() ? "Correct" : "Wrong") + "\n\n";
		}

		feedbackText2.setString(allFeedback2);
		start.drawTo(*window);
		window->setView(scrollView);   // Apply the scroll view
		window->draw(feedbackText2);
		window->setView(window->getDefaultView());  // Reset view to normal for buttons, etc.


		window->display();                         //display the window

		// Handle the event for scrolling with buttons
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			scrollView.move(0, 2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			scrollView.move(0, -2);
		}
	}//display the window
}

void FeedBackPage3::render()
{
	if (p.getLevel() = 3) {
		window->clear();
		//Draw the home page
		window->draw(homeSprite);
		sf::Text feedbackText3;
		feedbackText3.setFont(font);  // Use the Level font
		feedbackText3.setCharacterSize(15);
		feedbackText3.setFillColor(sf::Color::Yellow);
		feedbackText3.setPosition(250, 50); // Start position
		std::string allFeedback3 = "Feedback Summary:\n\n";// a variable that holds the overall feedback1 summary

		vector<QuestionOutcome>::iterator it = outcomes.begin();

		for (; it != outcomes.end(); ++it) {
			allFeedback3 += "Question: " + it->question + "\n";
			allFeedback3 += "Your Answer: " + it->user_answer + "\n";
			allFeedback3 += "Correct Answer: " + it->correct_answer + "\n";
			allFeedback3 += std::string("Result: ") + (it->isCorrect() ? "Correct" : "Wrong") + "\n\n";
		}
		feedbackText3.setString(allFeedback3);
		start.drawTo(*window);

		window->setView(scrollView);   // Apply the scroll view
		window->draw(feedbackText3);
		window->setView(window->getDefaultView());  // Reset view to normal for buttons, etc.


		window->display();                         //display the window

		// Handle the event for scrolling with buttons
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			scrollView.move(0, 2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			scrollView.move(0, -2);
		}
	}
}
void FeedBackPage2::setSprites()
{
	FileLoader<Texture>::loadFile(homeTexture, "intropage.png");
	homeSprite.setTexture(homeTexture);
}

FeedBackPage3::FeedBackPage3(sf::RenderWindow* win)
{
	scrollView.setSize(1280, 300);           // The world portion visible at a time
	scrollView.setCenter(640, 150);          // Start centered at top portion
	scrollView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 700.f / 960.f)); // Clip to top 300px of 960px window

	window = win;
	setSprites();
	start = Button("Continue", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });
	setFont();

}

//NEW
bool FeedBackPage3::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* FeedBackPage3::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// Go to the last page of the game but for now it brings you back to home page:

	return new FinalPage(window);
}
//END NEW

void FeedBackPage3::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			nextLevelFeedBack3 = true;
			finished = true;
			std::cout << "You are moving to the next LEVEL\n";
		}
	}
}



void FeedBackPage3::setSprites()
{
	FileLoader<Texture>::loadFile(homeTexture, "feeT.png");
	homeSprite.setTexture(homeTexture);
}

// Handle what happens when time runs out
void Level::handleTimerExpired() {
	// What happens when time runs out - implement game logic here
	finished = true;
	text = message("Time's up! Press Enter to continue to next level...");
}


//Everything that needs to implemented for HOME PAGE*********************************************************
Home::Home(sf::RenderWindow* win)
{
	window = win;

	// set sprties and font
	setSprites();
	setFont();

	// draw the textbox
	sf::Vector2f size(400.f, 30.f);
	backgroundBox.setSize(size);
	backgroundBox.setFillColor(sf::Color::White);
	backgroundBox.setOutlineColor(sf::Color::Red);
	backgroundBox.setOutlineThickness(1.5f);

	// Calculate centered position
	float centerX = (win->getSize().x - size.x) / 2.f;
	float centerY = (win->getSize().y - size.y) / 2.f;
	backgroundBox.setPosition(centerX, 200);
	//
	inputText.setFont(font);
	inputText.setPosition(centerX, 200);
	inputText.setCharacterSize(20);
	inputText.setFillColor(sf::Color::Black);
	inputText.setString("@username(<20 char)");
	//
	start = Button("Start", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 570,600 });

}
//NEW
bool Home::isFinished() const {
	//might add some other logic later
	return finished;
}

Level* Home::createNext(sf::RenderWindow* window) {
	if (!finished)
		return nullptr;
	// now actually enter Story Page
	return new StoryPage(window);
}


void Home::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red);
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			std::cout << "You are moving to the page that is supposed to have a narrative\n";
		}
	}
	else if (event.type == sf::Event::TextEntered) {
		if (event.text.unicode == '\b') { // Backspace
			if (!userInput.empty())
				userInput.pop_back();
		}
		else if (event.text.unicode < 128 && userInput.length() < limit) { // ASCII only
			userInput += static_cast<char>(event.text.unicode);
		}
		inputText.setString(userInput);
	}
	else if (event.type == sf::Event::Closed) {
		window->close();
	}
}
void Home::render()
{
	window->clear();
	gameSound("RegularShowIntro.wav");

	//Draw the home page
	window->draw(homeSprite);
	//std::cout << "Mouse position:" << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << "\n";
	start.drawTo(*window);
	window->draw(backgroundBox);   // draw backgroundBox
	window->draw(inputText);   // draw inputText
	window->display();                   //display the window

}
void Home::setSprites()
{
	FileLoader<Texture>::loadFile(homeTexture, "Skull1.png");
	homeSprite.setTexture(homeTexture);
}

//Everything that needs to implemented for StoryPage*********************************************************

FinalPage::FinalPage(sf::RenderWindow* win)
{
	window = win;
	start = Button("RESTART", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	exit = Button("EXIT", { 200,50 }, 20, font, sf::Color::Red, sf::Color::Black);
	start.setPosition({ 680,600 });
	exit.setPosition({ 240,600 });

	setSprites();
	setFont();

}

void FinalPage::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		if (start.isMouseOver(*window)) {
			start.setBackColor(sf::Color::White);
		}
		else {
			start.setBackColor(sf::Color::Red); // default back
		}
		if (exit.isMouseOver(*window)) {
			exit.setBackColor(sf::Color::White);
		}
		else {
			exit.setBackColor(sf::Color::Red); // default back
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (start.isMouseOver(*window)) {
			finished = true;
			//nextLevelHome = true;
			std::cout << "You are moving to the page that supposed to have narrative\n";
		}
		if (exit.isMouseOver(*window)) {
			finished = true;
			window->close();
			//nextLevelHome = true;
			std::cout << "You are moving to the page that supposed to have narrative\n";
		}

	}
}

void FinalPage::setSprites()
{
	FileLoader<Texture>::loadFile(homeTexture, "GameOver.png");
	homeSprite.setTexture(homeTexture);
}

void FinalPage::render()
{
	window->clear();
	gameSound("RegularShowIntro.wav");

	//Draw the home page
	window->draw(homeSprite);
	//std::cout << "Mouse position:" << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << "\n";
	start.drawTo(*window);
	exit.drawTo(*window);

	window->display();                   //display the window

}

bool FinalPage::isFinished() const
{
	return finished;
}

Level* FinalPage::createNext(sf::RenderWindow* window)
{
	if (!finished)
		return nullptr;
	//restarting the game by creating the newHomePage
	return new Home(window);
}
