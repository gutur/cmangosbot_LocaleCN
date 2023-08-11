#pragma once

#include "../Action.h"

namespace ai
{
    class SetCombatStateAction : public Action 
    {
    public:
        SetCombatStateAction(PlayerbotAI* ai, string name = "set combat state") : Action(ai, name) {}
        bool Execute(Event& event) override;
    };

    class SetNonCombatStateAction : public Action
    {
    public:
        SetNonCombatStateAction(PlayerbotAI* ai, string name = "set non combat state") : Action(ai, name) {}
        bool Execute(Event& event) override;
    };

    class SetDeadStateAction : public Action
    {
    public:
        SetDeadStateAction(PlayerbotAI* ai, string name = "set dead state") : Action(ai, name) {}
        bool Execute(Event& event) override;
    };
}
