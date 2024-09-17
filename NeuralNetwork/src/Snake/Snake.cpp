#include "Snake.h"
#include "iostream"

Snake::Snake(const Snake& other)
{
    this->life = 100;// life must be variable
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 0;
    food = other.food;
    f_i = 0;
    net = other.net;
    input = VectorXd(24);
    size(0) = 120; //scr size must be variable
    size(1) = 30;
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody();
}

void Snake::eat()
{
    score++;
    f_i++;
    life += 100;
    createBody();
    if (f_i <= food->size())
        createFood();
}

void Snake::createFood()
{
    food->push_back(Eigen::Vector2i((int)rnd.getRand(30, 90), (int)rnd.getRand(10, 20)));
}

void Snake::createBody()
{
    if (body.size() > 0)
    {
        if (dir % 2)
            body.push_back(Eigen::Vector2i(body[body.size() - 1](0) - dir, body[body.size() - 1](1)));
        else
            body.push_back(Eigen::Vector2i(body[body.size() - 1](0), body[body.size() - 1](1) - dir));
    }
    else
    {
        if (dir % 2)
            body.push_back(Eigen::Vector2i(hLoc(0) - dir, hLoc(1)));
        else
            body.push_back(Eigen::Vector2i(hLoc(0), hLoc(1) - dir));
    }
}

Snake::Snake()
{
}

Snake::Snake(Network brain, std::vector<Eigen::Vector2i>* f, int life, int x, int y)
{
    this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 0;
    food = f;
    f_i = 0;
    net = brain;
    input = VectorXd(24);
    size(0) = x;
    size(1) = y;
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody();
}

Snake::Snake(std::vector<int> brain, std::vector<Eigen::Vector2i>* f, int life, int x, int y)
{
	this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 0;
    f_i = 0;
    food = f;
	net = Network(brain);
	input = VectorXd(24);
	size(0) = x;
	size(1) = y;
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody();
}

bool Snake::getDead()
{
    return dead;
}

double Snake::getFitness()
{
    fitness = log((1 + lifeTime + abs(hLoc.x() - 60) + abs(hLoc.y() - 60))/2 + std::pow(3, score*3));
    return fitness;
}

void Snake::Move()
{
    if (!dead)
    {
        Look();
        Think();
        lifeTime++;
        life--;
        if (foodCollide(hLoc))
            eat();
        shiftBody();
        dead = life <= 0 || bodyCollide(hLoc) || wallCollide(hLoc);
    }
}

void Snake::Draw(std::string* str)
{
    std::string scr(120 * 30, ' ');
    scr[(*food)[f_i](0) + (*food)[f_i](1) * size(0)] = 'F';
    scr[hLoc(0) + hLoc(1) * size(0)] = 'H';
    for (int i = 0; i < body.size(); i++)
    {
        scr[body[i](0) + body[i](1) * size(0)] = 'B';
    }
    *str = scr;
}

Snake Snake::crossover(Snake& other)
{
    Snake newS(net.crossover(other.net, 0.05,0.1), food, 50, size(0), size(1));
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
    return pos == (*food)[f_i];
}

bool Snake::wallCollide(Eigen::Vector2i pos)
{
    return (pos(0) >= size(0) - 1) || (pos(0) < 1) || (pos(1) >= size(1) - 1) || (pos(1) < 1);
}

Eigen::Vector3d Snake::lookInDirection(Eigen::Vector2i _dir)
{
    Eigen::Vector2i dir = relative(_dir, this->dir);
    Eigen::Vector3d look = Eigen::Vector3d::Zero();
    Eigen::Vector2i pos = hLoc;
    float distance = 0;
    float foodFound = false;
    float bodyFound = false;
    pos = pos + dir;
    distance++;
    while (!wallCollide(pos))
    {
        if (!foodFound && foodCollide(pos))
        {
            foodFound = distance; //input size can be decreased to 12
            look(0) = distance;
        }
        if (!bodyFound && bodyCollide(pos))
        {
            bodyFound = distance;
            look(1) = distance;
        }
        pos = pos + dir;
        distance++;
    }
    look[2] = distance;
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
    {
        int temp = rltVector(0);
        rltVector(0) = rltVector(1);
        rltVector(1) = temp;
        rltVector *= dir;
    }
    return rltVector;
}
