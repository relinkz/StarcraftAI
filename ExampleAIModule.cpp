#include "ExampleAIModule.h" 
#include "ConstructionAgent.h"
#include <cstdlib>
#include <ctime>
using namespace BWAPI;

TilePosition getBuildLocation(TilePosition startLocation, UnitType building);

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;

BWAPI::UnitType test = BWAPI::UnitTypes::Terran_SCV;
BWAPI::UnitType test2 = BWAPI::UnitTypes::Terran_Supply_Depot;
BWAPI::UnitType test3 = BWAPI::UnitTypes::Terran_Barracks;
BWAPI::UnitType test4 = BWAPI::UnitTypes::Terran_Marine;

BWAPI::UnitType test5 = BWAPI::UnitTypes::Terran_Academy;
BWAPI::UnitType test6 = BWAPI::UnitTypes::Terran_Refinery;
BWAPI::UnitType test7 = BWAPI::UnitTypes::Terran_Medic;

BWAPI::UnitType test8 = BWAPI::UnitTypes::Terran_Factory;
BWAPI::UnitType test9 = BWAPI::UnitTypes::Terran_Machine_Shop;
BWAPI::UnitType test10 = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;

//UnitTypes test = BWAPI::UnitTypes::Terran_SCV;
Goal AI_Goal = Goal();
ConstructionAgent CAgent = ConstructionAgent();
ToBuild *defaultGoal = new ToBuild();

//This is the startup method. It is called once
//when a new game has been started with the bot.
void ExampleAIModule::onStart()
{
	
	this->startUp = true;
	//step 1
	AI_Goal.addObjective(test, 2);
	AI_Goal.addObjective(test2, 3);
	AI_Goal.addObjective(test3, 2);
	AI_Goal.addObjective(test4, 15);
	//step 2
	AI_Goal.addObjective(test5, 1);
	AI_Goal.addObjective(test2, 1);
	AI_Goal.addObjective(test6, 1);
	AI_Goal.addObjective(test7, 3);
	AI_Goal.addObjective(test, 5);
	//step3
	AI_Goal.addObjective(test8, 1);
	AI_Goal.addObjective(test9, 1);
	//AI_Goal.addObjective(test10, 3);

	Broodwar->sendText("Hello world!");
	CAgent = ConstructionAgent(Broodwar->self()->getStartLocation());
	
	//Enable flags
	Broodwar->enableFlag(Flag::UserInput);
	//Uncomment to enable complete map information
	Broodwar->enableFlag(Flag::CompleteMapInformation);
	Broodwar->enableFlag(1);
	
	//Start analyzing map data
	BWTA::readMap();
	analyzed=false;
	analysis_just_finished=false;
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL); //Threaded version
	AnalyzeThread();

	//AI_Goal.addObjective(UnitTypes::Terran_SCV,2);

    //Send each worker to the mineral field that is closest to it
    /*for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
		if ((*i)->getType().isWorker())
		{
			Unit* closestMineral=NULL;
			for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
			{
				if (closestMineral==NULL || (*i)->getDistance(*m)<(*i)->getDistance(closestMineral))
				{	
					closestMineral=*m;
				}
			}
			if (closestMineral!=NULL)
			{
				(*i)->rightClick(closestMineral);
				Broodwar->printf("Send worker %d to mineral %d", (*i)->getID(), closestMineral->getID());
			}
		}
	}*/
	for (int i = 0; i < Flag::Max; ++i)
	{
		Broodwar->enableFlag(i);
	}
	Broodwar->sendText("power overwhelming");
	Broodwar->sendText("show me the money");
	Broodwar->sendText("/show visibility");

	Broodwar->printf("/cheats");
}

//Called when a game is ended.
//No need to change this.
void ExampleAIModule::onEnd(bool isWinner)
{
	if (isWinner)
	{
		Broodwar->sendText("I won!");
	}
}

