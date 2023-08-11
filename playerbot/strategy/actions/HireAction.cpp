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
        ai->TellPlayer(GetMaster(), "You already have the maximum number of characters");
        return false;
    }

    if ((int)bot->GetLevel() > (int)master->GetLevel())
    {
        ai->TellPlayer(GetMaster(), "You cannot hire higher level characters than you");
        return false;
    }

    uint32 discount = sRandomPlayerbotMgr.GetTradeDiscount(bot, master);
    uint32 m = 1 + (bot->GetLevel() / 10);
    uint32 moneyReq = m * 5000 * bot->GetLevel();
    if ((int)discount < (int)moneyReq)
    {
        ostringstream out;
        out << "You cannot hire me - I barely know you. Make sure you have at least " << chat->formatMoney(moneyReq) << " as a trade discount";
        ai->TellPlayer(GetMaster(), out.str());
        return false;
    }

    ai->TellPlayer(GetMaster(), "I will join you at your next relogin");

    bot->SetMoney(moneyReq);
    sRandomPlayerbotMgr.Remove(bot);
    CharacterDatabase.PExecute("update characters set account = '%u' where guid = '%u'",
            account, bot->GetGUIDLow());

    return true;
}
