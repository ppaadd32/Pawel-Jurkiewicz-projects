#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <queue>
#include <algorithm>

#include <fstream>
#include <random>


#include <future>
#include <thread> //+ aktualizacja wyników
#include <mutex> // + blokada wektora users z danymi uzytkownikow
#include <regex> //+ nazwy uzytkownikow
#include <filesystem> //+ pliki z wynikami

//TODO odbijanie narożniki

using namespace std;
namespace fs = std::filesystem;


vector<sf::Color> kolory {sf::Color::Blue,sf::Color::Green, sf::Color::Yellow, sf::Color::Cyan, sf::Color::Red, sf::Color(255, 150, 50)};

sf::Color randColor(int i){
    return kolory[rand()%i];
}

class User {
private:
    string nazwa;
    int poziom;
    int punkty;

    sf::Text nazwaT;
    sf::Text poziomT;
    sf::Text punktyT;

    sf::Font font;

    void initText(int position, int size){

        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")){
            cout << "Loading font failed" << endl;
        }
        float pos2 = 300.f;
        float pos3 = 500.f;

        if (position == 1 || position == size){
            pos2 = 1000.f;
            pos3 = 1000.f;
        }

        this->nazwaT.setFont(this->font);
        this->nazwaT.setCharacterSize(24);
        this->nazwaT.setString(nazwa);
        this->nazwaT.setPosition(100.f, position * 100.f);
        this->nazwaT.setFillColor(sf::Color::Blue);

        this->poziomT.setFont(this->font);
        this->poziomT.setCharacterSize(24);
        this->poziomT.setString(to_string(poziom));
        this->poziomT.setPosition(pos2, position * 100.f);
        this->poziomT.setFillColor(sf::Color::Blue);


        this->punktyT.setFont(this->font);
        this->punktyT.setCharacterSize(24);
        this->punktyT.setString(to_string(punkty));
        this->punktyT.setPosition(pos3, position * 100.f);
        this->punktyT.setFillColor(sf::Color::Blue);
    }



public:
    User (string n, int p, int pkt, int count){
        nazwa = n;
        poziom = p;
        punkty = pkt;
    }

    const int retPoints (){
        return punkty;
    }

    void renderUser (sf::RenderTarget& target, int c, int s){
        initText(c, s);
        target.draw(this->nazwaT);
        target.draw(this->poziomT);
        target.draw(this->punktyT);

    }
};

class Pole{
private:
    sf::Text uiText;
    sf::Font font;

    sf::RectangleShape zarys;

    sf::Color blockC;
    float blockW;
    float blockH;
    sf::Color outlineC;


    void initVariablesBlock(float dl, float wy = NAN) {
        this->blockC = sf::Color(100, 100, 120);
        this->blockH = 66;

        if (std::isnan(wy)) {
            this->blockW = dl;       // tylko szerokość podana
        } else {
            this->blockW = dl;       // możesz też użyć wy do czegoś innego
            this->blockH = wy;
        }

        this->outlineC = sf::Color::Blue;
    }



    void initText(sf::Vector2f position, string text){
        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(44);
        this->uiText.setString(text);
        this->uiText.setPosition(position);
        this->uiText.setFillColor(sf::Color(200, 200, 200));
    }

    void iniPole(sf::Vector2f position){
        this->zarys.setPosition(position);
        this->zarys.setSize(sf::Vector2f(blockW, blockH));
        this->zarys.setFillColor(blockC);
        this->zarys .setOutlineColor(outlineC);
    }

    void initFont(){
        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")){
            cout << "Loading font failed" << endl;
        }
    }

public:
    Pole(string nazwa, sf::Vector2f positionT, sf::Vector2f positionP, float lenght, float hight = NAN){
        initFont();
        initText(positionT, nazwa);
        initVariablesBlock(lenght, hight);
        iniPole(positionP);
    }

    bool is_in (sf::Vector2f point){
        return zarys.getGlobalBounds().contains(point);
    }

    void renderPole(sf::RenderTarget& target){
        target.draw(this->zarys);
        target.draw(this->uiText);
    }
};

