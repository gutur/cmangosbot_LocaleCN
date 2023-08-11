#include "botpch.h"
#include "../../playerbot.h"
#include "GuildManagementActions.h"
#include "../../ServerFacade.h"

using namespace std;
using namespace ai;

Player* GuidManageAction::GetPlayer(Event event)
{
    Player* player = nullptr;
    ObjectGuid guid = event.getObject();

    if (guid)
    {
        player = sObjectMgr.GetPlayer(guid);

        if (player)
            return player;
    }

    string text = event.getParam();

    if (!text.empty())
    {
        if (normalizePlayerName(text))
        {
            player = ObjectAccessor::FindPlayerByName(text.c_str());

            if (player)
                return player;
        }

        return nullptr;
    }
        
    Player* master = GetMaster();
    if (master && master == event.getOwner())
        guid = bot->GetSelectionGuid();
    
    player = sObjectMgr.GetPlayer(guid);

    if (player)
        return player;

    player = event.getOwner();

    if (player)
       return player;
    
    return nullptr;
}

bool GuidManageAction::Execute(Event& event)
{
    Player* player = GetPlayer(event);

    if (!player || !PlayerIsValid(player) || player == bot)
        return false;

    WorldPacket data = GetPacket(player);

    SendPacket(data, event);

    return true;
}

bool GuildManageNearbyAction::Execute(Event& event)
{
    uint32 found = 0;

    Guild* guild = sGuildMgr.GetGuildById(bot->GetGuildId());
    MemberSlot* botMember = guild->GetMemberSlot(bot->GetObjectGuid());

    list<ObjectGuid> nearGuids = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("nearest friendly players")->Get();
    for (auto& guid : nearGuids)
    {
        Player* player = sObjectMgr.GetPlayer(guid);

        if (!player || bot == player)
            continue;

        if (player->isDND())
            continue;


        if(player->GetGuildId()) //Promote or demote nearby members based on chance.
        {          
            MemberSlot* member = guild->GetMemberSlot(player->GetObjectGuid());
            uint32 dCount = AI_VALUE(uint32, "death count");

            if (!urand(0, 30) && dCount < 2 && guild->HasRankRight(botMember->RankId, GR_RIGHT_PROMOTE))
            {
                if (sPlayerbotAIConfig.guildFeedbackRate && frand(0, 100) <= sPlayerbotAIConfig.guildFeedbackRate && bot->GetGuildId() && !urand(0, 10) && sRandomPlayerbotMgr.IsFreeBot(bot))
                {
                    map<string, string> placeholders;
                    placeholders["%name"] = player->GetName();

                    guild->BroadcastToGuild(bot->GetSession(), BOT_TEXT2("干得好,%name.你值得这个奖励.", placeholders), LANG_UNIVERSAL);
                }

                ai->DoSpecificAction("guild promote", Event("guild management", guid), true);
                continue;
            }

            if (!urand(0, 30) && dCount > 2 && guild->HasRankRight(botMember->RankId, GR_RIGHT_DEMOTE))
            {
                if (sPlayerbotAIConfig.guildFeedbackRate && frand(0, 100) <= sPlayerbotAIConfig.guildFeedbackRate && bot->GetGuildId() && !urand(0, 10) && sRandomPlayerbotMgr.IsFreeBot(bot))
                {
                    map<string, string> placeholders;
                    placeholders["%name"] = player->GetName();

                    guild->BroadcastToGuild(bot->GetSession(), BOT_TEXT2("真糟糕,%name.我很不愿意这样做,但是...", placeholders), LANG_UNIVERSAL);
                }

                ai->DoSpecificAction("guild demote", Event("guild management", guid), true);
                continue;
            }

            continue;
        }

        if (!sPlayerbotAIConfig.randomBotGuildNearby)
            return false;

        if (guild->GetMemberSize() > 1000)
            return false;

        if (guild->HasRankRight(botMember->RankId, GR_RIGHT_INVITE))
            continue;

        if (player->GetGuildIdInvited())
            continue;

        if (!sPlayerbotAIConfig.randomBotInvitePlayer && player->isRealPlayer())
            continue;

        PlayerbotAI* botAi = player->GetPlayerbotAI();

        if (botAi)
        {            
            if (botAi->GetGuilderType() == GuilderType::SOLO && !botAi->HasRealPlayerMaster()) //Do not invite solo players.
                continue;
            
            if (botAi->HasActivePlayerMaster()) //Do not invite alts of active players. 
                continue;
        }

        bool sameGroup = bot->GetGroup() && bot->GetGroup()->IsMember(player->GetObjectGuid());

        if (!sameGroup && sServerFacade.GetDistance2d(bot, player) > sPlayerbotAIConfig.spellDistance)
            continue;

        if (sPlayerbotAIConfig.inviteChat && sRandomPlayerbotMgr.IsFreeBot(bot))
        {
            map<string, string> placeholders;
            placeholders["%name"] = player->GetName();
            placeholders["%members"] = guild->GetMemberSize();
            placeholders["%guildname"] = guild->GetName();
            placeholders["%place"] = WorldPosition(player).getAreaName(false, false);

            vector<string> lines;

            switch ((urand(0, 10)* urand(0, 10))/10)
            {
            case 0:
                lines.push_back(BOT_TEXT2("嘿,%name,你想加入我的公会吗?", placeholders));
                break;
            case 1:
                lines.push_back(BOT_TEXT2("嘿,伙计,你想加入我的公会 %name 吗?", placeholders));
                break;
            case 2:
                lines.push_back(BOT_TEXT2("我觉得你会是 %guildname 的一份子.你想加入 %name 吗?", placeholders));
                break;
            case 3:
                lines.push_back(BOT_TEXT2("我的公会 %guildname 有 %members 名优秀成员.你想成为第 %name 吗?", placeholders));
                break;
            case 4:
                lines.push_back(BOT_TEXT2("嘿,%name,你想加入 %guildname 吗?我们有 %members 名成员,力争成为服务器第一.", placeholders));
                break;
            case 5:
                lines.push_back(BOT_TEXT2("我不太擅长闲聊.你想加入我的公会 %name 吗/r?", placeholders));
                break;
            case 6:
                lines.push_back(BOT_TEXT2("欢迎来到 %place... 你想加入我的公会 %name 吗?", placeholders));
                break;
            case 7:
                lines.push_back(BOT_TEXT2("%name,你应该加入我的公会!", placeholders));
                break;
            case 8:
                lines.push_back(BOT_TEXT2("%name,我有个公会....", placeholders));
                break;
            case 9:
                lines.push_back(BOT_TEXT2("你真的要加入我的公会 %name 吗?", placeholders));
                lines.push_back(BOT_TEXT2("哈哈,你真爽快!我们将去开荒熔火之心...", placeholders));
                break;
            case 10:
                lines.push_back(BOT_TEXT2("嘿嘿!你们想加入我的公会吗????", placeholders));
                lines.push_back(BOT_TEXT2("我们有一群高等级玩家,而且我们非常友好..", placeholders));
                lines.push_back(BOT_TEXT2("..而且会照顾你的狗,帮你做家庭作业...", placeholders));
                lines.push_back(BOT_TEXT2("..我们每周进行一次团队副本,并且正在进行熔火之心的团队副本...", placeholders));
                lines.push_back(BOT_TEXT2("..而且我们工会不只是我一个成员...", placeholders));
                lines.push_back(BOT_TEXT2("..别说了,我很孤独,整天都可以坐一辆载具...", placeholders));
                lines.push_back(BOT_TEXT2("..这真的太美了,我感觉要哭了...", placeholders));
                lines.push_back(BOT_TEXT2("那么,你们说呢?你们会加入吗?", placeholders));
                break;
            }

            for (auto line : lines)
                if (sameGroup)
                {
                    WorldPacket data;
                    ChatHandler::BuildChatPacket(data, bot->GetGroup()->IsRaidGroup() ? CHAT_MSG_RAID : CHAT_MSG_PARTY, line.c_str(), LANG_UNIVERSAL, CHAT_TAG_NONE, bot->GetObjectGuid(), bot->GetName());
                    bot->GetGroup()->BroadcastPacket(data,true);
                }
                else
                    bot->Say(line, (bot->GetTeam() == ALLIANCE ? LANG_COMMON : LANG_ORCISH));
        }
        
        if (ai->DoSpecificAction("guild invite", Event("guild management", guid), true))
        {
            if (sPlayerbotAIConfig.inviteChat)
                return true;
            found++;
        }
    }

    return found > 0;
}

