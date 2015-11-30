#pragma once
#include <BWAPI.h>

#include <BWTA.h>
#include <windows.h>
#include <List>

extern bool analyzed;
extern bool analysis_just_finished;
extern BWTA::Region* home;
extern BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();

using namespace BWAPI;
using namespace BWTA;

class ExampleAIModule : public BWAPI::AIModule
{
public:
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
//
//class General
//{
//};
//
//class Agent
//{
//};
//
//class ConstructAgent : public Agent
//{
//};
//
//class UnitAgent : public Agent
//{
//};

class ToBuild
{
private:
	UnitType* unitType;
	int quantity;
public:
	ToBuild(UnitType *type,int quantity);
	ToBuild();
	int getQuantity() const;
	UnitType* getUnitType() const;
	
	void setQuantity(const int &newQuantity);
	void setUnitType(UnitType *unitType);
};

class Goal
{
//array av det som finns
private:
	ToBuild[5] toDo;
public:
	Goal();
	void addObjective(UnitType *unit, int nrOfTypes);
	string nextUnitName() const;
};

