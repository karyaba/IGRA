#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

enum class GameState { MENU, DIFFICULTY, GAME, END, EXIT};

class Game {
public:
    Game() {
        currentState = GameState::MENU;
        matches = 100;

        BackgroundMusic.openFromFile("Resources/Elton.wav");
        BackgroundMusic.setLoop(true);
        BackgroundMusic.play();

        menuBackgroundTexture.loadFromFile("Resources/Pirat.png");
        StartTexture.loadFromFile("Resources/1.png");
        ExitTexture.loadFromFile("Resources/2.png");
        EasyTexture.loadFromFile("Resources/Easy.png");
        HardTexture.loadFromFile("Resources/Hard.png");
        BackToMainMenuTexture.loadFromFile("Resources/Back.png");
        RestartTexture.loadFromFile("Resources/Restart.png");

        font.loadFromFile("Resources/Bahnschrift.ttf");

        menuBg.setTexture(menuBackgroundTexture);
        Start.setTexture(StartTexture);
        Exit.setTexture(ExitTexture);
        Easy.setTexture(EasyTexture);
        Hard.setTexture(HardTexture);
        BackToMainMenu.setTexture(BackToMainMenuTexture);
        Restart.setTexture(RestartTexture);

        Start.setPosition(825, 400);
        Exit.setPosition(810, 475); 
        Easy.setPosition(825, 400);
        Hard.setPosition(815, 475);
        BackToMainMenu.setPosition(675, 535);
        Restart.setPosition(775, 475);

        textPlayerMoveInfo.setFont(font);
        textPlayerMoveInfo.setCharacterSize(35);
        textPlayerMoveInfo.setFillColor(Color::Red);
        textPlayerMoveInfo.setPosition(800, 250);

        textRobotMoveInfo.setFont(font);
        textRobotMoveInfo.setCharacterSize(35);
        textRobotMoveInfo.setFillColor(Color::Red);
        textRobotMoveInfo.setPosition(925, 250);

        textMatches.setFont(font);
        textMatches.setPosition(875, 215);
        textMatches.setCharacterSize(35);
        textMatches.setFillColor(Color::Black);

        textMoveInfo.setFont(font);
        textMoveInfo.setCharacterSize(35);
        textMoveInfo.setFillColor(Color::Yellow);
        textMoveInfo.setPosition(870, 280);

        textErr.setFont(font);
        textErr.setCharacterSize(35);
        textErr.setFillColor(Color::White);
        textErr.setPosition(700, 280);

        textEndMessage.setFont(font);
        textEndMessage.setCharacterSize(30);
        textEndMessage.setFillColor(Color::Yellow);
        textEndMessage.setPosition(850, 360);

        BackgroundMusic.setVolume(BackgroundMusic.getVolume() * 0.45f);
    }

    void RunGame(RenderWindow& window) {
        while (window.isOpen()) {
            ProcessEvent(window);
            Render(window);
        }
    }

private:
    int Difficulty;
    int matches;
    int PlayerMove;
    int FirstPlayerT;
    Music BackgroundMusic;
    Texture menuBackgroundTexture, StartTexture, ExitTexture, EasyTexture, HardTexture, BackToMainMenuTexture, RestartTexture;
    Sprite menuBg, Start, Exit, Easy, Hard, BackToMainMenu, Restart;
    Font font;
    Text textMatches, textPrompt, textPlayerMove, textPlayerMoveInfo, textRobotMoveInfo, textMoveInfo, textEndMessage, textErr;
    GameState currentState;

