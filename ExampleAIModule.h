#pragma once
#include <BWAPI.h>

#include <BWTA.h>
#include <windows.h>
#include <string>
#include <vector>

extern bool analyzed;
extern bool analysis_just_finished;
extern BWTA::Region* home;
extern BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();

using namespace BWAPI;
using namespace BWTA;
using namespace std;

class ExampleAIModule : public BWAPI::AIModule
{
public:
	bool startUp;
	//Methods inherited from BWAPI:AIModule
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player* player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player* player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit* unit);
	virtual void onUnitEvade(BWAPI::Unit* unit);
	virtual void onUnitShow(BWAPI::Unit* unit);
	virtual void onUnitHide(BWAPI::Unit* unit);
	virtual void onUnitCreate(BWAPI::Unit* unit);
	virtual void onUnitDestroy(BWAPI::Unit* unit);
	virtual void onUnitMorph(BWAPI::Unit* unit);
	virtual void onUnitRenegade(BWAPI::Unit* unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit *unit);

	//Own methods
	void drawStats();
	void drawTerrainData();
	void showPlayers();
	void showForces();
	Position findGuardPoint();
};

class ToBuild
{
private:
public:
	UnitType wantedUnits;
	int quantity;
	ToBuild(BWAPI::UnitType unit,int quantity);
	ToBuild();
	int getQuantity() const;
	UnitType getUnitType() const;
	
	void setQuantity(const int &newQuantity);
	void setUnitType(UnitType unit);


	void decreaseQuant();
};

class Goal
{
//array av det som finns
public:
	std::vector<ToBuild*> buildingsToBuild;
	std::vector<ToBuild*> unitsToBuild;
	std::vector<BWAPI::Position> attackPosition;
	Goal();
	void addObjective(UnitType unit,int nrOfTypes);
	void addAttackObjective(BWAPI::Position);
	void addScoutObj();
	string nextUnitName() const;

	ToBuild* getBuildingGoal();
	ToBuild* getUnitBuildGoal();
	void objectiveComplete();
	void pop();

	vector<ToBuild*> getToDo() const; 
};
