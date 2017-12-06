#pragma once
#include "Entity.h"
#include "Renderable.h"
#include "Movement.h"
#include "Targetable.h"
#include "GeometryUtil.h"
#include "Particle.h"


static std::string const UnitActions[] = { "Stop","Move","Attack","AttackMove","Hold","Harvest", "Build" };
class Unit : public Component
{

public:
	enum Action { Stop, Move, Attack, AttackMove, Hold, Harvest, Build};
protected:
	// What action this unit is to perform.
	Action action = Move;
	// What entity is it looking to attack?
	std::shared_ptr<Entity> targetEntity = NULL;

	// what team this is on.
	int team;

	double weaponRange=10;
	double sightRange=12;

	double fireRate = 0.5;
	double timeSinceLastFire = 0;

	bool targetAcquired = false;
	bool canShoot = true;

	// Bullet container.
	std::vector<BulletParticle> projectiles;

	void from_json(const nlohmann::json &j) {};
public:

	Unit() : Component("Unit") {};
	Unit(std::string type) : Component(type) {};
	~Unit() {};


	void SetAction(Action act);
	Action GetAction() { return action; }
	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }

	double GetFireRate() { return fireRate; }
	void SetFireRate(float fr) { fireRate = fr; }

	void SetEntityToTarget(std::shared_ptr<Entity>& target);

	void AcquireTarget();
	virtual void AttackEntity();
	
	// Methods that worker will build upon.
	virtual void HarvestResource() {}
	virtual void BuildStructure() {}


	void Update(double deltaTime) override;
	void Render();
};