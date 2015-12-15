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
	int nrOfCollectors;
	bool isBuilderSet;
	bool recources;
	bool barracksBuilt;
	std::vector<TilePosition> buildPos;
	Unit* bulider;
	Unit* startLocation;
	Unit* vespinGas;
	ToBuild* currentBuildingObj;
	ToBuild* currentUnitBuildObj;
	std::vector<Unit*> workers;
public:
	ConstructionAgent();
	ConstructionAgent(TilePosition start);
	void onFrame(ToBuild* buildingsToBuild,ToBuild* unitsToBuild,Goal AI_Goal);
	TilePosition findBuildSpot(UnitType toBuild);
	void onUnitComplete(BWAPI::Unit* unit);
	void onUnitCreated(BWAPI::Unit* unit,Goal AI_Goal);
	void produceExtraUnit(Goal AI_Goal);
	UnitType whatBuilds(UnitType unit);

};

#endif