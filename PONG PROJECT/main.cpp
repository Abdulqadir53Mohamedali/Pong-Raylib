#include "raylib.h"
// #include "ball.h"
#include <iostream>

Color Green = Color{38,185,154,255};
Color Dark_Green = Color{20,160,133,255};
Color Light_Green = Color{129,204,184,255};
Color Yellow = Color{243,213,98,255};

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius, Yellow);
    }
    
    void Update(){
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight () || y - radius <= 0){
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth ()){ // cpu wins
            cpu_score ++;
            ResetBall();


        }
        
        
        if(x-radius <= 0){
            player_score ++;
            ResetBall();

        }
        // || x - radius <= 0
        //     speed_x *= -1;
        

    } 

    void ResetBall(){
        x= GetScreenWidth()/2;
        y= GetScreenHeight()/2;

        int speed_choice[2] = {-1,1};
        speed_x *= speed_choice[GetRandomValue(0,1)];
        speed_y *= speed_choice[GetRandomValue(0,1)];



    }
};


class Paddle{
    protected:
    void LimitMovement(){
        if(y <= 0){
            y = 0;
        }
        if (y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height ;
        }
    }
    

    public:
    float x,y;
    float width,height;
    int speed; 

    void Draw(){
        DrawRectangle(x,y,width,height,WHITE);

    }

    void Update(){
        if(IsKeyDown(KEY_UP)){
            y = y - speed;

        }
        if (IsKeyDown(KEY_DOWN)){
            y = y + speed;
        }
        LimitMovement();

    }

};

class Player2Paddle: public Paddle{
    public:
    void Update(){
        if (IsKeyDown(KEY_W)){
                y= y-speed;
        }
        if (IsKeyDown(KEY_S)){
            y = y + speed;

        }
        }
};

class CpuPaddle: public Paddle{
    public:

    void Update(int ball_y){
        if(y+ height/2 > ball_y){
            y = y - speed;
        }
        if (y + height/2 <= ball_y){
            y = y + speed;
        }
        LimitMovement();


    }

};

enum GameState{MENU,IN_GAME};

bool isTwoPlayerMode = false;

GameState gameState = MENU;
Ball ball;
Paddle player;
CpuPaddle cpu;
Player2Paddle player2;


int main()
{
    std::cout << "starting the game" << std::endl;
    const int screen_width = 1280; //  window width 
    const int screen_height = 800; // window height 
    InitWindow(screen_width,screen_height,"My pong game"); // the function to create a window using the values in the variables 
    SetTargetFPS(60); // the amount of frames we want per second

    //BALL//
    ball.radius = 20;
    ball.x  = screen_width/2;
    ball.y = screen_height/2; 
    ball.speed_x = 7;
    ball.speed_y = 7;

    //PLAYER//
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height/2;
    player.speed = 6;

    //PLAYER2//
    player2.width = 25;
    player2.height = 120;
    player2.x = 10;
    player2.y = screen_height / 2 - player2.height/2;
    player2.speed = 6;

    //AI//
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10  ;
    cpu.y = screen_height/2 - cpu.height;
    cpu.speed = 6;

    while (WindowShouldClose() == false){  
        BeginDrawing(); // creates a blank canvas so we can draw our game object on 

        if (gameState == MENU){
            ClearBackground(WHITE);

            int menuX = screen_width / 2 - 50;
            float menuY1 = screen_height / 2.5;
            int menuY2 = screen_height / 2;

            int TitleX = screen_width/2 - 120 ;
            int TitleY =screen_height / 9 ;

            //Draw Menu Options
            DrawText("PONG",TitleX,TitleY,100,BLACK);
            DrawText("1 Player vs AI",menuX,menuY1,20,BLACK);
            DrawText("2 Player",menuX,menuY2,20,BLACK);

            // check for option selection

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Vector2 mousePos = GetMousePosition();

                if(menuX <= mousePos.x && mousePos.x <= menuX + 150){
                    if (menuY1 <= mousePos.y && mousePos.y <= menuY1 + 30){
                        // 1 player vs AI selecteduj
                        gameState = IN_GAME;
                        isTwoPlayerMode = false;

                        // will be adding the necessayr chnages for 1 player 

                    }
                    else if (menuY2 <= mousePos.y && mousePos.y <= menuY2 + 30){
                        // 2 players selected
                        gameState = IN_GAME;
                        isTwoPlayerMode = true;

                        // will be adding the necessayr chnages for 2 player 
                    }
                }
            }


        }
        else if(gameState == IN_GAME){

            //UPDATE//
            ball.Update();
            player.Update();

            if(isTwoPlayerMode){
                player2.Update();
            }
            else{
                cpu.Update(ball.y);
            }

            //Checking for collisions 

            if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
                ball.speed_x *= -1;
            }

            if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player2.x,player2.y,player2.width,player2.height})){
                ball.speed_x *= -1;
            }
            else{
                if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
                    ball.speed_x *= -1;
            }

            }
            

            //Drawing
            ClearBackground(Dark_Green);
            DrawRectangle(screen_width/2,0,screen_width/2,screen_height,Green);
            DrawCircle(screen_width/2,screen_height/2,150,Light_Green);
            DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);
            ball.Draw();
            player.Draw();


            if(isTwoPlayerMode){
                player2.Draw();
            }
            else{
                cpu.Draw();
            }

            DrawText(TextFormat("%i",cpu_score),screen_width/4 -20,20,80,WHITE);
            DrawText(TextFormat("%i",player_score),3*screen_width/4 -20,20,80,WHITE);
            

            }

            EndDrawing();


    }
   
    CloseWindow();
    return 0;
}
