#include "ConstructionAgent.h"


ConstructionAgent::ConstructionAgent()
{
}
ConstructionAgent::ConstructionAgent(TilePosition start)
{
	this->recources = false;
	this->barracksBuilt = false;
	this->vespinGas = NULL;
	int xStart = -5;
	int xEnd = 5;
	int yStart = -5;
	int yEnd = 5;
	if(BWAPI::Broodwar->self()->getStartLocation().y() < BWAPI::Broodwar->mapHeight() / 2)
	{
		yStart += 5;
		yEnd += 5;
	}
	else
	{
		yStart -= 5;
		yEnd -= 5;
	}
	if(BWAPI::Broodwar->self()->getStartLocation().x() < BWAPI::Broodwar->mapWidth() / 2)
	{
		xStart += 5;
		xEnd += 5;
	}
	else
	{
		xStart -= 5;
		xEnd -= 5;
	}


	this->nrOfCollectors = 0;
	this->isBuilderSet = false;
	for(int y =yStart; y < yEnd; y ++)
	{
		for(int x = xStart; x < xEnd; x ++)
		{
			TilePosition temp = TilePosition(start.x() + x,start.y() +y);
			this->buildPos.push_back(temp);
		}
	}
	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
	{
		if((*i)->getType().isResourceDepot())
		{
			this->startLocation = *i;
			break;
		}
	}
	/*TilePosition temp = TilePosition(start.x() - 5,start.y() -5);
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x()  ,start.y() - 5);
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x() + 5 ,start.y() - 5);
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x() - 5 ,start.y() );
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x() + 5 ,start.y() );
	this->buildPos.push_back(temp);

		temp = TilePosition(start.x() - 5 ,start.y() + 5);
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x()  ,start.y() + 5 );
	this->buildPos.push_back(temp);

	temp = TilePosition(start.x() + 5 ,start.y() + 5 );
	this->buildPos.push_back(temp);*/


	for(std::set<Unit*>::const_iterator i=BWAPI::Broodwar->self()->getUnits().begin();i!=BWAPI::Broodwar->self()->getUnits().end();i++)
	{
		if((*i)->getType().isWorker() && (*i)->getPlayer() == BWAPI::Broodwar->self())
		{
			if(this->isBuilderSet == false)
			{
				//this->nrOfWorkers++;
				this->bulider = (*i);
				this->isBuilderSet = true;
			}
			else
			{
				this->nrOfCollectors++;
				this->workers.push_back((*i));
			}
		}
	}

	char tesar[10];
	int temp = this->nrOfCollectors;
	_itoa(temp,tesar,10);
	BWAPI::Broodwar->printf(tesar);

	Unit* closestMineral=NULL;
	for(int i = 0; i < this->nrOfCollectors; i++)
	{
			for(std::set<Unit*>::iterator m=BWAPI::Broodwar->getMinerals().begin();m!=BWAPI::Broodwar->getMinerals().end();m++)
			{
				if (closestMineral==NULL || this->workers.at(i)->getDistance(*m)<this->workers.at(i)->getDistance(closestMineral))
				{	
					closestMineral=*m;
				}
			}
			if (closestMineral!=NULL && this->workers.at(i)->isIdle() && this->workers.at(i) != this->bulider)
			{
				this->workers.at(i)->rightClick(closestMineral);
			}
	}
	this->bulider->rightClick(closestMineral);

}

