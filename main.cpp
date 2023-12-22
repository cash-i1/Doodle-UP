#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

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

struct Platform {
    Vector2 position;
    Color color;
    Vector2 size;
};

vector<Platform> platforms;

bool isOnGround(Player p, bool check_collision = false, Rectangle col = Rectangle()) {
    if (p.position.y + p.texture.height * 6 >= HEIGHT) {
        return true;
    } else {
        if (check_collision) {
            for (int platform = 0; platform < platforms.size(); platform++) {
                Rectangle platform_rect = Rectangle();
                platform_rect.x = platforms[platform].position.x;
                platform_rect.y = platforms[platform].position.y;
                platform_rect.width = platforms[platform].size.x;
                platform_rect.height = platforms[platform].size.y;

                if (CheckCollisionRecs(col, platform_rect)) {
                    return true;
                }
            }
            
        } else {
            return false;
        }
    }
}


float platformYOffset = 0;

void genPlatforms() {
    Platform newPlatform = Platform();

    newPlatform.color = RED;
    newPlatform.position.y = platformYOffset + 100;
    newPlatform.position.x = GetRandomValue(0, 400);
    newPlatform.size.x = 80;
    newPlatform.size.y = 20;

    platforms.push_back(newPlatform);

    platformYOffset += 100;
}

void drawPlatforms() {
    for (int platform = 0; platform < platforms.size(); platform++) {
        DrawRectangle(platforms[platform].position.x, platforms[platform].position.y, platforms[platform].size.x, platforms[platform].size.y, RED);
    }
}


int main() {
    InitWindow(WIDTH, HEIGHT, "Doodle UP!");

    
    Player p = Player();

    p.texture = LoadTexture("assets/player_1.png");
    p.velocity.y = 1;
    p.speed = Vector2 {2.0, 2.0};
    p.jumpForce = 500;
    Rectangle playerCollision;

    int time;
    const char* debug_info;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        playerCollision = {p.position.x, p.position.y, p.texture.width*(float)6.0, 10};

        if (!isOnGround(p, true, playerCollision)) {
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

        Rectangle test = Rectangle();
        test.width = 100;
        test.height = 100;
        test.x = 30;
        test.y = 700;

        genPlatforms();


        BeginDrawing();
            ClearBackground(BLUE);

            DrawTextureEx(p.texture, p.position, 0, 6.0f, WHITE);
            DrawRectangleRec(test, RED);
            drawPlatforms();

            DrawText(debug_info, 10, 34, 40, GREEN);
        EndDrawing();
    }
}