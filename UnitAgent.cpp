#include "UnitAgent.h"

UnitAgent::UnitAgent()
{
	this->nrOfUnits = 0;
}

UnitAgent::~UnitAgent()
{
}

void UnitAgent::onUnitComplete(BWAPI::Unit *unit)
{
	if(
		unit->getType().isBuilding() == false 
		&& 
		unit->getType() != BWAPI::UnitTypes::Terran_SCV
		&&
		unit->getPlayer() == BWAPI::Broodwar->self()
		)
	{
		Broodwar->printf("New battle unit registred in UnitAgent");
		this->battleUnits.push_back(unit);
		this->nrOfUnits++;
	}	
}

void UnitAgent::setNewChokePoint(BWAPI::Position position)
{
	this->recentChokePoint = position;
}

void UnitAgent::onFrame()
{
	for(int i = 0; i < this->nrOfUnits; i++)
	{
		this->battleUnits.at(i)->move(this->recentChokePoint);
	}
	//in attack the units are attacking when the third tank has been built
	this->attack(Broodwar->enemy()->getStartLocation());
}

int UnitAgent::getNrOfUnits() const
{
	return this->nrOfUnits;
}

void UnitAgent::attack(BWAPI::TilePosition EnemyPos)
{
	int nrOfTanks = 0;
	BWAPI::Position pos(EnemyPos.x() * 32, EnemyPos.y() * 32);
	//BWAPI::Unit* attackingUnits[15];
	for(int i = 0; i < this->nrOfUnits; i++)
	{
		
		//attackingUnits[i] = this->battleUnits.at(i);
		//if tank && enemy spotted, go siege
		if( this->battleUnits.at(i)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
			||this->battleUnits.at(i)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
		{
			nrOfTanks++;
			bool canAttack = false;
			for(std::set<Unit*>::const_iterator a=BWAPI::Broodwar->enemy()->getUnits().begin();a!=BWAPI::Broodwar->enemy()->getUnits().end() && canAttack == false;a++)
			{
				if((*a)->getDistance(this->battleUnits.at(i)->getPosition()) <= BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.seekRange())
				{
					//gå in i siege
					canAttack = true;
				}
			}
			if(canAttack)
			{
				this->battleUnits.at(i)->siege();
			}
			else
			{
				this->battleUnits.at(i)->unsiege();
			}
		}
		//this is the check that needs to be true if the units are attacking
		if(nrOfTanks == 3 || areAttacking == true)
		{
			this->areAttacking = true;
			//fler units hänger med efter andra iterationen
			this->battleUnits.at(i)->attack(pos);
		}
	}
}