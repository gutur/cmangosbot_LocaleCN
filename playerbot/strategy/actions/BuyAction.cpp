#include "botpch.h"
#include "../../playerbot.h"
#include "BuyAction.h"
#include "../ItemVisitors.h"
#include "../values/ItemCountValue.h"
#include "../values/ItemUsageValue.h"
#include "../values/BudgetValues.h"
#include "../values/MountValues.h"
#include "AuctionHouseBot/AuctionHouseBot.h"

using namespace ai;

bool BuyAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    bool buyUseful = false;
    ItemIds itemIds;
    string link = event.getParam();

    if (link == "vendor")
        buyUseful = true;
    else
    {
        itemIds = chat->parseItems(link);
    }

    list<ObjectGuid> vendors = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("nearest npcs")->Get();
    bool vendored = false, result = false;
    for (list<ObjectGuid>::iterator i = vendors.begin(); i != vendors.end(); ++i)
    {
        ObjectGuid vendorguid = *i;
        Creature *pCreature = bot->GetNPCIfCanInteractWith(vendorguid, UNIT_NPC_FLAG_VENDOR);
        if (!pCreature)
            continue;

        vendored = true;

        if (buyUseful)
        {
            //Items are evaluated from high-level to low level.
            //For each item the bot checks again if an item is useful.
            //Bot will buy until no useful items are left.

            VendorItemData const* tItems = pCreature->GetVendorItems();
            VendorItemData const* vItems = {};
#ifndef MANGOSBOT_ZERO                
            vItems = pCreature->GetVendorTemplateItems();
#endif
            if (!tItems && !vItems)
                continue;
            
            VendorItemList m_items_sorted;
            
            if (tItems)
                m_items_sorted.insert(m_items_sorted.begin(), tItems->m_items.begin(), tItems->m_items.end());
            if (vItems)
                m_items_sorted.insert(m_items_sorted.begin(), vItems->m_items.begin(), vItems->m_items.end());
            

            m_items_sorted.erase(std::remove_if(m_items_sorted.begin(), m_items_sorted.end(), [](VendorItem* i) {ItemPrototype const* proto = sObjectMgr.GetItemPrototype(i->item); return !proto; }), m_items_sorted.end());

            if (m_items_sorted.empty())
                continue;

            std::sort(m_items_sorted.begin(), m_items_sorted.end(), [](VendorItem* i, VendorItem* j) {return sObjectMgr.GetItemPrototype(i->item)->ItemLevel > sObjectMgr.GetItemPrototype(j->item)->ItemLevel; });

            for (auto& tItem : m_items_sorted)
            {
                for (uint32 i=0; i<10; i++) //Buy 10 times or until no longer usefull/possible
                {
                    ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", tItem->item);
                    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(tItem->item);

                    uint32 price = proto->BuyPrice;

                    // reputation discount
                    price = uint32(floor(price * bot->GetReputationPriceDiscount(pCreature)));

                    NeedMoneyFor needMoneyFor = NeedMoneyFor::none;

                    unordered_map <ItemUsage, uint32> freeMoney;

                    freeMoney[ItemUsage::ITEM_USAGE_EQUIP] = (uint32)NeedMoneyFor::gear;
                    freeMoney[ItemUsage::ITEM_USAGE_USE] = (uint32)NeedMoneyFor::consumables;
                    freeMoney[ItemUsage::ITEM_USAGE_SKILL] = (uint32)NeedMoneyFor::tradeskill;
                    freeMoney[ItemUsage::ITEM_USAGE_AMMO] =  (uint32)NeedMoneyFor::ammo;
                    freeMoney[ItemUsage::ITEM_USAGE_QUEST] = freeMoney[ItemUsage::ITEM_USAGE_FORCE_NEED] = freeMoney[ItemUsage::ITEM_USAGE_FORCE_GREED] = (uint32)NeedMoneyFor::anything;
                    
                    AuctionHouseBotItemData itemInfo = sAuctionHouseBot.GetItemData(proto->ItemId);
                    if (itemInfo.Value > ((int32)proto->BuyPrice) * 1.1f)
                        freeMoney[ItemUsage::ITEM_USAGE_AH] = (uint32)NeedMoneyFor::ah;
                
                    if (freeMoney.find(usage) == freeMoney.end())
                        continue;

                    RESET_AI_VALUE2(uint32, "free money for", freeMoney[usage]);
                    uint32 money = AI_VALUE2(uint32, "free money for", freeMoney[usage]);

                    if (price > money)
                        continue;

                    if (usage == ItemUsage::ITEM_USAGE_USE && ItemUsageValue::CurrentStacks(ai, proto) >= 1)
                        continue;

                    result |= BuyItem(requester, tItems, vendorguid, proto);
#ifndef MANGOSBOT_ZERO
                    if(!result)
                        result |= BuyItem(requester, vItems, vendorguid, proto);
#endif
                    if(!result)
                        break;   

                    RESET_AI_VALUE2(ItemUsage, "item usage", tItem->item);
                    RESET_AI_VALUE(vector<MountValue>, "mount list");

                    if (usage == ItemUsage::ITEM_USAGE_EQUIP || usage == ItemUsage::ITEM_USAGE_BAD_EQUIP) //Equip upgrades and stop buying this time.
                    {
                        RESET_AI_VALUE2(ItemUsage, "item usage", tItem->item);
                        ai->DoSpecificAction("equip upgrades", event, true);
                        break;
                    }
                } 
            }
        }
        else
        {
            if (itemIds.empty())
                return false;

            for (ItemIds::iterator i = itemIds.begin(); i != itemIds.end(); i++)
            {
                uint32 itemId = *i;
                const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itemId);
                if (!proto)
                    continue;

                result |= BuyItem(requester, pCreature->GetVendorItems(), vendorguid, proto);
#ifndef MANGOSBOT_ZERO
                result |= BuyItem(requester, pCreature->GetVendorTemplateItems(), vendorguid, proto);
#endif

                if (!result)
                {
                    ostringstream out; out << "附近没有人卖 " << ChatHelper::formatItem(proto) << " .";
                    ai->TellPlayer(requester, out.str(), PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, false);
                }
            }
        }
    }

    if (!vendored)
    {
        ai->TellError("附近没有商人");
        return false;
    }

    return result;
}

