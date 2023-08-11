#include "botpch.h"
#include "../../playerbot.h"
#include "../actions/GenericActions.h"
#include "HunterActions.h"

using namespace ai;

bool CastSerpentStingAction::isUseful()
{
    return CastRangedDebuffSpellAction::isUseful() && AI_VALUE2(uint8, "health", GetTargetName()) > 50 && !(AI_VALUE2(uint8, "mana", GetTargetName()) >= 10);
}

bool CastViperStingAction::isUseful()
{
    return CastRangedDebuffSpellAction::isUseful() && AI_VALUE2(uint8, "mana", GetTargetName()) >= 10;
}

bool CastAspectOfTheCheetahAction::isUseful()
{
    return !AI_VALUE(bool, "has attackers") && !ai->HasAnyAuraOf(GetTarget(), "aspect of the cheetah", "aspect of the pack", NULL);
}

bool FeedPetAction::Execute(Event& event)
{
    Pet* pet = bot->GetPet();
    if (pet && pet->getPetType() == HUNTER_PET && pet->GetHappinessState() != HAPPY)
        pet->SetPower(POWER_HAPPINESS, HAPPINESS_LEVEL_SIZE * 2);

    return true;
}

bool CastAutoShotAction::isUseful()
{
    if (ai->IsInVehicle() && !ai->IsInVehicle(false, false, true))
        return false;

    return ai->HasStrategy("ranged", BotState::BOT_STATE_COMBAT) && AI_VALUE(uint32, "active spell") != AI_VALUE2(uint32, "spell id", getName());
}

Value<Unit*>* CastScareBeastCcAction::GetTargetValue()
{
    return context->GetValue<Unit*>("cc target", "scare beast");
}

bool CastScareBeastCcAction::Execute(Event& event)
{
    return ai->CastSpell("scare beast", GetTarget());
}
