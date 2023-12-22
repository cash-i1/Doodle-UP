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
    float textureScale;
};

struct Platform {
    Vector2 position;
    Color color;
    Vector2 size;
};

vector<Platform> platforms;

bool isOnGround(Player p, bool check_collision = false, Rectangle col = Rectangle()) {
    if (p.position.y + p.texture.height * p.textureScale >= HEIGHT) {
        return true;
    } else {
        if (check_collision) {
            for (int platform = 0; platform < platforms.size(); platform++) {
                Rectangle platform_rect = Rectangle();
                platform_rect.x = platforms[platform].position.x;
                platform_rect.y = platforms[platform].position.y;
                platform_rect.width = platforms[platform].size.x;
                platform_rect.height = platforms[platform].size.y;

                if (CheckCollisionRecs(col, platform_rect) && col.y+1 < platforms[platform].position.y) {
                    cout << col.y << ", " << platforms[platform].position.y << endl;
                    return true;
                }
            }
            
        } else {
            return false;
        }
    }
}


float platformYOffset = 0.0;

void genPlatforms() {
    Platform newPlatform = Platform();

    newPlatform.color = RED;
    newPlatform.position.y = -platformYOffset + 1000;
    newPlatform.position.x = GetRandomValue(0, 400);
    newPlatform.size.x = 80;
    newPlatform.size.y = 20;

    platforms.push_back(newPlatform);

    platformYOffset += 100;
}

void drawPlatforms() {
    for (int platform = 0; platform < platforms.size(); platform++) {
        DrawRectangle(platforms[platform].position.x, platforms[platform].position.y, platforms[platform].size.x, platforms[platform].size.y, RED);

        // if (GetRandomValue(0,10)) {
        //     DrawRectangle(platforms[platform].position.x, platforms[platform].position.y - 30, 20, 20, GREEN);
        //     vector<Platform>::iterator it = platforms.begin() + 2;
        //     platforms.erase(it);
        // }
    }
}


int main() {
    InitWindow(WIDTH, HEIGHT, "Doodle UP!");
    InitAudioDevice();
    
    Sound sound = LoadSound("assets/sincity.mp3");
    PlaySound(sound);

    Sound fart = LoadSound("assets/fart.mp3");

    Player p = Player();

    p.texture = LoadTexture("assets/player_1.png");
    p.velocity.y = 1;
    p.speed = Vector2 {2.0, 2.0};
    p.jumpForce = 420;
    p.textureScale = 6;
    Rectangle playerCollision;

    int time;
    const char* debug_info;

    Camera2D cam;

    int score;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        playerCollision = {p.position.x, 180 + p.position.y, p.texture.width*p.textureScale, p.texture.height*p.textureScale - 180};

        if (!isOnGround(p, true, playerCollision)) {
            p.velocity.y += GRAV * delta;
            p.position.y += p.velocity.y * delta;        
            
        } else {
            if (IsKeyDown(KEY_SPACE)) {
                PlaySound(fart);
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

        debug_info = TextFormat("v.y: %f\n\nv.x: %f", p.velocity.y, p.velocity.x);

        Rectangle test = Rectangle();
        test.width = 100;
        test.height = 100;
        test.x = 30;
        test.y = 700;

        cam.zoom = 1.0f;
        if (p.position.y < HEIGHT/2) {
            cam.target.y = p.position.y - (GetScreenHeight()/2);
        } else {
            // cam.target.y += 1;
        }
        
        cam.target.x = p.position.x - GetScreenWidth()/2 + (p.texture.width*p.textureScale) / 2;
        cam.rotation = 0.0f;

        genPlatforms();

        // cout << p.position.x << ", " << p.position.y << "; " << cam.target.x << ", " << cam.target.y << endl;
        
        if ((int)p.position.y % 100 == 0) {
            score += 1;
        }

        BeginDrawing();

        
            BeginMode2D(cam);
            ClearBackground(BLUE);

            DrawTextureEx(p.texture, p.position, 0, p.textureScale, WHITE);
            DrawRectangleRec(playerCollision, GREEN);
            drawPlatforms();

            
            
            EndMode2D();

            // DrawText(debug_info, 10, 34, 40, GREEN);
            
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, GREEN);
        EndDrawing();
        
    }
}