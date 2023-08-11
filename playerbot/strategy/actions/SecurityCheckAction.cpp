#include "botpch.h"
#include "../../playerbot.h"
#include "../../RandomPlayerbotMgr.h"
#include "SecurityCheckAction.h"

using namespace ai;


bool SecurityCheckAction::isUseful()
{
    return sRandomPlayerbotMgr.IsRandomBot(bot) && ai->GetMaster() && ai->GetMaster()->GetSession()->GetSecurity() < SEC_GAMEMASTER && !ai->GetMaster()->GetPlayerbotAI();
}

bool SecurityCheckAction::Execute(Event& event)
{
    Group* group = bot->GetGroup();
    if (group)
    {
        LootMethod method = group->GetLootMethod();
        ItemQualities threshold = group->GetLootThreshold();
        if (method == MASTER_LOOT || method == FREE_FOR_ALL || threshold > ITEM_QUALITY_UNCOMMON)
        {
            if ((ai->GetGroupMaster()->GetSession()->GetSecurity() == SEC_PLAYER) && (!bot->GetGuildId() || bot->GetGuildId() != ai->GetGroupMaster()->GetGuildId()))
            {
                ai->TellError("我不会用这种拾取方式,除非我在你的公会中 :/");
                ai->ChangeStrategy("+passive,+stay", BotState::BOT_STATE_NON_COMBAT);
                ai->ChangeStrategy("+passive,+stay", BotState::BOT_STATE_COMBAT);
            }
            return true;
        }
    }
    return false;
}