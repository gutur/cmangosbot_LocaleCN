#include "botpch.h"
#include "../../playerbot.h"
#include "TaxiAction.h"
#include "../../../../../game/Server/DBCStructure.h"
#include "../values/LastMovementValue.h"

using namespace ai;

bool TaxiAction::Execute(Event& event)
{
    ai->RemoveShapeshift();

    LastMovement& movement = context->GetValue<LastMovement&>("last taxi")->Get();

    WorldPacket& p = event.getPacket();
    string param = event.getParam();
    if ((!p.empty() && (p.GetOpcode() == CMSG_TAXICLEARALLNODES || p.GetOpcode() == CMSG_TAXICLEARNODE)) || param == "clear")
    {
        movement.taxiNodes.clear();
        movement.Set(NULL);
        ai->TellPlayer(GetMaster(), "我已准备好下一趟飞行.");
        return true;
    }

    list<ObjectGuid> units = *context->GetValue<list<ObjectGuid> >("nearest npcs");
    for (list<ObjectGuid>::iterator i = units.begin(); i != units.end(); i++)
    {
        Creature *npc = bot->GetNPCIfCanInteractWith(*i, UNIT_NPC_FLAG_FLIGHTMASTER);
        if (!npc)
            continue;

        uint32 curloc = sObjectMgr.GetNearestTaxiNode(npc->GetPositionX(), npc->GetPositionY(), npc->GetPositionZ(), npc->GetMapId(), bot->GetTeam());

        vector<uint32> nodes;
        for (uint32 i = 0; i < sTaxiPathStore.GetNumRows(); ++i)
        {
            TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i);
            if (entry && entry->from == curloc)
            {
                uint8  field = uint8((i - 1) / 32);
                if (field < TaxiMaskSize) nodes.push_back(i);
            }
        }

        if (param == "?")
        {
            ai->TellPlayerNoFacing(GetMaster(), "=== 航点 ===");
            int index = 1;
            for (vector<uint32>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(*i);
                if (!entry) continue;

                TaxiNodesEntry const* dest = sTaxiNodesStore.LookupEntry(entry->to);
                if (!dest) continue;

                ostringstream out;
                out << index++ << ": " << dest->name[0];
                ai->TellPlayerNoFacing(GetMaster(), out.str());
            }
            return true;
        }

        int selected = atoi(param.c_str());
        if (selected)
        {
            uint32 path = nodes[selected - 1];
            TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(path);
            if (!entry) return false;

            return bot->ActivateTaxiPathTo({ entry->from, entry->to }, npc, 0);
        }

        if (!movement.taxiNodes.empty() && !bot->ActivateTaxiPathTo(movement.taxiNodes, npc))
        {
            movement.taxiNodes.clear();
            movement.Set(NULL);
            ai->TellError("我不能和你一起飞行.");
            return false;
        }

        return true;
    }

    ai->TellError("找不到任何可以交谈的飞行管理员.");
    return false;
}
