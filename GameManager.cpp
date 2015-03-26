#include "GameManager.h"
#include <vector>

/* Still need to fix the bolt functions and make sure no drawing is done in update */

GameManager::GameManager()
	: Dx9Base()
{
	// Default constructor
}

GameManager::GameManager(HWND window)
	: Dx9Base(window)
{
	// Pass this constructor a window, that is passed to base constructor
	// and stored as the window to be used for drawing...
}

GameManager::~GameManager()
{
	Shutdown();		// Shutdown calls UnloadContent(), and shuts down D3D
}

void GameManager::PlayS(char* filename)
{
	// Play sound (give 10ms to load)
	PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
	Sleep(10);
}

bool GameManager::LoadContent()
{
	// Initialize Direct3D
	if (!Direct3D_Init(_window, SCREENW, SCREENH, _fullscreen))	// !Fullscreen
		return false;
	if (!DirectInput_Init(_window))
		return false;

	srand(time(NULL));		// Seed random generators

	// Starts at runtime
	// Play background music
	PlaySound(TEXT("C:\\Users\\Austin\\Documents\\Visual Studio 2013\\Projects\\Space_Invaders\\Space_Invaders\\WorkIt.wav"),
		NULL, SND_LOOP | SND_ASYNC);

	// Load background
	background = LoadSurface("space_bg.bmp");

	// Load gameover screens
	gameOver1 = LoadSurface("GameOver1.bmp");
	gameOver2 = LoadSurface("GameOver2.bmp");

	// Load logo
	logo = new Sprite(LoadTexture("Logo.png", _white), (SCREENW / 2) - 295, 50, 0, 590, 300, 0, 0,
		1.0f, 1, 1.0f);

	// Load text sprites
	for (int i = 0; i < 10; i++)
	{
		numbers[i] = new Sprite(LoadTexture("numbers_spritesheet.bmp", _white), 0, 0, 0, 50, 31, 0, 0,
			1.0f, 5, 1.0f, 0, 9, 1, 0);
	}
	score_sprite = new Sprite(LoadTexture("score_sprite.bmp"), 120, SCREENH - 20, 0, 67, 20, 0, 0,
		1.0f, 1, 1.0f);
	lives_sprite = new Sprite(LoadTexture("lives_sprite.bmp"), 10, SCREENH - 20, 0, 67, 20, 0, 0,
		1.0f, 1, 1.0f);
	UIInit();	// Initialize UI positioning

	// Load menu & buttons
	menu = new Menu("space_bg.bmp", _window);
	btnStart = new Button("StartButton.png", "HL_StartButton.png",
		(SCREENW / 2) - 162, SCREENH / 2 + 150, 325, 36);
	btnClose = new Button("CloseButton.png", "HL_CloseButton.png",
		(SCREENW / 2) - 162, (SCREENH / 2 + 150) + 45, 325, 36);
	menu->AddButton(btnStart); menu->AddButton(btnClose);	// Add buttons to menu

	// Load ship
	ship = new Sprite(LoadTexture("ship.bmp"), (SCREENW / 2) - 25, SCREENH - 75,
		0, 50, 50, 3, 3, 1.0f, 1, 1.0f);

	// Load enemies
	const int nCol = 11;
	const int nRow = 5;
	int x = 52;
	int y = 15;
	hordeWidth = (((nCol - 1) * 65) + 32);
	hordeSpeed = 1;
	for (int i = 0; i < nRow; i++)
	{
		std::vector<Enemy*> temp;	// Temp vector for a single row
		for (int j = 0; j < nCol; j++)
		{
			// Create an enemy
			Enemy *enemy = new Enemy(LoadTexture("enemy1.bmp"), x, y,
				0, 39, 32, 1, 15, 1.0f, 2, 1.0f, 0, 1, 1, 250, _white);
			temp.push_back(enemy);	// Add enemy to temp (row) vector

			if (j == 0)				// Set hordeLeft to the x position of 
				hordeLeft = x;		// first enemy in a row. (This original pos		
									// is very important)
			x += 65;				// Increment starting x
		}

		x = 52;						// Reset x for next row
		y += 45;					// Increment starting y
		enemies.push_back(temp);	// Add temp vector to enemies vector
	}

	// Load bonus enemy
	bonusEnemy = new Sprite(LoadTexture("enemy2.bmp"), 0, 5, 0, 35, 32,
		3, 0, 1.0f, 1, 1.0f, 0, 1, 1, 250);

	return true;
}

