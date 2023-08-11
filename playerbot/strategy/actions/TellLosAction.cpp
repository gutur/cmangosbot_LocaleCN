#include "botpch.h"
#include "../../playerbot.h"
#include "TellLosAction.h"

using namespace ai;

bool TellLosAction::Execute(Event& event)
{
    string param = event.getParam();

    if (param.empty() || param == "targets")
    {
        ListUnits("--- 目标 ---", *context->GetValue<list<ObjectGuid> >("possible targets"));
        ListUnits("--- 目标 (全部) ---", *context->GetValue<list<ObjectGuid> >("all targets"));
    }

    if (param.empty() || param == "npcs")
    {
        ListUnits("--- NPCs ---", *context->GetValue<list<ObjectGuid> >("nearest npcs"));
    }

    if (param.empty() || param == "corpses")
    {
        ListUnits("--- 尸体 ---", *context->GetValue<list<ObjectGuid> >("nearest corpses"));
    }

    if (param.empty() || param == "gos" || param == "game objects")
    {
        ListGameObjects("--- 物体 ---", *context->GetValue<list<ObjectGuid> >("nearest game objects"));
    }

    if (param.empty() || param == "players")
    {
        ListUnits("--- 友善玩家 ---", *context->GetValue<list<ObjectGuid> >("nearest friendly players"));
    }

    return true;
}

void TellLosAction::ListUnits(string title, list<ObjectGuid> units)
{
    ai->TellPlayer(GetMaster(), title);

    for (list<ObjectGuid>::iterator i = units.begin(); i != units.end(); i++)
    {
        Unit* unit = ai->GetUnit(*i);
        if (unit)
            ai->TellPlayer(GetMaster(), unit->GetName());
    }

}
void TellLosAction::ListGameObjects(string title, list<ObjectGuid> gos)
{
    ai->TellPlayer(GetMaster(), title);

    for (list<ObjectGuid>::iterator i = gos.begin(); i != gos.end(); i++)
    {
        GameObject* go = ai->GetGameObject(*i);
        if (go)
            ai->TellPlayer(GetMaster(), chat->formatGameobject(go));
    }
}
