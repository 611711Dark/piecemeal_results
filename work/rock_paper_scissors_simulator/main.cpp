#include <opencv2/opencv.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace cv;
using namespace std;

const float PI = 3.141592653589793f;

enum GameState {
    WAITING,    // 等待空格键开始
    RUNNING,    // 模拟运行中
    GAME_OVER   // 游戏结束
};

struct Circle {
    Point2f center;
    float radius;
    int type; // 0=rock, 1=paper, 2=scissors
    Vec3b color;
    bool active = true;
};

// 音频相关变量
Mix_Chunk* collisionSound = nullptr;

bool initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << endl;
        return false;
    }

    collisionSound = Mix_LoadWAV("collision.wav");
    if (!collisionSound) {
        cerr << "Failed to load collision sound effect! Mix_Error: " << Mix_GetError() << endl;
        return false;
    }

    return true;
}

void closeAudio() {
    if (collisionSound) Mix_FreeChunk(collisionSound);
    Mix_CloseAudio();
    SDL_Quit();
}

float calculateSmallRadius() {
    return 250.0f * (2.0f * sqrt(3.0f) - 3.0f);
}

float calculateCenterDistance(float smallRadius) {
    return 250.0f - smallRadius;
}

vector<Point2f> calculateInitialPositions(float d) {
    vector<Point2f> positions;
    Point2f center(400, 300);
    
    float angles[] = {0.0f, 120.0f, 240.0f};
    
    for (float angle : angles) {
        float rad = angle * PI / 180.0f;
        positions.push_back(Point2f(
            center.x + d * cos(rad),
            center.y + d * sin(rad)
        ));
    }
    
    return positions;
}

bool checkCollision(const Circle& a, const Circle& b) {
    if (!a.active || !b.active) return false;
    float dx = a.center.x - b.center.x;
    float dy = a.center.y - b.center.y;
    float distance = sqrt(dx*dx + dy*dy);
    return distance < (a.radius + b.radius);
}

void resolveCollision(Circle& a, Circle& b) {
    // 播放碰撞音效
    if (collisionSound) {
        Mix_PlayChannel(-1, collisionSound, 0);
    }
    float growth_value = 5;
    float loss_value = 10;
    float max_radius = 150;
    if (a.type == 0 && b.type == 2) {
        a.radius = min(a.radius + growth_value, max_radius);
        b.radius = max(b.radius - loss_value, 0.0f);
    } 
    else if (a.type == 2 && b.type == 1) {
        a.radius = min(a.radius + growth_value, max_radius);
        b.radius = max(b.radius - loss_value, 0.0f);
    } 
    else if (a.type == 1 && b.type == 0) {
        a.radius = min(a.radius + growth_value, max_radius);
        b.radius = max(b.radius - loss_value, 0.0f);
    }
    else if (b.type == 0 && a.type == 2) {
        b.radius = min(b.radius + growth_value, max_radius);
        a.radius = max(a.radius - loss_value, 0.0f);
    } 
    else if (b.type == 2 && a.type == 1) {
        b.radius = min(b.radius + growth_value, max_radius);
        a.radius = max(a.radius - loss_value, 0.0f);
    } 
    else if (b.type == 1 && a.type == 0) {
        b.radius = min(b.radius + growth_value, max_radius);
        a.radius = max(a.radius - loss_value, 0.0f);
    }
    
    if (a.radius < 15) a.active = false;
    if (b.radius < 15) b.active = false;
}

