#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>
#include <utility>     
#include <vector>
#include<string>

const int CELL_SIZE = 50;
const int GRID_COLS = 16;
const int GRID_ROWS = 12;


const int WORLD_W = CELL_SIZE * GRID_COLS;   // 800px Grid
const int HUD_W = 250;                       // 250px Inventory Sidebar
const int SCREEN_W = WORLD_W + HUD_W;        // 1050px Total Width
const int SCREEN_H = CELL_SIZE * GRID_ROWS;   // 600px Total Height

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
std::unordered_map<ItemType, SDL_Color> itemColors = {
	{ ItemType::Coin,   {230, 200,  50, 255} },   // gold
	{ ItemType::Gem,    { 80, 220, 220, 255} },   // cyan
	{ ItemType::Key,    {220, 220, 220, 255} },   // silver
	{ ItemType::Potion, {180,  80, 220, 255} },   // purple
	{ ItemType::Heart,  {230,  70,  90, 255} },   // red
};
//bitmap font renderer
uint8_t getCharBitmap(char c, int row) {
	if (c >= '0' && c <= '9') {
		static const uint8_t nums[10][8] = {
			{0x3E,0x66,0x6E,0x76,0x66,0x66,0x3E,0x00}, // 0
			{0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00}, // 1
			{0x3E,0x66,0x0C,0x18,0x30,0x60,0x7E,0x00}, // 2
			{0x3E,0x66,0x06,0x1C,0x06,0x66,0x3E,0x00}, // 3
			{0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00}, // 4
			{0x7E,0x60,0x7C,0x06,0x06,0x66,0x3E,0x00}, // 5
			{0x3E,0x66,0x60,0x7C,0x66,0x66,0x3E,0x00}, // 6
			{0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00}, // 7
			{0x3E,0x66,0x66,0x3E,0x66,0x66,0x3E,0x00}, // 8
			{0x3E,0x66,0x66,0x3E,0x06,0x66,0x3E,0x00}  // 9
		};
		return nums[c - '0'][row];
	}
	if (c >= 'A' && c <= 'Z') {
		static const uint8_t alpha[26][8] = {
			{0x3C,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // A
			{0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
			{0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
			{0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, // D
			{0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00}, // E
			{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00}, // F
			{0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // G
			{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // H
			{0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00}, // I
			{0x1E,0x06,0x06,0x06,0x06,0x66,0x3C,0x00}, // J
			{0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // K
			{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // L
			{0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, // M
			{0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00}, // N
			{0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // O
			{0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, // P
			{0x3C,0x66,0x66,0x66,0x6A,0x6C,0x36,0x00}, // Q
			{0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00}, // R
			{0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00}, // S
			{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, // T
			{0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // U
			{0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // V
			{0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, // W
			{0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, // X
			{0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00}, // Y
			{0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}  // Z
		};
		return alpha[c - 'A'][row];
	}
	switch (c) {
	case ':': { static const uint8_t col[8] = { 0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00 }; return col[row]; }
	case '/': { static const uint8_t slsh[8] = { 0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00 }; return slsh[row]; }
	case '!': { static const uint8_t exc[8] = { 0x18,0x18,0x18,0x18,0x00,0x18,0x00,0x00 }; return exc[row]; }
	default: return 0x00;
	}
}

void drawText(SDL_Renderer* renderer, const std::string& text, float startX, float startY, float scale, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	float cursorX = startX;
	for (char c : text) {
		if (c >= 'a' && c <= 'z') c -= 32; // Convert lower to upper case

		for (int r = 0; r < 8; ++r) {
			uint8_t rowBits = getCharBitmap(c, r);
			for (int col = 0; col < 8; ++col) {
				if (rowBits & (1 << (7 - col))) {
					SDL_FRect pixel = {
						cursorX + (col * scale),
						startY + (r * scale),
						scale,
						scale
					};
					SDL_RenderFillRect(renderer, &pixel);
				}
			}
		}
		cursorX += 8.0f * scale;
	}
}

//game
void seedWorld(std::map<Cell, ItemType>& world, std::map<ItemType, int>& totalSpawned, const Cell& playerCell) {
	world.clear();
	totalSpawned.clear();

	int placed = 0;
	while (placed < ITEM_COUNT) {
		Cell c{ rand() % GRID_COLS, rand() % GRID_ROWS };
		if (c == playerCell) continue;
		if (world.find(c) != world.end()) continue;

		ItemType itm = randomItem();
		world[c] = itm;
		totalSpawned[itm]++;
		++placed;
	}
}

int main(int argc, char* argv[]) {
	srand((unsigned int)time(nullptr));

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Grid Loot Inventory Showcase", SCREEN_W, SCREEN_H, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	if (!window || !renderer) {
		SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	std::map<Cell, ItemType> world;
	std::map<ItemType, int> inventory;
	std::map<ItemType, int> totalSpawned;
	Cell player{ GRID_COLS / 2, GRID_ROWS / 2 };

	seedWorld(world, totalSpawned, player);

	bool running = true;
	SDL_Event event;

	const ItemType allItems[] = {
		ItemType::Coin,
		ItemType::Gem,
		ItemType::Key,
		ItemType::Potion,
		ItemType::Heart
	};

	while (running) {

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
					seedWorld(world, totalSpawned, player);
					break;
				default:
					break;
				}


				if (next.first < 0) next.first = 0;
				if (next.first >= GRID_COLS) next.first = GRID_COLS - 1;
				if (next.second < 0) next.second = 0;
				if (next.second >= GRID_ROWS) next.second = GRID_ROWS - 1;

				player = next;

		
				auto it = world.find(player);
				if (it != world.end()) {
					ItemType picked = it->second;
					inventory[picked] += 1;
					world.erase(it);
				}
			}
		}

		// render
		SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
		SDL_RenderClear(renderer);


		SDL_SetRenderDrawColor(renderer, 45, 45, 55, 255);
		for (int c = 0; c <= GRID_COLS; ++c) {
			SDL_RenderLine(renderer, (float)(c * CELL_SIZE), 0.0f, (float)(c * CELL_SIZE), (float)SCREEN_H);
		}
		for (int r = 0; r <= GRID_ROWS; ++r) {
			SDL_RenderLine(renderer, 0.0f, (float)(r * CELL_SIZE), (float)WORLD_W, (float)(r * CELL_SIZE));
		}

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

		SDL_SetRenderDrawColor(renderer, 80, 220, 100, 255);
		SDL_FRect pRect = {
			(float)(player.first * CELL_SIZE + 6),
			(float)(player.second * CELL_SIZE + 6),
			(float)(CELL_SIZE - 12),
			(float)(CELL_SIZE - 12)
		};
		SDL_RenderFillRect(renderer, &pRect);

	
		SDL_SetRenderDrawColor(renderer, 30, 32, 42, 255);
		SDL_FRect hudBg = { (float)WORLD_W, 0.0f, (float)HUD_W, (float)SCREEN_H };
		SDL_RenderFillRect(renderer, &hudBg);

		SDL_SetRenderDrawColor(renderer, 80, 85, 105, 255);
		SDL_RenderLine(renderer, (float)WORLD_W, 0.0f, (float)WORLD_W, (float)SCREEN_H);


		drawText(renderer, "INVENTORY", (float)(WORLD_W + 20), 20.0f, 2.5f, { 255, 255, 255, 255 });

		int totalCollected = 0;
		int startY = 70;
		int rowHeight = 55;

		for (int i = 0; i < 5; ++i) {
			ItemType type = allItems[i];
			SDL_Color col = itemColors[type];
			int count = inventory[type];
			int total = totalSpawned[type];
			totalCollected += count;

			float rowY = (float)(startY + i * rowHeight);

			SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
			SDL_FRect iconRect = { (float)(WORLD_W + 20), rowY, 20.0f, 20.0f };
			SDL_RenderFillRect(renderer, &iconRect);

	
			drawText(renderer, itemName(type), (float)(WORLD_W + 50), rowY + 2.0f, 2.0f, col);

			std::string countStr = std::to_string(count) + "/" + std::to_string(total);
			drawText(renderer, countStr, (float)(WORLD_W + 160), rowY + 2.0f, 2.0f, { 200, 200, 200, 255 });
		}

	
		SDL_SetRenderDrawColor(renderer, 60, 65, 80, 255);
		SDL_RenderLine(renderer, (float)(WORLD_W + 15), 365.0f, (float)(SCREEN_W - 15), 365.0f);

	
		std::string totalStr = "TOTAL: " + std::to_string(totalCollected) + "/" + std::to_string(ITEM_COUNT);
		drawText(renderer, totalStr, (float)(WORLD_W + 20), 385.0f, 2.2f, { 255, 220, 100, 255 });

		drawText(renderer, "WASD: MOVE", (float)(WORLD_W + 20), 450.0f, 1.8f, { 120, 120, 140, 255 });
		drawText(renderer, "R: RESET WORLD", (float)(WORLD_W + 20), 475.0f, 1.8f, { 120, 120, 140, 255 });

	
		if (world.empty()) {

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
			SDL_FRect victoryBg = { 80.0f, (float)(SCREEN_H / 2 - 40), (float)(WORLD_W - 160), 80.0f };
			SDL_RenderFillRect(renderer, &victoryBg);

	
			SDL_SetRenderDrawColor(renderer, 80, 220, 100, 255);
			SDL_RenderRect(renderer, &victoryBg);

			
			drawText(renderer, "VICTORY!", 310.0f, (float)(SCREEN_H / 2 - 25), 3.5f, { 255, 215, 0, 255 });
			drawText(renderer, "ALL ITEMS CLEARED!", 220.0f, (float)(SCREEN_H / 2 + 10), 2.2f, { 255, 255, 255, 255 });
		}

		SDL_RenderPresent(renderer);
	}

	// clean
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}