#include "Snake.h"
#include "iostream"

Snake::Snake(Network brain, int life, int x, int y)
{
    this->life = life;
    net = brain;
    input = VectorXd(24);
    size(0) = x;
    size(1) = y;
    food(0) = rnd.getRand(1, x - 1);
    food(1) = rnd.getRand(1, y - 1);
    hLoc = size / 2;
}

Snake::Snake(std::vector<int> brain, int life, int x, int y)
{
	this->life = life;
	net = Network(brain);
	input = VectorXd(24);
	size(0) = x;
	size(1) = y;
	food(0) = rnd.getRand(1, x - 1);
	food(1) = rnd.getRand(1, y - 1);
    hLoc = size / 2;
}

bool Snake::getDead()
{
    return dead;
}

void Snake::Move()
{
    if (!dead)
    {
        Look();
        Think();
        lifeTime++;
        life--;
        //if (foodCollide(hLoc))
            //eat(); eat function
        shiftBody();
        dead = life <= 0 || bodyCollide(hLoc) || wallCollide(hLoc);
    }
}

void Snake::Draw(std::string* str)
{
    std::string scr(120 * 30, ' ');
    scr[hLoc(0) + hLoc(1) * size(0)] = 'H';
    for (int i = 0; i < body.size(); i++)
    {
        scr[body[i](0) + body[i](1) * size(0)] = 'B';
    }
    *str = scr;
}

Snake Snake::crossover(Snake& other)
{
    Snake newS(net.crossover(other.net), life + lifeTime, size(0), size(1));
    return newS;
}

void Snake::shiftBody()
{
    Eigen::Vector2i temp = hLoc;
    hLoc += relative(Eigen::Vector2i(0, 1), dir);
    Eigen::Vector2i temp2;
    for (int i = 0; i < body.size(); i++)
    {
        temp2 = body[i];
        body[i] = temp;
        temp = temp2;
    }
}

bool Snake::bodyCollide(Eigen::Vector2i pos)
{
    for (int i = 0; i < body.size(); i++)
    {
        if (pos == body[i])
            return true;
    }
    return false;
}

bool Snake::foodCollide(Eigen::Vector2i pos)
{
    return pos == food;
}

bool Snake::wallCollide(Eigen::Vector2i pos)
{
    return (pos(0) >= size(0) - 1) || (pos(0) < 1) || (pos(1) >= size(1) - 1) || (pos(1) < 1);
}

Eigen::Vector3d Snake::lookInDirection(Eigen::Vector2i dir)
{
    dir = relative(dir, this->dir);
    Eigen::Vector3d look = Eigen::Vector3d::Zero();
    Eigen::Vector2i pos = hLoc;
    float distance = 0;
    bool foodFound = false;
    bool bodyFound = false;
    pos = pos + dir;
    distance++;
    while (!wallCollide(pos))
    {
        if (!foodFound && foodCollide(pos))
        {
            foodFound = true;
            look(0) = 1;
        }
        if (!bodyFound && bodyCollide(pos))
        {
            bodyFound = true;
            look(1) = 1;
        }
        pos = pos + dir;
        distance++;
    }
    look[2] = 1 / distance;
    return look;
}

void Snake::Look()
{
    Eigen::VectorXd temp = lookInDirection(Eigen::Vector2i(-1/*SIZE*/, 0));
    input.segment(0, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(-1/*SIZE*/, -1/*SIZE*/));
    input.segment(3, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(0, -1/*SIZE*/));
    input.segment(6, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(1/*SIZE*/, -1/*SIZE*/));
    input.segment(9, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(1/*SIZE*/, 0));
    input.segment(12, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(1/*SIZE*/, 1/*SIZE*/));
    input.segment(15, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(0, 1/*SIZE*/));
    input.segment(18, 3) = temp;

    temp = lookInDirection(Eigen::Vector2i(-1/*SIZE*/, 1/*SIZE*/));
    input.segment(21, 3) = temp;
}

void Snake::Think()
{
    net.calc(input);
    decision = net.getOut();
    int maxIndex = 0;
    decision.maxCoeff(&maxIndex);

    switch (maxIndex) {
    case 0:
        moveUp();
        break;
    case 1:
        moveRight();
        break;
    case 2:
        moveLeft();
        break;
    case 3:
        switch ((int)rnd.getRand(0, 2.99))
        {
        case 0:
            moveUp();
            break;
        case 1:
            moveRight();
            break;
        case 2:
            moveLeft();
            break;
        }
        break;
    }
}

void Snake::moveUp()
{
    //there is nothing to do :D
}

void Snake::moveLeft()
{
    if (!(dir % 2))
        dir = DIR(dir / -2);
    else
        dir = DIR(dir * 2);
}

void Snake::moveRight()
{
    if (!(dir % 2))
        dir = DIR(dir / 2);
    else
        dir = DIR(dir * -2);
}

Eigen::Vector2i Snake::relative(Eigen::Vector2i vec, DIR dir)
{
    Eigen::Vector2i rltVector = vec;
    
    if (!(dir % 2))
        rltVector *= dir / 2;
    else
        rltVector = rltVector.transpose() * -1;
    return rltVector;
}
