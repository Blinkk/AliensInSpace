#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Dx9Base.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Menu.h"
#include "Projectile.h"
#include "Font.h"
#include <sstream>
#include <vector>
#include <ctime>
#include <Windows.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// Public class to manage all game objects and functions
class GameManager : public Dx9Base
{
private:
	// Variables (Add anything you need for game here (ie. Sprites, etc.))
	const D3DCOLOR _white = D3DCOLOR_XRGB(255, 255, 255);
	const D3DCOLOR _black = D3DCOLOR_XRGB(0, 0, 0);
	const int BUFFERW = SCREENW * 2;
	const int BUFFERH = SCREENH * 2;
	enum Buttons{ START = 1, CLOSE };
	double scrollX, scrollY;				// Keep track of background scroll
	int frm = 0;							// Keep track of constant frame for all enemies	
	bool enemyExploding;					// Explosion flag
	bool shipExploding;						// Explosion flag
	bool prevShipExploding;					// Flag for previous status of ship explosion
	bool pause;								// Pause game flag
	bool playerDead;						// Flag for player status
	bool bonusActive;						// Flag for bonus enemy status
	int hordeDropCount;						// Keep track of how many times horde drops
	int hordeLeft;							// Left side of horde
	int hordeWidth;							// Width of the horde
	int hordeSpeed;							// Speed of horde
	int playerLives;						// Number of lives for player
	int playerScore;						// Player score
	int playerScoreMod;						// Player score %10
	Font* _fontObj;							// Pointer to a font object
	LPDIRECT3DSURFACE9 background;			// Background surface
	LPDIRECT3DSURFACE9 gameOver1;			// Gameover screen surface
	LPDIRECT3DSURFACE9 gameOver2;			// Gameover2 screen surface
	Sprite *logo;							// Logo sprite
	Button *btnStart;						// Menu button object
	Button *btnClose;						// Menu button object
	Menu *menu;								// Menu object
	Sprite *ship;							// Main ship sprite object
	Sprite *bonusEnemy;						// Bonus enemy sprite
	std::vector<Projectile*> userBolts;		// Bolts vector
	std::vector<Projectile*> enemyBolts;	// Enemy Bolts vector
	std::vector<std::vector<Enemy*>> enemies;// 2D vector of enemy pointers
	std::vector<Sprite*> explosions;		// Explosion vector
	

public:
	// Functions (Add any functions you need here)
	void ScrollBackground();
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