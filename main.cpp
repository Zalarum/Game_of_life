#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;

int main()
{
    const int width = 100;
    const int height = 50;
    const int sizeTile = 10;
    bool world[width][height] = {0};
    bool nextStep[width][height] = {0};
    int neighbours = 0;
    int Xpos, Ypos;
    float game_speed = 0.02;

    RectangleShape tileBlack(Vector2f(sizeTile, sizeTile));
    tileBlack.setFillColor(Color::Black);

    RenderWindow window(VideoMode(width * sizeTile, height * sizeTile), "Life");

    Clock clock; float time;

    while (window.isOpen())
    {
        time = clock.getElapsedTime().asSeconds();

        Event event;

        while (window.pollEvent(event)) if (event.type == Event::Closed) window.close();

        Vector2i pixelPos = Mouse::getPosition(window);
        Vector2f pos = window.mapPixelToCoords(pixelPos);

        Xpos = pos.x / sizeTile;
        Ypos = pos.y / sizeTile;

        if (Keyboard::isKeyPressed(Keyboard::R))
        {
            std::default_random_engine generator(time);
            std::uniform_int_distribution<int> x1(0, 99);
            std::uniform_int_distribution<int> y1(0, 49);

            for (int i = 0; i < (width * height) / 10; ++i)
            {
                world[x1(generator)][y1(generator)] = true;
            }
        }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            world[Xpos][Ypos] = true;
            nextStep[Xpos][Ypos] = true;
        }

        if (Mouse::isButtonPressed(Mouse::Right))
        {
            world[Xpos][Ypos] = false;
            nextStep[Xpos][Ypos] = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::C))
        {
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    world[i][j] = false; // если убрать будут кометы
                    nextStep[i][j] = false;
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && game_speed <= time)
        {
            clock.restart();

            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    neighbours = world[(i - 1 + width) % width][(j + 1 + height) % height] + world[i][(j + 1 + height) % height] + world[(i + 1 + width) % width][(j + 1 + height) % height] +
                                 world[(i - 1 + width) % width][j] + world[(i + 1 + width) % width][j] + world[(i - 1 + width) % width][(j - 1 + height) % height] +
                                 world[i][(j - 1 + height) % height] + world[(i + 1 + width) % width][(j - 1 + height) % height];
                    
                    if (world[i][j] == true && (neighbours < 2 || neighbours > 3)) nextStep[i][j] = false;
                    else if (world[i][j] == false && neighbours == 3) nextStep[i][j] = true;
                }
            }

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    world[i][j] = nextStep[i][j];
                }
            }
        }

        window.clear(Color::White);

        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                if (world[i][j] == true)
                {
                    tileBlack.setPosition(Vector2f(i * sizeTile, j * sizeTile));
                    window.draw(tileBlack);
                }
            }
        }

        window.display();
    }

    return 0;
}