#pragma once
#include "Player.h"
#include "BoundingSphere.h"
#include "Game.h"
class AiPlayer : public Player
{
	int workerCount;

	int factoryCount;
	int vehicleBayCount;
	int hangerCount;
	
	int droneCount;
	int wardenCount;
	int kestralCount;

	bool workerbuilding = false;
	bool moving = false;
	bool factoryMade = false;

	int unitsQueued = 0;
public:
	void Update(std::vector<Entity*>& enemyList);

	//Update unit, building and supply counts
	void CheckProperty();

	//Manage the economy
	void MacroCycle();

	//Manage all units
	void ArmyCycle();

	//Ai decisions are handled here.
	void HandleAiLogic(std::vector<Entity*>& enemyList);
};