#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Dx9Base.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Menu.h"
#include "Projectile.h"
#include <vector>
#include <ctime>
#include <Windows.h>

// Public class to manage all game objects and functions
class GameManager : public Dx9Base
{
private:
	// Variables (Add anything you need for game here (ie. Sprites, etc.))
	const D3DCOLOR _white = D3DCOLOR_XRGB(255, 255, 255);
	const D3DCOLOR _black = D3DCOLOR_XRGB(0, 0, 0);
	enum Buttons{ START = 1, CLOSE };
	int frm = 0;							// Keep track of constant frame for all enemies	
	bool enemyExploding = false;			// Explosion flag
	bool shipExploding = false;				// Explosion flag
	bool prevShipExploding = false;			// Flag for previous status of ship explosion
	bool pause = false;						// Pause game flag
	bool playerDead = false;				// Flag for player status
	bool bonusActive = false;				// Flag for bonus enemy status
	int hordeDropCount = 0;					// Keep track of how many times horde drops
	int hordeLeft;							// Left side of horde
	int hordeWidth;							// Width of the horde
	int hordeSpeed;							// Speed of horde
	int playerLives = 3;					// Number of lives for player
	int playerScore = 0;					// Player score
	int playerScoreMod = 0;					// Player score %10
	LPDIRECT3DSURFACE9 background = NULL;	// Background surface
	LPDIRECT3DSURFACE9 gameOver1 = NULL;	// Gameover screen surface
	LPDIRECT3DSURFACE9 gameOver2 = NULL;	// Gameover2 screen surface
	Sprite *logo = NULL;					// Logo sprite
	Button *btnStart = NULL;				// Menu button object
	Button *btnClose = NULL;				// Menu button object
	Menu *menu = NULL;						// Menu object
	Sprite *score_sprite = NULL;			// Score (text) sprite object
	Sprite *lives_sprite = NULL;			// Lives (text) sprite object
	Sprite *numbers[10];					// Numbers spritesheet x10
	Sprite *ship = NULL;					// Main ship sprite object
	Sprite *bonusEnemy = NULL;				// Bonus enemy sprite
	std::vector<Projectile*> userBolts;		// Bolts vector
	std::vector<Projectile*> enemyBolts;	// Enemy Bolts vector
	std::vector<std::vector<Enemy*>> enemies;// 2D vector of enemy pointers
	std::vector<Sprite*> explosions;		// Explosion vector

public:
	// Functions (Add any functions you need here)
	void DrawShip();
	void UpdateShip();
	void DrawEnemies();
	void UpdateEnemies();
	void SpawnBonusEnemy();
	void DrawUserBolts();
	void UpdateUserBolts();
	void DrawEnemyBolts();
	void UpdateEnemyBolts();
	void CheckCollision();
	void DrawExplosions();
	void UIInit();
	void DrawUI();
	void ReverseHorde();
	void ResetShip();
	void ResetGame();
	bool CollisionD(Sprite *sprite1, Sprite *sprite2);

	// Constant Functions (Leave these alone)
	GameManager();
	GameManager(HWND window);
	~GameManager();
	void PlayS(char* filename);
	bool LoadContent() override;	// Overridden LoadContent()
	void Update() override;			// Overridden Update()
	void Render() override;			// Overridden Render()
	void UnloadContent() override;	// Overridden UnloadContent()
};

#endif