class Menu {
private:

    vector <Pole*> pola;


public:
    Menu() {
        float centerX = 800.f / 2.f; // połowa szerokości okna
        float buttonWidth = 400.f;
        float startY = 100.f; // pierwszy przycisk
        float stepY = 95.f; // odstęp między przyciskami

        pola.push_back(new Pole("ARKANOID\n    (rules)", {centerX - buttonWidth / 2.f + 80.f, startY - 70},
                                {centerX - buttonWidth / 2.f, startY - 70 }, buttonWidth, 130));
        pola.push_back(new Pole("START", {centerX - buttonWidth / 2.f + 130, startY + stepY},
                                {centerX - buttonWidth / 2.f, startY + stepY}, buttonWidth));
        pola.push_back(new Pole("SET NICK", {centerX - buttonWidth / 2.f + 100.f, startY + 2 * stepY},
                                {centerX - buttonWidth / 2.f, startY + 2 * stepY}, buttonWidth));
        pola.push_back(new Pole("SCOREBOARD", {centerX - buttonWidth / 2.f + 50, startY + 3 * stepY},
                                {centerX - buttonWidth / 2.f, startY + 3 * stepY}, buttonWidth));
        pola.push_back(new Pole("EXIT", {centerX - buttonWidth / 2.f + 150, startY + 4 * stepY},
                                {centerX - buttonWidth / 2.f, startY + 4 * stepY}, buttonWidth));
    }


    bool arkanoidPressed (sf::Vector2f p){
        return pola[0]->is_in(p);
    }

    bool startPressed (sf::Vector2f p){
        return pola[1]->is_in(p);
    }

    bool setNickPressed (sf::Vector2f p){
        return pola[2]->is_in(p);
    }

    bool setScoresPressed (sf::Vector2f p){
        return pola[3]->is_in(p);
    }

    bool setClosePressed (sf::Vector2f p){
        return pola[4]->is_in(p);
    }


    void renderMenu(sf::RenderTarget& target){
        for (auto pole : pola){
            pole->renderPole(target);
        }
    }
};

class Rules {
private:
    sf::Text title;
    sf::Text rulesText;
    sf::Font font;

    void initFont() {
        if (!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")) {
            cout << "Loading font failed" << endl;
        }
    }

    void initTexts() {
        // Nagłówek
        this->title.setFont(this->font);
        this->title.setCharacterSize(48);
        this->title.setString("GAME RULES");
        this->title.setPosition(250.f, 50.f);
        this->title.setFillColor(sf::Color::Blue);

        // Tekst zasad
        string tekst =
                "1. Bounce the ball with the paddle.\n"
                "2. Break blocks only if their color matches the ball.\n"
                "3. Destroy entire groups of the same color.\n"
                "4. If the ball falls to the bottom - game over.\n"
                "5. Points are awarded for removed blocks.\n"
                "6. After clearing all blocks, you advance to the next level.\n"
                "   - Each level adds more colors and increases difficulty.\n"
                "7. Press Enter to return to the main menu.";

        this->rulesText.setFont(this->font);
        this->rulesText.setCharacterSize(26);
        this->rulesText.setString(tekst);
        this->rulesText.setPosition(50.f, 150.f);
        this->rulesText.setFillColor(sf::Color::White);
    }

public:
    Rules() {
        initFont();
        initTexts();
    }

    void renderRules(sf::RenderTarget& target) {
        target.draw(this->title);
        target.draw(this->rulesText);
    }
};

class Nick{
private:
    string napis;
    string nazwaU;

    sf::Text nick;
    sf::Text pytanie;
    sf::Font font;
    regex pattern;

