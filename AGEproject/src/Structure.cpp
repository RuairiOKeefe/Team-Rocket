#include "Structure.h"
#include "Spawner.h"
void Structure::from_json(const nlohmann::json & j)
{
}

Structure::Structure() : building(true), constructionTime(0.0f), Component("Structure")
{

}

Structure::~Structure()
{
}

void Structure::Build(double delta)
{
	ammountBuilt += (float)delta;
	if (ammountBuilt >= constructionTime)
	{
		building = false;
		ammountBuilt = 0.0f;
	}
}

void Structure::AddProduct(int& bal,int hotkey, glm::vec3 destination)
{
	if (spawnData.size() == 0)
		return;
	if (hotkey > spawnData.size() - 1)
		return;
	int newBalance = bal - spawnData[hotkey].cost;
	// If the balance is invalid, do not allow object to be queued.
	if (newBalance < 0)
		return;
	value += spawnData[hotkey].cost;
	std::cout << GetTeam() << " balance is now: " << newBalance << std::endl;
	bal = newBalance;
	Product tempProduct;
	tempProduct.productName = spawnData[hotkey].unitType;
	tempProduct.buildTime = spawnData[hotkey].buildTime;
	tempProduct.destination = destination;
	productQueue.push(tempProduct);
}

void Structure::Produce(double delta)
{	
	ammountBuilt += delta;
	if (ammountBuilt >= productQueue.front().buildTime)
	{
		// Spawn unit should be from factory pattern class, not game!
		collectionQueue.push_back(Spawner::Get().CreateEntity(productQueue.front().productName,productQueue.front().destination,team));
		ammountBuilt = 0.0f;
		productQueue.pop();
	}
}

void Structure::Update(double delta)
{

	// Handle worker logic.
	if (GetParent()->GetName() == "Worker" && productQueue.size() > 0)
	{
		// Make unit go to destination first.
		GetParent()->GetCompatibleComponent<Movement>()->SetDestination(productQueue.front().destination);
		if (glm::distance(GetParent()->GetPosition(), glm::dvec3(productQueue.front().destination)) > 7)
			return;
		else
			GetParent()->GetCompatibleComponent<Movement>()->SetDestination(GetParent()->GetPosition());
		// If it is in range, allow construction.
	}

	if (building)
	{
		Build(delta);
		return;
	}
	if(productQueue.size() != 0)
		Produce(delta);
}

void Structure::Collect(std::vector<Entity*>& ents)
{
	for (Entity*&e:collectionQueue)
		ents.push_back(e);
	collectionQueue.clear();
	
}

void Structure::IsController(bool act)
{
	isControlled = act;
	// If it is being selected.
	if (act)
	{
		// Hold current emissive value.
		tempCol = glm::vec4(GetParent()->GetComponent<Renderable>().GetMaterial().emissive);
		// Set objects emissive value to blue (for now). 
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 0, 1, 1);
	}
	else
	{
		// Return the emissive colour back to its original value.
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(tempCol);
		tempCol = glm::vec4();
	}
}