//Finds a guard point around the home base.
//A guard point is the center of a chokepoint surrounding
//the region containing the home base.
Position ExampleAIModule::findGuardPoint()
{
	//Get the chokepoints linked to our home region
	std::set<BWTA::Chokepoint*> chokepoints = home->getChokepoints();
	double min_length = 10000;
	BWTA::Chokepoint* choke = NULL;

	//Iterate through all chokepoints and look for the one with the smallest gap (least width)
	for(std::set<BWTA::Chokepoint*>::iterator c = chokepoints.begin(); c != chokepoints.end(); c++)
	{
		double length = (*c)->getWidth();
		if (length < min_length || choke==NULL)
		{
			min_length = length;
			choke = *c;
		}
	}

	return choke->getCenter();
}

//This is the method called each frame. This is where the bot's logic
//shall be called.
void ExampleAIModule::onFrame()
{
	/*if(AI_Goal.toDo.size() != 0 && AI_Goal.toDo.front()->getQuantity() == 0)
	{
		AI_Goal.toDo.erase(AI_Goal.toDo.begin());
		BWAPI::Broodwar->printf("Goal finnished");
		if(this->toDo.size() != 0 && this->toDo.front()->getQuantity() == 0)
		{
			this->toDo.erase(toDo.begin());
			BWAPI::Broodwar->printf("Goal finnished");
		}
	}*/
	if (Broodwar->getFrameCount() > 100)
	{
		CAgent.onFrame(AI_Goal.getBuildingGoal(),AI_Goal.getUnitBuildGoal(),AI_Goal);
		AI_Goal.pop();
		this->startUp = false;
	}
	if(Broodwar->getFrameCount() == 1)
	{
	Broodwar->printf("power overwhelming");
	Broodwar->printf("show me the money");
	Broodwar->printf("/cheats");
	}
	//Call every 100:th frame
	if (Broodwar->getFrameCount() % 100 == 0)
	{
		//getBuildLocation(Broodwar->self()->getStartLocation(),AI_Goal.getGoal().getUnitType());
		//Order one of our workers to guard our chokepoint.
		//Iterate through the list of units.
		for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
		{
			bool commandedFlag = false;
			//Check if unit is a worker.
			/*if ((*i)->getType().isWorker())
			{

				Unit* closestMineral=NULL;
				for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
				{
					if (closestMineral==NULL || (*i)->getDistance(*m)<(*i)->getDistance(closestMineral))
					{	
						closestMineral=*m;
					}
				}
				if(AI_Goal.getGoal().getUnitType().isBuilding() && Broodwar->self()->minerals() >= 150 && commandedFlag == false)
				{

					TilePosition test = TilePosition(0, -5);
					Broodwar->printf("cannot build, something in the way");
					(*i)->build(Broodwar->self()->getStartLocation()- test,AI_Goal.getGoal().getUnitType());
					commandedFlag = true;
				}
				else if (closestMineral!=NULL)
				{
					(*i)->rightClick(closestMineral);
					Broodwar->printf("Send worker %d to mineral %d", (*i)->getID(), closestMineral->getID());
				}
			}

			else if((*i)->getType().isResourceDepot() && AI_Goal.getGoal().getUnitType() != UnitTypes::Buildings)
			{
				if((*i)->isIdle())
				{
					if(AI_Goal.getGoal().getQuantity() > 0 && Broodwar->self()->minerals() >= 50)
					{
						//((*i)->train(AI_Goal.getGoal().getUnitType()));
						//AI_Goal.getGoal().decreaseQuant();		
					}
					
					
				}
			}*/
		}
	}
  
	//Draw lines around regions, chokepoints etc.
	if (analyzed)
	{
		drawTerrainData();
	}
}

//Is called when text is written in the console window.
//Can be used to toggle stuff on and off.
void ExampleAIModule::onSendText(std::string text)
{
	if (text=="/show players")
	{
		showPlayers();
	}
	else if (text=="/show forces")
	{
		showForces();
	}
	else
	{
		Broodwar->printf("You typed '%s'!",text.c_str());
		Broodwar->sendText("%s",text.c_str());
	}
}

//Called when the opponent sends text messages.
//No need to change this.
void ExampleAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
	Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
}

//Called when a player leaves the game.
//No need to change this.
void ExampleAIModule::onPlayerLeft(BWAPI::Player* player)
{
	Broodwar->sendText("%s left the game.",player->getName().c_str());
}