    void initFont(){
        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")){
            cout << "Loading font failed" << endl;
        }
    }

    void initVariablesN(){
        napis = "Type nick";
        pattern = "[A-Za-z0-9]{1,}";
        nazwaU = "";
    }

    void initNick(){
        this->pytanie.setFont(this->font);
        this->pytanie.setCharacterSize(64);
        this->pytanie.setString(napis);
        this->pytanie.setPosition(230.f, 150.f);
        this->pytanie.setFillColor(sf::Color::Red);

        this->nick.setFont(this->font);
        this->nick.setCharacterSize(64);
        this->nick.setString(nazwaU);
        this->nick.setPosition(390.f, 250.f);
        this->nick.setFillColor(sf::Color::Blue);
    }
public:
    Nick(){
        initFont();
        initVariablesN();
        initNick();
    }

    bool nickOK (){
        std::smatch matches;
        if (std::regex_search(nazwaU, matches, pattern)) {
            return true;
        }
        cout << "nazwa niepoprawna" << endl;
        return false;
    }

    void reset(){
        initFont();
        initVariablesN();
        initNick();
    }

    string retNick (){
        return nazwaU;
    }

    void clear(){
        nazwaU = "";
        this->nick.setPosition(395.f, 250.f);
    }



    void dodajZnak(char c){
        nick.move(-20.f, 0.f);
        nazwaU += c;
        updateNick();
    }

    void usunZnak(){
        if (!nazwaU.empty()) {
            nick.move(+20.f, 0.f);
            nazwaU.pop_back();
            updateNick();
        }
        cout << nazwaU << endl;
    }

    void updateNick(){
        nick.setString(nazwaU);
    }

    void renderNick(sf::RenderTarget& target){
        target.draw(this->pytanie);
        target.draw(this->nick);
    }
};




class File {
    fs::path fullPath;

    void initVariablesF(){
        fullPath = fs::current_path() / "pliki" / "wyniki";
    }

public:
    static std::mutex users_mutex;
    static vector<User*> users;

    File() {
        initVariablesF();
    }

    static bool compareUserPointers(User* a, User* b) {
        return a->retPoints() > b->retPoints();
    }

    fs::path getFullPath() {
        return fullPath;
    }

    void dodajRekord(string nazwaU, int levels, int points) {
        string nazwa = nazwaU + ".txt";
        fs::create_directories(fullPath);

        fs::path file_path = fullPath / nazwa;

        if (fs::exists(file_path)) {
            ifstream file(file_path);

            string line = "";
            int line_count = 0;
            int third_line_value = 0;
            while (getline(file, line)) {
                line_count++;
                if (line_count == 3) {
                    third_line_value = stoi(line);
                    break;
                }
            }

            file.close();

            if (points > third_line_value) {
                ofstream plik(file_path);

                plik << nazwaU << endl;
                plik << levels << endl;
                plik << points << endl;

                plik.close();
            }
        } else {
            ofstream plik(file_path);

            plik << nazwaU << endl;
            plik << levels << endl;
            plik << points << endl;

            plik.close();
        }
    }

    void odczytaj(fs::path path) {
        string line1;
        int line2;
        int line3;

        fstream file(path);

        file >> line1;
        file >> line2;
        file >> line3;

        User* u = new User(line1, line2, line3, users.size() + 2);

        std::lock_guard<std::mutex> lock(users_mutex);
        users.push_back(u);
    }

    void renderUsers(sf::RenderTarget& target) {
        int counter = 1;

        std::lock_guard<std::mutex> lock(users_mutex);
        for (auto el : users) {
            el->renderUser(target, counter, users.size());
            counter++;
            if (counter == 6) {
                break;
            }
        }
    }
};

vector<User*> File::users;
std::mutex File::users_mutex;


