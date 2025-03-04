#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

class Obstacle
{
public:
    static int speed;
    static int MaxSpeed;

    Obstacle(bool isBird = false);
    void update();

    int getX() const { return x; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool getIsBird() const { return isBird; }

private:
    int x;
    int width;
    int height;
    bool isBird;
};

#endif // OBSTACLE_HPP