    void ProcessEvent(RenderWindow& window) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (currentState == GameState::MENU) {
                HandleMenuEvents(event, window);
            }
            else if (currentState == GameState::DIFFICULTY) {
                HandleDiffEvents(event, window);
            }
            else if (currentState == GameState::GAME) {
                HandleGameEvents(event, window);
            }
            else if (currentState == GameState::END) {
                HandleEndEvents(event, window);
            }
        }
    }

    void HandleMenuEvents(Event& event, RenderWindow& window) {
        Vector2u windowSize = window.getSize();
        int width = windowSize.x;
        int height = windowSize.y;

        menuBg.setScale((float)width / menuBackgroundTexture.getSize().x, (float)height / menuBackgroundTexture.getSize().y);

        if (IntRect(825, 400, 300, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                currentState = GameState::DIFFICULTY;  
            }
        }

        if (IntRect(810, 475, 350, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                window.close();  
            }
        }
    }

    void HandleDiffEvents(Event& event, RenderWindow& window) {
        Vector2u windowSize = window.getSize();
        int width = windowSize.x;
        int height = windowSize.y;

        menuBg.setScale((float)width / menuBackgroundTexture.getSize().x, (float)height / menuBackgroundTexture.getSize().y);

        if (IntRect(825, 400, 200, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Difficulty = 1;
                FirstPlayerTurn();
            }
        }

        if (IntRect(810, 475, 225, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Difficulty = 2;
                FirstPlayerTurn();
            }
        }

        if (IntRect(700, 550, 500, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                currentState = GameState::MENU;
            }
        }     
    }

    void HandleGameEvents(Event& event, RenderWindow& window) {

        textMatches.setString(L"Спичек осталось: " + to_string(matches));

        if (matches == 1) {
            textEndMessage.setString(FirstPlayerT == 0 ? L"Победил Робот!" : L"Победил Игрок!");
            sleep(seconds(1));

            currentState = GameState::END;
            return;
        }

        if (FirstPlayerT == 1) {
            textPlayerMoveInfo.setString(L"Ход игрока! Возьмите от 1 до 9 спичек!");

            window.clear(Color(129, 181, 221));
            window.draw(menuBg);
            window.draw(textMatches);
            window.draw(textPlayerMoveInfo);
            window.draw(textMoveInfo);
            window.display();

            if (event.type == Event::KeyPressed) {
                int playerMove = 0;
                if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num9) {
                    playerMove = event.key.code - Keyboard::Num0;
                }

                if (playerMove >= 1 && playerMove <= min(9, matches - 1)) {
                    matches -= playerMove;       
                    textMoveInfo.setString(L"Игрок взял " + to_string(playerMove) + L" спичек.");

                    window.clear(Color(129, 181, 221));
                    window.draw(menuBg);
                    window.draw(textMatches);
                    window.draw(textPlayerMoveInfo);
                    window.draw(textMoveInfo);
                    window.display();
                    sleep(seconds(1));

                    if (matches == 1) {
                        currentState = GameState::GAME;
                    }
                    else {
                        FirstPlayerT = 0;
                    }
                }
                else {
                    textErr.setString(L"Неверный выбор! Вы можете взять от 1 до " + to_string(std::min(9, matches - 1)) + L" спичек.");
                    window.clear(Color(129, 181, 221));
                    window.draw(menuBg);
                    window.draw(textMatches);
                    window.draw(textPlayerMoveInfo);
                    window.draw(textErr);
                    window.display();
                    sleep(seconds(1));
                }
            }
        }
        else {
            textRobotMoveInfo.setString(L"Ход робота!");
            window.clear(Color(129, 181, 221));
            window.draw(menuBg);
            window.draw(textMatches);
            window.draw(textRobotMoveInfo);
            window.display();

            if (RobotTurn(matches)) {
                currentState = GameState::GAME;
            }
            else {
                window.clear(Color(129, 181, 221));
                window.draw(menuBg);
                window.draw(textMatches);
                window.draw(textRobotMoveInfo);
                window.draw(textMoveInfo);
                window.display();
                sleep(seconds(1));

                FirstPlayerT= 1;
            }
        }
    }

    void HandleEndEvents(Event& event, RenderWindow& window) {
        Vector2u windowSize = window.getSize();
        int width = windowSize.x;
        int height = windowSize.y;

        menuBg.setScale((float)width / menuBackgroundTexture.getSize().x, (float)height / menuBackgroundTexture.getSize().y);

        if (IntRect(775, 475, 350, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                FirstPlayerTurn();
            }
        }
        if (IntRect(700, 550, 500, 50).contains(Mouse::getPosition(window))) {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                currentState = GameState::MENU;
            }
        }
    }

    void FirstPlayerTurn() {
        matches = 100;

        srand(static_cast<unsigned int>(time(0)));
        
        FirstPlayerT = rand() % 2;
        currentState = GameState::GAME;
    }

    void Render(RenderWindow& window) {
        window.clear(Color(129, 181, 221));
        window.draw(menuBg);

        if (currentState == GameState::MENU) {
            HighlightButton(Start, IntRect(825, 400, 300, 50), window);
            HighlightButton(Exit, IntRect(810, 475, 350, 50), window);
            window.draw(Start);
            window.draw(Exit);
        }
        else if (currentState == GameState::DIFFICULTY) {
            HighlightButton(Easy, IntRect(825, 400, 200, 50), window);
            HighlightButton(Hard, IntRect(810, 475, 225, 50), window);
            HighlightButton(BackToMainMenu, IntRect(700, 550, 500, 50), window);
            window.draw(Easy);
            window.draw(Hard);
            window.draw(BackToMainMenu);
        }
        else if (currentState == GameState::GAME) {
            window.draw(textMatches);
            window.draw(textPrompt);
            if (FirstPlayerT == 0){
                window.draw(textRobotMoveInfo);
            }
            else {
                window.draw(textPlayerMoveInfo);
            }
        }
        else if (currentState == GameState::END) {
            HighlightButton(Restart, IntRect(775, 475, 350, 50), window);
            HighlightButton(BackToMainMenu, IntRect(700, 550, 500, 50), window);\
            window.draw(textEndMessage);
            window.draw(Restart);
            window.draw(BackToMainMenu);
        }

        window.display();
    }

    void HighlightButton(Sprite& button, const IntRect& buttonRect, RenderWindow& window) {
        if (buttonRect.contains(Mouse::getPosition(window))) {
            button.setColor(Color::Blue);
        }
        else {
            button.setColor(Color::White);
        }
    }

    bool RobotTurn(int& matches) {
        int robotMove = 0;
        if (Difficulty == 1) {
            robotMove = rand() % min(9, matches) + 1;
            if (robotMove >= matches) {
                robotMove = matches - 1;
            }
        }
        if (Difficulty == 2) {
            robotMove = calculateRobotMove(matches);
        }

        matches -= robotMove;
        textMoveInfo.setString(L"Робот взял " + to_string(robotMove) + L" спичек.");

        return matches == 1;
    }

    int calculateRobotMove(int matches) {
        if (matches > 10) {
            int move = matches % 10;
            return move == 0 ? 1 : move;
        }
        else {
            return matches - 1;
        }
    }
};

int main() {
    RenderWindow window(VideoMode::getDesktopMode(), L"ИГРА 100 СПИЧЕК!", Style::Fullscreen);
    setlocale(LC_ALL, "RU");
    Game game;
    game.RunGame(window);
    return 0;
}