void ConstructionAgent::onFrame(ToBuild* buildingsToBuild,ToBuild* unitsToBuild,Goal AI_Goal)
{
	this->currentBuildingObj = buildingsToBuild;
	this->currentUnitBuildObj = unitsToBuild;
	int size = this->workers.size();

	char tesar[10];
	/*_itoa(this->nrOfWorkers,tesar,10);
	BWAPI::Broodwar->printf(tesar);*/
	if(this->currentBuildingObj->getUnitType().isBuilding() == true && BWAPI::Broodwar->self()->minerals() >= this->currentBuildingObj->getUnitType().mineralPrice() &&
		BWAPI::Broodwar->self()->gas() >= this->currentBuildingObj->getUnitType().gasPrice() && this->currentBuildingObj->getQuantity() > 0)
	{
		if(this->currentBuildingObj->getUnitType() == BWAPI::UnitTypes::Terran_Refinery)
		{
			Unit* closestGas=NULL;
			for(std::set<Unit*>::iterator m=BWAPI::Broodwar->getGeysers().begin();m!=BWAPI::Broodwar->getGeysers().end();m++)
			{
				if (closestGas==NULL || this->workers.at(0)->getDistance(*m)<this->workers.at(0)->getDistance(closestGas))
				{	
					closestGas=*m;
				}
			}
			if(closestGas!=NULL)
			{
				this->workers.at(0)->build(closestGas->getTilePosition(),this->currentBuildingObj->getUnitType());
				BWAPI::Broodwar->printf("build Refinery");
			}
		}
		else if(BWAPI::Broodwar->self()->minerals() >= 150 && this->bulider->isGatheringMinerals() && this->recources == false)
		{
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->rightClick(this->startLocation);
			this->bulider->getLastCommand().stop(this->bulider,true);
			this->recources = true;
		}
		else if(this->currentBuildingObj->getUnitType() == UnitTypes::Terran_Machine_Shop && BWAPI::Broodwar->self()->minerals() >= this->currentBuildingObj->getUnitType().mineralPrice() &&
																							 BWAPI::Broodwar->self()->gas() >= this->currentBuildingObj->getUnitType().gasPrice())
		{
			//this->bulider->buildAddon(this->currentBuildingObj->getUnitType());
			for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
			{
				if((*i)->getType() == UnitTypes::Terran_Factory)
				{
					(*i)->buildAddon(this->currentBuildingObj->getUnitType());
				}
			}
		}
		else if(this->bulider->isIdle() )
		{
			
			TilePosition buildSpot = this->findBuildSpot(this->currentBuildingObj->getUnitType());
			if(buildSpot.x() != -1 || buildSpot.y() != -1)
			{
				this->bulider->build(buildSpot,this->currentBuildingObj->getUnitType());
			}
			else
			{
				BWAPI::Broodwar->printf("du får icke bygga där");
			}
		}
	}
	if(BWAPI::Broodwar->self()->minerals() >= this->currentUnitBuildObj->getUnitType().mineralPrice() && this->currentUnitBuildObj->getQuantity() > 0)
	{
		for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
		{
			if(this->currentUnitBuildObj->getUnitType() == BWAPI::UnitTypes::Terran_SCV)
			{
				
				if( (*i)->getType().isResourceDepot() && (*i)->isIdle())
				{
					(*i)->train(this->currentUnitBuildObj->getUnitType());
				}
			}
			if(this->currentUnitBuildObj->getUnitType() == BWAPI::UnitTypes::Terran_Marine || this->currentUnitBuildObj->getUnitType() == BWAPI::UnitTypes::Terran_Medic)
			{
				if( (*i)->getType() == BWAPI::UnitTypes::Terran_Barracks && (*i)->isIdle())
				{
					(*i)->train(this->currentUnitBuildObj->getUnitType());
				}
			}
			if(this->currentUnitBuildObj->getUnitType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode)
			{
				if( (*i)->getType() == BWAPI::UnitTypes::Terran_Factory && (*i)->isIdle())
				{
					(*i)->train(this->currentUnitBuildObj->getUnitType());
				}
			}
		}

	}
	if(this->barracksBuilt == true)
	{
		this->produceExtraUnit(AI_Goal);
	}
	if(buildingsToBuild->getUnitType() == BWAPI::UnitTypes::Terran_Refinery)
	{
		for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
		{
			if((*i)->getType() == BWAPI::UnitTypes::Terran_Refinery && (*i)->getRemainingBuildTime() == 0)
			{
				BWAPI::Broodwar->printf("Refinery complete");
				this->currentBuildingObj->decreaseQuant();
				this->vespinGas = *i;				
				for(int i = 8; i < this->nrOfCollectors; i++)
				{
					this->workers.at(i)->rightClick(this->vespinGas);
				}
				break;
			}
		}
	}

	
	
	for(int i = 0; i < this->nrOfCollectors; i++)
	{
		Unit* closestMineral=NULL;
		for(std::set<Unit*>::iterator m=BWAPI::Broodwar->getMinerals().begin();m!=BWAPI::Broodwar->getMinerals().end();m++)
		{
			if (closestMineral==NULL || this->workers.at(i)->getDistance(*m)<this->workers.at(i)->getDistance(closestMineral))
			{	
				closestMineral=*m;
			}
		}
		if (closestMineral!=NULL && this->workers.at(i)->isIdle() && this->workers.at(i) != this->bulider && (this->vespinGas == NULL || i < 8))
		{
			this->workers.at(i)->rightClick(closestMineral);
		}
		else if(this->vespinGas != NULL && this->workers.at(i)->isIdle())
		{
			this->workers.at(i)->rightClick(this->vespinGas);
		}
	}
}
TilePosition ConstructionAgent::findBuildSpot(UnitType toBuild)
{
	TilePosition result = TilePosition(-1,-1);
	bool foundValidSpot = false;
	if(this->buildPos.size() != 0)
	{
	
		/*for(int i = 0; i < 8 && foundValidSpot == false; i++)
		{
			bool isValid = true;
			for(int y = 0; y < toBuild.tileHeight() && isValid == true; y++)
			{
				for(int x = 0; x < toBuild.tileWidth() && isValid == true; x++)
				{

					TilePosition temp = TilePosition((this->buildPos.at(i).x() + x),(this->buildPos.at(i).y() + y));
					if(temp.isValid() == false)
					{
						isValid = false;
					}
				}
			}
			if(isValid == true)
			{
				foundValidSpot = true;
				result = this->buildPos.at(i);
				this->buildPos.erase(this->buildPos.begin());
				BWAPI::Broodwar->printf("pop");
				

			}
		}*/
		result = this->buildPos.at(0);
		this->buildPos.erase(this->buildPos.begin());
		//BWAPI::Broodwar->printf("pop");
	}

	return result;
}
void ConstructionAgent::onUnitComplete(Unit* unit)
{
	//BWAPI::Broodwar->printf("ConstructionAgent registerd unit complete");
	if((unit->getType().isBuilding() || unit->getType().isRefinery()) && unit->getPlayer() == BWAPI::Broodwar->self())
	{
		BWAPI::Broodwar->printf("building complete");
		this->currentBuildingObj->decreaseQuant();
		if(unit->getType() == BWAPI::UnitTypes::Terran_Barracks)
		{
			this->barracksBuilt = true;
		}
		/*char tesar[10];

		int x = unit->getTilePosition().x();
		_itoa(x,tesar,10);
		BWAPI::Broodwar->printf("x");
		BWAPI::Broodwar->printf(tesar);

		int y = unit->getTilePosition().y();	
		_itoa(y,tesar,10);
		BWAPI::Broodwar->printf("y");
		BWAPI::Broodwar->printf(tesar);*/
	}
	else if( unit->getPlayer() == BWAPI::Broodwar->self())
	{
		if(unit->getType().isWorker())
		{
			this->workers.push_back(unit);
			this->nrOfCollectors++;
		}
	}
}
void ConstructionAgent::onUnitCreated(BWAPI::Unit* unit,Goal AI_Goal)
{
	if( unit->getPlayer() == BWAPI::Broodwar->self())
	{
		for(int i = 0; i < AI_Goal.unitsToBuild.size();i++)
		{
			if(unit->getType() == AI_Goal.unitsToBuild.at(i)->getUnitType())
			{
				BWAPI::Broodwar->printf("unit complete");
				AI_Goal.unitsToBuild.at(i)->decreaseQuant();
				break;
			}
		}
		/*if(unit->getType() == this->currentUnitBuildObj->getUnitType())
		{
			BWAPI::Broodwar->printf("unit complete");
			this->currentUnitBuildObj->decreaseQuant();
		}*/
	}
}
void ConstructionAgent::produceExtraUnit(Goal AI_Goal)
{
	for(int i = 1; i < AI_Goal.unitsToBuild.size();i++)
	{
		if(this->whatBuilds(AI_Goal.unitsToBuild.at(i)->getUnitType()) != this->whatBuilds(this->currentUnitBuildObj->getUnitType()))
		{
			if(BWAPI::Broodwar->self()->minerals() >= this->currentUnitBuildObj->getUnitType().mineralPrice() && AI_Goal.unitsToBuild.at(i)->quantity > 0)
			{
				for(std::set<Unit*>::const_iterator a=Broodwar->self()->getUnits().begin();a!=Broodwar->self()->getUnits().end();a++)
				{	
					if((*a)->isIdle() && (*a)->getType() == this->whatBuilds(AI_Goal.unitsToBuild.at(i)->getUnitType()))
					{
						(*a)->train(AI_Goal.unitsToBuild.at(i)->getUnitType());
						BWAPI::Broodwar->printf("new unit can be produced");
					}
				}
			}
		}
	}
}
UnitType ConstructionAgent::whatBuilds(UnitType unit)
{
	UnitType result = UnitTypes::None;
	if(unit == UnitTypes::Terran_SCV)
	{
		result = UnitTypes::Terran_Command_Center;
	}
	if(unit == UnitTypes::Terran_Marine || unit == UnitTypes::Terran_Medic)
	{
		result = UnitTypes::Terran_Barracks;
	}
	if(unit == UnitTypes::Terran_Siege_Tank_Tank_Mode)
	{
		result = UnitTypes::Terran_Factory;
	}
	return result;
}
