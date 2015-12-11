#include "ConstructionAgent.h"


ConstructionAgent::ConstructionAgent()
{
}
ConstructionAgent::ConstructionAgent(TilePosition start)
{
	this->nrOfWorkers = 0;
	this->isBuilderSet = false;
	for(int y =-5; y < 5; y ++)
	{
		for(int x = -5; x < 5; x ++)
		{
			if(x != 0 && y != 0)
			{
				TilePosition temp = TilePosition(start.x() + x,start.y() +y);
				this->buildPos.push_back(temp);
			}
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
		if((*i)->getType().isWorker())
		{
			if(this->isBuilderSet == false)
			{
				this->bulider = (*i);
				this->isBuilderSet = true;
			}
			else
			{
				this->nrOfWorkers++;
				this->workers.push_back((*i));
			}
		}
	}
}

void ConstructionAgent::onFrame(ToBuild* toBuild)
{
	currentObj = toBuild;
	int size = this->workers.size();

	char tesar[10];
	/*_itoa(this->nrOfWorkers,tesar,10);
	BWAPI::Broodwar->printf(tesar);*/
	if(BWAPI::Broodwar->self()->minerals() >= toBuild->getUnitType().mineralPrice() && this->bulider->isIdle() && toBuild->getQuantity() > 0)
	{
		TilePosition buildSpot = this->findBuildSpot(toBuild->getUnitType());
		if(buildSpot.x() != -1 || buildSpot.y() != -1)
		{
			this->bulider->build(buildSpot,toBuild->getUnitType());
		}
		else
		{
			BWAPI::Broodwar->printf("du får icke bygga där");
		}
	}

	/*int x = buildSpot.x();
	_itoa(this->nrOfWorkers,tesar,10);
	BWAPI::Broodwar->printf("x");
	BWAPI::Broodwar->printf(tesar);

	int y = buildSpot.y();	
	_itoa(this->nrOfWorkers,tesar,10);
	BWAPI::Broodwar->printf("y");
	BWAPI::Broodwar->printf(tesar);*/


	
	
	for(int i = 0; i < size; i++)
	{
			Unit* closestMineral=NULL;
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
				//BWAPI::Broodwar->printf("Send worker %d to mineral %d", this->workers.at(i)->getID(), closestMineral->getID());
			}
	}
}
TilePosition ConstructionAgent::findBuildSpot(UnitType toBuild)
{
	TilePosition result = TilePosition(-1,-1);
	bool foundValidSpot = false;
	if(this->buildPos.size() != 0)
	{
	
		for(int i = 0; i < 8 && foundValidSpot == false; i++)
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
				

			}
		}
	}

	return result;
}
void ConstructionAgent::isUnitWorker(Unit* unit)
{
	//BWAPI::Broodwar->printf("ConstructionAgent registerd unit complete");
	if(unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self())
	{
		this->currentObj->decreaseQuant();
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
	if(unit->getType().isWorker())
	{
		this->workers.push_back(unit);
		this->nrOfWorkers++;
	}
}