bool GuildManageNearbyAction::isUseful()
{
    if (!bot->GetGuildId())
        return false;

    Guild* guild = sGuildMgr.GetGuildById(bot->GetGuildId());
    MemberSlot* botMember = guild->GetMemberSlot(bot->GetObjectGuid());

    return  guild->HasRankRight(botMember->RankId, GR_RIGHT_DEMOTE) || guild->HasRankRight(botMember->RankId, GR_RIGHT_PROMOTE) || guild->HasRankRight(botMember->RankId, GR_RIGHT_INVITE);
}

bool GuildLeaveAction::Execute(Event& event)
{
    Player* owner = event.getOwner();
    if (owner && !ai->GetSecurity()->CheckLevelFor(PlayerbotSecurityLevel::PLAYERBOT_SECURITY_INVITE, false, owner, true))
    {
        ai->TellError("对不起,我很高兴待在我的公会 :)");
        return false;
    }

    Guild* guild = sGuildMgr.GetGuildById(bot->GetGuildId()); 
    
    if (guild->GetMemberSize() >= 1000)
    {
        guild->BroadcastToGuild(bot->GetSession(), "I am leaving this guild to prevent it from reaching the 1064 member limit.", LANG_UNIVERSAL);
    }

    sPlayerbotAIConfig.logEvent(ai, "GuildLeaveAction", guild->GetName(), to_string(guild->GetMemberSize()));

    WorldPacket packet;
    bot->GetSession()->HandleGuildLeaveOpcode(packet);
    return true;
}