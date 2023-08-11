#pragma once
#include "../Value.h"
#include "PartyMemberValue.h"
#include "../../PlayerbotAIConfig.h"

namespace ai
{
    class PartyMemberWithoutItemValue : public PartyMemberValue, public Qualified
    {
    public:
        PartyMemberWithoutItemValue(PlayerbotAI* ai, string name = "party member without item", float range = sPlayerbotAIConfig.farDistance) :
          PartyMemberValue(ai, name), Qualified() {}

    protected:
        virtual Unit* Calculate();
        virtual FindPlayerPredicate* CreatePredicate();
    };

    class PartyMemberWithoutFoodValue : public PartyMemberWithoutItemValue
    {
    public:
        PartyMemberWithoutFoodValue(PlayerbotAI* ai, string name = "party member without food") : PartyMemberWithoutItemValue(ai, name) {}

    protected:
        virtual FindPlayerPredicate* CreatePredicate();
    };

    class PartyMemberWithoutWaterValue : public PartyMemberWithoutItemValue
    {
    public:
        PartyMemberWithoutWaterValue(PlayerbotAI* ai, string name = "party member without water") : PartyMemberWithoutItemValue(ai, name) {}

    protected:
        virtual FindPlayerPredicate* CreatePredicate();
    };
}
