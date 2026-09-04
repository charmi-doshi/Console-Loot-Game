#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>
#include <utility>     // std::pair
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
		case ItemType::Coin: return "Coin";
		case ItemType::Gem: return "Gem";
		case ItemType::Key: return "Key";
		case ItemType::Potion: return "Potion";
		case ItemType::Heart: return "Heart";
	}

	return "Unknown";
}

using Cell =std:: pair<int, int>;

//game
std::unordered_map<ItemType, SDL_Color> itemColors = {
		{ ItemType::Coin,   {230, 200,  50, 255} },   // gold
		{ ItemType::Gem,    { 80, 220, 220, 255} },   // cyan
		{ ItemType::Key,    {220, 220, 220, 255} },   // silver
		{ ItemType::Potion, {180,  80, 220, 255} },   // purple
		{ ItemType::Heart,  {230,  70,  90, 255} },   // red
};

std::map<Cell, ItemType> world;
std::map<ItemType, int> inventory;
Cell player;

void seedWorld(std::map<Cell,ItemType>& world,const Cell& playerCell) {
	world.clear();
	int placed = 0;
	while (placed < ITEM_COUNT) {
		Cell c{ rand() % GRID_COLS,rand() % GRID_ROWS};
		if (c == playerCell) continue;

		if (world.find(c) != world.end()) continue;
		world[c] = randomItem();
		++placed;

		Cell next = player;
		switch (event.key.scancode) {
		case SDL_SCANCODE_W: next.second -= 1; break;
		case SDL_SCANCODE_S: next.second += 1; break;
		case SDL_SCANCODE_A: next.first -= 1; break;
		case SDL_SCANCODE_D: next.first += 1; break;
		}

		if (next.first < 0) next.first = 0;
		if (next.first >= GRID_COLS)  next.first = GRID_COLS - 1;
		if (next.second < 0)           next.second = 0;
		if (next.second >= GRID_ROWS)  next.second = GRID_ROWS - 1;

		player = next;

		auto it = world.find(player);
		if (it != world.end() ) {
			ItemType picked = it->second;
			inventory[picked] += 1;
			world.erase(it);
			SDL_Log("Picked up a %s  (have %d)",
				itemName(picked), inventory[picked]);
			
			if (world.empty()) {
				SDL_Log("All loot collected! Press R for a new world.");
			}
		}
	}
}