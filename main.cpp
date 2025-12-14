#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <iostream>
using namespace sf;
using namespace std;


// -----------------------------
// Ball structure
// -----------------------------
struct Ball {
    CircleShape shape;
    Vector2f velocity;
    float mass, radius;
    Ball(float mass, Vector2f pos, Vector2f vel)
        : mass(mass), velocity(vel)
    {
        radius = sqrt(mass);     // radius = √mass
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

// -----------------------------
// Utility
// -----------------------------
float randFloat(float min, float max) {
    static mt19937 rng(random_device{}());
    uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
// -----------------------------
// Distance function
// -----------------------------
float distance(const Vector2f& a, const Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
// -----------------------------
// Momentum calculation
// -----------------------------
Vector2f momentum(float mass, const Vector2f& velocity) {
    return mass * velocity;
}
// -----------------------------
// Elastic collision velocity update
// -----------------------------
void resolveCollision(Ball& ball1, Ball& ball2) {
    Vector2f pos1 = ball1.shape.getPosition();
    Vector2f pos2 = ball2.shape.getPosition();
    
    // Normal vector from ball1 to ball2
    Vector2f normal = pos2 - pos1;
    float dist = sqrt(normal.x * normal.x + normal.y * normal.y);
    
    if (dist == 0) return; // Prevent division by zero
    
    normal /= dist; // Normalize
    
    // Relative velocity
    Vector2f relVel = ball1.velocity - ball2.velocity;
    
    // Velocity component along collision normal
    float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;
    
    // Don't resolve if velocities are separating
    if (velAlongNormal > 0) return;
    
    // Restitution (1.0 = perfectly elastic)
    float restitution = 1.0f;
    
    // Calculate impulse scalar
    float impulse = -(1 + restitution) * velAlongNormal;
    impulse /= (1 / ball1.mass + 1 / ball2.mass);
    
    // Apply impulse
    Vector2f impulseVec = impulse * normal;
    ball1.velocity += impulseVec / ball1.mass;
    ball2.velocity -= impulseVec / ball2.mass;
    
    // Separate balls to prevent overlap
    float overlap = (ball1.radius + ball2.radius) - dist;
    if (overlap > 0) {
        Vector2f separation = normal * (overlap / 2.0f);
        ball1.shape.setPosition(pos1 - separation);
        ball2.shape.setPosition(pos2 + separation);
    }
}

// -----------------------------
// Collision detection
// -----------------------------
bool collision(Ball& ball1, Ball& ball2) {
    float d = distance(ball1.shape.getPosition(), ball2.shape.getPosition());
    return d <= (ball1.radius + ball2.radius);
}
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
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Q))
                window.close();
        }
        float dt = clock.restart().asSeconds();
        ball1.update(dt, window);
        ball2.update(dt, window);
        // Collision check and resolution
        if (collision(ball1, ball2)) {
            resolveCollision(ball1, ball2);
            
            // Display momentum information
            Vector2f p1 = momentum(ball1.mass, ball1.velocity);
            Vector2f p2 = momentum(ball2.mass, ball2.velocity);
            Vector2f totalP = p1 + p2;
            
            cout << "Collision! Total momentum: (" 
                 << totalP.x << ", " << totalP.y << ")" << endl;
        }
        window.clear(Color(20, 20, 20));
        window.draw(ball1.shape);
        window.draw(ball2.shape);
        window.display();
    }
    return 0;
}