void updateUsers() {
    File f;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(File::users_mutex);
            File::users.clear();
        }

        for (const fs::directory_entry& zawartosc : fs::directory_iterator(f.getFullPath())) {
            f.odczytaj(zawartosc.path());
        }

        {
            std::lock_guard<std::mutex> lock(File::users_mutex);
            std::sort(File::users.begin(), File::users.end(), File::compareUserPointers);

            if (File::users.size() > 3) {
                File::users.resize(3);
            }

            User* u = new User("NICK\t\t\t\t\tLEVEL\t\t\t\t\tPOINTS", 0, -10, 1);
            File::users.insert(File::users.begin(), u);

            User* u2 = new User("To exit press ENTER", 0, -10, 5);
            File::users.push_back(u2);
        }
        this_thread::sleep_for(chrono::seconds(5));
    }
}


class GameWindow{


private:
    sf::VideoMode VM;
    int frameLimit;
    float VMheight;
    float VMwidth;

    void initVariablesW(){
        this->window = nullptr;
        this->frameLimit = 60;
        this->VMwidth = 800.f;
        this->VMheight = 600.f;

    }
    void initWindow(){

        this->VM.height = static_cast <int> (VMheight);
        this->VM.width = static_cast <int> (VMwidth);
        window = new sf::RenderWindow(this->VM, "BALL GAME", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(frameLimit);
    }

public:

    static sf::RenderWindow*  window;


    GameWindow(){
        initVariablesW();
        initWindow();
    }

    ~GameWindow(){
        delete window;
    }

};

class Player{
private:
    sf::RectangleShape player;
    sf::Color playerC;
    sf::Clock clock;
    float playerW;
    float playerH;
    bool left;
    bool right;

    void initVariablesP() {
        this->playerC = sf::Color::White;
        this->playerH = 20;
        this->playerW = 80;
        this->left = false;
        this->right = false;
    }

    void initPlayer(float width, float height){
        this->player.setPosition((static_cast<float>(GameWindow::window->getSize().x)/ 2.0f - width / 2.0f), (static_cast<float>(GameWindow::window->getSize().y) - height));
        this->player.setSize(sf::Vector2f(width, height));
        this->player.setFillColor(playerC);
    }


public:

    Player(){
        this->initVariablesP();
        this->initPlayer(playerW, playerH);
    }

    void reset(){
        this->initVariablesP();
        this->initPlayer(playerW, playerH);
    }

    const sf::RectangleShape& getShape() const {
        return player;
    }

    void setRight(bool newRight){
        this->right = newRight;
    }

    void setLeft(bool newLeft){
        this->right = newLeft;
    }

    void updatePlayer(sf::RenderWindow& window) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        float speed = 500.f;

        left = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        right = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

        if (this->left) {
            this->player.move(-speed * dt, 0.f);
        }
        if (this->right) {
            this->player.move(speed * dt, 0.f);
        }

        if (this->player.getPosition().x < 0 - this->player.getSize().x) {
            this->player.setPosition(static_cast<float>(window.getSize().x), this->player.getPosition().y);
        }
        else if (this->player.getPosition().x > static_cast<float>(window.getSize().x)) {
            this->player.setPosition(0 - this->player.getSize().x, this->player.getPosition().y);
        }
    }



    void renderPlayer(sf::RenderTarget& target){

        target.draw(this->player);
    }

};

class Block {
protected:
    sf::Color blockC;
    float blockW;
    float blockH;
    sf::Color outlineC;
public:
    sf::RectangleShape blok;

    Block() {}

    virtual void initVariablesBlock() = 0;



    void renderBlock(sf::RenderTarget& target) {
        target.draw(blok);
    }

    bool is_next_to(Block* b1) {
        if (b1->blok.getFillColor() == this->blockC) {
            float x = this->blok.getPosition().x;
            float y = this->blok.getPosition().y;
            if (b1->blok.getGlobalBounds().contains(x + blockW, y) ||
                b1->blok.getGlobalBounds().contains(x - blockW, y) ||
                b1->blok.getGlobalBounds().contains(x, y + blockH) ||
                b1->blok.getGlobalBounds().contains(x, y - blockH)) {
                return true;
            }
        }
        return false;
    }
};

