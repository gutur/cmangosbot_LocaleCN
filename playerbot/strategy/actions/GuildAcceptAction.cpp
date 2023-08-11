#include "botpch.h"
#include "../../playerbot.h"
#include "GuildAcceptAction.h"
#include "ServerFacade.h"

using namespace std;
using namespace ai;

bool GuildAcceptAction::Execute(Event& event)
{
    WorldPacket p(event.getPacket());
    p.rpos(0);
    Player* inviter = nullptr;
    std::string Invitedname;
    p >> Invitedname;

    if (normalizePlayerName(Invitedname))
        inviter = ObjectAccessor::FindPlayerByName(Invitedname.c_str());

    if (!inviter)
        return false;

    bool accept = true;
    uint32 guildId = inviter->GetGuildId();
    if (!guildId)
    {
        ai->TellError("你不在公会里!");
        accept = false;
    }
    else if (bot->GetGuildId())
    {
        ai->TellError("对不起,我已经加入了一个公会.");
        accept = false;
    }
    else if (!ai->GetSecurity()->CheckLevelFor(PlayerbotSecurityLevel::PLAYERBOT_SECURITY_INVITE, false, inviter, true))
    {
        ai->TellError("对不起,我不想加入你的公会 :(");
        accept = false;
    }

    Guild* guild = sGuildMgr.GetGuildById(guildId);

    if(guild && guild->GetMemberSize() > 1000)
    {
        ai->TellError("该公会成员已经达到1000人以上.为了防止达到1064人的限制,我拒绝加入.");
        accept = false;
    }

    if (sPlayerbotAIConfig.inviteChat && sServerFacade.GetDistance2d(bot, inviter) < sPlayerbotAIConfig.spellDistance * 1.5 && inviter->GetPlayerbotAI() && sRandomPlayerbotMgr.IsFreeBot(bot))
    {
        map<string, string> placeholders;
        placeholders["%name"] = inviter->GetName();

        if (urand(0, 3))
            bot->Say(BOT_TEXT2("听起来不错,%name,带我一个!", placeholders), (bot->GetTeam() == ALLIANCE ? LANG_COMMON : LANG_ORCISH));
        else
            bot->Say(BOT_TEXT2("我很乐意加入!", placeholders), (bot->GetTeam() == ALLIANCE ? LANG_COMMON : LANG_ORCISH));
    }

    WorldPacket packet;
    if (accept)
    {
        bot->GetSession()->HandleGuildAcceptOpcode(packet);

        sPlayerbotAIConfig.logEvent(ai, "GuildAcceptAction", guild->GetName(), to_string(guild->GetMemberSize()));
    }
    else
    {
        bot->GetSession()->HandleGuildDeclineOpcode(packet);
    }
    return true;
}