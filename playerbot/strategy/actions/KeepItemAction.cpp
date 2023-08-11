#include "botpch.h"
#include "../../playerbot.h"
#include "KeepItemAction.h"

#include "../values/ItemCountValue.h"
#include "../values/ItemUsageValue.h"

using namespace ai;

bool KeepItemAction::Execute(Event& event)
{
    string text = event.getParam();

    string type = text.substr(0, text.find(" "));

    if (text.empty()) //No param = help feedback
    {
        ostringstream out;
        out << "请指定要保留的物品.请参阅 " << ChatHelper::formatValue("help", "action:keep", "keep help") << " 获取更多信息.";
        ai->TellPlayer(GetMaster(), out.str());
        return false;
    }
    else if (string("none,keep,equip,greed,need,?,").find(type + ",") == string::npos) //Non type = using keep.
        type = "keep";
    else if(text.find(" ") != string::npos) //Remove type from param.
        text = text.substr(text.find(" ") + 1);

    ItemIds ids = chat->parseItems(text);

    if (ids.empty())
    {

        IterateItemsMask mask = IterateItemsMask((uint8)IterateItemsMask::ITERATE_ITEMS_IN_EQUIP | (uint8)IterateItemsMask::ITERATE_ITEMS_IN_BAGS | (uint8)IterateItemsMask::ITERATE_ITEMS_IN_BANK);

        list<Item*> found = ai->InventoryParseItems(text, mask);

        for (auto& item : found)
            ids.insert(item->GetProto()->ItemId);
    }
   
    if (ids.empty())
    {
        ostringstream out;

        if (type == "keep")
            out << "请指定要保留的物品.请参阅 " << ChatHelper::formatValue("help", "action:keep", "keep help") << " 获取更多信息.";
        else
            out << "未找到物品.";

        ai->TellPlayer(GetMaster(), out.str());
        return true;
    }

    if (type == "?")
    {
        for (auto& id : ids)
        {
            ostringstream out;
            ItemQualifier qualifier(id);
            out << chat->formatItem(qualifier);
            out << ": " << keepName[AI_VALUE2_EXISTS(ForceItemUsage, "force item usage", id, ForceItemUsage::FORCE_USAGE_NONE)] << " the item.";
            ai->TellPlayer(GetMaster(), out.str());

        }
        return true;
    }

    uint32 changed = 0;

    ForceItemUsage usage = ForceItemUsage::FORCE_USAGE_NONE;
    if (type == "keep")
        usage = ForceItemUsage::FORCE_USAGE_KEEP;
    else if (type == "equip")
        usage = ForceItemUsage::FORCE_USAGE_EQUIP;
    else if (type == "greed")
        usage = ForceItemUsage::FORCE_USAGE_GREED;
    else if (type == "need")
        usage = ForceItemUsage::FORCE_USAGE_NEED;

    for (auto& id : ids)
    {
        if (AI_VALUE2_EXISTS(ForceItemUsage, "force item usage", id, ForceItemUsage::FORCE_USAGE_NONE) != usage)
        {
            changed++;
            SET_AI_VALUE2(ForceItemUsage, "force item usage", id, usage);
        }
    }

    ostringstream out;
    out << changed;
    out << " 件物品更改为: " << keepName[usage] << " 该物品.";
    ai->TellPlayer(GetMaster(), out.str());

    sPlayerbotDbStore.Save(ai);

    return true;
}