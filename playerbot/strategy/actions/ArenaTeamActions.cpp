#include "botpch.h"
#include "../../playerbot.h"
#include "ArenaTeamActions.h"
#ifndef MANGOSBOT_ZERO
#ifdef CMANGOS
#include "Arena/ArenaTeam.h"
#endif
#ifdef MANGOS
#include "ArenaTeam.h"
#endif
#endif

using namespace std;
using namespace ai;

bool ArenaTeamAcceptAction::Execute(Event& event)
{
#ifndef MANGOSBOT_ZERO
    WorldPacket p(event.getPacket());
    p.rpos(0);
    Player* inviter = nullptr;
    std::string Invitedname;
    p >> Invitedname;

    if (normalizePlayerName(Invitedname))
        inviter = ObjectAccessor::FindPlayerByName(Invitedname.c_str());

    if (!inviter)
        return false;

    ArenaTeam* at = sObjectMgr.GetArenaTeamById(bot->GetArenaTeamIdInvited());
    if (!at)
        return false;

    bool accept = true;

    if (bot->GetArenaTeamId(at->GetSlot()))
    {
        // bot is already in an arena team
        bot->Say("抱歉,我已经加入了一个团队.", LANG_UNIVERSAL);
        accept = false;
    }

    if (accept)
    {
        WorldPacket data(CMSG_ARENA_TEAM_ACCEPT);
        bot->GetSession()->HandleArenaTeamAcceptOpcode(data);
        bot->Say("谢谢邀请!", LANG_UNIVERSAL);
        sLog.outBasic("机器人 #%d <%s> 接受竞技场战队邀请", bot->GetGUIDLow(), bot->GetName());
        return true;
    }
    else
    {
        WorldPacket data(CMSG_ARENA_TEAM_DECLINE);
        bot->GetSession()->HandleArenaTeamDeclineOpcode(data);
        sLog.outBasic("机器人 #%d <%s> 拒绝竞技场战队邀请", bot->GetGUIDLow(), bot->GetName());
        return false;
    }
#endif
    return false;
}