bool BuyAction::BuyItem(Player* requester, VendorItemData const* tItems, ObjectGuid vendorguid, const ItemPrototype* proto)
{
    uint32 oldCount = AI_VALUE2(uint32, "item count", proto->Name1);

    if (!tItems)
        return false;

    uint32 itemId = proto->ItemId;
    for (uint32 slot = 0; slot < tItems->GetItemCount(); slot++)
    {
        if (tItems->GetItem(slot)->item == itemId)
        {       
            uint32 botMoney = bot->GetMoney();
            if (ai->HasCheat(BotCheatMask::gold))
            {
                bot->SetMoney(10000000);
            }

#ifdef MANGOSBOT_TWO
            bot->BuyItemFromVendorSlot(vendorguid, slot, itemId, 1, NULL_BAG, NULL_SLOT);
#else
            bot->BuyItemFromVendor(vendorguid, itemId, 1, NULL_BAG, NULL_SLOT);
#endif
            if (ai->HasCheat(BotCheatMask::gold))
            {
                bot->SetMoney(botMoney);
            }

            if (oldCount < AI_VALUE2(uint32, "item count", proto->Name1)) //BuyItem Always returns false (unless unique) so we have to check the item counts.
            {
                sPlayerbotAIConfig.logEvent(ai, "BuyAction", proto->Name1, to_string(proto->ItemId));

                ostringstream out; out << "购买 " << ChatHelper::formatItem(proto);
                ai->TellPlayer(requester, out.str(), PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, false);
                return true;
            }
 
            return false;
        }
    }

    return false;
}

bool BuyBackAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    string text = event.getParam();

    list<Item*> found = ai->InventoryParseItems(text, IterateItemsMask::ITERATE_ITEMS_IN_BUYBACK);

    //Sort items on itemLevel descending.
    found.sort([](Item* i, Item* j) {return i->GetProto()->ItemLevel > j->GetProto()->ItemLevel; });

    if (found.empty())
    {
        ai->TellError("没有找到任何回购物品");
        return false;
    }

    bool hasVendor = false;
    //Find vendor to interact with.
    ObjectGuid vendorguid;

    list<ObjectGuid> vendors = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("nearest npcs")->Get();
    for (list<ObjectGuid>::iterator i = vendors.begin(); i != vendors.end(); ++i)
    {
        vendorguid = *i;
        Creature* pCreature = bot->GetNPCIfCanInteractWith(vendorguid, UNIT_NPC_FLAG_VENDOR);
        if (!pCreature)
            continue;

        hasVendor = true;
        sServerFacade.SetFacingTo(bot, pCreature);
        break;
    }

    if (!hasVendor)
    {
        ai->TellError("附近没有商人");
        return false;
    }

    bool result = false;

    for (auto& item : found)
    {
        uint32 slot = BUYBACK_SLOT_START;

        while (slot < BUYBACK_SLOT_END && bot->GetItemFromBuyBackSlot(slot) != item)
            slot++;

        if (slot == BUYBACK_SLOT_END)
            continue;

        uint32 price = bot->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + slot - BUYBACK_SLOT_START);
        if (bot->GetMoney() < price)
            continue;

        WorldPacket p1(CMSG_BUYBACK_ITEM);
        p1 << vendorguid;
        p1 << slot;
        bot->GetSession()->HandleBuybackItem(p1);
        if (bot->GetItemFromBuyBackSlot(slot) == nullptr)
            result = true;
    }

    return result;
}
