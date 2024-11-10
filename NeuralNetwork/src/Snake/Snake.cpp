#include "Snake.h"
#include "iostream"

Snake::Snake(const Snake& other)
{
    this->life = 200;// life must be variable
    fitness = 0;
    dead = false;
    lifeTime = 0;
    score = 3;
    f_i = 0;
    net = other.net;
    input = VectorXd(24);
    size = other.size;
    food = std::make_shared<Food>(size.x(), size.y());
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody(); createBody();
}

Snake& Snake::operator=(const Snake& other) 
{
    if (this != &other) {
        life = other.life;
        fitness = other.fitness;
        dead = other.dead;
        lifeTime = other.lifeTime;
        score = other.score;
        f_i = other.f_i;
        net = other.net;
        input = other.input;
        size = other.size;
        hLoc = other.hLoc;
        dir = other.dir;
        food = std::make_unique<Food>(*other.food); // Deep copy
        body = other.body;
        decision = other.decision;
    }
    return *this;
}

void Snake::eat()
{
    score++;
    f_i++;
    if (life < 500)
        life += 100;
    createBody();
    do 
    {
        food->create();
    } while (bodyCollide(food->food));
}

void Snake::createBody()
{
    Eigen::Vector2i lastSegment = (body.size() > 0) ? body.back() : hLoc;
    body.push_back(lastSegment + ((dir % 2) ? Eigen::Vector2i(-dir, 0) : Eigen::Vector2i(0, -dir)));
}

Snake::Snake()
{
}

Snake::Snake(Network brain, int life, int x, int y)
    : life(life), fitness(0), dead(false), lifeTime(0), score(3)
    , f_i(0), net(brain), input(VectorXd(24)), size(x, y),
    hLoc(size / 2), dir(UP) {
    food = std::make_shared<Food>(size.x(), size.y());
    createBody(); createBody(); createBody(); createBody();
}

Snake::Snake(std::vector<int> brain, int life, int x, int y)
{
	this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    score = 3;
    f_i = 0;
    food = std::make_shared<Food>(size.x(), size.y());
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
    if (score < 10) {
        fitness = floor(lifeTime * lifeTime) + pow(2, score);
    }
    else {
        fitness = floor(lifeTime * lifeTime);
        fitness += pow(2, 10);
        fitness *= (score - 9);
    }

    return (fitness);
}

int Snake::getScore()
{
    return score;
}

void Snake::Move()
{
    if (dead) return;

    Look();
    Think();
    lifeTime++;
    life--;

    if (foodCollide(hLoc))
        eat();
    shiftBody();
    dead = life <= 0 || bodyCollide(hLoc) || wallCollide(hLoc);
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

Snake Snake::crossover(Snake& other, double mutationRate, double mutationStrength)
{
    Snake newS(net.crossover(other.net, mutationRate, mutationStrength), 200, size(0), size(1));
    return newS;
}

void Snake::shiftBody()
{
    for (int i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    body[0] = hLoc;
    hLoc += relative(Eigen::Vector2i(0, 1), dir);
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
            look(0) = 1 / (distance);
        }
        if (!bodyFound && bodyCollide(pos))
        {
            bodyFound = true;
            look(1) =  1 / (distance);
        }
        pos = pos + dir;
        distance++;
    }
    look[2] = 1 / (distance);
    return look;
}

void Snake::Look()
{
    std::vector<Eigen::Vector2i> directions = {
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}
    };
    for (int i = 0; i < directions.size(); i++) {
        input.segment(i * 3, 3) = lookInDirection(directions[i]);
    }
}

void Snake::Think()
{
    int maxIndex;
    net.calc(input);
    decision = net.getOut();
    decision.maxCoeff(&maxIndex);

    static std::array<void(Snake::*)(), 3> moves = { &Snake::moveUp, &Snake::moveRight, &Snake::moveLeft };
    (this->*moves[maxIndex])();
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
