#include "Snake.h"
#include "iostream"

Snake::Snake(const Snake& other)
{
    this->life = 100;// life must be variable
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 3;
    f_i = 0;
    net = other.net;
    input = VectorXd(24);
    size = other.size;
    food = new Food(size.x(), size.y());
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody(); createBody();
}

void Snake::eat()
{
    score++;
    f_i++;
    if (life < 300)
        life += 200;
    createBody();
    do 
    {
        food->create();
    } while (bodyCollide(food->food));
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

Snake::Snake(Network brain, int life, int x, int y)
{
    this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 3;
    food = new Food(x, y);
    f_i = 0;
    net = brain;
    input = VectorXd(24);
    size(0) = x;
    size(1) = y;
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody(); createBody();
}

Snake::Snake(std::vector<int> brain, int life, int x, int y)
{
	this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    lastTime = 0;
    score = 3;
    f_i = 0;
    food = new Food(x, y);
	net = Network(brain);
	input = VectorXd(24);
	size(0) = x;
	size(1) = y;
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody(); createBody();
}

bool Snake::getDead()
{
    return dead;
}

double Snake::getFitness()
{
    fitness = log(pow(3, score) * lifeTime);
    return fitness;
}

int Snake::getScore()
{
    return score;
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

void Snake::Draw(std::string* str, int maxScore)
{
    std::string scr(size(0) * size(1), ' ');
    scr[(food->food)(0) + (food->food)(1) * size(0)] = 'F';
    scr[hLoc(0) + hLoc(1) * size(0)] = 'H';
    for (int i = 0; i < body.size(); i++)
    {
        scr[body[i](0) + body[i](1) * size(0)] = 'B';
    }
    scr[0] = ((score / 100) % 10) + '0';
    scr[1] = ((score / 10) % 10) + '0';
    scr[2] = (score % 10) + '0';

    scr[4] = ((maxScore / 100) % 10) + '0';
    scr[5] = ((maxScore / 10) % 10) + '0';
    scr[6] = (maxScore % 10) + '0';
    *str = scr;
}

Snake Snake::crossover(Snake& other)
{
    Snake newS(net.crossover(other.net, 0.1,0.1), 200, size(0), size(1));
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
    return pos == (food->food);
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
            foodFound = true; //input size can be decreased to 12
            look(0) = 1 / distance;
        }
        if (!bodyFound && bodyCollide(pos))
        {
            bodyFound = true;
            look(1) = 1 / distance;
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
