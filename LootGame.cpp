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
Cell