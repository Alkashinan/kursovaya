#include <SFML/Audio.hpp> // ���������� ���������� ��� ������ � ����� SFML
#include <SFML/Graphics.hpp> // ���������� ���������� ��� ������ � �������� SFML

#include <filesystem> // ���������� ���������� ��� ������ � �������� ��������
#include <fstream> // ���������� ���������� ��� ������ � �������
#include <string> // ���������� ���������� ��� ������ �� ��������
#include <vector> // ���������� ���������� ��� ������������� ��������

// ����������� ��������� ����
enum class GameState {
    Menu,     // ��������� ����
    Playing,  // ��������� ����
    Settings, // ��������� ��������
    SaveLoad, // ��������� ����������/��������
    Choice    // ��������� ������
};

// ��������� ��� ����� �������
struct DialogueOption {
    std::string text;                       // ����� �����
    std::vector<std::string> nextDialogues; // ��������� �������
};

// ��������� �������
void saveProgress( int dialogueIndex1,
                   bool isCharacter1Speaking ); // ������� ���������� ���������
void loadProgress( int& dialogueIndex1,
                   bool& isCharacter1Speaking ); // ������� �������� ���������
void skipToChoices( int& currentDialogueIndex1,
                    std::vector<std::string>& dialogues1,
                    GameState& state,
                    sf::Text& dialogue1 ); // ������� �������� � ������

int main() {
    sf::RenderWindow window( sf::VideoMode( 1920, 1080 ),
                             "Menu Example" ); // ������� ���� ����������

    sf::Font font; // ������� ������ ������
    if ( !font.loadFromFile(
             "assets/Arial.ttf" ) ) { // ��������� ����� �� �����
        return -1; // ���� �� ������� ��������� �����, ���������� ������
    }

    // ������� � ����������� ������ ����
    sf::Text startButton( "START", font, 24 );       // ������ "START"
    sf::Text settingsButton( "SETTINGS", font, 24 ); // ������ "SETTINGS"
    sf::Text saveButton( "SAVES", font, 24 );        // ������ "SAVES"
    sf::Text quitButton( "QUIT", font, 24 );         // ������ "QUIT"

    startButton.setPosition( 860, 450 ); // ������������� ������� ������ "START"
    settingsButton.setPosition(
        860, 500 ); // ������������� ������� ������ "SETTINGS"
    saveButton.setPosition( 860, 550 ); // ������������� ������� ������ "SAVES"
    quitButton.setPosition( 860, 600 ); // ������������� ������� ������ "QUIT"

    // ������� � ����������� ������������� ��� ����
    sf::RectangleShape menuBox(
        sf::Vector2f( 400, 300 ) ); // ������������� ��� ���� ����
    menuBox.setFillColor(
        sf::Color( 0, 0, 0, 150 ) ); // ������������� ���� � ������������
    menuBox.setPosition( 760, 400 ); // ������������� �������

    // ������� � ����������� ������ ���������� � ��������
    sf::Text saveProgressButton(
        "SAVE PROGRESS", font, 24 ); // ������ "SAVE PROGRESS"
    sf::Text loadProgressButton(
        "LOAD PROGRESS", font, 24 ); // ������ "LOAD PROGRESS"

    saveProgressButton.setPosition(
        860, 450 ); // ������������� ������� ������ "SAVE PROGRESS"
    loadProgressButton.setPosition(
        860, 500 ); // ������������� ������� ������ "LOAD PROGRESS"

    GameState state =
        GameState::Menu; // ������������� ��������� ��������� ���� ��� ����

    // ��������� ���� ��� ���� � ����
    sf::Texture menuBackgroundTexture; // �������� ��� ���� ����
    if ( !menuBackgroundTexture.loadFromFile(
             "assets/night.jpg" ) ) { // ��������� �������� �� �����
        return -1; // ���� �� ������� ��������� ��������, ���������� ������
    }
    sf::Sprite menuBackground(
        menuBackgroundTexture ); // ������� ������ ��� ���� ����

    sf::Texture gameBackgroundTexture; // �������� ��� ���� ����
    if ( !gameBackgroundTexture.loadFromFile(
             "assets/map2.png" ) ) { // ��������� �������� �� �����
        return -1; // ���� �� ������� ��������� ��������, ���������� ������
    }
    sf::Sprite gameBackground(
        gameBackgroundTexture ); // ������� ������ ��� ���� ����

    // ������������� ��������
    std::vector<std::string> dialogues1 = {
        "Zvvvv, Zvvvvv",
        "GG: �What?...�",
        "The alarm clock woke me up on a hot spring day",
        "GG: �Aah, shut up�",
        "I rejected the call, thinking it was an alarm clock",
        "Zvvvv, Zvvvvv",
        "I sat up and picked up the ringing phone.",
        "GG: the headman?",
        "I swiped the green handset to the right and held the phone to my ear.",
        "GG: ``Hello?..''",
        "The headman: �where the hell are you, today is the last day of "
        "retaking the matanalysis, or do you want to fly out of the HSE like a "
        "cork out of a bottle?!�",
        "Masha is as usual in her repertoire.",
        "Almost screams, why is she so worried about the whole group?",
        "Her voice was so high and loud that my ears curled up into a tube.",
        "GG: �I'll come, I just prepared all night, I almost overslept because "
        "of it.�",
        "It's good that I hardly communicate with my classmates and no one "
        "will turn me in, saying that I played Dota all night.",
        "Headman Masha: �it's good that I was preparing, otherwise I'm already "
        "ashamed of our group, 8 people at the retake, this is wrong.�",
        "Her words make sense ... But I'd rather keep silent about her "
        "transfer in English",
        "GG: �Come on, I'm getting dressed and going out.",
        "I started to get dressed quickly, but my stomach rumbled.",
        "GG: �Damn, I need to have breakfast, although I may be late�" };

    // ������� ����� ��� ����������� ��������
    sf::Text dialogue1( dialogues1[0], font, 24 ); // ����� ��� ������� �������
    dialogue1.setPosition( 960, 940 ); // ������������� ������� ������ �������

    // ���������� ����� �������
    sf::FloatRect textRect1 =
        dialogue1.getLocalBounds(); // �������� ������� ������
    dialogue1.setOrigin( textRect1.left + textRect1.width / 2.0f,
                         textRect1.top +
                             textRect1.height /
                                 2.0f ); // ������������� �������������
    dialogue1.setPosition(
        sf::Vector2f( 1920 / 2.0f, 940 ) ); // ������������� ������� ������

    int currentDialogueIndex1 = 0; // ������ �������� �������
    bool isCharacter1Speaking = true; // ����, ������� �� ������ ��������

    // ������� ������������� ��� ���� ��������
    sf::RectangleShape dialogueBox(
        sf::Vector2f( 1920, 150 ) ); // ������������� ��� ��������
    dialogueBox.setFillColor(
        sf::Color( 0, 0, 0, 150 ) ); // ������������� ���� � ������������
    dialogueBox.setPosition( 0, 900 ); // ������������� �������

    // ��������� ������
    sf::Music music; // ������� ������ ������

    if ( !music.openFromFile(
             "assets/01.Hollywood.mp3" ) ) { // ��������� ������ �� �����
        return -1; // ���� �� ������� ��������� ������, ���������� ������
    }

    music.setLoop( true ); // ������������� ������������ ������
    music.play(); // ������������� ������

    // ������� � ����������� �������� ��� ��������
    sf::RectangleShape volumeSlider(
        sf::Vector2f( 200, 10 ) ); // ������� ���������
    volumeSlider.setFillColor( sf::Color::White ); // ������������� ����
    volumeSlider.setPosition( 860, 500 ); // ������������� �������

    sf::RectangleShape brightnessSlider(
        sf::Vector2f( 200, 10 ) ); // ������� �������
    brightnessSlider.setFillColor( sf::Color::White ); // ������������� ����
    brightnessSlider.setPosition( 860, 600 ); // ������������� �������

    sf::RectangleShape volumeKnob(
        sf::Vector2f( 10, 30 ) ); // �������� ���������
    volumeKnob.setFillColor( sf::Color::Red ); // ������������� ����
    volumeKnob.setPosition(
        860 + music.getVolume() * 2,
        485 ); // ������������� ������� � ����������� �� ������� ���������

    sf::RectangleShape brightnessKnob(
        sf::Vector2f( 10, 30 ) ); // �������� �������
    brightnessKnob.setFillColor( sf::Color::Red ); // ������������� ����
    brightnessKnob.setPosition(
        860 + 100, 585 ); // ������������� �������, ������� �� ��������� 100%

    // ������� � ����������� ����� ��� ����� ������
    sf::Text choice1(
        "Take a Breakfast?", font, 24 ); // ����� "Take a Breakfast?"
    sf::Text choice2(
        "Don't breakfast.", font, 24 ); // ����� "Don't breakfast."
    choice1.setPosition(
        860, 500 ); // ������������� ������� ����� "Take a Breakfast?"
    choice2.setPosition(
        860, 550 ); // ������������� ������� ����� "Don't breakfast."

    // ������� � ����������� ������������� ��� ���� ������ �����
    sf::RectangleShape choiceBox(
        sf::Vector2f( 400, 150 ) ); // ������������� ��� ������ �����
    choiceBox.setFillColor(
        sf::Color( 0, 0, 0, 150 ) ); // ������������� ���� � ������������
    choiceBox.setPosition( 760, 450 ); // ������������� �������

    // ������������� ����� �������
    std::vector<DialogueOption> choices = {
        { "Take a Breakfast?",
          { "I made myself 2 sandwiches with sausage and cheese, quickly made "
            "them and washed them down with water.",
            "I got dressed quickly and ran to the streets.",
            "Opening the entrance door, the sun began to shine in my eyes.",
            "It is so bright that I would gladly go to the beach right now.",
            "I open my phone to look at the bus and commuter train "
            "schedules" } },
        { "Don't breakfast.",
          { "GG: �no, I won't eat, otherwise I'll be late�",
            "I got dressed quickly and ran to the streets.",
            "I opened the entrance door",
            "The sun is shining brightly in my eyes.",
            "It's so bright that I would love to go to the beach right now.",
            "I open my phone to look at the bus and commuter train "
            "schedules" } } };

    // �������� ���� ����
    while ( window.isOpen() ) {
        sf::Event event; // ������� ������ �������
        while ( window.pollEvent( event ) ) { // ������������ �������
            if ( event.type ==
                 sf::Event::Closed ) { // ���� ������� - �������� ����
                window.close(); // ��������� ����
            }

            // ��������� ������� � ����������� �� ��������� ����
            if ( state == GameState::Menu ) { // ���� ��������� - ����
                if ( event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button ==
                         sf::Mouse::Left ) { // ���� ������ ����� ������ ����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(
                        window ); // �������� ������� ����
                    if ( startButton.getGlobalBounds().contains(
                             mousePos.x,
                             mousePos.y ) ) { // ���� ������ ������ "START"
                        state =
                            GameState::Playing; // ��������� � ��������� ����
                    } else if ( settingsButton.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos.y ) ) { // ���� ������ ������
                                                     // "SETTINGS"
                        state = GameState::Settings; // ��������� � ���������
                                                     // ��������
                    } else if ( saveButton.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos
                                        .y ) ) { // ���� ������ ������ "SAVES"
                        state = GameState::SaveLoad; // ��������� � ���������
                                                     // ����������/��������
                    } else if ( quitButton.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos
                                        .y ) ) { // ���� ������ ������ "QUIT"
                        window.close(); // ��������� ����
                    }
                }
            } else if ( state == GameState::Playing ) { // ���� ��������� - ����
                if ( event.type ==
                     sf::Event::KeyPressed ) { // ���� ������ �������
                    if ( event.key.code ==
                         sf::Keyboard::Escape ) { // ���� ������ ������� ESC
                        state = GameState::Menu; // ������������ � ����
                    } else if ( event.key.code == sf::Keyboard::LControl ||
                                event.key.code ==
                                    sf::Keyboard::
                                        RControl ) { // ���� ������ �������
                                                     // LControl ��� RControl
                        skipToChoices( currentDialogueIndex1,
                                       dialogues1,
                                       state,
                                       dialogue1 ); // ���������� � ������
                    }
                } else if ( event.type == sf::Event::MouseButtonPressed &&
                            event.mouseButton.button ==
                                sf::Mouse::Left ) { // ���� ������ ����� ������
                                                    // ����
                    if ( isCharacter1Speaking ) { // ���� ������� ������
                                                  // ��������
                        currentDialogueIndex1++; // ��������� � ����������
                                                 // �������
                        if ( currentDialogueIndex1 <
                             dialogues1.size() ) { // ���� ���� ��� �������
                            dialogue1.setString(
                                dialogues1
                                    [currentDialogueIndex1] ); // �������������
                                                               // �����
                                                               // ��������
                                                               // �������

                            sf::FloatRect textRect1 =
                                dialogue1.getLocalBounds(); // �������� �������
                                                            // ������
                            dialogue1.setOrigin(
                                textRect1.left + textRect1.width / 2.0f,
                                textRect1.top + textRect1.height /
                                                    2.0f ); // ���������� �����
                            dialogue1.setPosition( sf::Vector2f(
                                1920 / 2.0f,
                                940 ) ); // ������������� ������� ������

                            if ( currentDialogueIndex1 ==
                                 dialogues1.size() -
                                     1 ) { // ���� ��� ��������� ������
                                state = GameState::Choice; // ��������� �
                                                           // ��������� ������
                            }
                        }
                    }
                    isCharacter1Speaking =
                        !isCharacter1Speaking; // ������ ���� ����������
                                               // ���������
                }
            } else if ( state ==
                        GameState::Settings ) { // ���� ��������� - ���������
                if ( event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button ==
                         sf::Mouse::Left ) { // ���� ������ ����� ������ ����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(
                        window ); // �������� ������� ����
                    if ( volumeSlider.getGlobalBounds().contains(
                             mousePos.x, mousePos.y ) ) { // ���� ������ �������
                                                          // �������� ���������
                        float volume =
                            ( mousePos.x - volumeSlider.getPosition().x ) /
                            2.0f; // ��������� ���������
                        volume = std::max(
                            0.0f,
                            std::min( 100.0f,
                                      volume ) ); // ������������ ���������
                                                  // ���������� �� 0 �� 100
                        music.setVolume(
                            volume ); // ������������� ��������� ������
                        volumeKnob.setPosition(
                            volumeSlider.getPosition().x + volume * 2,
                            volumeKnob.getPosition().y ); // ���������� ��������
                    } else if ( brightnessSlider.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos.y ) ) { // ���� ������ �������
                                                     // �������� �������
                        float brightness =
                            ( mousePos.x - brightnessSlider.getPosition()
                                               .x ); // ��������� �������
                        brightness = std::max(
                            0.0f,
                            std::min( 200.0f,
                                      brightness ) ); // ������������ �������
                                                      // ���������� �� 0 �� 200
                        brightnessKnob.setPosition(
                            brightnessSlider.getPosition().x + brightness,
                            brightnessKnob.getPosition()
                                .y ); // ���������� ��������

                        // ����������� ������� ����
                        sf::Color color =
                            gameBackground
                                .getColor(); // �������� ������� ���� ����
                        color.r = color.g = color.b =
                            255 *
                            ( brightness / 200.0f ); // ��������� ����� ����
                        gameBackground.setColor(
                            color ); // ������������� ����� ����
                    }
                } else if ( event.type == sf::Event::KeyPressed &&
                            event.key.code ==
                                sf::Keyboard::Escape ) { // ���� ������ �������
                                                         // ESC
                    state = GameState::Menu; // ������������ � ����
                }
            } else if ( state == GameState::Choice ) { // ���� ��������� - �����
                if ( event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button ==
                         sf::Mouse::Left ) { // ���� ������ ����� ������ ����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(
                        window ); // �������� ������� ����
                    if ( choice1.getGlobalBounds().contains(
                             mousePos.x,
                             mousePos.y ) ) { // ���� ������ ������ �����
                        dialogues1 =
                            choices[0]
                                .nextDialogues; // ������������� ����� �������
                        currentDialogueIndex1 = 0; // ���������� ������ �������
                        dialogue1.setString(
                            dialogues1
                                [currentDialogueIndex1] ); // �������������
                                                           // �����
                                                           // ��������
                                                           // �������
                        isCharacter1Speaking =
                            true; // ������������� ���� ���������� ���������
                        state =
                            GameState::Playing; // ������������ � ��������� ����
                    } else if ( choice2.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos.y ) ) { // ���� ������ ������ �����
                        dialogues1 =
                            choices[1]
                                .nextDialogues; // ������������� ����� �������
                        currentDialogueIndex1 = 0; // ���������� ������ �������
                        dialogue1.setString(
                            dialogues1
                                [currentDialogueIndex1] ); // �������������
                                                           // �����
                                                           // ��������
                                                           // �������
                        isCharacter1Speaking =
                            true; // ������������� ���� ���������� ���������
                        state =
                            GameState::Playing; // ������������ � ��������� ����
                    }
                }
            } else if ( state == GameState::SaveLoad ) { // ���� ��������� -
                                                         // ����������/��������
                if ( event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button ==
                         sf::Mouse::Left ) { // ���� ������ ����� ������ ����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(
                        window ); // �������� ������� ����
                    if ( saveProgressButton.getGlobalBounds().contains(
                             mousePos.x,
                             mousePos.y ) ) { // ���� ������ ������ ����������
                        saveProgress(
                            currentDialogueIndex1,
                            isCharacter1Speaking ); // ��������� ��������
                    } else if ( loadProgressButton.getGlobalBounds().contains(
                                    mousePos.x,
                                    mousePos
                                        .y ) ) { // ���� ������ ������ ��������
                        loadProgress(
                            currentDialogueIndex1,
                            isCharacter1Speaking ); // ��������� ��������
                        dialogue1.setString(
                            dialogues1
                                [currentDialogueIndex1] ); // �������������
                                                           // �����
                                                           // ��������
                                                           // �������
                        state =
                            GameState::Playing; // ������������ � ��������� ����
                    }
                } else if ( event.type == sf::Event::KeyPressed &&
                            event.key.code ==
                                sf::Keyboard::Escape ) { // ���� ������ �������
                                                         // ESC
                    state = GameState::Menu; // ������������ � ����
                }
            }
        }

        window.clear(); // ������� ����

        // ��������� ��������� � ����������� �� ��������� ����
        if ( state == GameState::Menu ) { // ���� ��������� - ����
            window.draw( menuBackground ); // ������ ��� ����
            window.draw( menuBox ); // ������ ������������� ����
            window.draw( startButton );    // ������ ������ "START"
            window.draw( settingsButton ); // ������ ������ "SETTINGS"
            window.draw( saveButton );     // ������ ������ "SAVES"
            window.draw( quitButton );     // ������ ������ "QUIT"
        } else if ( state == GameState::Playing ) { // ���� ��������� - ����
            window.draw( gameBackground ); // ������ ��� ����
            window.draw( dialogueBox ); // ������ ������������� ��� ��������
            window.draw( dialogue1 ); // ������ ������� ������
        } else if ( state ==
                    GameState::Settings ) { // ���� ��������� - ���������
            window.draw( gameBackground ); // ������ ��� ����
            window.draw( dialogueBox ); // ������ ������������� ��� ��������
            window.draw( volumeSlider ); // ������ ������� ���������
            window.draw( volumeKnob ); // ������ �������� ���������
            window.draw( brightnessSlider ); // ������ ������� �������
            window.draw( brightnessKnob ); // ������ �������� �������
        } else if ( state == GameState::SaveLoad ) { // ���� ��������� -
                                                     // ����������/��������
            window.draw( gameBackground ); // ������ ��� ����
            window.draw( dialogueBox ); // ������ ������������� ��� ��������
            window.draw( saveProgressButton ); // ������ ������ "SAVE PROGRESS"
            window.draw( loadProgressButton ); // ������ ������ "LOAD PROGRESS"
        } else if ( state == GameState::Choice ) { // ���� ��������� - �����
            window.draw( gameBackground ); // ������ ��� ����
            window.draw( choiceBox ); // ������ ������������� ��� ������ �����
            window.draw( choice1 ); // ������ ������ �����
            window.draw( choice2 ); // ������ ������ �����
        }

        window.display(); // ���������� ���������� ����
    }

    return 0; // ��������� ���������
}

