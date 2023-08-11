#include "botpch.h"
#include "../../playerbot.h"
#include "SayAction.h"
#include "../../PlayerbotTextMgr.h"
#include "ChannelMgr.h"
#include "../../ServerFacade.h"
#include <regex>

using namespace ai;

SayAction::SayAction(PlayerbotAI* ai) : Action(ai, "say"), Qualified()
{
}

bool SayAction::Execute(Event& event)
{
    string text = "";
    map<string, string> placeholders;
    Unit* target = AI_VALUE(Unit*, "tank target");
    if (!target) target = AI_VALUE(Unit*, "current target");

    // set replace strings
    if (target) placeholders["<target>"] = target->GetName();
    placeholders["<randomfaction>"] = IsAlliance(bot->getRace()) ? "Alliance" : "Horde";
    if (qualifier == "low ammo" || qualifier == "no ammo")
    {
        Item* const pItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
        if (pItem)
        {
            switch (pItem->GetProto()->SubClass)
            {
            case ITEM_SUBCLASS_WEAPON_GUN:
                placeholders["<ammo>"] = "bullets";
                break;
            case ITEM_SUBCLASS_WEAPON_BOW:
            case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                placeholders["<ammo>"] = "arrows";
                break;
            }
        }
    }

    if (bot->GetMap())
    {
        if (AreaTableEntry const* area = GetAreaEntryByAreaID(sServerFacade.GetAreaId(bot)))
            placeholders["<subzone>"] = area->area_name[0];
    }

    // set delay before next say
    time_t lastSaid = AI_VALUE2(time_t, "last said", qualifier);
    uint32 nextTime = time(0) + urand(1, 30);
    ai->GetAiObjectContext()->GetValue<time_t>("last said", qualifier)->Set(nextTime);

    Group* group = bot->GetGroup();
    if (group)
    {
        vector<Player*> members;
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* member = ref->getSource();
            PlayerbotAI* memberAi = member->GetPlayerbotAI();
            if (memberAi) members.push_back(member);
        }

        uint32 count = members.size();
        if (count > 1)
        {
            for (uint32 i = 0; i < count * 5; i++)
            {
                int i1 = urand(0, count - 1);
                int i2 = urand(0, count - 1);

                Player* item = members[i1];
                members[i1] = members[i2];
                members[i2] = item;
            }
        }

        int index = 0;
        for (vector<Player*>::iterator i = members.begin(); i != members.end(); ++i)
        {
            PlayerbotAI* memberAi = (*i)->GetPlayerbotAI();
            if (memberAi)
                memberAi->GetAiObjectContext()->GetValue<time_t>("last said", qualifier)->Set(nextTime + (20 * ++index) + urand(1, 15));
        }
    }

    // load text based on chance
    if (!sPlayerbotTextMgr.GetBotText(qualifier, text, placeholders))
        return false;

    if (text.find("/y ") == 0)
        bot->Yell(text.substr(3), (bot->GetTeam() == ALLIANCE ? LANG_COMMON : LANG_ORCISH));
    else
        bot->Say(text, (bot->GetTeam() == ALLIANCE ? LANG_COMMON : LANG_ORCISH));

    return true;
}


bool SayAction::isUseful()
{
    if (!ai->AllowActivity())
        return false;

    time_t lastSaid = AI_VALUE2(time_t, "last said", qualifier);
    return (time(0) - lastSaid) > 30;
}