class BlueBlock : public Block {
public:
    BlueBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color::Blue;
    }
};

class GreenBlock : public Block {
public:
    GreenBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color::Green;
    }
};

class YellowBlock : public Block {
public:
    YellowBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color::Yellow;
    }
};

class CyanBlock : public Block {
public:
    CyanBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color::Cyan;
    }
};

class RedBlock : public Block {
public:
    RedBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color::Red;
    }
};

class OrangeBlock : public Block {
public:
    OrangeBlock(sf::Vector2f ballPosition) : Block() {
        initVariablesBlock();
        initBlock(ballPosition);
    }

    void initBlock(sf::Vector2f position) {
        blok.setPosition(position);
        blok.setSize(sf::Vector2f(blockW, blockH));
        blok.setFillColor(blockC);
        blok.setOutlineColor(outlineC);
    }

    void initVariablesBlock() override {
        blockH = 20;
        blockW = 50;
        blockC = sf::Color(255, 150, 50);
    }
};

class Blocks {
private:
    float rows;
    float cols;

public:
    bool is_empty() {
        return bloki.empty();
    }

    void initVariablesBlocks() {
        cols = static_cast<float>(GameWindow::window->getSize().x) / 50;
        rows = 5;
    }

    Blocks() {
        initVariablesBlocks();
    }

    ~Blocks() {
        for (auto block : bloki) {
            delete block;
        }
    }

    Block* createRandomBlock(sf::Vector2f position, int colorCount) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(0, colorCount - 1);
        int colorIndex = dis(gen);

        switch (colorIndex) {
            case 0:
                return new BlueBlock(position);
            case 1:
                return new GreenBlock(position);
            case 2:
                return new YellowBlock(position);
            case 3:
                return new CyanBlock(position);
            case 4:
                return new RedBlock(position);
            case 5:
                return new OrangeBlock(position);
            default:
                return new BlueBlock(position);
        }
    }

    void resetBlocks(int colorCount) {
        bloki.clear();

        for (float i = 0; i < rows; i++) {
            for (float j = 0; j < cols; j += 1.f) {
                sf::Vector2f pos{ j * 50.f, i * 20.f };
                bloki.push_back(createRandomBlock(pos, colorCount));
            }
        }
    }

    int delBlock(int startIndex) {
        std::queue<int> toCheck;
        std::vector<bool> markedToDelete(bloki.size(), false);
        toCheck.push(startIndex);
        markedToDelete[startIndex] = true;

        while (!toCheck.empty()) {

            int currentIndex = toCheck.front();
            toCheck.pop();

            for (int j = 0; j < bloki.size(); ++j) {
                if (!markedToDelete[j] && bloki[currentIndex]->is_next_to(bloki[j])) {
                    toCheck.push(j);
                    markedToDelete[j] = true;
                }
            }
        }

        for (int i = bloki.size() - 1; i >= 0; --i) {
            if (markedToDelete[i]) {
                bloki.erase(bloki.begin() + i);
            }
        }


        return std::count(markedToDelete.begin(), markedToDelete.end(), true);
    }

    void renderBlocks(sf::RenderTarget& targetB) {
        for (int i = 0; i < bloki.size(); i++) {
            bloki[i]->renderBlock(targetB);
        }
    }

    std::vector<Block*> bloki;
};

class Ball {
private:
    sf::CircleShape ball;
    sf::Color ballColor;
    sf::Color nextColor;
    float ballSpeedX;
    float ballSpeedY;
    float radius;
    float positionX;
    float positionY;

    void initVariables() {
        ballColor = sf::Color::Blue;
        nextColor = sf::Color::Blue;
        ballSpeedX = 3.f;
        ballSpeedY = 3.f;
        radius = 10.f;
        positionX = static_cast<float>(rand() % (static_cast<int>(GameWindow::window->getSize().x) - 5 * static_cast<int>(radius)));
        positionY = 100.f;
    }

