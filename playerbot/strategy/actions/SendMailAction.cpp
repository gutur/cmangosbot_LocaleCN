#include "botpch.h"
#include "Mail.h"
#include "../../playerbot.h"
#include "SendMailAction.h"

#include "../../../ahbot/AhBot.h"
#include "../../PlayerbotAIConfig.h"
#include "../ItemVisitors.h"

using namespace ai;

bool SendMailAction::Execute(Event& event)
{
    uint32 account = sObjectMgr.GetPlayerAccountIdByGUID(bot->GetObjectGuid());
    bool randomBot = sPlayerbotAIConfig.IsInRandomAccountList(account);

    string text = event.getParam();
    Player* receiver = GetMaster();
    Player* tellTo = receiver;
    vector<string> ss = split(text, ' ');
    if (ss.size() > 1)
    {
        Player* p = sObjectMgr.GetPlayer(ss[ss.size() - 1].c_str());
        if (p) receiver = p;
    }

    if (!receiver) receiver = event.getOwner();

    if (!receiver || receiver == bot)
    {
        return false;
    }

    if (!tellTo) tellTo = receiver;

    ItemIds ids = chat->parseItems(text);
    if (ids.size() > 1)
    {
        bot->Whisper("你不能请求超过一个物品.", LANG_UNIVERSAL, tellTo->GetObjectGuid());
        return false;
    }

    if (ids.empty())
    {
        uint32 money = chat->parseMoney(text);
        if (!money)
            return false;

        if (randomBot)
        {
            bot->Whisper("我不能送钱.", LANG_UNIVERSAL, tellTo->GetObjectGuid());
            return false;
        }

        if (bot->GetMoney() < money)
        {
            ai->TellError("我身上的钱不够.");
            return false;
        }

        ostringstream body;
        body << "你好, " << receiver->GetName() << ",\n";
        body << "\n";
        body << "这是你需要的金币";
        body << "\n";
        body << "谢谢,\n";
        body << bot->GetName() << "\n";


        MailDraft draft("你请求的金币", body.str());
        draft.SetMoney(money);
        bot->SetMoney(bot->GetMoney() - money);
        draft.SendMailTo(MailReceiver(receiver), MailSender(bot));

        ostringstream out; out << "寄邮件给 " << receiver->GetName();
        ai->TellPlayer(GetMaster(), out.str());
        return true;
    }

    ostringstream body;
    body << "你好, " << receiver->GetName() << ",\n";
    body << "\n";
    body << "这是你想要的东西.";
    body << "\n";
    body << "多谢,\n";
    body << bot->GetName() << "\n";

    MailDraft draft("你要的东西.", body.str());
    for (ItemIds::iterator i =ids.begin(); i != ids.end(); i++)
    {
        FindItemByIdVisitor visitor(*i);
        IterateItemsMask mask = IterateItemsMask((uint8)IterateItemsMask::ITERATE_ITEMS_IN_BAGS | (uint8)IterateItemsMask::ITERATE_ITEMS_IN_EQUIP | (uint8)IterateItemsMask::ITERATE_ITEMS_IN_BANK);
        ai->InventoryIterateItems(&visitor, mask);
        list<Item*> items = visitor.GetResult();
        for (list<Item*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            Item* item = *i;
            if (item->IsSoulBound() || item->IsConjuredConsumable())
            {
                ostringstream out;
                out << "不能邮给 " << ChatHelper::formatItem(item);
                bot->Whisper(out.str(), LANG_UNIVERSAL, tellTo->GetObjectGuid());
                continue;
            }

            ItemPrototype const *proto = item->GetProto();
            bot->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
            item->DeleteFromInventoryDB();
            item->SetOwnerGuid(receiver->GetObjectGuid());
            item->SaveToDB();
            draft.AddItem(item);
            if (randomBot)
            {
                uint32 price = item->GetCount() * auctionbot.GetSellPrice(proto);
                if (!price)
                {
                    ostringstream out;
                    out << ChatHelper::formatItem(item) << ": 这东西不卖.";
                    bot->Whisper(out.str(), LANG_UNIVERSAL, tellTo->GetObjectGuid());
                    return false;
                }
                draft.SetCOD(price);
            }
            draft.SendMailTo(MailReceiver(receiver), MailSender(bot));

            ostringstream out; out << "邮寄给:" << receiver->GetName();
            bot->Whisper(out.str(), LANG_UNIVERSAL, tellTo->GetObjectGuid());
            return true;
        }
    }

    return false;
}