// DONT FORGET TO CHANGE THIS...
void GameManager::Update()
{
	// Update input
	DirectInput_Update();

	// Call update functions
	if (!pause && !playerDead && !menu->IsActive())
	{
		UpdateShip();
		UpdateEnemies();
		UpdateUserBolts();
		UpdateEnemyBolts();
		CheckCollision();
	}

	// ResetShip() if it is done exploding
	if (prevShipExploding
		&& !shipExploding)
		ResetShip();
	prevShipExploding = shipExploding;	// Update previous explosion state

	// Exit with escape
	if (Key_Down(DIK_ESCAPE))
		gameover = true;
}

// DONT FORGET TO CHANGE THIS...
void GameManager::Render()
{
	// Validate Direct3D device
	if (!d3ddev)
		return;

	// Clear backbuffer
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 0, 255), 1.0f, 0);

	if (d3ddev->BeginScene())
	{
		// If menu active draw menu, else check buttons
		if (menu->IsActive())
		{
			menu->DrawMenu();
			logo->Draw_Current_Frame();
		}
		else
		{
			// If player dead, draw gameOver screen
			if (playerDead && !pause)
			{
				d3ddev->StretchRect(gameOver1, NULL, backbuffer, NULL, D3DTEXF_NONE);
				//int random = rand() % 10 + 5;
				//if (random < 10)
				//	d3ddev->StretchRect(gameOver1, NULL, backbuffer, NULL, D3DTEXF_NONE);
				//else
				//	d3ddev->StretchRect(gameOver2, NULL, backbuffer, NULL, D3DTEXF_NONE);
			}
				

			// If start button was pressed, draw game...
			else if (menu->CheckButtonStatus(START))
			{
				// Draw background
				d3ddev->StretchRect(background, NULL, backbuffer, NULL, D3DTEXF_NONE);

				// Call draw functions
				DrawShip();
				DrawEnemies();
				DrawEnemyBolts();
				DrawUserBolts();
				DrawExplosions();
				DrawUI();
			}
			// If close was pressed, end game
			else if (menu->CheckButtonStatus(CLOSE))
				gameover = true;
		}

		// Stop Rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void GameManager::UnloadContent()
{
	// Release memory
	if (background) background->Release();
	if (numbers)
	{
		for (int i = 0; i < 10; i++)
		{
			delete numbers[i];
			numbers[i] = NULL;
		}

	}
	if (score_sprite)
	{
		delete score_sprite;
		score_sprite = NULL;
	}
	if (lives_sprite)
	{
		delete lives_sprite;
		lives_sprite = NULL;
	}
	if (ship)
	{
		delete ship;
		ship = NULL;
	}
	if (menu)	// Delete menu (and buttons)
	{
		delete menu;
		menu = NULL;
	}
	if (btnClose)
	{
		delete btnClose;
		btnClose = NULL;
	}
	if (btnStart)
	{
		delete btnStart;
		btnStart = NULL;
	}
	if (logo)
	{
		delete logo;
		logo = NULL;
	}
	// Empty vector of enemies
	if (enemies.size() > 0)
	{
		std::vector<std::vector<Enemy*>>::iterator mIt = enemies.begin();
		std::vector<Enemy*>::iterator iIt;
		while (enemies.size() > 0)
		{
			if ((*mIt).size() > 0)
			{
				iIt = mIt->begin();
				while ((*mIt).size() > 0)	// Delete each enemy in internal vector
				{
					delete(*iIt);
					mIt->erase(iIt);
					iIt = mIt->begin();
				}
				mIt->clear();				// Make sure the vector is empty
			}
			enemies.erase(mIt);				// Erase internal vector from main vector		
			mIt = enemies.begin();			// Reset iterator
		}
		enemies.clear();					// Make sure main vector is empty
	}

	// Delete remaining userBolts
	if (userBolts.size() > 0)
	{
		std::vector<Projectile*>::iterator bIt = userBolts.begin();
		while (userBolts.size() > 0)
		{
			delete (*bIt);
			userBolts.erase(bIt);
			bIt = userBolts.begin();
		}
		userBolts.clear();
	}

	// Delete remaining enemyBolts
	if (enemyBolts.size() > 0)
	{
		std::vector<Projectile*>::iterator bIt = enemyBolts.begin();
		while (enemyBolts.size() > 0)
		{
			delete (*bIt);
			enemyBolts.erase(bIt);
			bIt = enemyBolts.begin();
		}
		enemyBolts.clear();
	}

	// Delete remaining explosions
	if (explosions.size() > 0)
	{
		std::vector<Sprite*>::iterator eIt = explosions.begin();
		while (explosions.size() > 0)
		{
			delete (*eIt);
			explosions.erase(eIt);
			eIt = explosions.begin();
		}
		explosions.clear();
	}
}

void GameManager::DrawShip()
{
	if (ship->IsActive())
	{
		// Draw ship
		ship->Draw_Current_Frame();	
	}
}

void GameManager::UpdateShip()
{
	// Ship movement
	if (Key_Down(DIK_LEFT))
		ship->SetX(ship->GetX() - ship->GetVelX());
	if (Key_Down(DIK_RIGHT))
		ship->SetX(ship->GetX() + ship->GetVelX());

	// Keep within boundaries
	if (ship->GetX() <= 0)
		ship->SetX(0);
	if (ship->GetX() + ship->GetWidth() >= SCREENW)
		ship->SetX(SCREENW - ship->GetWidth());
}

void GameManager::DrawEnemies()
{
	std::vector<std::vector<Enemy*>>::iterator mIt;	// Main vector iterator
	std::vector<Enemy*>::iterator iIt;				// Internal vector(s) iterator
	for (mIt = enemies.begin(); mIt != enemies.end(); mIt++)
	{
		for (iIt = mIt->begin(); iIt != mIt->end(); iIt++)
		{
			// If enemy is active draw it
			if ((*iIt)->IsActive())
				(*iIt)->Draw_Frame(frm);
		}
	}

	// Draw and animate bonus enemy, if active
	if (bonusActive && bonusEnemy->IsActive())
	{
		if (!pause)
			bonusEnemy->Sprite_Animate(bonusEnemy->GetStartFrame(),
				bonusEnemy->GetEndFrame(), bonusEnemy->GetDelay());
		bonusEnemy->Draw_Current_Frame();
	}
		
}

void GameManager::UpdateEnemies()
{
	// Manage enemy movement
	static bool goingRight = true;		// Flag for horde direction
	static bool goingLeft = false;		// Flag for horde direction
	static bool drop = false;			// Flag to drop enemies
	static int enemyTimer = 0;			// Timer for movement increment

	// Call bonus enemy function
	SpawnBonusEnemy();

	// Only update position every (x) ticks
	int x = 5;
	if (GetTickCount() - enemyTimer >= x)
	{
		enemyTimer = GetTickCount();

		// Update bonus enemy
		if (bonusActive)
		{
			if (bonusEnemy->IsActive())
			{
				bonusEnemy->SetX(bonusEnemy->GetX() + bonusEnemy->GetVelX());

				// Check ship position
				if ((bonusEnemy->GetX() + bonusEnemy->GetWidth() < 0	// If x + width < )
					&& bonusEnemy->GetVelX() == -3)						// && velocity is negative
					|| (bonusEnemy->GetX() >= SCREENW					// OR if x > SCREENW
					&& bonusEnemy->GetVelX() == 3))						// && velocity is positive
				{
					bonusActive = false;
					bonusEnemy->Deactivate();
				}		
			}
		}

		if (!pause)
			hordeLeft += hordeSpeed;		// Move the horde of enemies

		std::vector<std::vector<Enemy*>>::iterator mIt;	// Main vector iterator
		std::vector<Enemy*>::iterator iIt;				// Internal vector(s) interator
		for (mIt = enemies.begin(); mIt != enemies.end(); mIt++)
		{
			for (iIt = mIt->begin(); iIt != mIt->end(); iIt++)
			{
				// Move enemies on x-axis
				if (!pause)
					(*iIt)->SetX((*iIt)->GetX() + (*iIt)->GetVelX());

				if (goingRight)
				{
					// If horde reaches right side of screen, reverse horde
					if (hordeLeft + hordeWidth >= SCREENW)
					{
						ReverseHorde();
						goingRight = false;
						goingLeft = true;
						hordeSpeed *= -1;
						hordeDropCount++;
					}
				}
				else if (goingLeft)
				{
					// If horde reaches left side of screen, reverse horde
					if (hordeLeft <= 0)
					{
						ReverseHorde();
						goingRight = true;
						goingLeft = false;
						hordeSpeed *= -1;
						hordeDropCount++;
					}
				}

				// Delete inactive enemies
				if (!(*iIt)->IsActive())
				{
					delete (*iIt);
					mIt->erase(iIt);
					break;
				}
			}
		}
	}

	// Manage enemy frame 
	static int timer = 0;			// Timer for animation
	if (GetTickCount() - timer >= 200)
	{
		timer = GetTickCount();
		frm++;						// Change animation frame
		if (frm > 1)
			frm = 0;
	}
}

void GameManager::SpawnBonusEnemy()
{
	static bool started = false;	// Flag for when first bonus starts
	static bool prevBonus = false;	// Flag for prev bonusenemy status
	static int timer = 0;			// Bonus enemy timer
	int random = rand() % 10 + 5;	// Bonus enemy direction randomizer

	if (hordeDropCount >= 2 
		&& !started)				// Spawn first enemy only after horde has dropped 2+ times
	{
		started = true;
		timer = GetTickCount();
	}
		
	// If bonus enemy is now inactive, being checking timer again
	if (prevBonus == false && bonusActive == false && started == true)
	{
		if (GetTickCount() - timer >= 5000)
		{
			if (random < 10)
			{
				bonusEnemy->Activate();
				bonusEnemy->SetX(SCREENW);
				bonusEnemy->SetVelX(-3);
				bonusActive = true;
			}
			else
			{
				bonusEnemy->Activate();
				bonusEnemy->SetX(0 - bonusEnemy->GetWidth());
				bonusEnemy->SetVelX(3);
				bonusActive = true;
			}
			prevBonus = bonusActive;		// Update prev status
		}
	}

	// If it was active, but now is not active, reset timer and prevBonus status
	if (prevBonus == true
		&& bonusActive == false)
	{
		prevBonus = bonusActive;
		timer = GetTickCount();
	}
}

void GameManager::DrawUserBolts()
{
	std::vector<Projectile*>::iterator bIt;
	for (bIt = userBolts.begin(); bIt != userBolts.end(); bIt++)
	{
		if ((*bIt)->IsActive())
			(*bIt)->Draw_Current_Frame();
		else
			return;
	}
}

void GameManager::UpdateUserBolts()
{
	static int shottimer = 0;		// Shot timer

	// Add new bolts
	if (Key_Down(DIK_SPACE)
		&& GetTickCount() - shottimer >= 550)
	{
		shottimer = GetTickCount();
		Projectile *bolt = new Projectile(LoadTexture("bolt.bmp"),
			ship->GetX() + (ship->GetWidth() / 2), ship->GetY(),
			0, 10, 20, 7, 7, 1.0f, 1, 1.0f);
		userBolts.push_back(bolt);
		PlayS("laser.wav");
	}

	// Move user bolts
	std::vector<Projectile*>::iterator bIt;
	for (bIt = userBolts.begin(); bIt != userBolts.end(); bIt++)
	{
		if ((*bIt)->IsActive())
			(*bIt)->MoveUp();
		else
		{
			delete (*bIt);
			userBolts.erase(bIt);
			break;
		}
	}
}

void GameManager::DrawEnemyBolts()
{
	std::vector<Projectile*>::iterator bIt;
	for (bIt = enemyBolts.begin(); bIt != enemyBolts.end(); bIt++)
	{
		if ((*bIt)->IsActive())
			(*bIt)->Draw_Current_Frame();
		else
			break;
	}
}

void GameManager::UpdateEnemyBolts()
{
	// Add new enemy bolts
	static int enemyShootTimer = 0;			// Time enemy shots
	static int numOfRowMin1 = 5 - 1;		// Number of enemy rows minus 1
	static int numOfColMin1 = 11 - 1;		// Number of enemy columns minus 1
	int delay = (rand() % 1200) + 1000;		// Random delay between 1000-2200 ticks
	int randEnemyRow = (rand() % numOfRowMin1) + 1;	// Random enemy row number
	int randEnemyCol = (rand() % numOfColMin1) + 1;	// Random enemy column number
	int colCntr = 0;						// Counter for columns
	int rowCntr = 0;						// Counter for rows
	bool boltFired = false;

	std::vector<std::vector<Enemy*>>::iterator mIt;
	std::vector<Enemy*>::iterator iIt;
	// Spawn shot from a random enemy every 1000-2200 ticks
	if (GetTickCount() - enemyShootTimer >= delay)
	{
		enemyShootTimer = GetTickCount();	// Reset timer
		for (mIt = enemies.begin(); mIt != enemies.end(); mIt++)
		{		
			rowCntr++;
			for (iIt = mIt->begin(); iIt != mIt->end(); iIt++)
			{
				colCntr++;
				if (randEnemyRow == rowCntr
					&& randEnemyCol == colCntr)
				{
					Projectile *bolt = new Projectile(LoadTexture("eBolt.bmp"),
						(*iIt)->GetX() + ((*iIt)->GetWidth() / 2), (*iIt)->GetY(),
						0, 10, 20, 5, 5, 1.0f, 1, 1.0f);
					enemyBolts.push_back(bolt);
					boltFired = true;
					break;
				}
			}
			colCntr = 0;	// reset column cntr
			if (boltFired)
				break;
		}
	}

	// Move user bolts
	std::vector<Projectile*>::iterator bIt;
	for (bIt = enemyBolts.begin(); bIt != enemyBolts.end(); bIt++)
	{
		if ((*bIt)->IsActive())
			(*bIt)->MoveDown();
		else
		{
			delete (*bIt);
			enemyBolts.erase(bIt);
			break;
		}
	}
}

void GameManager::CheckCollision()
{
	std::vector<std::vector<Enemy*>>::iterator mIt;	// Main iterator
	std::vector<Projectile*>::iterator bIt;			// Projectile iterator
	std::vector<Enemy*>::iterator iIt;				// Internal iterator

	// Check collision for enemy and userBolts
	for (mIt = enemies.begin(); mIt != enemies.end(); mIt++)
	{
		for (iIt = mIt->begin(); iIt != mIt->end(); iIt++)
		{
			for (bIt = userBolts.begin(); bIt != userBolts.end(); bIt++)
			{
				// If bolt hits an enemy, deactivate both
				if (CollisionD((*iIt), (*bIt)))
				{
					(*iIt)->Deactivate();
					(*bIt)->Deactivate();

					// Intialize new explosion and add to vector
					Sprite *explosion = new Sprite(LoadTexture("Explosion_Sprite_Sheet.png", _white),
						(*iIt)->GetX(), (*iIt)->GetY() - 20, 0, 64, 64, 0, 0, 1.0f, 5, 1.0f, 0, 24, 1, 10);
					explosions.push_back(explosion);
					enemyExploding = true;
					playerScore++;
				}

				// Check collision w/ bonusEnemy
				if (bonusActive)
				{
					if (CollisionD((*bIt), bonusEnemy))
					{
						bonusEnemy->Deactivate();
						(*bIt)->Deactivate();
						bonusActive = false;

						// Intialize new explosion and add to vector
						Sprite *explosion = new Sprite(LoadTexture("Explosion_Sprite_Sheet.png", _white),
							bonusEnemy->GetX(), bonusEnemy->GetY() - 20, 0, 64, 64, 0, 0, 1.0f, 5, 1.0f, 0, 24, 1, 10);
						explosions.push_back(explosion);
						enemyExploding = true;
						playerScore += 5;
						PlayS("explosion2.wav");
					}
				}
			}

			if (CollisionD(ship, (*iIt)))
			{
				ship->Deactivate();
				(*iIt)->Deactivate();
				Sprite *explosion = new Sprite(LoadTexture("Explosion_Sprite_Sheet.png", _white),
					ship->GetX(), ship->GetY() - 20, 0, 64, 64, 0, 0, 1.0f, 5, 1.0f, 0, 24, 1, 35);
				explosions.push_back(explosion);
				explosion = new Sprite(LoadTexture("Explosion_Sprite_Sheet.png", _white),
					(*iIt)->GetX(), (*iIt)->GetY() - 20, 0, 64, 64, 0, 0, 1.0f, 5, 1.0f, 0, 24, 1, 10);
				explosions.push_back(explosion);
				enemyExploding = true;
				shipExploding = true;
				PlayS("explosion3.wav");
			}
		}
	}

	// Check collision for ship and enemyBolts
	for (bIt = enemyBolts.begin(); bIt != enemyBolts.end(); bIt++)
	{
		if (CollisionD((*bIt), ship))
		{
			(*bIt)->Deactivate();
			ship->Deactivate();
			Sprite *explosion = new Sprite(LoadTexture("Explosion_Sprite_Sheet.png", _white),
				ship->GetX(), ship->GetY() - 20, 0, 64, 64, 0, 0, 1.0f, 5, 1.0f, 0, 24, 1, 35);
			explosions.push_back(explosion);
			shipExploding = true;
			pause = true;
			playerLives--;
			if (playerLives <= 0)
				playerDead = true;
			PlayS("explosion3.wav");
		}
	}
}

void GameManager::DrawExplosions()
{
	// If no explosions exist, stop drawing explosions
	if (explosions.size() <= 0)
	{
		enemyExploding = false;
		shipExploding = false;
	}
	else
	{
		// For each explosion, draw and animate
		std::vector<Sprite*>::iterator eIt;
		for (eIt = explosions.begin(); eIt != explosions.end(); eIt++)
		{
			if ((*eIt)->IsActive())
			{
				(*eIt)->Draw_Current_Frame();
				(*eIt)->Sprite_Animate((*eIt)->GetStartFrame(),
					(*eIt)->GetEndFrame(), (*eIt)->GetDelay());

				// If at end of animation deactivate explosion
				if ((*eIt)->GetFrame() >= (*eIt)->GetEndFrame())
					(*eIt)->Deactivate();
			}
			else
			{
				delete (*eIt);
				explosions.erase(eIt);
				break;
			}
		}
	}
}

void GameManager::UIInit()
{
	// Load lives number pos
	numbers[0]->SetX(lives_sprite->GetX() + lives_sprite->GetWidth() - 15);
	numbers[0]->SetY(lives_sprite->GetY() - 5);

	// Load score number pos
	numbers[1]->SetX(score_sprite->GetX() + score_sprite->GetWidth() - 13);
	numbers[1]->SetY(score_sprite->GetY() - 5);
	numbers[2]->SetX(numbers[1]->GetX() + numbers[1]->GetWidth() - 35);
	numbers[2]->SetY(score_sprite->GetY() - 5);
}

void GameManager::DrawUI()
{
	// Draw lives text
	lives_sprite->Draw_Current_Frame();
	numbers[0]->Draw_Frame(playerLives);

	// Draw score text
	score_sprite->Draw_Current_Frame();
	if (playerScore < 10)
		numbers[1]->Draw_Frame(playerScore);
	else if (playerScore >= 10)
	{
		numbers[1]->Draw_Frame(playerScore / 10);
		numbers[2]->Draw_Frame(playerScore % 10);
	}
}

void GameManager::ReverseHorde()
{
	std::vector<std::vector<Enemy*>>::iterator mIt;// Main vector iterator 
	std::vector<Enemy*>::iterator iIt;
	for (mIt = enemies.begin(); mIt != enemies.end(); mIt++)
	{
		for (iIt = mIt->begin(); iIt != mIt->end(); iIt++)
		{
			(*iIt)->SetVelX((*iIt)->GetVelX() * -1);
			(*iIt)->SetY((*iIt)->GetY() + (*iIt)->GetVelY());
			if ((*iIt)->GetY() + (*iIt)->GetHeight() >= SCREENH)
			{
				Sleep(100);
				playerDead = true;
			}
				
		}
	}
}

void GameManager::ResetShip()
{
	// Reset the ship to original position
	ship->Activate();
	ship->SetX((SCREENW / 2) - (ship->GetWidth() / 2));
	ship->SetY(SCREENH - 75);
	pause = false;
}

void GameManager::ResetGame()
{

}

// Distance Collision
bool GameManager::CollisionD(Sprite *sprite1, Sprite *sprite2)
{
	double radius1, radius2;

	// Calculate radius 1
	if (sprite1->GetWidth() > sprite1->GetHeight())
		radius1 = (sprite1->GetWidth()) / 2.0;
	else
		radius1 = (sprite1->GetHeight()) / 2.0;

	// Center point 1
	double x1 = sprite1->GetX() + radius1;
	double y1 = sprite1->GetY() + radius1;
	D3DXVECTOR2 vector1(x1, y1);
	// Calculate radius 2
	if (sprite2->GetWidth() > sprite2->GetHeight())
		radius2 = (sprite2->GetWidth()) / 2.0;
	else
		radius2 = (sprite2->GetHeight()) / 2.0;

	// Center point 2
	double x2 = sprite2->GetX() + radius2;
	double y2 = sprite2->GetY() + radius2;
	D3DXVECTOR2 vector2(x2, y2);

	// Calculate distance
	double deltax = vector1.x - vector2.x;
	double deltay = vector2.y - vector1.y;
	double dist = sqrt((deltax * deltax) + (deltay * deltay));

	// Return distance comparison
	return (dist < radius1 + radius2);
}