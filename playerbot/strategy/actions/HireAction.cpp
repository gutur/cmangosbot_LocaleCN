#include "botpch.h"
#include "../../playerbot.h"
#include "HireAction.h"

using namespace ai;

bool HireAction::Execute(Event& event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    if (!sRandomPlayerbotMgr.IsRandomBot(bot))
        return false;

    uint32 account = sObjectMgr.GetPlayerAccountIdByGUID(master->GetObjectGuid());
    QueryResult* results = CharacterDatabase.PQuery("SELECT count(*) FROM characters where account = '%u'", account);

    uint32 charCount = 10;
    if (results)
    {
        Field* fields = results->Fetch();
        charCount = fields[0].GetUInt32();
        delete results;
    }

    if (charCount >= 10)
    {
        ai->TellPlayer(GetMaster(), "你已经达到最大角色数量限制.");
        return false;
    }

    if ((int)bot->GetLevel() > (int)master->GetLevel())
    {
        ai->TellPlayer(GetMaster(), "你不能雇佣比你级别高的角色.");
        return false;
    }

    uint32 discount = sRandomPlayerbotMgr.GetTradeDiscount(bot, master);
    uint32 m = 1 + (bot->GetLevel() / 10);
    uint32 moneyReq = m * 5000 * bot->GetLevel();
    if ((int)discount < (int)moneyReq)
    {
        ostringstream out;
        out << "你不能雇佣我,我基本上都不认识你,去报你至少拥有 " << chat->formatMoney(moneyReq) << " 作为交易费用";
        ai->TellPlayer(GetMaster(), out.str());
        return false;
    }

    ai->TellPlayer(GetMaster(), "我会在你下次重新登录时加入你的队伍.");

    bot->SetMoney(moneyReq);
    sRandomPlayerbotMgr.Remove(bot);
    CharacterDatabase.PExecute("update characters set account = '%u' where guid = '%u'",
            account, bot->GetGUIDLow());

    return true;
}
