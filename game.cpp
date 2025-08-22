#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

const int ROWS = 20;
const int COLS = 10;
const int TILE_SIZE = 30;

class Block {
public:
    sf::RectangleShape shape;
    Block(int x, int y, sf::Color color) {
        shape.setSize(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
        shape.setFillColor(color);
        shape.setPosition(sf::Vector2f(float(x * TILE_SIZE), float(y * TILE_SIZE)));
    }
};

class Piece {
public:
    std::vector<Block> blocks;
    int offsetX, offsetY;

    Piece(std::vector<std::pair<int,int>> coords, sf::Color color) {
        offsetX = COLS / 2 - 2;
        offsetY = 0;
        for (auto [x,y] : coords)
            blocks.push_back(Block(x + offsetX, y + offsetY, color));
    }

    void move(int dx, int dy, const std::vector<std::vector<int>>& grid) {
        if (!canMove(dx, dy, grid)) return;
        offsetX += dx;
        offsetY += dy;
        for (auto& block : blocks)
            block.shape.move(sf::Vector2f(float(dx * TILE_SIZE), float(dy * TILE_SIZE)));
    }

    bool canMove(int dx, int dy, const std::vector<std::vector<int>>& grid) {
        for (auto& block : blocks) {
            sf::Vector2f pos = block.shape.getPosition();
            int x = int(pos.x / TILE_SIZE) + dx;
            int y = int(pos.y / TILE_SIZE) + dy;
            if (x < 0 || x >= COLS || y >= ROWS) return false;
            if (y >= 0 && grid[y][x]) return false;
        }
        return true;
    }
};

class Game {
    sf::RenderWindow window;
    std::vector<std::vector<int>> grid;
    std::vector<Block> settledBlocks;
    Piece* currentPiece;
    float fallTimer, fallDelay;
    bool dragging;
    int dragStartX;

public:
    Game() 
        : window(sf::VideoMode(sf::Vector2u(COLS*TILE_SIZE, ROWS*TILE_SIZE)), "Falling Puzzle"),
          grid(ROWS, std::vector<int>(COLS, 0)),
          currentPiece(nullptr), fallTimer(0), fallDelay(0.5f),
          dragging(false), dragStartX(0) 
    {
        srand(unsigned(time(nullptr)));
        spawnPiece();
    }

    void spawnPiece() {
        std::vector<std::vector<std::pair<int,int>>> shapes = {
            {{0,0},{1,0},{2,0},{3,0}}, // I
            {{0,0},{0,1},{1,0},{1,1}}, // O
            {{0,0},{1,0},{2,0},{2,1}}, // L
            {{1,0},{0,1},{1,1},{2,1}}  // T
        };
        /*std::vector<sf::Color> colors = {
            sf::Color::Cyan, sf::Color::Yellow, sf::Color::Blue, sf::Color::Magenta
        };*/
        int idx = rand() % shapes.size();
        currentPiece = new Piece(shapes[idx], /*colors[idx]*/sf::Color::Cyan);
    }

    void lockPiece() {
        bool gameOver = false;
        for (auto& block : currentPiece->blocks) {
            sf::Vector2f pos = block.shape.getPosition();
            int x = int(pos.x / TILE_SIZE);
            int y = int(pos.y / TILE_SIZE);

            if (y >= 0 && y < ROWS) {
                grid[y][x] = 1;
                settledBlocks.push_back(block);
            }

            if (y == 0) gameOver = true;
        }

        delete currentPiece;
        currentPiece = nullptr;

        if (gameOver) {
            showGameOver();
            window.close();
            return;
        }

        clearLines();
        spawnPiece();
    }

    void clearLines() {
        for (int y = ROWS-1; y >= 0; y--) {
            bool full = true;
            for (int x = 0; x < COLS; x++)
                if (!grid[y][x]) { full = false; break; }

            if (full) {
                for (int x = 0; x < COLS; x++) grid[y][x] = 0;
                settledBlocks.erase(
                    std::remove_if(settledBlocks.begin(), settledBlocks.end(),
                        [y](Block& b){ return int(b.shape.getPosition().y/TILE_SIZE) == y; }),
                    settledBlocks.end()
                );
                for (auto& b : settledBlocks) {
                    if (b.shape.getPosition().y < y * TILE_SIZE)
                        b.shape.move(sf::Vector2f(0, float(TILE_SIZE)));
                }
                for (int ty = y; ty > 0; ty--)
                    grid[ty] = grid[ty-1];
                y++; // recheck this row
            }
        }
    }

    void showGameOver() {
        sf::Font font;
        if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) return;

        sf::Text text(font, "GAME OVER", 50);
        text.setFillColor(sf::Color::Red);
        text.setPosition(sf::Vector2f(float(COLS*TILE_SIZE/2 - 150), float(ROWS*TILE_SIZE/2 - 50)));

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
        sf::sleep(sf::seconds(3)); // show for 3 seconds
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            fallTimer += dt;

            // Event handling
            while (auto eventOpt = window.pollEvent()) {
                sf::Event event = *eventOpt;

                if (event.is<sf::Event::Closed>()) window.close();
                if (event.is<sf::Event::MouseButtonPressed>()) {
                    dragging = true;
                    dragStartX = sf::Mouse::getPosition(window).x;
                }
                if (event.is<sf::Event::MouseButtonReleased>()) {
                    dragging = false;
                }
            }

            // Mouse drag
            if (dragging && currentPiece) {
                int mouseX = sf::Mouse::getPosition(window).x;
                int dx = (mouseX - dragStartX) / TILE_SIZE;
                if (dx != 0) {
                    currentPiece->move(dx, 0, grid);
                    dragStartX = mouseX;
                }
            }

            // Automatic fall
            if (fallTimer >= fallDelay && currentPiece) {
                if (currentPiece->canMove(0, 1, grid)) {
                    currentPiece->move(0, 1, grid);
                } else {
                    lockPiece();
                }
                fallTimer = 0;
            }

            // Draw
            window.clear(sf::Color::Black);
            for (auto& b : settledBlocks) window.draw(b.shape);
            if (currentPiece)
                for (auto& b : currentPiece->blocks) window.draw(b.shape);
            window.display();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}