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
    float VMheight;
    float VMwidth;
    int frameLimit;
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
    //indicates if right and left arrow were pressed
    bool right;
    bool left;



//game objects
    //keeps all the enemies
    vector<sf::RectangleShape> enemies;
    //represents one enemy
    sf::RectangleShape enemy;
    //represents player
    sf::RectangleShape player;
//initializers


    /*
    * loads font from a file
    */
    void initFont(){
        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")){
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
        this->pauseT.setPosition(static_cast<float>(this->window->getSize().x)/2.f - 170.f, static_cast<float>(this->window->getSize().y)/2.f - 24.f);
    }


    /*
    * initializes private game variables
    */
    void initVariables(){


        this->frameLimit = 60;
        this->VMwidth = 800.f;
        this->VMheight = 600.f;
        //game logic
        this-> gameEnd = false;
        this->points = 0;
        this->health = 5;
        this->enemiesMax = 5;
        this->spawnEnemyTimerMax = 100.f;
        this->spawnEnemyTimer = this->spawnEnemyTimerMax;
        this->mouseHeld = false;
        this->pause = false;
        this->left = false;
        this->right = false;
    }


    /*
    * param width - width of the window
    * param height - height of the window
    * initializes window and sets its parameters
    */
    void initWindow(){

        this->VM.height = static_cast <int> (VMheight);
        this->VM.width = static_cast <int> (VMwidth);
        this->window = new sf::RenderWindow(this->VM, "SFML GAME", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(frameLimit);

    }

    /*
    * sets enemy's parameters just in case
    */
    void initEnemies(){
        this->enemy.setPosition(100.f, 100.f);
        this->enemy.setSize(sf::Vector2f(100.f,100.f));
        this->enemy.setFillColor(sf::Color::Cyan);
    }

    void initPlayer(float width, float height){
        this->player.setPosition(static_cast<float>(this->window->getSize().x) / 2.0f - width / 2.0f, static_cast<float>(this->window->getSize().y) - height);
        this->player.setSize(sf::Vector2f(width, height));
        this->player.setFillColor(sf::Color::White); // Set the player's color to red (or any color you prefer)
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
        this->initPlayer(50, 20);

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
    const bool getEndGame() const{

        return gameEnd;
    }


//Logic function


    /*
    * prints scored points
    */
    void printPoints(){

        cout << "Congratulations! You scored: " << this->points << endl;
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
    void renderpauseT(sf::RenderTarget& target){

        target.draw(this->pauseT);
    }

    void renderPlayer(sf::RenderTarget& target){

        target.draw(this->player);
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
                    if(this->event.type == sf::Event::KeyPressed){
                        if(this->event.key.code == sf::Keyboard::A){
                            left = true;
                        }
                        else if(this->event.key.code == sf::Keyboard::D){
                            right = true;
                        }
                    }
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
                this->updatePlayer();
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
        this->renderPlayer(*this->window);
        this->renderpauseT(*this->window);
        this->renderText(*this->window);


        this->window->display();
    }

    /*
    * sets enemy position, type, size and adds to the vector
    */
    void spawnEnemies(){

        //randomize enemy type
        int type = rand() % 5;

        this->spawnEnemyTimerMax -= 1.f;

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

        //sets position to top (y value = 0) and random x value
        this->enemy.setPosition(
                static_cast<float>(rand()%static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
                0.f
        );
        //adds enemy to the vector that stores the enemies
        this->enemies.push_back(this->enemy);


    }

    sf::Clock clock;

    void updatePlayer(){
        // Calculate delta time
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Define your player speed, e.g., pixels per second
        float speed = 500.f; // Adjust as necessary for faster movement

        // Check real-time state of the keys and set your boolean flags accordingly
        left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        // Use the left and right flags to move the player smoothly
        if (this->left) {
            this->player.move(-speed * dt, 0.f);
        }
        if (this->right) {
            this->player.move(speed * dt, 0.f);
        }

        // Here we get the current position of the player
        sf::Vector2f playerPosition = this->player.getPosition();

        // Check if player goes beyond the left side of the screen
        if (playerPosition.x < 0 - this->player.getSize().x) {
            this->player.setPosition(this->window->getSize().x, playerPosition.y);
        }
            // Check if player goes beyond the right side of the screen
        else if (playerPosition.x > this->window->getSize().x) {
            this->player.setPosition(0 - this->player.getSize().x, playerPosition.y);
        }
    }






    /*
    * spawns and moves the enemies
    */
    void updateEnemies(){
        // Spawning enemies
        if(this->enemies.size() < this->enemiesMax) {
            if(this->spawnEnemyTimer >= this->spawnEnemyTimerMax){
                this->spawnEnemies();
                this->spawnEnemyTimer = 0.f; // Reset spawn timer
            }
            else{
                this->spawnEnemyTimer++; // Increment spawn timer
            }
        }

        // Iterate through the enemies
        for(int i = 0; i < this->enemies.size(); i++) {
            // Move the enemies downwards
            this->enemies[i].move(0.f, 3.f);

            // Check collision with the top edge of the player
            if(this->enemies[i].getPosition().y + this->enemies[i].getSize().y >= this->player.getPosition().y &&
               this->enemies[i].getPosition().x < this->player.getPosition().x + this->player.getSize().x &&
               this->enemies[i].getPosition().x + this->enemies[i].getSize().x > this->player.getPosition().x &&
               this->enemies[i].getPosition().y < this->player.getPosition().y + this->player.getSize().y) { // This line ensures that the enemy is actually colliding with the player and not just passing by above
                // Enemy is touching the top of the player, "kill" it
                // Update points according to the color of the enemy
                if (this->enemies[i].getFillColor() == sf::Color::Magenta) {
                    this->points += 10;
                }
                else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
                    this->points += 7;
                }
                else if (this->enemies[i].getFillColor() == sf::Color::Cyan) {
                    this->points += 5;
                }
                else if (this->enemies[i].getFillColor() == sf::Color::Yellow) {
                    this->points += 3;
                }
                else if (this->enemies[i].getFillColor() == sf::Color::Green) {
                    this->points += 1;
                }
                // Remove the enemy from the vector
                this->enemies.erase(this->enemies.begin() + i);
                // Since we're erasing an enemy, we need to decrement i to not skip the next enemy
                i--;
            }
                // Check if enemy has moved past the bottom of the window and decrease health
            else if(this->enemies[i].getPosition().y > this->window->getSize().y) {
                // Remove the enemy from the vector
                this->enemies.erase(this->enemies.begin() + i);
                // Decrease health
                this->health--;
                // Since we're erasing an enemy, we need to decrement i to not skip the next enemy
                i--;
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

class GameOver:public Game{
    //TODO
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