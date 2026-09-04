#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>
#include <utility>     
#include <vector>

const int CELL_SIZE = 50;
const int GRID_COLS = 16;
const int GRID_ROWS = 12;
const int SCREEN_W = CELL_SIZE * GRID_COLS;   // 800
const int SCREEN_H = CELL_SIZE * GRID_ROWS;   // 600
const int ITEM_COUNT = 20;

enum class ItemType {
	Coin,
	Gem,
	Key,
	Potion,
	Heart
};

const char* itemName(ItemType t) {
	switch (t) {
	case ItemType::Coin:   return "Coin";
	case ItemType::Gem:    return "Gem";
	case ItemType::Key:    return "Key";
	case ItemType::Potion: return "Potion";
	case ItemType::Heart:  return "Heart";
	}
	return "Unknown";
}

using Cell = std::pair<int, int>;

ItemType randomItem() {
	return static_cast<ItemType>(rand() % 5);
}

// Game palette
std::unordered_map<ItemType, SDL_Color> itemColors = {
	{ ItemType::Coin,   {230, 200,  50, 255} },   // gold
	{ ItemType::Gem,    { 80, 220, 220, 255} },   // cyan
	{ ItemType::Key,    {220, 220, 220, 255} },   // silver
	{ ItemType::Potion, {180,  80, 220, 255} },   // purple
	{ ItemType::Heart,  {230,  70,  90, 255} },   // red
};

void seedWorld(std::map<Cell, ItemType>& world, const Cell& playerCell) {
	world.clear();
	int placed = 0;
	while (placed < ITEM_COUNT) {
		Cell c{ rand() % GRID_COLS, rand() % GRID_ROWS };
		if (c == playerCell) continue;
		if (world.find(c) != world.end()) continue;

		world[c] = randomItem();
		++placed;
	}
}

int main(int argc, char* argv[]) {
	srand((unsigned int)time(nullptr));

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Grid Loot Collector", SCREEN_W, SCREEN_H, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	if (!window || !renderer) {
		SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	std::map<Cell, ItemType> world;
	std::map<ItemType, int> inventory;
	Cell player{ GRID_COLS / 2, GRID_ROWS / 2 };

	seedWorld(world, player);

	bool running = true;
	SDL_Event event;

	// --- GAME LOOP ---
	while (running) {
		// 1. INPUT PASS
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (event.type == SDL_EVENT_KEY_DOWN) {
				Cell next = player;

				switch (event.key.scancode) {
				case SDL_SCANCODE_W: next.second -= 1; break;
				case SDL_SCANCODE_S: next.second += 1; break;
				case SDL_SCANCODE_A: next.first -= 1; break;
				case SDL_SCANCODE_D: next.first += 1; break;
				case SDL_SCANCODE_R:
					inventory.clear();
					seedWorld(world, player);
					SDL_Log("--- World Reset! ---");
					break;
				case SDL_SCANCODE_TAB:
					SDL_Log("--- Inventory ---");
					if (inventory.empty()) {
						SDL_Log("(nothing yet)");
					}
					else {
						for (const std::pair<const ItemType, int>& item : inventory) {
							ItemType type = item.first;
							int count = item.second;
							SDL_Log("  %-7s x %d", itemName(type), count);
						}
					}
					SDL_Log("Items remaining in world: %d", (int)world.size());
					break;
				default:
					break;
				}

				// Screen Boundary Clamping
				if (next.first < 0) next.first = 0;
				if (next.first >= GRID_COLS) next.first = GRID_COLS - 1;
				if (next.second < 0) next.second = 0;
				if (next.second >= GRID_ROWS) next.second = GRID_ROWS - 1;

				player = next;

				// Loot Pickup Check
				auto it = world.find(player);
				if (it != world.end()) {
					ItemType picked = it->second;
					inventory[picked] += 1;
					world.erase(it);
					SDL_Log("Picked up a %s (have %d)", itemName(picked), inventory[picked]);

					if (world.empty()) {
						SDL_Log("All loot collected! Press R for a new world.");
					}
				}
			}
		}

		// 2. RENDER PASS
		SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
		SDL_RenderClear(renderer);

		// Draw Grid Lines
		SDL_SetRenderDrawColor(renderer, 45, 45, 55, 255);
		for (int c = 0; c <= GRID_COLS; ++c) {
			SDL_RenderLine(renderer, (float)(c * CELL_SIZE), 0.0f, (float)(c * CELL_SIZE), (float)SCREEN_H);
		}
		for (int r = 0; r <= GRID_ROWS; ++r) {
			SDL_RenderLine(renderer, 0.0f, (float)(r * CELL_SIZE), (float)SCREEN_W, (float)(r * CELL_SIZE));
		}

		// Draw Loot Items
		for (const auto& entry : world) {
			const Cell& cell = entry.first;
			ItemType type = entry.second;

			SDL_Color col = itemColors[type];
			SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
			SDL_FRect r = {
				(float)(cell.first * CELL_SIZE + 12),
				(float)(cell.second * CELL_SIZE + 12),
				(float)(CELL_SIZE - 24),
				(float)(CELL_SIZE - 24)
			};
			SDL_RenderFillRect(renderer, &r);
		}

		// Draw Player
		SDL_SetRenderDrawColor(renderer, 80, 220, 100, 255);
		SDL_FRect pRect = {
			(float)(player.first * CELL_SIZE + 6),
			(float)(player.second * CELL_SIZE + 6),
			(float)(CELL_SIZE - 12),
			(float)(CELL_SIZE - 12)
		};
		SDL_RenderFillRect(renderer, &pRect);

		SDL_RenderPresent(renderer);
	}

	// 3. CLEANUP PASS
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}