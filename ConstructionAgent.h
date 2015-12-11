#ifndef CONSTRUCTIONAGENT_H
#define CONSTRUCTIONAGNET_H

#include "ExampleAIModule.h"
#include <vector>
#include <BWAPI.h>

#include <BWTA.h>
#include <windows.h>

using namespace BWAPI;
using namespace BWTA;

class ConstructionAgent
{
private:
	int nrOfWorkers;
	bool isBuilderSet;
	std::vector<TilePosition> buildPos;
	Unit* bulider;
	ToBuild* currentObj;
	std::vector<Unit*> workers;
public:
	ConstructionAgent();
	ConstructionAgent(TilePosition start);
	void onFrame(ToBuild* toBuild);
	TilePosition findBuildSpot(UnitType toBuild);
	void isUnitWorker(BWAPI::Unit* unit);

};

#endif