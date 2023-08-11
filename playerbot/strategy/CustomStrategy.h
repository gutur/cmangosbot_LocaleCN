#pragma once
#include "Strategy.h"

namespace ai
{
    class CustomStrategy : public Strategy, public Qualified
    {
    public:
        CustomStrategy(PlayerbotAI* ai) : Strategy(ai), Qualified() {}
        string getName() override { return "custom::" + qualifier; }
        void Reset();

    private:
        void InitNonCombatTriggers(std::list<TriggerNode*> &triggers) override;
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;

        void LoadActionLines(uint32 owner);

    private:
        list<string> actionLines;
        
    public:
        static map<string, string> actionLinesCache;
    };
}
