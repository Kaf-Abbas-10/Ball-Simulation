#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <iostream>

using namespace sf;
using namespace std;

// -----------------------------
// Utility
// -----------------------------
float randFloat(float min, float max) {
    static mt19937 rng(random_device{}());
    uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// -----------------------------
// Ball structure
// -----------------------------
struct Ball {
    CircleShape shape;
    Vector2f velocity;
    float mass;

    Ball(float mass, Vector2f pos, Vector2f vel)
        : mass(mass), velocity(vel)
    {
        float radius = sqrt(mass);     // radius = √mass
        shape.setRadius(radius);
        shape.setFillColor(Color::White);
        shape.setOrigin(radius, radius);
        shape.setPosition(pos);
    }

    void update(float dt, const RenderWindow& wnd) {
        Vector2f pos = shape.getPosition();
        Vector2u size = wnd.getSize();
        float r = shape.getRadius();

        pos += velocity * dt;

        // Bounce off walls
        if (pos.x - r < 0) {
            pos.x = r;
            velocity.x *= -1;
        }
        if (pos.x + r > size.x) {
            pos.x = size.x - r;
            velocity.x *= -1;
        }
        if (pos.y - r < 0) {
            pos.y = r;
            velocity.y *= -1;
        }
        if (pos.y + r > size.y) {
            pos.y = size.y - r;
            velocity.y *= -1;
        }

        shape.setPosition(pos);
    }
};

int main() {
    RenderWindow window(VideoMode(800, 600), "SFML Two Balls");

    Vector2f center(400, 300);

    // ----------- Ball 1 (2nd quadrant) ----------
    float m1 = randFloat(500, 2000);
    Vector2f pos1(center.x - randFloat(50, 200),
                  center.y - randFloat(50, 200));
    Vector2f vel1(randFloat(-100, 100), randFloat(-100, 100));
    Ball ball1(m1, pos1, vel1);

    // ----------- Ball 2 (4th quadrant) ----------
    float m2 = randFloat(500, 2000);
    Vector2f pos2(center.x + randFloat(50, 200),
                  center.y + randFloat(50, 200));
    Vector2f vel2(randFloat(-100, 100), randFloat(-100, 100));
    Ball ball2(m2, pos2, vel2);

    Clock clock;

    // --------------------- Main Loop ---------------------
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        ball1.update(dt, window);
        ball2.update(dt, window);

        window.clear(Color(20, 20, 20));
        window.draw(ball1.shape);
        window.draw(ball2.shape);
        window.display();
    }

    return 0;
}

