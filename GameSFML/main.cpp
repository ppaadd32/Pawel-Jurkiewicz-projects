#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

// Game class that stands for the game engine
class Game{

private:
//game variables
    //window variables
    sf::RenderWindow*  window;
    sf::VideoMode VM;
    //variable that is used to poll events
    sf::Event event;
    //mouse position related to the window as int and then as float as most SFML functions use float
    sf::Vector2i MouPosWin;
    sf::Vector2f MouPosView;

//resources
    sf::Font font;
    //text that will be displayed in the window
    sf::Text uiText;
    sf::Text pauseT;

//game logic
    //indicates if the game is over
    bool gameEnd;
    // keeps points and health
    unsigned points ;
    int health;
    //spawn timer
    float spawnEnemyTimer;
    float spawnEnemyTimerMax;
    //max number of enemies in one wave
    int enemiesMax;
    //indicates if mouse button is constantly held
    bool mouseHeld;
    //indicates if there is a pause in game
    bool pause;
    //indicates if the enemy should be deleted
    bool deleted;

//game objects
    //keeps all the enemies
    vector<sf::RectangleShape> enemies;
    //represents one enemy
    sf::RectangleShape enemy;
//initializers


    /*
    * loads font from a file
    */
    void initFont(){
        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/SFML-PROJECTS/GameSFML/Fonts/Dosis-Bold.ttf")){
            cout << "Loading font failed" << endl;
        }
    }


    /*
    * sets text font, size, color and text to default
    */
    void initText(){
        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(24);
        this->uiText.setFillColor(sf::Color::White);
        this->uiText.setString("Text");
    }
    /*
    * sets text font, size, color and text to default
    */
    void initPauseText(){
        this->pauseT.setFont(this->font);
        this->pauseT.setCharacterSize(48);
        this->pauseT.setFillColor(sf::Color::Red);
        this->pauseT.setString("GAME PAUSED");
        this->pauseT.setPosition(static_cast<float>(this->window->getSize().x)/2.0f - 170.0f, static_cast<float>(this->window->getSize().y)/2.0f - 24.0f);

    }

    /*
    * initializes private game variables
    */
    void initVariables(){

        this->window = nullptr;
        //game logic
        this-> gameEnd = false;
        this->points = 0;
        this->health = 20;
        this->enemiesMax = 5;
        this->spawnEnemyTimerMax = 30.f;
        this->spawnEnemyTimer = this->spawnEnemyTimerMax;
        this->mouseHeld = false;
        this->pause = false;
        this->VM.height = 600.f;
        this->VM.width = 800.f;

    }


    /*
    * param width - width of the window
    * param height - height of the window
    * initializes window and sets its parameters
    */
    void initWindow(){

        this->window = new sf::RenderWindow(this->VM, "SFML GAME", sf::Style::Titlebar | sf::Style::Close);
        this->window->setFramerateLimit(60);
    }

    /*
    * sets enemy's parameters just in case
    */
    void initEnemies(){
        this->enemy.setPosition(100.f, 100.f);
        this->enemy.setSize(sf::Vector2f(100.f,100.f));
        this->enemy.setFillColor(sf::Color::Cyan);
    }


public:

//constructor and destructor

    /*
    * constructor of the Game class
    * uses initializers to initialize all variables
    */
    Game(){

        this->initVariables();
        this->initWindow();
        this->initFont();
        this->initText();
        this->initEnemies();
        this->initPauseText();
    };


    /*
    * destructor of the Game class
    * deletes window
    */
    virtual ~Game() {

        delete this->window;
    }
//accessors

    /*
    * @return bool
    * returns true if the window is open and false if the window is close
    */
    bool running () const{

        return this->window->isOpen();
    }

    /*
    * @return bool
    * returns true if the game has ended. otherwise false
    */
    bool getEndGame() const{
        return gameEnd;
    }


//Logic function


    /*
    * prints scored points
    */
    void printPoints() const{

        cout << "Congratulations! You scored: " << this->points << endl;
    }

    /*
     * @return const bool
     * returns value of "deleted" variable
     */
    bool getDeleted() const {
        return this->deleted;
    }

    /*
    * param target - target to which text will be printed
    * renders text to the given target
    */
    void renderText(sf::RenderTarget& target){

        target.draw(this->uiText);
    }
    /*
    * param target - target to which text will be printed
    * renders text to the given target
    */
    void renderPauseT(sf::RenderTarget& target){

        target.draw(this->pauseT);
    }

    /*
    * param ss - string stream that keeps text
    * updates text to with current variables
    */
    void updateText() {
        stringstream ss;

        ss << "Points: " << this->points << endl << "Health: " << this->health;
        this->uiText.setString(ss.str());
    }
    /*
    * param ss - string stream that keeps text
    * sets text to empty
    */
    void updatePauseTEmpty() {
        stringstream ss;

        ss << "";
        this->pauseT.setString(ss.str());
    }
    /*
    * param ss - string stream that keeps text
    * sets text to game paused
    */
    void updatePauseTP() {
        stringstream ss;

        ss << "GAME PAUSED";
        this->pauseT.setString(ss.str());
    }
    /*
    * polls events
    * if window is closed or Esc is pressed - closes window
    * if keys P or Esc are pressed set "pause" variable to its opposite value
    */
    void pollEvents(){

        while(this->window->pollEvent(this->event)){

            switch(this->event.type){
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::KeyPressed:
                    if(this->event.key.code == sf::Keyboard::Escape || this->event.key.code == sf::Keyboard::P){
                        pause = !pause;
                    }
                    break;
                default:
                    break;


            }
        }
    }

    /*
    * updates game - mouse position related to the window, text(points and health) and enemies(move etc)
    */
    void update(){

        this->pollEvents();

        if (!this->gameEnd){
            this->upMouPosWin();
            this->updateText();
            if (!pause) {
                this->updateEnemies();
                this->updatePauseTEmpty();
            }
            else{
                this->updatePauseTP();
            }
        }
        //endgame condition
        if (this->health <= 0){
            this->gameEnd = true;
            this->printPoints();
            window->close();

        }
    }

    /*
    * updates mouse position
    */
    void upMouPosWin(){


        this->MouPosWin = sf::Mouse::getPosition(*this->window);
        //the same but as float;
        this->MouPosView = this->window->mapPixelToCoords(this->MouPosWin);
    }