    void initBall() {
        ball.setPosition(positionX, positionY);
        ball.setRadius(radius);
        ball.setFillColor(ballColor);
    }

public:
    Ball() {
        initVariables();
        initBall();
    }

    void reset() {
        initVariables();
        initBall();
    }

    bool collisionTopEdge(const sf::RectangleShape &player) const {
        return ball.getPosition().y <= player.getPosition().y + player.getSize().y &&
               ball.getPosition().y + 2 * radius > player.getPosition().y &&
               ball.getPosition().x + radius > player.getPosition().x &&
               ball.getPosition().x + radius < player.getPosition().x + player.getSize().x;
    }

    bool collisionBotEdge(const sf::RectangleShape &player) const {
        return ball.getPosition().y + 2 * radius > player.getPosition().y &&
               ball.getPosition().x < player.getPosition().x + player.getSize().x &&
               ball.getPosition().x + 2 * radius > player.getPosition().x &&
               ball.getPosition().y < player.getPosition().y + player.getSize().y;
    }

    bool collisionRightEdge(const sf::RectangleShape &player) const {
        return ball.getPosition().x + 2 * radius >= player.getPosition().x &&
               ball.getPosition().x < player.getPosition().x + player.getSize().x &&
               ball.getPosition().y + radius > player.getPosition().y &&
               ball.getPosition().y + radius < player.getPosition().y + player.getSize().y;
    }

    bool collisionLeftEdge(const sf::RectangleShape &player) const {
        return ball.getPosition().x <= player.getPosition().x + player.getSize().x &&
               ball.getPosition().x + 2 * radius > player.getPosition().x &&
               ball.getPosition().y + radius > player.getPosition().y &&
               ball.getPosition().y + radius < player.getPosition().y + player.getSize().y;
    }


    bool collision(const sf::RectangleShape &player) const {
        return ball.getPosition().y + 2 * radius > player.getPosition().y &&
               ball.getPosition().x < player.getPosition().x + player.getSize().x &&
               ball.getPosition().x + 2 * radius > player.getPosition().x &&
               ball.getPosition().y < player.getPosition().y + player.getSize().y;
    }

    int updateBall(sf::RenderWindow &window, const sf::RectangleShape &player, Blocks &blocks, bool &endGame, int &points, int &color) {
        ball.move(ballSpeedX, ballSpeedY);

        if (ball.getPosition().x <= 5.f && ball.getPosition().y <= 5.f) {
            ball.move(5.f, 5.f);
            ballSpeedY = -ballSpeedY;
            ballSpeedX = -ballSpeedX;
        } else if (ball.getPosition().x >= 795.f && ball.getPosition().y <= 5.f) {
            ball.move(5.f, 5.f);
            ballSpeedY = -ballSpeedY;
            ballSpeedX = -ballSpeedX;
        } else if (ball.getPosition().y < 0.f) {
            ballSpeedY = -ballSpeedY;
        } else if (ball.getPosition().x < 0.f || ball.getPosition().x + 2 * radius > static_cast<float>(window.getSize().x)) {
            ballSpeedX = -ballSpeedX;
        } else if (ball.getPosition().y + 2 * radius > static_cast<float>(window.getSize().y)) {
            endGame = true;
        } else if (collision(player)) {
            ball.setFillColor(nextColor);
            nextColor = randColor(color - 1);
            ballSpeedY = -ballSpeedY;
            while (collision(player)) {
                ball.move(0.f, -1.f);
            }
        }

        for (size_t i = 0; i < blocks.bloki.size(); ++i) {
            if (collision(blocks.bloki[i]->blok)) {
                if (collisionLeftEdge(blocks.bloki[i]->blok) || collisionRightEdge(blocks.bloki[i]->blok)) {
                    ballSpeedX = -ballSpeedX;
                }
                if (collisionTopEdge(blocks.bloki[i]->blok) || collisionBotEdge(blocks.bloki[i]->blok)) {
                    ballSpeedY = -ballSpeedY;
                }
                if (blocks.bloki[i]->blok.getFillColor() == ball.getFillColor()) {
                    points += blocks.delBlock(i);
                    ballSpeedX *= 1.01;
                    ballSpeedY *= 1.01;
                }
            }
        }

        return points;
    }

