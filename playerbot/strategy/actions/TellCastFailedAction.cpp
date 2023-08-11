#include "botpch.h"
#include "../../playerbot.h"
#include "TellCastFailedAction.h"
#include "../../ServerFacade.h"

using namespace ai;

bool TellCastFailedAction::Execute(Event& event)
{
    WorldPacket p(event.getPacket());
    p.rpos(0);
    uint8 castCount, result;
    uint32 spellId;
    p >> castCount >> spellId >> result;
    ai->SpellInterrupted(spellId);

    if (result == SPELL_CAST_OK)
        return false;

    const SpellEntry *const pSpellInfo =  sServerFacade.LookupSpellInfo(spellId);
    ostringstream out; out << chat->formatSpell(pSpellInfo) << ": ";
    switch (result)
    {
    case SPELL_FAILED_NOT_READY:
        out << "未准备好.";
        break;
    case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
        out << "需要法术焦点.";
        break;
    case SPELL_FAILED_REQUIRES_AREA:
        out << "无法在此处施放.";
        break;
    case SPELL_FAILED_EQUIPPED_ITEM_CLASS:
        out << "需要物品.";
        break;
    case SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND:
    case SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND:
        out << "需要武器.";
        break;
    case SPELL_FAILED_PREVENTED_BY_MECHANIC:
        out << "已被打断.";
        break;
    default:
        out << "无法施放.";
    }
    int32 castTime = GetSpellCastTime(pSpellInfo
#ifdef CMANGOS
            , bot
#endif
    );
    if (castTime >= 2000)
        ai->TellError(out.str());
    return true;
}

bool TellSpellAction::Execute(Event& event)
{
    string spell = event.getParam();
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    if (!spellId)
        return false;

    SpellEntry const *spellInfo = sServerFacade.LookupSpellInfo(spellId );
    if (!spellInfo)
        return false;

    ostringstream out; out << chat->formatSpell(spellInfo);
    ai->TellError(out.str());
    return true;
}