//rendering function


    /*
    * clears the window
    * draws objects
    * displays objects
    */
    void render(){
        this->window->clear();
        //draw game objects
        this->renderEnemies(*this->window);
        this->renderPauseT(*this->window);
        this->renderText(*this->window);

        this->window->display();
    }

    /*
    * sets enemy position, type, size and adds to the vector
    */
    void spawnEnemies(){
        //sets position to top (y value = 0) and random x value
        this->enemy.setPosition(
                static_cast<float>(rand()%static_cast<int>(static_cast<float>(this->window->getSize().x) - this->enemy.getSize().x)),
                0.f
        );

        //randomize enemy type
        int type = rand() % 5;

        switch (type) {
            case 0:
                this->enemy.setSize(sf::Vector2f(20.f,20.f));
                this->enemy.setFillColor(sf::Color::Magenta);
                break;
            case 1:
                this->enemy.setSize(sf::Vector2f(40.f,40.f));
                this->enemy.setFillColor(sf::Color::Blue);
                break;
            case 2:
                this->enemy.setSize(sf::Vector2f(50.f,50.f));
                this->enemy.setFillColor(sf::Color::Cyan);
                break;
            case 3:
                this->enemy.setSize(sf::Vector2f(70.f,70.f));
                this->enemy.setFillColor(sf::Color::Yellow);
                break;
            case 4:
                this->enemy.setSize(sf::Vector2f(100.f,100.f));
                this->enemy.setFillColor(sf::Color::Green);
                break;
            default:
                this->enemy.setSize(sf::Vector2f(100.f,100.f));
                this->enemy.setFillColor(sf::Color::Green);

        }
        //adds enemy to the vector that stores the enemies
        this->enemies.push_back(this->enemy);


    }


    /*
    * spawns and moves the enemies
    */
    void updateEnemies(){

        //spawning
        if(this->enemies.size() < enemiesMax){

            if(this->spawnEnemyTimer == this->spawnEnemyTimerMax){
                this->spawnEnemies();
                this->spawnEnemyTimer = 0.f;
            }
            else{
                this->spawnEnemyTimer++;
            }
        }

        for(int i = 0; i < this->enemies.size(); i++){
            //Moving
            deleted = false;
            this->enemies[i].move(0.f,3.f);

            if(this->enemies[i].getPosition().y > static_cast<float>(window->getSize().y)){
                this->enemies.erase(this->enemies.begin() + i);
                this->health--;
            }

            //Attacking
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!this->mouseHeld) {
                    this->mouseHeld = true;
                    deleted = false;
                    for (int j = 0; j < this->enemies.size() && !this->getDeleted(); j++) {
                        if (this->enemies[j].getGlobalBounds().contains(this->MouPosView)) {
                            // Delete the enemy and update points
                            if (this->enemies[j].getFillColor() == sf::Color::Magenta) {
                                this->points += 10;
                            }
                            else if (this->enemies[j].getFillColor() == sf::Color::Blue) {
                                this->points += 7;
                            }
                            else if (this->enemies[j].getFillColor() == sf::Color::Cyan) {
                                this->points += 5;
                            }
                            else if (this->enemies[j].getFillColor() == sf::Color::Yellow) {
                                this->points += 3;
                            }
                            else if (this->enemies[j].getFillColor() == sf::Color::Green) {
                                this->points += 1;
                            }
                            this->enemies.erase(this->enemies.begin() + j);
                            deleted = true;
                        }
                    }
                }
            } else {
                this->mouseHeld = false;
            }

        }

    }


    /*
    * draws the enemies
    */
    void renderEnemies(sf::RenderTarget& target){

        for(auto &en : this->enemies){
            target.draw(en);
        }
    }
};



int main() {
// seed to use rand()
    srand(static_cast<unsigned>(time(nullptr)));

//creating game engine;
    Game game;

    //game loop
    while(game.running() &&  !game.getEndGame()){

        //update
        game.update();

        //rendering
        game.render();


    }
    return 0;
}
