#include <raylib.h>
#include <raymath.h>
#include <iostream>

using namespace std;

const int HEIGHT = 800;
const int WIDTH = 400;
const float GRAV = 1050;

struct Player {
    Texture2D texture;
    Vector2 position;
    Vector2 velocity;
    Vector2 speed;
    float jumpForce;
};

bool isOnGround(Player p) {
    if (p.position.y + p.texture.height * 6 >= HEIGHT) {
        return true;
    }
    else { return false; }
}


int main() {
    InitWindow(WIDTH, HEIGHT, "Doodle UP!");


    Player p = Player();

    p.texture = LoadTexture("assets/player_1.png");
    p.velocity.y = 1;
    p.speed = Vector2 {2.0, 2.0};
    p.jumpForce = 500;
    
    int time;
    const char* debug_info;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        if (!isOnGround(p)) {
            p.velocity.y += GRAV * delta;
            p.position.y += p.velocity.y * delta;        
            
        } else {
            if (IsKeyDown(KEY_SPACE)) {
                p.velocity.y = -p.jumpForce;
            } else {
                p.velocity.y = 0;
            }
            
        }

        p.position.y += p.velocity.y * delta;   
        
        if (IsKeyDown(KEY_A)) {
            p.position.x -= 220.0 * p.speed.x * delta;
        }
        if (IsKeyDown(KEY_D)) {
            p.position.x += 220.0 * p.speed.x * delta;
        }

        debug_info = TextFormat("v.y: %d\n\nv.x: %d", p.velocity.y, p.velocity.x);

        cout << p.velocity.y << endl;

        BeginDrawing();
            ClearBackground(BLUE);

            DrawTextureEx(p.texture, p.position, 0, 6.0f, WHITE);

            DrawText(debug_info, 10, 34, 40, GREEN);
        EndDrawing();
    }
}