void ChatReplyAction::ChatReplyDo(Player* bot, uint32 type, uint32 guid1, uint32 guid2, std::string msg, std::string chanName, std::string name)
{
    ChatReplyType replyType = REPLY_NOT_UNDERSTAND; // default not understand
    std::string respondsText = "";

    // Chat Logic
    int32 verb_pos = -1;
    int32 verb_type = -1;
    int32 is_quest = 0;
    bool found = false;
    std::stringstream text(msg);
    std::string segment;
    std::vector<std::string> word;
    while (std::getline(text, segment, ' '))
    {
        word.push_back(segment);
    }

    for (uint32 i = 0; i < 15; i++)
    {
        if (word.size() < i)
            word.push_back("");
    }

    if (msg.find("?") != std::string::npos)
        is_quest = 1;
    if (word[0].find("what") != std::string::npos)
        is_quest = 2;
    else if (word[0].find("who") != std::string::npos)
        is_quest = 3;
    else if (word[0] == "when")
        is_quest = 4;
    else if (word[0] == "where")
        is_quest = 5;
    else if (word[0] == "why")
        is_quest = 6;

    // Responds
    for (uint32 i = 0; i < 8; i++)
    {
        // blame gm with chat tag
        if (Player* plr = sObjectMgr.GetPlayer(ObjectGuid(HIGHGUID_PLAYER, guid1)))
        {
            if (plr->isGMChat())
            {
                replyType = REPLY_ADMIN_ABUSE;
                found = true;
                break;
            }
        }

        if (word[i] == "hi" || word[i] == "hey" || word[i] == "hello" || word[i] == "wazzup")
        {
            replyType = REPLY_HELLO;
            found = true;
            break;
        }

        if (verb_type < 4)
        {
            if (word[i] == "am" || word[i] == "are" || word[i] == "is")
            {
                verb_pos = i;
                verb_type = 2; // present
            }
            else if (word[i] == "will")
            {
                verb_pos = i;
                verb_type = 3; // future
            }
            else if (word[i] == "was" || word[i] == "were")
            {
                verb_pos = i;
                verb_type = 1; // past
            }
            else if (word[i] == "shut" || word[i] == "noob")
            {
                if (msg.find(bot->GetName()) == std::string::npos)
                {
                    continue; // not react
                    uint32 rnd = urand(0, 2);
                    std::string msg = "";
                    if (rnd == 0)
                        msg = "对不起 %s, 我不说了";
                    if (rnd == 1)
                        msg = "好吧好吧 %s";
                    if (rnd == 2)
                        msg = "好吧,我不跟你说话了 %s";

                    msg = std::regex_replace(msg, std::regex("%s"), name);
                    respondsText = msg;
                    found = true;
                    break;
                }
                else
                {
                    replyType = REPLY_GRUDGE;
                    found = true;
                    break;
                }
            }
        }
    }
    if (verb_type < 4 && is_quest && !found)
    {
        switch (is_quest)
        {
        case 2:
        {
            uint32 rnd = urand(0, 3);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "我不知道.";
                break;
            case 1:
                msg = "我不知道 %s";
                break;
            case 2:
                msg = "谁在乎呢.";
                break;
            case 3:
                msg = "恐怕那是在我出现或关注之前的事情.";
                break;
            }

            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 3:
        {
            uint32 rnd = urand(0, 4);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "没有人.";
                break;
            case 1:
                msg = "我们都是.";
                break;
            case 2:
                msg = "也许是你,%s.";
                break;
            case 3:
                msg = "不知道 %s.";
                break;
            case 4:
                msg = "是我吗?";
                break;
            }

            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 4:
        {
            uint32 rnd = urand(0, 6);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "也许很快 %s";
                break;
            case 1:
                msg = "可能以后.";
                break;
            case 2:
                msg = "永远不会.";
                break;
            case 3:
                msg = "我长得像先知吗?";
                break;
            case 4:
                msg = "可能几分钟,也许一小时 ... 几年也说不定?";
                break;
            case 5:
                msg = "什么时候?好问题.";
                break;
            case 6:
                msg = "不知道 %s";
                break;
            }

            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 5:
        {
            uint32 rnd = urand(0, 6);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "真的希望我回答吗?";
                break;
            case 1:
                msg = "在地图上吗?";
                break;
            case 2:
                msg = "谁在乎呢";
                break;
            case 3:
                msg = "离开键按下了吗?";
                break;
            case 4:
                msg = "和你无关";
                break;
            case 5:
                msg = "是啊,在哪里?";
                break;
            case 6:
                msg = "不知道 %s";
                break;
            }

            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 6:
        {
            uint32 rnd = urand(0, 6);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "不知道 %s";
                break;
            case 1:
                msg = "为什么?只是因为 %s";
                break;
            case 2:
                msg = "为什么天空是蓝色的?";
                break;
            case 3:
                msg = "别问我 %s,我只是一个机器人.";
                break;
            case 4:
                msg = "你问错人了.";
                break;
            case 5:
                msg = "谁知道呢?";
                break;
            case 6:
                msg = "不知道 %s";
                break;
            }
            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        default:
        {
            switch (verb_type)
            {
            case 1:
            {
                uint32 rnd = urand(0, 3);
                std::string msg = "";

                switch (rnd)
                {
                case 0:
                    msg = "是的, " + word[verb_pos + 1] + " " + word[verb_pos] + " " + word[verb_pos + 2] + " " + word[verb_pos + 3] + " " + word[verb_pos + 4] + " " + word[verb_pos + 4];
                    break;
                case 1:
                    msg = "是的 %s,但那已经是过去了.";
                    break;
                case 2:
                    msg = "不, 但是 " + word[verb_pos + 1] + " 将再一次 " + word[verb_pos + 3] + " , %s";
                    break;
                case 3:
                    msg = "恐怕那是在我出现或关注之前的事情.";
                    break;
                }
                msg = std::regex_replace(msg, std::regex("%s"), name);
                respondsText = msg;
                found = true;
                break;
            }
            case 2:
            {
                uint32 rnd = urand(0, 6);
                std::string msg = "";

                switch (rnd)
                {
                case 0:
                    msg = "没错, " + word[verb_pos + 1] + " " + word[verb_pos] + " " + word[verb_pos + 2] + " " + word[verb_pos + 3] + " " + word[verb_pos + 4] + " " + word[verb_pos + 5];
                    break;
                case 1:
                    msg = "是的 %s,那是真的.";
                    break;
                case 2:
                    msg = "也许 " + word[verb_pos + 1] + " " + word[verb_pos] + " " + word[verb_pos + 2] + " " + word[verb_pos + 3] + " " + word[verb_pos + 4] + " " + word[verb_pos + 5];
                    break;
                case 3:
                    msg = "不知道 %s";
                    break;
                case 4:
                    msg = "我不这么认为 %s";
                    break;
                case 5:
                    msg = "是的.";
                    break;
                case 6:
                    msg = "不.";
                    break;
                }
                msg = std::regex_replace(msg, std::regex("%s"), name);
                respondsText = msg;
                found = true;
                break;
            }
            case 3:
            {
                uint32 rnd = urand(0, 8);
                std::string msg = "";

                switch (rnd)
                {
                case 0:
                    msg = "不知道 %s";
                    break;
                case 1:
                    msg = "我不知道 %s";
                    break;
                case 2:
                    msg = "我怎么知道 %s";
                    break;
                case 3:
                    msg = "别问我 %s, 我只是一个机器人.";
                    break;
                case 4:
                    msg = "你问错人了.";
                    break;
                case 5:
                    msg = "我长得像先知吗?";
                    break;
                case 6:
                    msg = "当然 %s";
                    break;
                case 7:
                    msg = "我不这么认为 %s";
                    break;
                case 8:
                    msg = "也许.";
                    break;
                }
                msg = std::regex_replace(msg, std::regex("%s"), name);
                respondsText = msg;
                found = true;
                break;
            }
            }
        }
        }
    }
    else if (!found)
    {
        switch (verb_type)
        {
        case 1:
        {
            uint32 rnd = urand(0, 2);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "是的 %s, 关键词是 " + word[verb_pos] + " " + word[verb_pos + 1];
                break;
            case 1:
                msg = "是的 %s 但那已经是过去了";
                break;
            case 2:
                msg = word[verb_pos - 1] + " 会再次 " + word[verb_pos + 1] + "  %s";
                break;
            }
            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 2:
        {
            uint32 rnd = urand(0, 2);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "%s, 你是什么意思 " + word[verb_pos + 1] + "?";
                break;
            case 1:
                msg = "%s,  " + word[verb_pos + 1] + " 是什么?";
                break;
            case 2:
                msg = "是的我知道 " + word[verb_pos - 1] + " 是 " + word[verb_pos + 1];
                break;
            }
            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        case 3:
        {
            uint32 rnd = urand(0, 1);
            std::string msg = "";

            switch (rnd)
            {
            case 0:
                msg = "你确定那会发生吗  %s?";
                break;
            case 1:
                msg = "%s,接下来会发生什么 %s?";
                break;
            case 2:
                msg = "你是在说 " + word[verb_pos - 1] + " 会 " + word[verb_pos + 1] + " " + word[verb_pos + 2] + " %s?";
                break;
            }
            msg = std::regex_replace(msg, std::regex("%s"), name);
            respondsText = msg;
            found = true;
            break;
        }
        }
    }

    if (!found)
    {
        // Name Responds
        if (msg.find(bot->GetName()) != std::string::npos)
        {
            replyType = REPLY_NAME;
            found = true;
        }
        else // Does not understand
        {
            replyType = REPLY_NOT_UNDERSTAND;
            found = true;
        }
    }

    // send responds
        // 
    if (found)
    {
        // load text if needed
        if (respondsText.empty())
        {
            respondsText = BOT_TEXT2(replyType, name);
        }
        const char* c = respondsText.c_str();
        if (strlen(c) > 255)
            return;

        if (chanName == "World")
        {
            if (ChannelMgr* cMgr = channelMgr(bot->GetTeam()))
            {
                std::string worldChan = "World";
#ifndef MANGOSBOT_ZERO
                if (Channel* chn = cMgr->GetJoinChannel(worldChan.c_str(), 0))
#else
                if (Channel* chn = cMgr->GetJoinChannel(worldChan.c_str()))
#endif
                {
                    if (bot->GetTeam() == ALLIANCE)
                    {
                        chn->Say(bot, c, LANG_COMMON);
                    }
                    else
                    {
                        chn->Say(bot, c, LANG_ORCISH);
                    }
                }
            }
        }
        else
        {
            if (type == CHAT_MSG_WHISPER)
            {
                ObjectGuid receiver = sObjectMgr.GetPlayerGuidByName(name.c_str());
                Player* rPlayer = sObjectMgr.GetPlayer(receiver);
                if (rPlayer)
                {
                    if (bot->GetTeam() == ALLIANCE)
                    {
                        bot->Whisper(c, LANG_COMMON, receiver);
                    }
                    else
                    {
                        bot->Whisper(c, LANG_ORCISH, receiver);
                    }
                }
            }

            if (type == CHAT_MSG_SAY)
            {
                if (bot->GetTeam() == ALLIANCE)
                    bot->Say(respondsText, LANG_COMMON);
                else
                    bot->Say(respondsText, LANG_ORCISH);
            }

            if (type == CHAT_MSG_YELL)
            {
                if (bot->GetTeam() == ALLIANCE)
                    bot->Yell(respondsText, LANG_COMMON);
                else
                    bot->Yell(respondsText, LANG_ORCISH);
            }

            if (type == CHAT_MSG_GUILD)
            {
                if (!bot->GetGuildId())
                    return;

                if (Guild* guild = sGuildMgr.GetGuildById(bot->GetGuildId()))
                    guild->BroadcastToGuild(bot->GetSession(), respondsText, LANG_UNIVERSAL);
            }
        }
        bot->GetPlayerbotAI()->GetAiObjectContext()->GetValue<time_t>("last said", "chat")->Set(time(0) + urand(5, 25));
    }
}