    void renderBall(sf::RenderTarget &target) const {
        target.draw(ball);
    }
};

class Engine {
private:

    sf::Event event;
    sf::Text uiText;
    sf::Text pauseT;
    sf::Font font;

    bool endGame;
    bool pause;
    int points;
    int colors;

    bool start;
    bool nick;
    bool scores;
    bool close;
    bool info;
    bool menuGlowne;

    sf::Vector2i mousePos;
    sf::Vector2f mousePosV;

    Menu menu;
    GameWindow window2;
    Ball ball;
    Player player;
    Blocks blocks;
    Nick nazwaU;
    Rules rules;
    File file;



    void initText(){
        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(128);
        this->uiText.setFillColor(sf::Color(100, 100, 120));
        this->uiText.setString("Text");
        this->uiText.setPosition(130.f, 250.f);
    }

    void initFont(){
        if(!this->font.loadFromFile("/Users/paweljurkiewicz/CLionProjects/font.ttf")){
            cout << "Loading font failed" << endl;
        }
    }

    void initPauseText(){
        this->pauseT.setFont(this->font);
        this->pauseT.setCharacterSize(48);
        this->pauseT.setFillColor(sf::Color::Red);
        this->pauseT.setString("GAME PAUSED");
        this->pauseT.setPosition(static_cast<float>(GameWindow::window->getSize().x)/2 - 170, static_cast<float>(GameWindow::window->getSize().x)/2 - 24);
    }

    void upMouPosWin(){


        this->mousePos = sf::Mouse::getPosition(*GameWindow::window);
        this->mousePosV = GameWindow::window->mapPixelToCoords(this->mousePos);
    }

    void resetGame(){
        initVariables();
        ball.reset();
        player.reset();
        nazwaU.reset();
    }

    void nextLevel(){
        endGame = false;
        ball.reset();
        player.reset();
    }

    void initVariables(){

        this->points = 0;
        this->colors = 2;
        this->endGame = false;
        this->pause = false;
        this->start = false;
        this->nick = false;
        this->scores = false;
        this->close = false;
        this->info = false;
        this->menuGlowne = true;
    }

    void updatePauseTEmpty() {
        stringstream ss;

        ss << "";
        this->pauseT.setString(ss.str());
    }


    void updatePauseTP() {
        stringstream ss;

        ss << "GAME PAUSED";
        this->pauseT.setString(ss.str());
    }

    void updateText() {
        stringstream ss;

        if (this->points > 9 && this->uiText.getPosition().x >= 130.f)
            this->uiText.move(-32.f, 0.f);


        if (this->points > 99 && this->uiText.getPosition().x >= 98.f)
            this->uiText.move(-32.f, 0.f);


        ss << "Points: " << this->points << endl;
        this->uiText.setString(ss.str());
    }

    void renderpauseT(sf::RenderTarget& target){

        target.draw(this->pauseT);
    }

    void renderText(sf::RenderTarget& target){

        target.draw(this->uiText);
    }

public:

    Engine(){
        this->initVariables();
        this->initFont();
        this->initPauseText();
        this->initText();

        std::thread(updateUsers).detach();

    }


    static bool running () {
        return GameWindow::window->isOpen();
    }