//Called when a nuclear launch is detected.
//No need to change this.
void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{
	if (target!=Positions::Unknown)
	{
		Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
	}
	else
	{
		Broodwar->printf("Nuclear Launch Detected");
	}
}

//No need to change this.
void ExampleAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

//No need to change this.
void ExampleAIModule::onUnitEvade(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

//No need to change this.
void ExampleAIModule::onUnitShow(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

//No need to change this.
void ExampleAIModule::onUnitHide(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

//Called when a new unit has been created.
//Note: The event is called when the new unit is built, not when it
//has been finished.
void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}
	if(this->startUp == false)
	{
		//Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
		CAgent.onUnitCreated(unit,AI_Goal);
	}
}

//Called when a unit has been destroyed.
void ExampleAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		Broodwar->sendText("My unit %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}
	else
	{
		Broodwar->sendText("Enemy unit %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}
}

//Only needed for Zerg units.
//No need to change this.
void ExampleAIModule::onUnitMorph(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

//No need to change this.
void ExampleAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
	//Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
}

//No need to change this.
void ExampleAIModule::onSaveGame(std::string gameName)
{
	//Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

//Analyzes the map.
//No need to change this.
DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();

	//Self start location only available if the map has base locations
	if (BWTA::getStartLocation(BWAPI::Broodwar->self())!=NULL)
	{
		home = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
	}
	//Enemy start location only available if Complete Map Information is enabled.
	if (BWTA::getStartLocation(BWAPI::Broodwar->enemy())!=NULL)
	{
		enemy_base = BWTA::getStartLocation(BWAPI::Broodwar->enemy())->getRegion();
	}
	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

//Prints some stats about the units the player has.
//No need to change this.
void ExampleAIModule::drawStats()
{
	std::set<Unit*> myUnits = Broodwar->self()->getUnits();
	Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
	std::map<UnitType, int> unitTypeCounts;
	for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
	{
		if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
		{
			unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
		}
		unitTypeCounts.find((*i)->getType())->second++;
	}
	int line=1;
	for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
	{
		Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
		line++;
	}
}

//Draws terrain data aroung regions and chokepoints.
//No need to change this.
void ExampleAIModule::drawTerrainData()
{
	//Iterate through all the base locations, and draw their outlines.
	for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
	{
		TilePosition p=(*i)->getTilePosition();
		Position c=(*i)->getPosition();
		//Draw outline of center location
		Broodwar->drawBox(CoordinateType::Map,p.x()*32,p.y()*32,p.x()*32+4*32,p.y()*32+3*32,Colors::Blue,false);
		//Draw a circle at each mineral patch
		for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getStaticMinerals().begin();j!=(*i)->getStaticMinerals().end();j++)
		{
			Position q=(*j)->getInitialPosition();
			Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Cyan,false);
		}
		//Draw the outlines of vespene geysers
		for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getGeysers().begin();j!=(*i)->getGeysers().end();j++)
		{
			TilePosition q=(*j)->getInitialTilePosition();
			Broodwar->drawBox(CoordinateType::Map,q.x()*32,q.y()*32,q.x()*32+4*32,q.y()*32+2*32,Colors::Orange,false);
		}
		//If this is an island expansion, draw a yellow circle around the base location
		if ((*i)->isIsland())
		{
			Broodwar->drawCircle(CoordinateType::Map,c.x(),c.y(),80,Colors::Yellow,false);
		}
	}
	//Iterate through all the regions and draw the polygon outline of it in green.
	for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
	{
		BWTA::Polygon p=(*r)->getPolygon();
		for(int j=0;j<(int)p.size();j++)
		{
			Position point1=p[j];
			Position point2=p[(j+1) % p.size()];
			Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Green);
		}
	}
	//Visualize the chokepoints with red lines
	for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
	{
		for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
		{
			Position point1=(*c)->getSides().first;
			Position point2=(*c)->getSides().second;
			Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Red);
		}
	}
}

//Show player information.
//No need to change this.
void ExampleAIModule::showPlayers()
{
	std::set<Player*> players=Broodwar->getPlayers();
	for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
	{
		Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
	}
}

