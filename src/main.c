#include "raylib.h"
#include <math.h>

bool CheckRayCollision(Vector2 origin, Vector2 aim, Vector2 target, int target_radius);
double CalculateDistance(Vector2 origin, Vector2 target);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    Vector2 player_pos = {screenWidth/2, screenHeight/2};
    Vector2 enemy_pos = {screenWidth/2 + 200, screenHeight/2};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        Vector2 aim = GetMousePosition();
        // TODO: Update your variables here
        if (IsMouseButtonPressed(0))
        {
            if (CheckRayCollision(player_pos, aim, enemy_pos, 5) == true) 
            {
                enemy_pos.y -= 5;
            }
        }

        // Draw
      
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            //PLAYER
            DrawCircle(screenWidth/2, screenHeight/2, 5, BLACK);
            //TARGET TO AIM FOR
            DrawCircle(enemy_pos.x, enemy_pos.y, 5, RED);


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

bool CheckRayCollision(Vector2 origin, Vector2 aim, Vector2 target, int target_radius)
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