    void update(){

        upMouPosWin();

        if (close){
            GameWindow::window->close();
        }

        if (info){
            menuGlowne = false;
        }

        if (nick){
            menuGlowne = false;
            nazwaU.updateNick();
        }

        if (start) {
            menuGlowne = false;

            if (!this->getEndGame()) {
                if (!pause) {

                    if (!blocks.is_empty()) {
                        this->updateText();
                        this->player.updatePlayer(*GameWindow::window);
                        this->points = ball.updateBall(*GameWindow::window, player.getShape(), blocks, this->endGame,
                                                       this->points, this->colors);
                        this->updatePauseTEmpty();
                        file.dodajRekord(nazwaU.retNick(), colors-1, points);
                    } else {
                        colors++;

                        if (colors > 7){
                            colors = 1;
                            resetGame();
                            start = false;
                            menuGlowne = true;
                        }else {
                            nextLevel();
                            blocks.resetBlocks(colors - 1);
                        }

                    }

                } else{
                    this->updatePauseTP();
                }
            } else {
                blocks.resetBlocks(colors);
                file.dodajRekord(nazwaU.retNick(), colors-1, points);
                resetGame();
                blocks.resetBlocks(2);
                start = false;
                menuGlowne = true;
            }
        }
        if (scores){
            menuGlowne = false;
        }
    }



    void render(){

        this->pollEvents();
        GameWindow::window->clear();

        if (menuGlowne){
            this->menu.renderMenu(*GameWindow::window);
        }

        if (nick) {
            nazwaU.renderNick(*GameWindow::window);
        }
        if (start){
            blocks.renderBlocks(*GameWindow::window);
            this->renderText(*GameWindow::window);
            player.renderPlayer(*GameWindow::window);
            ball.renderBall(*GameWindow::window);
            this->renderpauseT(*GameWindow::window);
        }
        if (scores){
            file.renderUsers(*GameWindow::window);
        }
        if (info){
            rules.renderRules(*GameWindow::window);
        }


        GameWindow::window->display();

    }


    void pollEvents() {
        while (GameWindow::window->pollEvent(this->event)) {
            switch (this->event.type) {
                case sf::Event::Closed:
                    GameWindow::window->close();
                    break;
                case sf::Event::KeyPressed:
                    if (this->event.key.code == sf::Keyboard::Escape || this->event.key.code == sf::Keyboard::P) {
                        pause = !pause;
                    }
                    if (this->event.key.code == sf::Keyboard::A) {
                        player.setLeft(true);
                    }
                    else if (this->event.key.code == sf::Keyboard::D) {
                        player.setRight(true);
                    }
                    if (nick) {
                        if (this->event.key.code == sf::Keyboard::Enter && nazwaU.nickOK()) {
                            nick = false;
                            cout << nazwaU.retNick();
                            menuGlowne = true;
                        }
                        else if (this->event.key.code == sf::Keyboard::Enter && !nazwaU.nickOK()) {
                            nazwaU.clear();
                        }
                    }
                    if (scores && this->event.key.code == sf::Keyboard::Enter) {
                        scores = false;
                        menuGlowne = true;
                    }
                    if (info && this->event.key.code == sf::Keyboard::Enter) {
                        info = false;
                        menuGlowne = true;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left && menuGlowne) {
                        if (menu.startPressed(mousePosV) && menuGlowne)
                            start = true;
                        else if (menu.setNickPressed(mousePosV)) {
                            nick = true;
                            nazwaU.clear();
                        }
                        else if (menu.setScoresPressed(mousePosV)) {
                            scores = true;
                        }
                        else if (menu.setClosePressed(mousePosV)) {
                            close = true;
                        }
                        else if (menu.arkanoidPressed(mousePosV)){
                            info = true;
                        }
                    }
                    break;
                case sf::Event::TextEntered:
                    if (nick && event.text.unicode < 128) {
                        char inputChar = static_cast<char>(event.text.unicode);
                        if (inputChar == '\b') {
                            nazwaU.usunZnak();
                        } else if (isprint(inputChar)) {
                            nazwaU.dodajZnak(inputChar);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }





    const bool getEndGame() const{
        return endGame;
    }

};

sf::RenderWindow* GameWindow::window = nullptr;

int main() {

    srand(static_cast<unsigned>(time(0)));

    Engine engine;

    while(engine.running()){

        engine.update();
        engine.render();

    }

    return 0;
}

