#ifndef UNITAGENT_H
#define UNITAGENT_H

#include <windows.h>
#include "ExampleAIModule.h"
#include <vector>
#include <BWTA.h>
#include <BWAPI.h>

using namespace BWAPI;
using namespace BWTA;

class UnitAgent
{
private:
	bool areAttacking;
	int nrOfUnits;
	BWAPI::Position enemyPos;

	std::vector<Unit*> battleUnits;
	BWAPI::Position recentChokePoint;
public:
	UnitAgent();
	virtual ~UnitAgent();
	void onUnitComplete(BWAPI::Unit *unit);
	void setNewChokePoint(BWAPI::Position position);
	void onFrame();
	int getNrOfUnits() const;
	void attack(BWAPI::TilePosition EnemyPos);
};




#endif