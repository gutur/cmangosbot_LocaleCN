#pragma once
#include "../Value.h"
#include "NearestUnitsValue.h"
#include "../../PlayerbotAIConfig.h"
#include "PossibleTargetsValue.h"

namespace ai
{
    class NearestAddsValue : public PossibleTargetsValue
	{
	public:
        NearestAddsValue(PlayerbotAI* ai, float range = sPlayerbotAIConfig.fleeDistance) :
            PossibleTargetsValue(ai, "nearest adds", range, true) {}

    protected:
        bool AcceptUnit(Unit* unit);
	};
}
