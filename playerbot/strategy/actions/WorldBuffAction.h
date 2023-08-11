#pragma once

#include "../Action.h"

namespace ai
{
    class WorldBuffAction : public Action {
    public:
        WorldBuffAction(PlayerbotAI* ai) : Action(ai, "world buff") {}
        virtual bool Execute(Event& event);
        static vector<uint32> NeedWorldBuffs(Unit* unit);
        //static bool AddAura(Unit* unit, uint32 spellId);
    private:
    };

}
