#include "Snake.h"
#include "iostream"

Snake::Snake(const Snake& other)
{
    this->life, this->start_life = other.start_life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    score = 3;
    f_i = 0;
    net = other.net;
    input = VectorXd(net.getInput());
    size = other.size;
    food = std::make_shared<Food>(size.x(), size.y());
    hLoc = size / 2;
    dir = UP;
    createBody(); createBody(); createBody(); createBody();
}

Snake& Snake::operator=(const Snake& other) 
{
    if (this != &other) {
        start_life = other.start_life;
        life = other.start_life;
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
    if (life < 2000 + score)
        life += 100 + score;
    createBody();
    do 
    {
        food->create();
    } while (bodyCollide(food->food) || wallCollide(food->food) || hLoc == food->food);
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
    : start_life(life), life(life), fitness(0), dead(false), lifeTime(0), score(3)
    , f_i(0), net(brain), input(brain.getInput()), size(x, y),
    hLoc(size / 2), dir(UP) {
    food = std::make_shared<Food>(size.x(), size.y());
    createBody(); createBody(); createBody(); createBody();
}

Snake::Snake(std::vector<int> brain, int life, int x, int y)
{
    start_life = life;
	this->life = life;
    fitness = 0;
    dead = false;
    lifeTime = 0;
    score = 3;
    f_i = 0;
    food = std::make_shared<Food>(size.x(), size.y());
	net = Network(brain);
	input = VectorXd(brain[0]);
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
        fitness = floor( lifeTime)/log(size(0)* size(1)) + pow(2, score);
    }
    else 
    {
        fitness = floor(lifeTime) / log(size(0) * size(1));
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
    // Define the frame dimensions
    int width = size(0);
    int height = size(1);

    // Create the screen with the frame dimensions
    std::string scr(width * height, ' ');

    // Draw the frame (using single-line characters)
    for (int y = 0; y < height; ++y)
    {
        scr[y * width] = '\xB3'; // Left edge (│)
        scr[(y + 1) * width - 1] = '\xB3'; // Right edge (│)
    }
    for (int x = 0; x < width; ++x)
    {
        scr[x] = '\xC4'; // Top edge (─)
        scr[x + (height - 1) * width] = '\xC4'; // Bottom edge (─)
    }

    // Corners
    scr[0] = '\xDA'; // Top-left corner (┌)
    scr[width - 1] = '\xBF'; // Top-right corner (┐)
    scr[(height - 1) * width] = '\xC0'; // Bottom-left corner (└)
    scr[height * width - 1] = '\xD9'; // Bottom-right corner (┘)

    // Draw the food
    scr[(food->food)(0) + (food->food)(1) * width] = '#';

    // Draw the head
    scr[hLoc(0) + hLoc(1) * width] = '\xFE'; // Character chosen for the head

    // Draw the body
    for (int i = 0; i < body.size(); i++)
    {
        char part = 'o';

        if (i == 0)
        {
            auto diff = hLoc - body[i];
            if (abs(diff.x()) == 1) part = '\xCD';          // Horizontal (═)
            else if (abs(diff.y()) == 1) part = '\xBA';    // Vertical (║)
        }
        else
        {
            auto prevDiff = body[i - 1] - body[i];
            auto nextDiff = (i + 1 < body.size()) ? (body[i + 1] - body[i]) : (body[i] - body[i - 1]);

            if ((prevDiff.x() == 1 && nextDiff.y() == 1) || (prevDiff.y() == 1 && nextDiff.x() == 1))
                part = '\xC9'; // Top-left corner (╔)
            else if ((prevDiff.x() == -1 && nextDiff.y() == 1) || (prevDiff.y() == 1 && nextDiff.x() == -1))
                part = '\xBB'; // Top-right corner (╗)
            else if ((prevDiff.x() == 1 && nextDiff.y() == -1) || (prevDiff.y() == -1 && nextDiff.x() == 1))
                part = '\xC8'; // Bottom-left corner (╚)
            else if ((prevDiff.x() == -1 && nextDiff.y() == -1) || (prevDiff.y() == -1 && nextDiff.x() == -1))
                part = '\xBC'; // Bottom-right corner (╝)
            else if (prevDiff.x() != 0 || nextDiff.x() != 0)
                part = '\xCD'; // Horizontal line (═)
            else if (prevDiff.y() != 0 || nextDiff.y() != 0)
                part = '\xBA'; // Vertical line (║)
        }

        scr[body[i](0) + body[i](1) * width] = part;
    }

    // Draw the score and maximum score (placed at the top of the screen)
    if (scr.size() / size(0) > 9)
    {
        scr[2] = ((score / 100) % 10) + '0';
        scr[3] = ((score / 10) % 10) + '0';
        scr[4] = (score % 10) + '0';

        scr[6] = ((maxScore / 100) % 10) + '0';
        scr[7] = ((maxScore / 10) % 10) + '0';
        scr[8] = (maxScore % 10) + '0';
    }

    // Return the drawing
    *str = scr;
}

Snake Snake::crossover(Snake& other, double mutationRate, double mutationStrength)
{
    Snake newS(net.crossover(other.net, mutationRate, mutationStrength), other.start_life, size(0), size(1));
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
            foodFound = true; 
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


// Get Snake Relative Direction
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
