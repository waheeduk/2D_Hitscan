#include "raylib.h"
#include <math.h>

bool CheckRayCircleCollision(Vector2 origin, Vector2 aim, Vector2 target, int target_radius);
bool CheckRayRectCollision(Vector2 origin, Vector2 aim, Vector2 enemy, int target_radius, int rectWidth, int rectHeight);
double CalculateDistance(Vector2 origin, Vector2 target);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "2D Hitscan");
    
    Vector2 player_pos = {screenWidth/2, screenHeight/2}; //initalises position of player circle
    Vector2 enemy_pos = {screenWidth/2 + 200, screenHeight/2}; //initalises position of circular target
    Vector2 rectPos = {screenWidth/2 - 200, screenHeight/2}; //initalises position of rectangular target
    //rectangle variables, used later in collision detection formula
    int rectWidth = 8;
    int rectHeight = 16;
    double rectRad = sqrt(pow(rectWidth, 2) + pow(rectHeight, 2));
    //used when targets are hit
    int circHit = 1;
    int rectHit = 1;
    int speed = 4;    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        
        Vector2 aim = GetMousePosition();
        
        // TODO: Update your variables here
        
        //player movement
        if (IsKeyDown(KEY_W)) player_pos.y -= speed;
        if (IsKeyDown(KEY_A)) player_pos.x -= speed;
        if (IsKeyDown(KEY_S)) player_pos.y += speed;
        if (IsKeyDown(KEY_D)) player_pos.x += speed;
        //player target system
        if (IsMouseButtonPressed(0))
        {
            DrawLineEx(player_pos, aim, 2.0f, BLACK);
            if (CheckRayCircleCollision(player_pos, aim, enemy_pos, 5) == true) 
            {
                circHit++;
            }
            if (CheckRayRectCollision(player_pos, aim, rectPos, rectRad, rectWidth, rectHeight) == true)
            {
                rectHit++;
            }
        }
        
        // Draw
      
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            //PLAYER
            DrawCircle(player_pos.x, player_pos.y, 5, BLACK);
            //CIRCLE TARGET TO AIM FOR
            if (circHit % 2 == 0)
            {
                DrawCircle(enemy_pos.x, enemy_pos.y, 5, RED);
            }
            else
            {
                DrawCircle(enemy_pos.x, enemy_pos.y, 5, BLUE);
            }
            //SQUARE TARGET TO AIM FOR
            if (rectHit % 2 == 0)
            {
                DrawRectangle(rectPos.x, rectPos.y, 8, 16, RED);

            }
            else
            {
                DrawRectangle(rectPos.x, rectPos.y, 8, 16, BLUE);
            }
       


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

  
    CloseWindow();        // Close window and OpenGL context

    return 0;
}

double CalculateDistance(Vector2 origin, Vector2 target)
{
    double distance = sqrt( ((target.x - origin.x) * (target.x - origin.x)) + ((target.y - origin.y) * (target.y - origin.y)) );
    return distance;
}

bool CheckRayCircleCollision(Vector2 origin, Vector2 aim, Vector2 target, int target_radius)
{
    double aim_distance = CalculateDistance(origin, aim);
    
    //a normalised vector representing the angle to where the player aims
    Vector2 direction;
    direction.x = (aim.x - origin.x) / aim_distance;
    direction.y = (aim.y - origin.y) / aim_distance;
    
    //distance to the enemy directly from the player, also the hypotenuse
    double enemy_distance = CalculateDistance(origin, target);
    
    //#calculates the adjacent side
    double adj_length = ((target.x - origin.x)*direction.x) + ((target.y - origin.y)*direction.y);
    
    //#calculate the opposite side
    double opp_length_sq = (enemy_distance*enemy_distance) - (adj_length*adj_length);
    int rad_sq = pow(target_radius, 2);
    
    if (adj_length < 0)
    {
        return false;
    }
    else if (opp_length_sq < rad_sq) //squared values are compared to maintain computing accuracy
    {
        return true;
    }
    else
    {
        return false;
    }
        
}

bool CheckRayRectCollision(Vector2 origin, Vector2 aim, Vector2 enemy, int target_radius, int rectWidth, int rectHeight)
{
    double aim_distance = CalculateDistance(origin, aim);
    
    //rectangles and sprites are not drawn from the centre, but from the top left
    //this algorithm depends on the centre of the rectange, so we must first adjust the target vector2 position
    Vector2 target;
    target.x = enemy.x + (rectWidth * 0.5);
    target.y = enemy.y + (rectHeight * 0.5);
    
    //a normalised vector representing the angle to where the player aims
    Vector2 direction;
    direction.x = (aim.x - origin.x) / aim_distance;
    direction.y = (aim.y - origin.y) / aim_distance;
    
    //distance to the enemy directly from the player, also the hypotenuse
    double enemy_distance = CalculateDistance(origin, target);
    
    //#calculates the adjacent side
    double adj_length = ((target.x - origin.x)*direction.x) + ((target.y - origin.y)*direction.y);
    
    //#calculate the opposite side
    double opp_length_sq = (enemy_distance*enemy_distance) - (adj_length*adj_length);
    int rad_sq = pow(target_radius, 2);

    //calculates nearest point on ray to enemy
    Vector2 point;
    point.x = origin.x + (adj_length * direction.x);
    point.y = origin.y + (adj_length * direction.y);
    //enemy vector2 is topleft of rectangle
    if (adj_length < 0 )
    {
        return false;
    }
    else if ( (point.x >= enemy.x) && (point.x <= (enemy.x + rectWidth)))
    {
        if ( (point.y > enemy.y) && (point.y < (enemy.y + rectHeight)))
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
}
    
        