int main() {
    if (!initAudio()) {
        cerr << "Failed to initialize audio system! Continuing without sound..." << endl;
    }

    Mat img(600, 800, CV_8UC3, Scalar(20, 20, 20));
    
    Circle outerCircle;
    outerCircle.center = Point2f(400, 300);
    outerCircle.radius = 250;
    
    float smallRadius = calculateSmallRadius();
    float d = calculateCenterDistance(smallRadius);
    vector<Point2f> initialPositions = calculateInitialPositions(d);
    
    vector<Circle> smallCircles(3);
    
    for (int i = 0; i < 3; ++i) {
        smallCircles[i].center = initialPositions[i];
        smallCircles[i].radius = smallRadius/1.05f;
        smallCircles[i].type = i;
        smallCircles[i].active = true;
    }
    
    smallCircles[0].color = Vec3b(70, 70, 255);
    smallCircles[1].color = Vec3b(70, 255, 70);
    smallCircles[2].color = Vec3b(255, 70, 70);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> vel_dist(-5.0f, 5.0f);
    
    vector<Point2f> velocities(3);
    for (int i = 0; i < velocities.size(); ++i) {
        velocities[i] = Point2f(vel_dist(gen), vel_dist(gen));
    }
    
    namedWindow("Rock Paper Scissors Simulation", WINDOW_AUTOSIZE);
    
    vector<string> labels = {"ROCK", "PAPER", "SCISSORS"};
    string winnerText;
    GameState gameState = WAITING;
    Mat finalFrame;
    
    while (true) {
        img.setTo(Scalar(20, 20, 20));
        
        // 绘制外圆
        circle(img, outerCircle.center, outerCircle.radius, Scalar(255, 255, 255), 2);
        
        // 绘制小圆
        for (int i = 0; i < smallCircles.size(); ++i) {
            auto& c = smallCircles[i];
            if (!c.active) continue;
            
            circle(img, c.center, c.radius, Scalar(c.color[0], c.color[1], c.color[2]), FILLED);
            circle(img, c.center, c.radius, Scalar(255, 255, 255), 2);
            
            int baseline = 0;
            Size textSize = getTextSize(labels[c.type], FONT_HERSHEY_DUPLEX, 0.6, 2, &baseline);
            Point textOrg(c.center.x - textSize.width/2, c.center.y + textSize.height/2);
            putText(img, labels[c.type], textOrg, 
                   FONT_HERSHEY_DUPLEX, 0.6, 
                   Scalar(255, 255, 255), 2);
        }
        
        // 状态处理
        switch (gameState) {
            case WAITING: {
                string startText = "Press SPACE to start";
                Size textSize = getTextSize(startText, FONT_HERSHEY_DUPLEX, 1.0, 2, 0);
                Point textOrg((img.cols - textSize.width)/2, 50);
                putText(img, startText, textOrg, 
                       FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 255, 255), 2);
                break;
            }
            
            case RUNNING: {
                int activeCount = 0;
                int lastActiveIndex = -1;
                
                for (int i = 0; i < smallCircles.size(); ++i) {
                    auto& c = smallCircles[i];
                    if (!c.active) continue;
                    
                    auto& v = velocities[i];
                    c.center += v;
                    
                    // 边界碰撞检测
                    Point2f toCenter = outerCircle.center - c.center;
                    float distance = norm(toCenter);
                    float minDistance = outerCircle.radius - c.radius;
                    
                    if (distance > minDistance) {
                        toCenter *= 1.0f/distance;
                        c.center = outerCircle.center - toCenter * minDistance;
                        
                        float dotProduct = v.x*toCenter.x + v.y*toCenter.y;
                        v -= 2.0f * dotProduct * toCenter;
                    }
                    
                    // 小圆间碰撞
                    for (int j = i+1; j < smallCircles.size(); ++j) {
                        if (smallCircles[j].active && checkCollision(c, smallCircles[j])) {
                            resolveCollision(c, smallCircles[j]);
                            
                            Point2f delta = c.center - smallCircles[j].center;
                            float distance = norm(delta);
                            Point2f normal = delta / distance;
                            
                            float dotProduct = (v.x - velocities[j].x)*normal.x + 
                                              (v.y - velocities[j].y)*normal.y;
                            
                            v -= dotProduct * normal;
                            velocities[j] += dotProduct * normal;
                        }
                    }
                    
                    if (c.active) {
                        activeCount++;
                        lastActiveIndex = i;
                    }
                }
                
                if (activeCount <= 1) {
                    gameState = GAME_OVER;
                    if (lastActiveIndex != -1) {
                        winnerText = "WINNER: " + labels[smallCircles[lastActiveIndex].type];
                    } else {
                        winnerText = "DRAW!";
                    }
                    finalFrame = img.clone();
                }
                break;
            }
            
            case GAME_OVER: {
                img = finalFrame.clone();
                int baseline = 0;
                Size textSize = getTextSize(winnerText, FONT_HERSHEY_DUPLEX, 1.5, 3, &baseline);
                Point textOrg((img.cols - textSize.width)/2, 50);
                putText(img, winnerText, textOrg, 
                       FONT_HERSHEY_DUPLEX, 1.5, Scalar(0, 255, 255), 3);
                break;
            }
        }
        
        imshow("Rock Paper Scissors Simulation", img);
        
        int key = waitKey(30);
        if (key == 27) break; // ESC退出
        if (key == 32 && gameState == WAITING) { // 空格开始
            gameState = RUNNING;
        }
    }
    
    closeAudio();
    return 0;
}