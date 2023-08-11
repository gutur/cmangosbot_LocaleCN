#pragma once
#include "../Value.h"
#include "TargetValue.h"

namespace ai
{
   
    class GrindTargetValue : public TargetValue
	{
	public:
        GrindTargetValue(PlayerbotAI* ai, string name = "grind target") : TargetValue(ai, name) {}

    public:
        Unit* Calculate();

    private:
        int GetTargetingPlayerCount(Unit* unit);
        Unit* FindTargetForGrinding(int assistCount);

        bool needForQuest(Unit* target);
    };
}
