/* #include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include<vector>
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <cmath>     // cosf(), sinf()

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const float MIN_SPEED = 180.0f;
const float MAX_SPEED = 360.0f;
const float MIN_SIZE = 12.0f;
const float MAX_SIZE = 24.0f;

struct Particle {
	float x, y; //position
	float vx, vy; //velocity
	float size;
	Uint8 r, g, b; //color
};

float randFloat(float lo, float hi)
{
	return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

int main(int argc, char* argv[]) {

	srand((unsigned)time(nullptr));

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Click Particle", WINDOW_W, WINDOW_H, 0);
	if (!window)
	{
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
	{
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//particle
    std::vector<Particle*> particles;
    particles.reserve(1024);

	Uint64 lastTime = SDL_GetTicks();
	bool running = true;
	SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_ESCAPE)
                running = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                

                Particle* p = new Particle();

                p->x = event.button.x;
                p->y = event.button.y;

                float angle = randFloat(0.0f, 6.2832f);
                float speed = randFloat(MIN_SPEED, MAX_SPEED);
                p->vx = cosf(angle) * speed;
                p->vy = sinf(angle) * speed;

                p->size = randFloat(MIN_SIZE, MAX_SIZE);

                p->r = (Uint8)(rand() % 156 + 100);
                p->g = (Uint8)(rand() % 156 + 100);
                p->b = (Uint8)(rand() % 156 + 100);

                particles.push_back(p);
            }
        }

        Uint64 now = SDL_GetTicks();
        float  dt = (now - lastTime) / 1000.0f;
        lastTime = now;
        if (dt > 0.05f) dt = 0.05f;
        //update
        for (Particle* particle : particles) {
            particle->x += particle->vx * dt;
            particle->y += particle->vy * dt;

            if (particle->x < 0.0f)
            {
                particle->x = 0.0f;
                particle->vx = -particle->vx;
            }
            else if (particle->x + particle->size > WINDOW_W)
            {
                particle->x = WINDOW_W - particle->size;
                particle->vx = -particle->vx;
            }

            if (particle->y < 0.0f)
            {
                particle->y = 0.0f;
                particle->vy = -particle->vy;
            }
            else if (particle->y + particle->size > WINDOW_H)
            {
                particle->y = WINDOW_H - particle->size;
                particle->vy = -particle->vy;
            }
        }
        
        //
       

        SDL_SetRenderDrawColor(renderer, 12, 12, 24, 255);
        SDL_RenderClear(renderer);

        for (Particle* particle : particles)
        {
            SDL_SetRenderDrawColor(renderer,
                particle->r, particle->g, particle->b, 255);
            SDL_FRect rect = {
                particle->x, particle->y,
                particle->size, particle->size
            };
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
    }

    for (Particle* p : particles)
        delete p;
    particles.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}*/