//Show forces information.
//No need to change this.
void ExampleAIModule::showForces()
{
	std::set<Force*> forces=Broodwar->getForces();
	for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++)
	{
		std::set<Player*> players=(*i)->getPlayers();
		Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
		for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
		{
			Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
		}
	}
}

//Called when a unit has been completed, i.e. finished built.
void ExampleAIModule::onUnitComplete(BWAPI::Unit *unit)
{

	if(this->startUp == false)
	{
		//Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
		CAgent.onUnitComplete(unit);
	}
}

TilePosition getBuildLocation(TilePosition startLocation, UnitType building)
{
	/*
	Super advänced building location finder. copyright axel and sweppzi software.
	*/
	/*std::srand((unsigned) time(NULL));
	int randomX = std::rand();
	int randomY = std::rand() * randomX;
	randomY %= 10;
	randomX %=10;

	char tesar[10];
	char tesar1[10];

	_itoa(randomY,tesar,10);
	_itoa(randomX,tesar1,10);

	Broodwar->printf(tesar);
	Broodwar->printf(tesar1);

	

	for(int i = 0; i < 4; i++)
	{
		if(test1.isValid())
		{
			if(test1.x - 1)
			{
			}
		}
	
	}*/
	TilePosition test1(-1,-1);

	
	
	return test1;
}
/*
Axel unt Sweppes klasser
*/

ToBuild::ToBuild()
{
	this->wantedUnits = BWAPI::UnitTypes::None;
	this->quantity = 2;
}

ToBuild::ToBuild(UnitType unit, int quantity)
{
	this->wantedUnits = unit;
	this->quantity = quantity;
}

int ToBuild::getQuantity() const
{
	return this->quantity;
}

UnitType ToBuild::getUnitType() const
{
	return this->wantedUnits;
}

void ToBuild::setQuantity(const int &newQuantity)
{
	this->quantity = newQuantity;
}

void ToBuild::setUnitType(UnitType unit)
{

	this->wantedUnits = unit;
}

void ToBuild::decreaseQuant()
{
	this->quantity--;
	char tesar[10];
	_itoa(this->quantity,tesar,10);
	BWAPI::Broodwar->printf(tesar);
}

Goal::Goal()
{
	this->buildingsToBuild.empty();
	this->unitsToBuild.empty();
}

void Goal::addObjective(UnitType unit, int nrOfTypes)
{
	
	if(unit.isBuilding() || unit.isRefinery())
	{
		ToBuild* toAdd = new ToBuild(unit, nrOfTypes);

		this->buildingsToBuild.push_back(toAdd);
	}
	else
	{
		ToBuild* toAdd = new ToBuild(unit, nrOfTypes);

		this->unitsToBuild.push_back(toAdd);
	}
}

string Goal::nextUnitName() const
{
	string test2 = "derp";
	return test2;
}

ToBuild* Goal::getBuildingGoal()
{
	ToBuild* temp = defaultGoal;

	if(this->buildingsToBuild.size() != 0)
	{
		temp = this->buildingsToBuild.front();
	}

	return temp;
}
ToBuild* Goal::getUnitBuildGoal()
{
	ToBuild* temp = defaultGoal;

	if(this->unitsToBuild.size() != 0)
	{
		temp = this->unitsToBuild.front();
	}

	return temp;
}
void Goal::pop()
{
	if(this->buildingsToBuild.size() != 0 && this->buildingsToBuild.front()->getQuantity() == 0)
	{
		this->buildingsToBuild.erase(buildingsToBuild.begin());
		BWAPI::Broodwar->printf("Goal finnished");
	}
	for(int i = 0; i < this->unitsToBuild.size();i++)
	{
		if(this->unitsToBuild.size() != 0 && this->unitsToBuild.at(i)->getQuantity() == 0)
		{
		this->unitsToBuild.erase(unitsToBuild.begin() + i);
		BWAPI::Broodwar->printf("Goal finnished");
		}
	}
	/*if(this->unitsToBuild.size() != 0 && this->unitsToBuild.front()->getQuantity() == 0)
	{
		this->unitsToBuild.erase(unitsToBuild.begin());
		BWAPI::Broodwar->printf("Goal finnished");
	}*/
}