// ������� ���������� ���������
void saveProgress( int dialogueIndex1, bool isCharacter1Speaking ) {
    // std::filesystem::create_directory(
    //     "saves" ); // ������� ���������� ��� ����������
    std::ofstream saveFile( "saves/mysave.txt" ); // ��������� ���� ��� ������
    if ( saveFile.is_open() ) {
        saveFile << dialogueIndex1 << "\n" << isCharacter1Speaking << "\n";
        saveFile.close(); // ��������� ����
    }
}

// ������� �������� ���������
void loadProgress( int& dialogueIndex1, bool& isCharacter1Speaking ) {
    std::ifstream loadFile( "saves/mysave.txt" ); // ��������� ���� ��� ������
    if ( loadFile.is_open() ) { // ���� ���� ������
        loadFile >> dialogueIndex1; // ������ ������ �������� �������
        loadFile >> isCharacter1Speaking; // ������ ���� ���������� ���������
        loadFile.close(); // ��������� ����
    }
}

// ������� ��� �������� � ������
void skipToChoices( int& currentDialogueIndex1,
                    std::vector<std::string>& dialogues1,
                    GameState& state,
                    sf::Text& dialogue1 ) {
    currentDialogueIndex1 =
        dialogues1.size() - 1; // ������������� ������ �� ��������� ������
    dialogue1.setString(
        dialogues1[currentDialogueIndex1] ); // ������������� ����� ����������
                                             // �������
    sf::FloatRect textRect1 =
        dialogue1.getLocalBounds(); // �������� ������� ������
    dialogue1.setOrigin( textRect1.left + textRect1.width / 2.0f,
                         textRect1.top +
                             textRect1.height / 2.0f ); // ���������� �����
    dialogue1.setPosition(
        sf::Vector2f( 1920 / 2.0f, 940 ) ); // ������������� ������� ������
    state = GameState::Choice; // ��������� � ��������� ������
}
