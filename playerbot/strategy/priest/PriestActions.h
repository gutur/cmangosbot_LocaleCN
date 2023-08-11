#pragma once
#include "../actions/GenericActions.h"

namespace ai
{
    // disc
    BUFF_ACTION(CastPowerWordFortitudeAction, "power word: fortitude");
    BUFF_PARTY_ACTION(CastPowerWordFortitudeOnPartyAction, "power word: fortitude");
    GREATER_BUFF_PARTY_ACTION(CastPrayerOfFortitudeOnPartyAction, "prayer of fortitude");
    BUFF_ACTION(CastPowerWordShieldAction, "power word: shield");
    HEAL_PARTY_ACTION(CastPowerWordShieldOnPartyAction, "power word: shield");
    BUFF_ACTION(CastInnerFireAction, "inner fire");
    CURE_ACTION(CastDispelMagicAction, "dispel magic");
    CURE_PARTY_ACTION(CastDispelMagicOnPartyAction, "dispel magic", DISPEL_MAGIC);
    SPELL_ACTION(CastDispelMagicOnTargetAction, "dispel magic");
    CC_ACTION(CastShackleUndeadAction, "shackle undead");
    SPELL_ACTION_U(CastManaBurnAction, "mana burn", AI_VALUE2(uint8, "mana", "self target") < 50 && AI_VALUE2(uint8, "mana", "current target") >= 20);
    BUFF_ACTION(CastLevitateAction, "levitate");
    BUFF_ACTION(CastDivineSpiritAction, "divine spirit");
    BUFF_PARTY_ACTION(CastDivineSpiritOnPartyAction, "divine spirit");
    GREATER_BUFF_PARTY_ACTION(CastPrayerOfSpiritOnPartyAction, "prayer of spirit");
    //disc 2.4.3
    SPELL_ACTION(CastMassDispelAction, "mass dispel");

    // disc talents
    BUFF_ACTION(CastPowerInfusionAction, "power infusion");
    BUFF_PARTY_ACTION(CastPowerInfusionOnPartyAction, "power infusion");
    BUFF_ACTION(CastInnerFocusAction, "inner focus");
    // disc 2.4.3 talents
    BUFF_ACTION(CastPainSuppressionAction, "pain suppression");
    PROTECT_ACTION(CastPainSuppressionProtectAction, "pain suppression");


    // holy
    HEAL_ACTION(CastLesserHealAction, "lesser heal");
    HEAL_PARTY_ACTION(CastLesserHealOnPartyAction, "lesser heal");
    HEAL_ACTION(CastHealAction, "heal");
    HEAL_PARTY_ACTION(CastHealOnPartyAction, "heal");
    HEAL_ACTION(CastGreaterHealAction, "greater heal");
    HEAL_PARTY_ACTION(CastGreaterHealOnPartyAction, "greater heal");
    HEAL_ACTION(CastFlashHealAction, "flash heal");
    HEAL_PARTY_ACTION(CastFlashHealOnPartyAction, "flash heal");
    HEAL_ACTION(CastRenewAction, "renew");
    HEAL_HOT_PARTY_ACTION(CastRenewOnPartyAction, "renew");
    // holy 2.4.3
    HEAL_PARTY_ACTION(CastPrayerOfMendingAction, "prayer of mending");
    HEAL_PARTY_ACTION(CastBindingHealAction, "binding heal");
    

    BUFF_ACTION(CastPrayerOfHealingAction, "prayer of healing");
    AOE_HEAL_ACTION(CastLightwellAction, "lightwell");
    AOE_HEAL_ACTION(CastCircleOfHealingAction, "circle of healing");

    SPELL_ACTION(CastSmiteAction, "smite");
    SPELL_ACTION(CastHolyNovaAction, "holy nova");

    RESS_ACTION(CastResurrectionAction, "resurrection");

    CURE_ACTION(CastCureDiseaseAction, "cure disease");
    CURE_PARTY_ACTION(CastCureDiseaseOnPartyAction, "cure disease", DISPEL_DISEASE);
    CURE_ACTION(CastAbolishDiseaseAction, "abolish disease");
    CURE_PARTY_ACTION(CastAbolishDiseaseOnPartyAction, "abolish disease", DISPEL_DISEASE);

    RANGED_DEBUFF_ACTION(CastHolyFireAction, "holy fire");

    // shadow
    RANGED_DEBUFF_ACTION(CastPowerWordPainAction, "shadow word: pain");
    RANGED_DEBUFF_ENEMY_ACTION(CastPowerWordPainOnAttackerAction, "shadow word: pain");
    SPELL_ACTION(CastMindBlastAction, "mind blast");
    SPELL_ACTION(CastPsychicScreamAction, "psychic scream");
    RANGED_DEBUFF_ACTION(CastMindSootheAction, "mind soothe");
    BUFF_ACTION_U(CastFadeAction, "fade", bot->GetGroup());
    BUFF_ACTION(CastShadowProtectionAction, "shadow protection");
    BUFF_PARTY_ACTION(CastShadowProtectionOnPartyAction, "shadow protection");
    GREATER_BUFF_PARTY_ACTION(CastPrayerOfShadowProtectionAction, "prayer of shadow protection");
    // shadow 2.4.3
    BUFF_ACTION(CastShadowfiendAction, "shadowfiend");
    SPELL_ACTION(CastShadowWordDeathAction, "shadow word: death");

    // shadow talents
    SPELL_ACTION(CastMindFlayAction, "mind flay");
    RANGED_DEBUFF_ACTION(CastVampiricEmbraceAction, "vampiric embrace");
    BUFF_ACTION(CastShadowformAction, "shadowform");
    SPELL_ACTION(CastSilenceAction, "silence");
    ENEMY_HEALER_ACTION(CastSilenceOnEnemyHealerAction, "silence");
    // shadow talents 2.4.3
    RANGED_DEBUFF_ACTION(CastVampiricTouchAction, "vampiric touch");

    // racials
    RANGED_DEBUFF_ACTION(CastDevouringPlagueAction, "devouring plague");
    BUFF_ACTION(CastTouchOfWeaknessAction, "touch of weakness");
    RANGED_DEBUFF_ACTION(CastHexOfWeaknessAction, "hex of weakness");
    BUFF_ACTION(CastShadowguardAction, "shadowguard");
    HEAL_ACTION(CastDesperatePrayerAction, "desperate prayer");
    BUFF_ACTION(CastFearWardAction, "fear ward");
    BUFF_PARTY_ACTION(CastFearWardOnPartyAction, "fear ward");
    SPELL_ACTION_U(CastStarshardsAction, "starshards", (AI_VALUE2(uint8, "mana", "self target") > 50 && AI_VALUE(Unit*, "current target") && AI_VALUE2(float, "distance", "current target") > 15.0f));
    BUFF_ACTION(CastElunesGraceAction, "elune's grace");
    BUFF_ACTION(CastFeedbackAction, "feedback");
    BUFF_ACTION(CastSymbolOfHopeAction, "symbol of hope");
    SPELL_ACTION(CastConsumeMagicAction, "consume magic");
    SNARE_ACTION(CastChastiseAction, "chastise");

    class CastRemoveShadowformAction : public Action 
    {
    public:
        CastRemoveShadowformAction(PlayerbotAI* ai) : Action(ai, "remove shadowform") {}
        virtual bool isUseful() { return ai->HasAura("shadowform", AI_VALUE(Unit*, "self target")); }
        virtual bool isPossible() { return true; }
        virtual bool Execute(Event& event) 
        {
            ai->RemoveAura("shadowform");
            return true;
        }
    };
}
