//compile command:
//gcc -o ray raytracing.c $(sdl2-config --cflags --libs) -lmgcc -o ray raytracing.c $(sdl2-config --cflags --libs) -

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_GREY 0xefefefef
#define RAYS_NUMBER 150

//any circle can be defined by its center point and a radius
struct Circle
{
    double x;
    double y;
    double radius;
};

struct Ray
{
    //ray starting pos
    double x_start, y_start;

    //angle
    double angle;

    //ray ending pos
    double x_end, y_end;

};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) 
{
    double radius_squared = pow(circle.radius, 2);

    //create a bounding box for the circle
    for (double x=circle.x - circle.radius; x < circle.x + circle.radius; x++) 
    {
        for (double y=circle.y - circle.radius; y < circle.y + circle.radius; y++)
        {
            //Check if (x, y) coords are on circle
            double coord_distance_squared = (pow((x - circle.x), 2) + pow((y - circle.y), 2));

            if (coord_distance_squared < radius_squared) 
            {
                SDL_Rect pixel = (SDL_Rect){x, y, 1 , 1}; //(x_coord, y_coord, x_size, y_size)
                SDL_FillRect(surface, &pixel, color);
            }
        } 
    }
}

void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER])
{
    for (int i = 0; i < RAYS_NUMBER; i++) 
    {
        double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
        
        // Directly assign values to the array element
        rays[i].x_start = circle.x;
        rays[i].y_start = circle.y;
        rays[i].angle = angle;
    }
}


void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color, struct Circle obstruction)
{

    //used to check if ray has hit obstruction
    double radius_squared = pow(obstruction.radius, 2);


    for (int i = 0; i < RAYS_NUMBER; i++) {
        struct Ray ray = rays[i];

        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1.0;  // Consider adjusting step size dynamically
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;
        int counter = 0;
        int max_iterations = WIDTH + HEIGHT;

        while (!end_of_screen && !object_hit && counter < max_iterations) 
        {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            if (x_draw < 0 || x_draw >= WIDTH || y_draw < 0 || y_draw >= HEIGHT) 
            {
                end_of_screen = 1;
                break; // Stop drawing if out of bounds
            }

            //check if ray has hit object
            double coord_distance_squared = (pow((x_draw - obstruction.x), 2) + pow((y_draw - obstruction.y), 2));

            //change flag
            if (coord_distance_squared < radius_squared) 
            {
                object_hit = 1;
            }

            // Draw pixel (rounding to avoid artifacts)
            SDL_Rect pixel = { (int)(x_draw + 0.5), (int)(y_draw + 0.5), 1, 1 };
            SDL_FillRect(surface, &pixel, color);

            counter += 1;
        }
    }
}

int SDL_main(int argc, char *argv[]) 
{

    //init the window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //circle that gets dragged
    struct Circle circle = {200, 200, 80};

    //stationary circle to produce shadow
    struct Circle shadow_circle = {650, 300, 120};

    SDL_Rect erase_rect = {0, 0, WIDTH, HEIGHT};

    struct Ray rays[RAYS_NUMBER];

    //update and delay command
    SDL_UpdateWindowSurface(window);
    SDL_Event event;

    int sim_running = 1;
    while (sim_running)
    {

        while (SDL_PollEvent(&event))
        {

            //check if user is trying to quit program
            if (event.type == SDL_QUIT)
            {
                sim_running = 0;
            }

            //check if user is trying to drag circle
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) 
            {

                //update circle coords to mouse coords
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(circle, rays);
            }
        }

        //erase the screen
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

        FillCircle(surface, circle, COLOR_WHITE);

        FillCircle(surface, shadow_circle, COLOR_WHITE);
        FillRays(surface, rays, COLOR_GREY, shadow_circle);

        //move_obstruction(&shadow_circle);

        //50 fps
        SDL_UpdateWindowSurface(window);
        SDL_Delay(20);
    }
}