#include "botpch.h"
#include "../../playerbot.h"
#include "RangeAction.h"
#include "../../PlayerbotAIConfig.h"

using namespace ai;

bool RangeAction::Execute(Event& event)
{
    string param = event.getParam();
    if (param == "?")
    {
        PrintRange("spell");
        PrintRange("heal");
        PrintRange("shoot");
        PrintRange("flee");
        PrintRange("follow");
        PrintRange("followraid");
        PrintRange("attack");
    }
    int pos = param.find(" ");
    if (pos == string::npos) return false;

    string qualifier = param.substr(0, pos);
    string value = param.substr(pos + 1);

    if (value == "?")
    {
        float curVal = AI_VALUE2(float, "range", qualifier);
        ostringstream out;
        out << qualifier << " 范围: ";
        if (abs(curVal) >= 0.1f) out << curVal;
        else out << ai->GetRange(qualifier) << " (默认)";
        ai->TellPlayer(GetMaster(), out.str());
        PrintRange(qualifier);
        return true;
    }

    float newVal = (float) atof(value.c_str());
    context->GetValue<float>("range", qualifier)->Set(newVal);
    ostringstream out;
    out << qualifier << " 范围设置为: " << newVal;
    ai->TellPlayer(GetMaster(), out.str());
    return true;
}

void RangeAction::PrintRange(string type)
{
    float curVal = AI_VALUE2(float, "range", type);

    ostringstream out;
    out << type << " 范围: ";
    if (abs(curVal) >= 0.1f) out << curVal;
    else out << ai->GetRange(type) << " (默认)";

    ai->TellPlayer(GetMaster(), out.str());
}

