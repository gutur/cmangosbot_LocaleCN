#include "botpch.h"
#include "../../playerbot.h"
#include "DebugAction.h"
#include "../../PlayerbotAIConfig.h"
#include <playerbot/TravelNode.h>
#include "ChooseTravelTargetAction.h"
#include "strategy/values/SharedValueContext.h"
#include "../../LootObjectStack.h"
#include "../../../game/GameEvents/GameEventMgr.h"
#include "../../TravelMgr.h"
#include "PlayerbotHelpMgr.h"
#include "Entities/Transports.h"
#include "MotionGenerators/PathFinder.h"

using namespace ai;

bool DebugAction::Execute(Event& event)
{
    
    bool isMod = event.getSource() == ".bot" || (event.getOwner() && event.getOwner()->GetSession() && event.getOwner()->GetSession()->GetSecurity() >= SEC_MODERATOR);

    Player* master = GetMaster();
    if (!master)
        master = bot;

    Unit* masterTarget = nullptr;
    if (master->GetSelectionGuid())
        masterTarget = ai->GetUnit(master->GetSelectionGuid());

    string text = event.getParam();
    if (text == "scan" && isMod)
    {
        sPlayerbotAIConfig.openLog("scan.csv", "w");

        uint32 i;
        for (auto p : WorldPosition().getCreaturesNear())
        {
            WorldPosition pos(p);

            const uint32 zoneId = sTerrainMgr.GetZoneId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());
            const uint32 areaId = sTerrainMgr.GetAreaId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());

            ostringstream out;
            out << zoneId << "," << areaId << "," << pos.getAreaFlag() << "," << (pos.getAreaName().empty() ? "none" : pos.getAreaName()) << ",";

            pos.printWKT(out);

            sPlayerbotAIConfig.log("scan.csv", out.str().c_str());

            if (zoneId == 0 && areaId == 0)
            {
                const uint32 zoneId = sTerrainMgr.GetZoneId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());
                const uint32 areaId = sTerrainMgr.GetAreaId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());
                sPlayerbotAIConfig.log("x", out.str().c_str());
            }
            else
            {
                const uint32 zoneId = sTerrainMgr.GetZoneId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());
                const uint32 areaId = sTerrainMgr.GetAreaId(pos.getMapId(), pos.getX(), pos.getY(), pos.getZ());
                sPlayerbotAIConfig.log("y", out.str().c_str());
            }

            i = zoneId;
        }
        return i == 0;
    }
    else if (text == "gy" && isMod)
    {
        for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        {
            if (!sMapStore.LookupEntry(i))
                continue;

            uint32 mapId = sMapStore.LookupEntry(i)->MapID;

            Map* map = sMapMgr.FindMap(mapId);

            if (!map)
                continue;

            GraveyardManager* gy = &map->GetGraveyardManager();

            if (!gy)
                continue;

            for (uint32 x = 0; x < TOTAL_NUMBER_OF_CELLS_PER_MAP; x++)
            {
                for (uint32 y = 0; y < TOTAL_NUMBER_OF_CELLS_PER_MAP; y++)
                {
                    CellPair c(x, y);

                    WorldPosition pos(mapId, c);

                    if (!pos.isValid())
                        continue;

                    pos.setZ(pos.getHeight());

                    const uint32 zoneId = sTerrainMgr.GetZoneId(mapId, pos.getX(), pos.getY(), pos.getZ());
                    const uint32 areaId = sTerrainMgr.GetAreaId(mapId, pos.getX(), pos.getY(), pos.getZ());

                    WorldSafeLocsEntry const* graveyard = nullptr;
                    if (areaId != 0)
                    {
                        WorldSafeLocsEntry const* ClosestGrave;
                        ClosestGrave = gy->GetClosestGraveYard(pos.getX(), pos.getY(), pos.getZ(), mapId, ALLIANCE);
                        ClosestGrave = gy->GetClosestGraveYard(pos.getX(), pos.getY(), pos.getZ(), mapId, HORDE);
                    }
                }
            }
        }
        return true;
    }
    else if (text.find("test" ) == 0 && isMod)
    {
        string param = "";
        if (text.length() > 4)
            param = text.substr(5);

        ai->GetAiObjectContext()->ClearExpiredValues(param, 10);

        return true;
    }
    else if (text.find("values") == 0)
    {
        string param = "";
        if (text.length() > 6)
            param = text.substr(7);

        
        set<string> names = context->GetValues();
        vector<string> values;
        for (auto name : names)
        {
            UntypedValue* value = context->GetUntypedValue(name);
            if (!value)
                continue;

            if (!param.empty() && name.find(param) == string::npos)
                continue;

            string text = value->Format();
            if (text == "?")
                continue;

            values.push_back(name + "=" + text);

        }        

        string valuestring = sPlayerbotHelpMgr.makeList(values, "[<part>]");

        vector<string> lines = Qualified::getMultiQualifiers(valuestring, "\n");

        for (auto& line : lines)
        {
            ai->TellPlayerNoFacing(GetMaster(), line, PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, true, false);
        }

        return true;
    }
    else if (text.find("do ") == 0)
    {
        return ai->DoSpecificAction(text.substr(3), Event(), true);
    }
    else if (text.find("poi ") == 0)
    {        
        WorldPosition botPos = WorldPosition(bot);

        WorldPosition poiPoint = botPos;
        string name = "bot";

        vector<string> args = Qualified::getMultiQualifiers(text.substr(4), " ");
        TravelDestination* dest = ChooseTravelTargetAction::FindDestination(bot, args[0]);

        if (dest)
        {
            vector <WorldPosition*> points = dest->nextPoint(&botPos, true);

            if (!points.empty())
            {
                poiPoint = *points.front();
                name = dest->getTitle();
            }
        }

        if (args.size() == 1)
            args.push_back("99");
        if (args.size() == 2)
            args.push_back("6");
        if (args.size() == 3)
            args.push_back("0");

        ai->Poi(poiPoint.coord_x, poiPoint.coord_y, name, nullptr, stoi(args[1]), stoi(args[2]), stoi(args[3]));

        return true;
    }
    else if (text.find("motion") == 0 && isMod)
    {
        Unit* motionBot = bot;
        Unit* motionTarget = masterTarget;

        if (text.find("motion") != 0)
        {
            if (masterTarget)
            {
                motionBot = masterTarget;
                motionTarget = master;
            }
            else
            {
                motionBot = master;
                motionTarget = bot;
            }
        }


        MotionMaster* mm = motionBot->GetMotionMaster();

        MovementGeneratorType type = mm->GetCurrentMovementGeneratorType();

        string sType = "TODO"; // GetMoveTypeStr(type);

        Unit* cTarget = sServerFacade.GetChaseTarget(motionBot);
        float cAngle = sServerFacade.GetChaseAngle(motionBot);
        float cOffset = sServerFacade.GetChaseOffset(motionBot);
        string cTargetName = cTarget ? cTarget->GetName() : "none";

        string motionName = motionBot->GetName();

        ai->TellPlayer(GetMaster(), motionName + " :" + sType + " (" + cTargetName + " 角度:" + to_string(cAngle) + " 偏移:" + to_string(cOffset) + ")");


        if (!masterTarget)
            masterTarget = master;

        if (text.size() > 7)
        {
            string cmd = text.substr(7);

            if (cmd == "clear")
                mm->Clear();
            else if (cmd == "reset")
                mm->Clear(true);
            else if (cmd == "clearall")
                mm->Clear(false, true);
            else if (cmd == "expire")
                mm->MovementExpired();
            else if (cmd == "flee")
                mm->MoveFleeing(motionTarget, 10);
            else if (cmd == "followmain")
                mm->MoveFollow(motionTarget, 5, 0, true, true);
            else if (cmd == "follow")
                mm->MoveFollow(motionTarget, 5, 0);
            else if (cmd == "dist")
                mm->DistanceYourself(10);
            else if (cmd == "update")
                mm->UpdateMotion(10);
            else if (cmd == "chase")
                mm->MoveChase(motionTarget, 5, 0);
            else if (cmd == "fall")
                mm->MoveFall();
            else if (cmd == "formation")
            {
                FormationSlotDataSPtr form = make_shared<FormationSlotData>(0, bot->GetObjectGuid(), nullptr, SpawnGroupFormationSlotType::SPAWN_GROUP_FORMATION_SLOT_TYPE_STATIC);
                mm->MoveInFormation(form);
            }

            string sType = "TODO"; // GetMoveTypeStr(type);
            ai->TellPlayer(GetMaster(), "新:" + sType);
        }
        return true;
    }
    else if (text.find("transport") == 0 && isMod) {
        for (auto trans : WorldPosition(bot).getTransports())
        {
            GameObjectInfo const* data = sGOStorage.LookupEntry<GameObjectInfo>(trans->GetEntry());
            if (WorldPosition(bot).isOnTransport(trans))
                ai->TellPlayer(GetMaster(), "在交通工具 " + string(data->name));
            else
                ai->TellPlayer(GetMaster(), "不在交通工具 " + string(data->name));
        }
    }
    else if (text.find("ontrans") == 0 && isMod) {
        if (bot->GetTransport())
            return false;

        uint32 radius = 10;

        if (text.length() > string("ontrans").size())
            radius = stoi(text.substr(string("ontrans").size() + 1));

        WorldPosition botPos(bot);
        GenericTransport* transport = nullptr;
        for (auto trans : botPos.getTransports())
            if (!transport || botPos.distance(trans) < botPos.distance(transport))
                transport = trans;

        if (!transport)
            return false;

        GenericTransport* trans = transport;

        WorldPosition pos(trans);

        pos.setZ(botPos.getZ() + 3.0f);

        //if(trans->GetGoType() == GAMEOBJECT_TYPE_MO_TRANSPORT)
        //    pos += WorldPosition(0, cos(pos.getAngleTo(botPos)) * 3.0f, sin(pos.getAngleTo(botPos)) * 3.0f);

        bot->SetTransport(trans);
        vector<WorldPosition> path = pos.getPathFrom(botPos, bot); //Use full pathstep to get proper paths on to transports.

        if (path.empty())
        {
            ai->TellPlayer(GetMaster(), "无路径.");
            bot->SetTransport(nullptr);
            return false;
        }

        WorldPosition entryPos = path.back();

        if (!entryPos.isOnTransport(trans))
        {
            for (auto p : path)
            {
                bool onTrans = pos.isOnTransport(trans);

                if (onTrans) //Summon creature needs to be Summoned on offset coordinates.
                {
                    p.CalculatePassengerOffset(trans);
                    bot->SetTransport(trans);
                }
                else //Generate wp off transport so it doesn't spawn on transport.
                    bot->SetTransport(nullptr);

                Creature* wpCreature = bot->SummonCreature(2334, p.getX(), p.getY(), p.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
                ai->AddAura(wpCreature, 246);

                if (p.isOnTransport(transport))
                    ai->AddAura(wpCreature, 1130);
            }

            ai->TellPlayer(GetMaster(), "船上没有路径.");
            bot->SetTransport(nullptr);
            return false;
        }

        for (auto p : path)
        {
            bool onTrans = pos.isOnTransport(trans);

            if (onTrans) //Summon creature needs to be Summoned on offset coordinates.
            {
                p.CalculatePassengerOffset(trans);
                bot->SetTransport(trans);
            }
            else //Generate wp off transport so it doesn't spawn on transport.
                bot->SetTransport(nullptr);

            Creature* wpCreature = bot->SummonCreature(2334, p.getX(), p.getY(), p.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
            ai->AddAura(wpCreature, 246);

            if (p.isOnTransport(transport))
                ai->AddAura(wpCreature, 1130);
        }

        transport->AddPassenger(bot);

        ai->StopMoving();
        bot->GetMotionMaster()->MovePoint(entryPos.getMapId(), entryPos.getX(), entryPos.getY(), botPos.getZ(), FORCED_MOVEMENT_RUN);

        return true;
    }
    else if (text.find("offtrans") == 0 && isMod) {
        uint32 radius = 10;

        if (text.length() > string("offtrans").size())
            radius = stoi(text.substr(string("offtrans").size() + 1));

        if (!bot->GetTransport())
            return false;

        WorldPosition botPos(bot);
        GenericTransport* transport = bot->GetTransport();

        if (!transport)
            return false;

        WorldPosition destPos = botPos + WorldPosition(0,cos(bot->GetOrientation()) * radius, sin(bot->GetOrientation()) * radius);

        vector<WorldPosition> path = destPos.getPathFrom(botPos, nullptr);

        if (path.empty())
        {
            ai->TellPlayer(GetMaster(), "无路径.");
            return false;
        }

        WorldPosition exitPos = path.back();

        if (exitPos.isOnTransport(transport))
        {
            ai->TellPlayer(GetMaster(), "路径仍在船上.");
            return false;
        }
        
        for (auto& p : path)
        {
            p.CalculatePassengerOffset(bot->GetTransport());

            Creature* wpCreature = bot->SummonCreature(2334, p.getX(), p.getY(), p.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
            ai->AddAura(wpCreature, 246);

            if (p.isOnTransport(transport))
                ai->AddAura(wpCreature, 1130);
        }

        ai->StopMoving();
        bot->GetMotionMaster()->MovePoint(exitPos.getMapId(), exitPos.getX(), exitPos.getY(), exitPos.getZ(),FORCED_MOVEMENT_RUN);

        return true;
    }
    else if (text.find("pathable") == 0 && isMod) {
        uint32 radius = 10;

        if (text.length() > string("pathable").size())
            radius = stoi(text.substr(string("pathable").size() + 1));

        GenericTransport* transport = nullptr;
        for (auto trans : WorldPosition(bot).getTransports())
            if (!transport || WorldPosition(bot).distance(trans) < WorldPosition(bot).distance(transport))
                transport = trans;

        for (float x = radius * -1.0f; x < radius; x += 1.0f)
        {
            for (float y = radius * -1.0f; y < radius; y += 1.0f)
            {
                if (x * x + y * y > radius * radius)
                    continue;

                WorldPosition botPos(bot);
                WorldPosition pos = botPos + WorldPosition(0, x, y, 3.0f);

                Player* pathBot = bot;
                GenericTransport* botTrans = bot->GetTransport();
                GenericTransport* trans = botTrans ? botTrans : transport;

                if (!pos.isOnTransport(trans)) //When trying to calculate a position off the transport, act like the bot is off the transport.
                    pathBot = nullptr;
                else 
                    bot->SetTransport(trans);

                vector<WorldPosition> path = pos.getPathFrom(botPos, pathBot); //Use full pathstep to get proper paths on to transports.

                if (path.empty())
                    continue;

                pos = path.back();

                bool onTrans = pos.isOnTransport(trans);

                if (onTrans) //Summon creature needs to be Summoned on offset coordinates.
                {
                    pos.CalculatePassengerOffset(trans);
                    bot->SetTransport(trans);
                }
                else //Generate wp off transport so it doesn't spawn on transport.
                    bot->SetTransport(nullptr);

                Creature* wpCreature = bot->SummonCreature(2334, pos.getX(), pos.getY(), pos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
                ai->AddAura(wpCreature, 246);

                if (onTrans)
                    ai->AddAura(wpCreature, 1130);

                bot->SetTransport(botTrans);
            }
        }

        return false;
    }
    else if (text.find("randomspot") == 0 && isMod) {
        uint32 radius = 10;
        if(text.length() > string("randomspot").size())
            radius = stoi(text.substr(string("randomspot").size()+1));

        WorldPosition botPos(bot);

        PathFinder pathfinder(bot);

        if (bot->GetTransport())
            botPos.CalculatePassengerOffset(bot->GetTransport());

        pathfinder.ComputePathToRandomPoint(botPos.getVector3(), radius);
        PointsArray points = pathfinder.getPath();
        vector<WorldPosition> path = botPos.fromPointsArray(points);

        if (path.empty())
            return false;
       
        Creature* wpCreature = bot->SummonCreature(6, path.back().getX(), path.back().getY(), path.back().getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
        wpCreature->SetObjectScale(0.5f);

        if (bot->GetTransport())
            bot->GetTransport()->AddPassenger(wpCreature,true);


        for (auto& p : path)
        {
            Creature* wpCreature = bot->SummonCreature(2334, p.getX(), p.getY(), p.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
            ai->AddAura(wpCreature, 246);
            if (bot->GetTransport())
                bot->GetTransport()->AddPassenger(wpCreature,true);
        }
        return true;
    }
    else if (text.find("printmap" ) == 0 && isMod)
    {
        sTravelNodeMap.printMap();
        sTravelNodeMap.printNodeStore();
        return true;
    }
    else if (text.find("corpse") == 0)
    {

        Corpse* corpse = bot->GetCorpse();

        if (!corpse)
        {
            ai->TellPlayerNoFacing(GetMaster(), "没有尸体");
            return true;
        }


        ostringstream out;

        if(corpse->GetType() == CORPSE_BONES)
            out << "CORPSE_BONES";

        if (corpse->GetType() == CORPSE_RESURRECTABLE_PVE)
            out << "CORPSE_RESURRECTABLE_PVE";

        if (corpse->GetType() == CORPSE_RESURRECTABLE_PVP)
            out << "CORPSE_RESURRECTABLE_PVP";

        WorldPosition(corpse).printWKT(out);

        out << "时间: " << corpse->GetGhostTime();

        ai->TellPlayerNoFacing(GetMaster(), out);

        return true;
    }
    else if (text.find("logouttime") == 0) {
        int32 time = sRandomPlayerbotMgr.GetValueValidTime(bot->GetGUIDLow(), "add");

        int32 min = 0, hr = 0;

        if (time > 3600)
        {
            hr = floor(time / 3600);
            time = time % 3600;
        }
        if (time > 60)
        {
            min = floor(time / 60);
            time = time % 60;
        }

        ostringstream out;

        out << "登出时间: " << hr << ":" << min << ":" << time;

        bot->Whisper(out.str().c_str(), LANG_UNIVERSAL, event.getOwner()->GetObjectGuid());

        return true;
    }
    else if (text.find("npc") == 0)
    {
        ostringstream out;

        GuidPosition guidP = GuidPosition(ai->GetMaster()->GetSelectionGuid(), ai->GetMaster()->GetMapId());

        if (text.size() > 4)
        {
            string link = text.substr(4);

            if (!link.empty())
            {
                list<int32> entries = chat->parseWorldEntries(link);
                if (!entries.empty())
                {

                    int32 entry = entries.front();

                    for (auto cre : WorldPosition(bot).getCreaturesNear(0.0f, entry))
                    {
                        guidP = GuidPosition(cre);
                        break;
                    }
                }
            }
        }

        if (!guidP)
            return false;

        if (guidP.GetWorldObject())
            out << chat->formatWorldobject(guidP.GetWorldObject());
        
        out << " (e:" << guidP.GetEntry();
        
        if (guidP.GetUnit())
            out << ",等级:" << guidP.GetUnit()->GetLevel();
            
        out << ") ";

        guidP.printWKT(out);

        out << "[区域:" << guidP.getArea()->area_name[0]; 

        if (guidP.getArea() && guidP.getAreaLevel())
            out << " 等级: " << guidP.getAreaLevel();
        if (guidP.getArea()->zone && GetAreaEntryByAreaID(guidP.getArea()->zone))
        {
            out << " 区域:" << GetAreaEntryByAreaID(guidP.getArea()->zone)->area_name[0];
            if (sTravelMgr.getAreaLevel(guidP.getArea()->zone))
                out << " 等级: " << sTravelMgr.getAreaLevel(guidP.getArea()->zone);
        }

        out << "] ";

        uint16 event_id = sGameEventMgr.GetGameEventId<Creature>(guidP.GetCounter());

        if (event_id)
            out << " 事件:" << event_id << (sGameEventMgr.IsActiveEvent(event_id) ? " active" : " inactive");

        uint16 topPoolId = sPoolMgr.IsPartOfTopPool<Creature>(guidP.GetCounter());

        if (topPoolId)
            out << " 池:" << topPoolId << (sGameEventMgr.GetGameEventId<Pool>(topPoolId) ? " event" : " nonevent");

        ai->TellPlayerNoFacing(GetMaster(), out);


        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_GOSSIP))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_GOSSIP");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_QUESTGIVER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_QUESTGIVER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_VENDOR))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_VENDOR");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_FLIGHTMASTER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_FLIGHTMASTER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_TRAINER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_TRAINER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_SPIRITHEALER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_SPIRITHEALER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_SPIRITGUIDE))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_SPIRITGUIDE");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_INNKEEPER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_INNKEEPER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_BANKER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_BANKER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_PETITIONER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_PETITIONER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_TABARDDESIGNER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_TABARDDESIGNER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_BATTLEMASTER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_BATTLEMASTER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_AUCTIONEER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_AUCTIONEER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_STABLEMASTER))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_STABLEMASTER");
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_REPAIR))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_REPAIR");
#ifdef MANGOSBOT_ZERO
        if (guidP.HasNpcFlag(UNIT_NPC_FLAG_OUTDOORPVP))
            ai->TellPlayerNoFacing(GetMaster(), "UNIT_NPC_FLAG_OUTDOORPVP");
#endif

        unordered_map<ReputationRank, string> reaction;

        reaction[REP_HATED] = "REP_HATED";
        reaction[REP_HOSTILE] = "REP_HOSTILE";
        reaction[REP_UNFRIENDLY] = "REP_UNFRIENDLY";
        reaction[REP_NEUTRAL] = "REP_NEUTRAL";
        reaction[REP_FRIENDLY] = "REP_FRIENDLY";
        reaction[REP_HONORED] = "REP_HONORED";
        reaction[REP_REVERED] = "REP_REVERED";
        reaction[REP_EXALTED] = "REP_EXALTED";
        
        if (guidP.GetUnit())
        {
            ostringstream out;
            out << "单位对机器人的反应:" << reaction[guidP.GetUnit()->GetReactionTo(bot)];

            Unit* ubot = bot;
            out << " 机器人对单位的反应:" << reaction[ubot->GetReactionTo(guidP.GetUnit())];

            out << " NPC对机器人的反应:" << reaction[guidP.GetReactionTo(bot)];
            out << " 机器人对NPC的反应:" << reaction[GuidPosition(bot).GetReactionTo(guidP)];

            if (GuidPosition(HIGHGUID_UNIT, guidP.GetEntry()).IsHostileTo(bot))
                out << "[敌对]";
            if (GuidPosition(HIGHGUID_UNIT, guidP.GetEntry()).IsFriendlyTo(bot))
                out << "[敌对]";

            ai->TellPlayerNoFacing(GetMaster(), out);
        }

        return true;
    }  
    else if (text.find("go ") == 0)
    {
        ostringstream out;

        if (text.size() < 4)
            return false;

        GuidPosition guidP;

        string link = text.substr(3);

        if (!link.empty())
        {
            list<ObjectGuid> gos = chat->parseGameobjects(link);
            if (!gos.empty())
            {
                for (auto go : gos)
                {
                    guidP = GuidPosition(go, bot->GetMapId());
                    break;
                }
            }
        }

        if (!guidP)
            return false;

        if (!guidP.IsGameObject())
            return false;

        if (guidP.GetWorldObject())
            out << chat->formatWorldobject(guidP.GetWorldObject());

        out << " (e:" << guidP.GetEntry();

        if (guidP.GetUnit())
            out << ",等级:" << guidP.GetUnit()->GetLevel();

        out << ") ";

        guidP.printWKT(out);

        ai->TellPlayerNoFacing(GetMaster(), out);

        unordered_map<uint32, string>  types;
        types[GAMEOBJECT_TYPE_DOOR] = "GAMEOBJECT_TYPE_DOOR";
        types[GAMEOBJECT_TYPE_BUTTON] = "GAMEOBJECT_TYPE_BUTTON";
        types[GAMEOBJECT_TYPE_QUESTGIVER] = "GAMEOBJECT_TYPE_QUESTGIVER";
        types[GAMEOBJECT_TYPE_CHEST] = "GAMEOBJECT_TYPE_CHEST";
        types[GAMEOBJECT_TYPE_BINDER] = "GAMEOBJECT_TYPE_BINDER";
        types[GAMEOBJECT_TYPE_GENERIC] = "GAMEOBJECT_TYPE_GENERIC";
        types[GAMEOBJECT_TYPE_TRAP] = "GAMEOBJECT_TYPE_TRAP";
        types[GAMEOBJECT_TYPE_CHAIR] = "GAMEOBJECT_TYPE_CHAIR";
        types[GAMEOBJECT_TYPE_SPELL_FOCUS] = "GAMEOBJECT_TYPE_SPELL_FOCUS";
        types[GAMEOBJECT_TYPE_TEXT] = "GAMEOBJECT_TYPE_TEXT";
        types[GAMEOBJECT_TYPE_GOOBER] = "GAMEOBJECT_TYPE_GOOBER";
        types[GAMEOBJECT_TYPE_TRANSPORT] = "GAMEOBJECT_TYPE_TRANSPORT";
        types[GAMEOBJECT_TYPE_AREADAMAGE] = "GAMEOBJECT_TYPE_AREADAMAGE";
        types[GAMEOBJECT_TYPE_CAMERA] = "GAMEOBJECT_TYPE_CAMERA";
        types[GAMEOBJECT_TYPE_MAP_OBJECT] = "GAMEOBJECT_TYPE_MAP_OBJECT";
        types[GAMEOBJECT_TYPE_MO_TRANSPORT] = "GAMEOBJECT_TYPE_MO_TRANSPORT";
        types[GAMEOBJECT_TYPE_DUEL_ARBITER] = "GAMEOBJECT_TYPE_DUEL_ARBITER";
        types[GAMEOBJECT_TYPE_FISHINGNODE] = "GAMEOBJECT_TYPE_FISHINGNODE";
        types[GAMEOBJECT_TYPE_SUMMONING_RITUAL] = "GAMEOBJECT_TYPE_SUMMONING_RITUAL";
        types[GAMEOBJECT_TYPE_MAILBOX] = "GAMEOBJECT_TYPE_MAILBOX";
#ifndef MANGOSBOT_TWO
        types[GAMEOBJECT_TYPE_AUCTIONHOUSE] = "GAMEOBJECT_TYPE_AUCTIONHOUSE";
#endif
        types[GAMEOBJECT_TYPE_GUARDPOST] = "GAMEOBJECT_TYPE_GUARDPOST";
        types[GAMEOBJECT_TYPE_SPELLCASTER] = "GAMEOBJECT_TYPE_SPELLCASTER";
        types[GAMEOBJECT_TYPE_MEETINGSTONE] = "GAMEOBJECT_TYPE_MEETINGSTONE";
        types[GAMEOBJECT_TYPE_FLAGSTAND] = "GAMEOBJECT_TYPE_FLAGSTAND";
        types[GAMEOBJECT_TYPE_FISHINGHOLE] = "GAMEOBJECT_TYPE_FISHINGHOLE";
        types[GAMEOBJECT_TYPE_FLAGDROP] = "GAMEOBJECT_TYPE_FLAGDROP";
        types[GAMEOBJECT_TYPE_MINI_GAME] = "GAMEOBJECT_TYPE_MINI_GAME";
#ifndef MANGOSBOT_TWO
        types[GAMEOBJECT_TYPE_LOTTERY_KIOSK] = "GAMEOBJECT_TYPE_LOTTERY_KIOSK";
#endif
        types[GAMEOBJECT_TYPE_CAPTURE_POINT] = "GAMEOBJECT_TYPE_CAPTURE_POINT";
        types[GAMEOBJECT_TYPE_AURA_GENERATOR] = "GAMEOBJECT_TYPE_AURA_GENERATOR";
#ifdef MANGOSBOT_TWO
        types[GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY] = "GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY";
        types[GAMEOBJECT_TYPE_BARBER_CHAIR] = "GAMEOBJECT_TYPE_BARBER_CHAIR";
        types[GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING] = "GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING";
        types[GAMEOBJECT_TYPE_GUILD_BANK] = "GAMEOBJECT_TYPE_GUILD_BANK";
        types[GAMEOBJECT_TYPE_TRAPDOOR] = "GAMEOBJECT_TYPE_TRAPDOOR";
#endif

        ai->TellPlayerNoFacing(GetMaster(), types[guidP.GetGameObjectInfo()->type]);

        if (guidP.GetGameObject())
        {
            GameObject* object = guidP.GetGameObject();

            GOState state = object->GetGoState();

            ostringstream out;

            out << "数据:";

            if (state == GO_STATE_ACTIVE)
                out << "GO_STATE_ACTIVE";
            if (state == GO_STATE_READY)
                out << "GO_STATE_READY";
            if (state == GO_STATE_ACTIVE_ALTERNATIVE)
                out << "GO_STATE_ACTIVE_ALTERNATIVE";

            out << (object->IsInUse() ? ", 使用中." : ", 未使用.");

            LootState lootState = object->GetLootState();

            out << " 掉落数据:";

            if (lootState == GO_NOT_READY)
                out << "GO_NOT_READY";
            if (lootState == GO_READY)
                out << "GO_READY";
            if (lootState == GO_ACTIVATED)
                out << "GO_ACTIVATED";
            if (lootState == GO_JUST_DEACTIVATED)
                out << "GO_JUST_DEACTIVATED";

            ai->TellPlayerNoFacing(GetMaster(), out);
        }

        return true;
    }
    else if (text.find("travel ") == 0)
    {
        WorldPosition botPos = WorldPosition(bot);

        string destination = text.substr(7);

        TravelDestination* dest = ChooseTravelTargetAction::FindDestination(bot, destination);
        if (dest)
        {
            vector <WorldPosition*> points = dest->nextPoint(&botPos, true);

            if (points.empty())
                return false;

            vector<WorldPosition> beginPath, endPath;
            TravelNodeRoute route = sTravelNodeMap.getRoute(botPos, *points.front(), beginPath, bot);

            ostringstream out; out << "前往 " << dest->getTitle() << ": ";

            for (auto node : route.getNodes())
            {
                out << node->getName() << ", ";
            }

            ai->TellPlayerNoFacing(GetMaster(), out.str());

            return true;
        }
        else
        {
            ai->TellPlayerNoFacing(GetMaster(), "地点 " + destination + " 未找到.");
            return true;
        }
    }
    else if (text.find("quest ") == 0)
    {
        WorldPosition botPos(bot);
        uint32 questId = stoi(text.substr(6));

        Quest const* quest = sObjectMgr.GetQuestTemplate(questId);

        if (!quest || sTravelMgr.getQuests().find(questId) == sTravelMgr.getQuests().end())
        {
            ai->TellPlayerNoFacing(GetMaster(), "任务 " + text.substr(6) + " 未找到.");
            return false;
        }

        ostringstream out;

        out << quest->GetTitle() << ": ";

        ai->TellPlayerNoFacing(GetMaster(), out);

        QuestContainer* cont = sTravelMgr.getQuests()[questId];

        uint32 i = 0;

        vector<QuestTravelDestination*> dests = cont->questGivers;

        std::sort(dests.begin(), dests.end(), [botPos](QuestTravelDestination* i, QuestTravelDestination* j) {return i->distanceTo(botPos) < j->distanceTo(botPos); });


        for (auto g : dests)
        {
            ostringstream out;

            if (g->isActive(bot))
                out << "(进行中)";

            out << g->getTitle().c_str();

            out << " (" << g->distanceTo(botPos) << "码)";

            ai->TellPlayerNoFacing(GetMaster(), out);

            if (i >= 10)
                break;

            i++;
        }

        for (uint32 o = 0; o < 4; o++)
        {
            i = 0;

            dests.clear();

            for (auto g : cont->questObjectives)
            {
                QuestObjectiveTravelDestination* d = (QuestObjectiveTravelDestination*)g;
                if (d->getObjective() == o)
                    dests.push_back(g);
            }

            std::sort(dests.begin(), dests.end(), [botPos](QuestTravelDestination* i, QuestTravelDestination* j) {return i->distanceTo(botPos) < j->distanceTo(botPos); });

            for (auto g : dests)
            {
                ostringstream out;

                if (g->isActive(bot))
                    out << "(激活中)";

                out << g->getTitle().c_str();

                QuestObjectiveTravelDestination* d = (QuestObjectiveTravelDestination*)g;

                if (d->getEntry())
                    out << "[" << ObjectMgr::GetCreatureTemplate(d->getEntry())->MaxLevel << "]";

                out << " (" << g->distanceTo(botPos) << "码)";

                ai->TellPlayerNoFacing(GetMaster(), out);

                if (i >= 5)
                    break;

                i++;
            }
        }

        i = 0;

       dests = cont->questGivers;

        std::sort(dests.begin(), dests.end(), [botPos](QuestTravelDestination* i, QuestTravelDestination* j) {return i->distanceTo(botPos) < j->distanceTo(botPos); });


        for (auto g : dests)
        {
            ostringstream out;

            if (g->isActive(bot))
                out << "(激活中)";

            out << g->getTitle().c_str();

            out << " (" << g->distanceTo(botPos) << "码)";

            ai->TellPlayerNoFacing(GetMaster(), out);

            if (i >= 10)
                break;

            i++;
        }

        return true;
    }
    else if (text.find("quest") == 0)
    {
        ostringstream out;
        out << sTravelMgr.getQuests().size() << " 个任务 ";

        uint32 noT = 0, noG = 0, noO = 0;

        for (auto q : sTravelMgr.getQuests())
        {
            if (q.second->questGivers.empty())
                noG++;

            if (q.second->questTakers.empty())
                noT++;

            if (q.second->questObjectives.empty())
                noO++;
        }

        out << noG << "|" << noT << "|" << noO << " 错误.";

        ai->TellPlayerNoFacing(GetMaster(), out);

        return true;
    }
    else if (text.find("bquest") == 0)
    {
        ostringstream out;
        out << "错误任务:";

        uint32 noT = 0, noG = 0, noO = 0;

        for (auto q : sTravelMgr.getQuests())
        {
            Quest const* quest = sObjectMgr.GetQuestTemplate(q.first);

            if (!quest)
            {
                out << " " << q.first << " 不存在.";
                continue;
            }

            if (q.second->questGivers.empty() || q.second->questTakers.empty() || q.second->questObjectives.empty())
                out << quest->GetTitle() << " ";

            if (q.second->questGivers.empty())
                out << " 无接取者.";

            if (q.second->questTakers.empty())
                out << " 无完成者.";

            if (q.second->questObjectives.empty())
                out << " 无目标.";
        }
        ai->TellPlayerNoFacing(GetMaster(), out);

    }
    else if (text.find("values ") == 0)
    {
        ai->TellPlayerNoFacing(GetMaster(), ai->GetAiObjectContext()->FormatValues(text.substr(7)));

        return true;
    }
    else if (text.find("loot ") == 0)
    {
        std::string textSubstr;
        std::ostringstream out;
        for (auto itemId : chat->parseItems(textSubstr = text.substr(5)))
        {
            list<int32> entries = GAI_VALUE2(list<int32>, "item drop list", itemId);

            if (entries.empty())
                out << chat->formatItem(sObjectMgr.GetItemPrototype(itemId), 0, 0) << " 没有来源.";
            else
                out << chat->formatItem(sObjectMgr.GetItemPrototype(itemId), 0, 0) << " " << to_string(entries.size()) << " 处来源:";

            ai->TellPlayerNoFacing(GetMaster(), out);
            out.str("");
            out.clear();

            vector<pair<int32, float>> chances;

            for (auto entry : entries)
            {
                std::vector<std::string> qualifiers = { to_string(entry), to_string(itemId) };
                std::string qualifier = Qualified::MultiQualify(qualifiers, " ");
                float chance = GAI_VALUE2(float, "loot chance", qualifier);
                if(chance > 0)
                    chances.push_back(make_pair(entry, chance));
            }

            std::sort(chances.begin(), chances.end(), [](std::pair<int32, float> i, std::pair<int32, float> j) {return i.second > j.second; });

            chances.resize(std::min(20, (int)chances.size()));

            for (auto chance : chances)
            {
                out << chat->formatWorldEntry(chance.first) << ": " << chance.second << "%";
                ai->TellPlayerNoFacing(GetMaster(), out);
                out.str("");
                out.clear();
            }
        }
        return true;
    }
    else if (text.find("loot") == 0)
    {
    bool doAction = ai->DoSpecificAction("add all loot", Event(), true);

    if (doAction)
        ai->TellPlayerNoFacing(GetMaster(), "添加新的战利品.");
    else
        ai->TellPlayerNoFacing(GetMaster(), "没有新的战利品.");

    LootObjectStack* loots = AI_VALUE(LootObjectStack*, "available loot");

    for (int i = 0; i < 2000; i++)
    {
        LootObject loot = loots->GetLoot();

        if (loot.IsEmpty())
            break;

        WorldObject* wo = ai->GetWorldObject(loot.guid);

        if (wo)
            ai->TellPlayerNoFacing(GetMaster(), chat->formatWorldobject(wo) + " " + (loot.IsLootPossible(bot) ? "可拾取." : "无法拾取."));
        else
            ai->TellPlayerNoFacing(GetMaster(), to_string(loot.guid) + " " + (loot.IsLootPossible(bot) ? "可拾取." : "无法拾取.") + " " + to_string(loot.guid.GetEntry()));

        if (loot.guid.IsGameObject())
        {
            GameObject* go = ai->GetGameObject(loot.guid);

            if (go->ActivateToQuest(bot))
                ai->TellPlayerNoFacing(GetMaster(), to_string(go->GetGoType()) + " 用于任务");
            else
                ai->TellPlayerNoFacing(GetMaster(), to_string(go->GetGoType()));
        }

        loots->Remove(loot.guid);
    }

    ai->DoSpecificAction("add all loot", Event(), true);

    return true;
    }
    else if (text.find("drops ") == 0)
    {
    std::string textSubstr;
    std::ostringstream out;
    for (auto entry : chat->parseWorldEntries(textSubstr = text.substr(6)))
    {
        list<uint32> itemIds = GAI_VALUE2(list<uint32>, "entry loot list", entry);

        if (itemIds.empty())
            out << chat->formatWorldEntry(entry) << " 没有掉落物品.";
        else
            out << chat->formatWorldEntry(entry) << "找到 " << to_string(itemIds.size()) << " 处掉落物品:";

        ai->TellPlayerNoFacing(GetMaster(), out);
        out.str("");
        out.clear();

        vector<pair<uint32, float>> chances;

        for (auto itemId : itemIds)
        {
            std::vector<std::string> qualifiers = { to_string(entry) , to_string(itemId) };
            std::string qualifier = Qualified::MultiQualify(qualifiers, " ");
            float chance = GAI_VALUE2(float, "loot chance", qualifier);
            if (chance > 0 && sObjectMgr.GetItemPrototype(itemId))
            {
                chances.push_back(make_pair(itemId, chance));
            }
        }

        std::sort(chances.begin(), chances.end(), [](std::pair<uint32, float> i, std::pair<int32, float> j) {return i.second > j.second; });

        chances.resize(std::min(20, (int)chances.size()));

        for (auto chance : chances)
        {
            out << chat->formatItem(sObjectMgr.GetItemPrototype(chance.first), 0, 0) << ": " << chance.second << "%";
            ai->TellPlayerNoFacing(GetMaster(), out);
            out.str("");
            out.clear();
        }
    }
    return true;
    }
    else if (text.find("taxi") == 0)
    {
        for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            if (!bot->m_taxi.IsTaximaskNodeKnown(i))
                continue;

            TaxiNodesEntry const* taxiNode = sTaxiNodesStore.LookupEntry(i);

            ostringstream out;

            out << taxiNode->name[0];

            ai->TellPlayerNoFacing(GetMaster(), out);
        }
    return true;
    }
    else if (text.find("add node") == 0 && isMod)
    {
        WorldPosition pos(bot);

        string name = "用户:" + text.substr(9);

        TravelNode* startNode = sTravelNodeMap.addNode(pos, name, false, false);

        for (auto& endNode : sTravelNodeMap.getNodes(pos, 2000))
        {
            endNode->setLinked(false);
        }

        ai->TellPlayerNoFacing(GetMaster(), "已创建节点." + name + " .");
        
        sTravelNodeMap.setHasToGen();

        return true;
    }
    else if (text.find("rem node") == 0 && isMod)
    {
        WorldPosition pos(bot);

        TravelNode* startNode = sTravelNodeMap.getNode(pos, nullptr, 50);

        if (!startNode)
            return false;

        if (startNode->isImportant())
        {
            ai->TellPlayerNoFacing(GetMaster(), "无法移除该节点.");
        }
        sTravelNodeMap.m_nMapMtx.lock();
        sTravelNodeMap.removeNode(startNode);
        ai->TellPlayerNoFacing(GetMaster(), "节点已移除.");
        sTravelNodeMap.m_nMapMtx.unlock();

        sTravelNodeMap.setHasToGen();

        return true;
    }
    else if (text.find("reset node") == 0 && isMod) {
        for (auto& node : sTravelNodeMap.getNodes())
            node->setLinked(false);
        return true;
    }
    else if (text.find("reset path") == 0 && isMod) {
        for (auto& node : sTravelNodeMap.getNodes())
            for (auto& path : *node->getLinks())
                node->removeLinkTo(path.first, true);
        return true;
    }
    else if (text.find("gen node") == 0 && isMod) {

        //Pathfinder
        sTravelNodeMap.generateNodes();
        return true;
    }
    else if (text.find("gen path") == 0 && isMod) {
    sTravelNodeMap.generatePaths(false);
    return true;
    }
    else if (text.find("crop path" ) == 0 && isMod) {
        sTravelNodeMap.removeUselessPaths();
        return true;
    }
    else if (text.find("save node" ) == 0 && isMod)
    {
        sTravelNodeMap.printNodeStore();
        sTravelNodeMap.saveNodeStore(true);
        return true;
    }
    else if (text.find("load node" ) == 0 && isMod)
    {
        std::thread t([] {if (sTravelNodeMap.removeNodes())
            sTravelNodeMap.loadNodeStore(); });

        t.detach();

        return true;
    }
    else if (text.find("show node" ) == 0 && isMod)
    {
        WorldPosition pos(bot);

        vector<TravelNode*> nodes = sTravelNodeMap.getNodes(pos, 500);

        for (auto& node : nodes)
        {
            for (auto& l : *node->getLinks())
            {
                Unit* start = nullptr;
                list<ObjectGuid> units;

                uint32 time = 60 * IN_MILLISECONDS;

                vector<WorldPosition> ppath = l.second->getPath();

                for (auto p : ppath)
                {
                    Creature* wpCreature = bot->SummonCreature(1, p.getX(), p.getY(), p.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 20000.0f);
                    //addAura(246, wpCreature);
                    units.push_back(wpCreature->GetObjectGuid());

                    if (!start)
                        start = wpCreature;
                }

                //FakeSpell(1064, bot, start, units.front(), units, {}, pos, pos);
            }
        }
        return true;
    }
    else if (text.find("dspell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(7));

        Unit* prev = bot;

        for (float i = 0; i < 60; i++)
        {
            float ang = i / 60 * M_PI_F * 4;
            float dist = i / 60 * 30;

            WorldPosition botPos(bot);
            WorldPosition botPos1 = botPos;

            botPos.setX(botPos.getX() + cos(ang) * dist);
            botPos.setY(botPos.getY() + sin(ang) * dist);
            botPos.setZ(botPos.getHeight() + 2);

            Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

            FakeSpell(spellEffect, wpCreature, wpCreature, prev->GetObjectGuid(), {}, {}, botPos, botPos);

            prev = wpCreature;
        }
        return true;
    }
    else if (text.find("vspell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(7));

        Unit* prev = bot;

        for (float i = 0; i < 60; i++)
        {
            float ang = i / 60 * M_PI_F * 4;
            float dist = i / 60 * 30;

            WorldPosition botPos(bot);
            WorldPosition botPos1 = botPos;

            botPos.setX(botPos.getX() + cos(ang) * dist);
            botPos.setY(botPos.getY() + sin(ang) * dist);
            botPos.setZ(botPos.getHeight() + 2);

            Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);            

            if (wpCreature)
            {
                WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
                data << wpCreature->GetObjectGuid();
                data << uint32(spellEffect);                               // index from SpellVisualKit.dbc
                wpCreature->SendMessageToSet(data, true);                
            }
        }
        return true;
    }
    else if (text.find("aspell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(7));

        Unit* prev = bot;

        for (float i = 0; i < 60; i++)
        {
            float ang = i / 60 * M_PI_F * 4;
            float dist = i / 60 * 30;

            WorldPosition botPos(bot);
            WorldPosition botPos1 = botPos;

            botPos.setX(botPos.getX() + cos(ang) * dist);
            botPos.setY(botPos.getY() + sin(ang) * dist);
            botPos.setZ(botPos.getHeight() + 2);

            Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 5000.0f + i * 100.0f);
            wpCreature->SetObjectScale(0.5f);

            if (wpCreature)
            {
                addAura(spellEffect, wpCreature);
            }
        }
        return true;
    }
    else if (text.find("cspell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(7));

        list<ObjectGuid> units;

        for (float i = 0; i < 60; i++)
        {
            float ang = i / 60 * M_PI_F * 4;
            float dist = i / 60 * 30;

            WorldPosition botPos(bot);

            botPos.setX(botPos.getX() + cos(ang) * dist);
            botPos.setY(botPos.getY() + sin(ang) * dist);
            botPos.setZ(botPos.getHeight() + 2);

            Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
            units.push_back(wpCreature->GetObjectGuid());
        }

        WorldPosition botPos(bot);

        if(urand(0,1))
            FakeSpell(spellEffect, bot, bot, ObjectGuid(), units, {}, botPos, botPos);
        else
            FakeSpell(spellEffect, bot, bot, units.front(), units, {}, botPos, botPos);

        return true;
    }
    else if (text.find("fspell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(7));

        {
            WorldPacket data(SMSG_SPELL_START, (8 + 8 + 4 + 2 + 4));

            data.Initialize(SMSG_SPELL_START);
            data << bot->GetPackGUID();
            data << bot->GetPackGUID();
            data << uint32(spellEffect);
            data << uint16(0);
            data << uint32(0);
            data << uint16(2);
            data << master->GetObjectGuid();
            bot->SendMessageToSet(data, true);
        }

        {
            WorldPacket data(SMSG_SPELL_GO, 53);                    // guess size
            data << bot->GetPackGUID();
            data << bot->GetPackGUID();
            data << uint32(spellEffect);  // spellID
            data << uint8(0) << uint8(1);   // flags
            data << uint8(1);			   // amount of targets
            data << master->GetObjectGuid();
            data << uint8(0);
            data << uint16(2);
            data << master->GetObjectGuid();
            bot->SendMessageToSet(data, true);
        }

        return true;
    }
    else if (text.find("spell ") == 0 && isMod)
    {
        uint32 spellEffect = stoi(text.substr(6));
        master->GetSession()->SendPlaySpellVisual(bot->GetObjectGuid(), spellEffect);
        return true;
    }
    else if (text.find("tspellmap" ) == 0 && isMod)
    {
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(6, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                if (wpCreature)
                {
                    ostringstream out;
                    out << "effect ";
                    out << effect;

                    const std::string& Cname = out.str();

                    wpCreature->MonsterSay(Cname.c_str(), 0, master);
                }
            }
        }
        return true;
    }
    else if (text.find("uspellmap" ) == 0 && isMod)
    {
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(effect, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
            }
        }
        return true;
    }
    else if (text.find("dspellmap" ) == 0 && isMod)
    {
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                FakeSpell(effect, bot, nullptr, ObjectGuid(), {}, {}, botPos, botPos, true);
            }
        }
        return true;
    }
    else if (text.find("vspellmap" ) == 0 && isMod)
    {
        vector<WorldPacket> datMap;
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);
                                   
                if (wpCreature)
                {
                    WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
                    data << wpCreature->GetObjectGuid();
                    data << uint32(effect); ;                               // index from SpellVisualKit.dbc
                    //wpCreature->SendMessageToSet(data, true);
                    datMap.push_back(data);

                    //wpCreature->MonsterMoveWithSpeed(botPos.getX(), botPos.getY()+80, botPos.getZ(), 8.0f,true,true);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (auto dat : datMap)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            bot->SendMessageToSet(dat, true);
        }
        for (auto dat : datMap)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            bot->SendMessageToSet(dat, true);
        }

        return true;
    }
    else if (text.find("ispellmap" ) == 0 && isMod)
    {
        vector<WorldPacket> datMap;
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(6, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                if (wpCreature)
                {
                    WorldPacket data(SMSG_PLAY_SPELL_IMPACT, 8 + 4);        // visual effect on player
                    data << wpCreature->GetObjectGuid();
                    data << uint32(effect);                                 // index from SpellVisualKit.dbc
                    //wpCreature->SendMessageToSet(data, true);
                    datMap.push_back(data);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (auto dat : datMap)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            bot->SendMessageToSet(dat, true);
        }
        for (auto dat : datMap)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            bot->SendMessageToSet(dat, true);
        }

        return true;
    }
    else if (text.find("cspellmap" ) == 0 && isMod)
    {
        Creature* wpCreature = nullptr;
        Creature* lCreature = nullptr;
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                wpCreature = bot->SummonCreature(6, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                if (wpCreature && lCreature)
                {
                    wpCreature->CastSpell(lCreature, effect, TRIGGERED_OLD_TRIGGERED);
                }

                lCreature = wpCreature;
            }
        }
        return true;
    }
    else if (text.find("aspellmap" ) == 0 && isMod)
    {
        Creature* wpCreature = nullptr;

        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                if (wpCreature)
                {
                    addAura(effect, wpCreature);
                }
            }
        }
        return true;
    }
    else if (text.find("gspellmap" ) == 0 && isMod)
    {
        vector<ObjectGuid> all_targets;// = { bot->GetObjectGuid(), master->GetObjectGuid() };
        //vector<ObjectGuid> all_dummies = { bot->GetObjectGuid(), master->GetObjectGuid() };

        /*list<ObjectGuid> a_targets = *context->GetValue<list<ObjectGuid> >("all targets");
        for (auto t : a_targets)
        {
            all_targets.push_back(t);
        }
        */


        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                all_targets.push_back(wpCreature->GetObjectGuid());
            }
        }

        all_targets.push_back(master->GetObjectGuid());
        all_targets.push_back(bot->GetObjectGuid());

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (uint32 i = 0; i < 2; i++)
            for (int32 dx = 0; dx < 10; dx++)
            {
                for (int32 dy = 0; dy < 10; dy++)
                {
                    uint32 spellEffect = stoi(text.substr(10));
                    uint32 effect = dx + dy * 10 + spellEffect * 100;

                    uint32 i = dx + dy * 10;
                    list<ObjectGuid> hits, miss;

                    SpellEntry const* spellInfo = sServerFacade.LookupSpellInfo(effect);

                    if (spellInfo && spellInfo->speed > 0)
                        for (auto tar : all_targets)
                        {
                            //hits.push_back(tar);

                            switch (urand(0, 10))
                            {
                            case 0:
                                hits.push_back(tar);
                                break;
                            case 1:
                                miss.push_back(tar);
                                break;
                            case 2:
                            case 3:
                                break;
                            }
                        }

                    Unit* realCaster = ai->GetUnit(all_targets[i]);//ai->GetUnit(all_targets[urand(0, all_targets.size() - 1)]);
                    Unit* caster = ai->GetUnit(all_targets[i]);//ai->GetUnit(all_targets[urand(0, all_targets.size() - 1)]);
                    Unit* target = ai->GetUnit(all_targets[i + 1]);

                    if (!realCaster)
                        realCaster = bot;

                    if (!caster)
                        caster = bot;

                    if (!target)
                        target = master;

                    FakeSpell(effect, realCaster, caster, target->GetObjectGuid(), hits, miss, WorldPosition(caster), WorldPosition(target));

                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
            }
        return true;
    }
    else if (text.find("mspellmap" ) == 0 && isMod)
    {
    vector<ObjectGuid> all_targets;

    for (int32 dx = 0; dx < 10; dx++)
    {
        for (int32 dy = 0; dy < 10; dy++)
        {
            WorldPosition botPos(bot);

            botPos.setX(botPos.getX() + (dx - 5) * 5);
            botPos.setY(botPos.getY() + (dy - 5) * 5);
            botPos.setZ(botPos.getHeight());

            Creature* wpCreature = bot->SummonCreature(2334, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

            all_targets.push_back(wpCreature->GetObjectGuid());
        }
    }

    all_targets.push_back(master->GetObjectGuid());
    all_targets.push_back(bot->GetObjectGuid());

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (uint32 i = 0; i < 2; i++)
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 spellEffect = stoi(text.substr(10));
                uint32 effect = dx + dy * 10 + spellEffect * 100;

                uint32 i = dx + dy * 10;
                list<ObjectGuid> hits, miss;

                SpellEntry const* spellInfo = sServerFacade.LookupSpellInfo(effect);

                    for (auto tar : all_targets)
                    {
                        //hits.push_back(tar);

                        switch (urand(0, 10))
                        {
                        case 0:
                            hits.push_back(tar);
                            break;
                        case 1:
                            miss.push_back(tar);
                            break;
                        case 2:
                        case 3:
                            break;
                        }
                    }

                Unit* realCaster = ai->GetUnit(all_targets[i]);//ai->GetUnit(all_targets[urand(0, all_targets.size() - 1)]);
                Unit* caster = ai->GetUnit(all_targets[i]);//ai->GetUnit(all_targets[urand(0, all_targets.size() - 1)]);
                Unit* target = ai->GetUnit(all_targets[i + 1]);

                if (!realCaster)
                    realCaster = bot;

                if (!caster)
                    caster = bot;

                if (!target)
                    target = master;

                master->GetSession()->SendPlaySpellVisual(caster->GetObjectGuid(), 5036);
                FakeSpell(effect, realCaster, caster, target->GetObjectGuid(), hits, miss, WorldPosition(caster), WorldPosition(target));

                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    return true;
    }
    else if (text.find("soundmap" ) == 0 && isMod)
    {
        uint32 soundEffects = stoi(text.substr(9));
        for (int32 dx = 0; dx < 10; dx++)
        {
            for (int32 dy = 0; dy < 10; dy++)
            {
                uint32 effect = dx + dy * 10 + soundEffects * 100;
                WorldPosition botPos(bot);

                botPos.setX(botPos.getX() + (dx - 5) * 5);
                botPos.setY(botPos.getY() + (dy - 5) * 5);
                botPos.setZ(botPos.getHeight());

                Creature* wpCreature = bot->SummonCreature(6, botPos.getX(), botPos.getY(), botPos.getZ(), 0, TEMPSPAWN_TIMED_DESPAWN, 10000.0f);

                wpCreature->PlayDistanceSound(effect);
            }
        }
    }
    else if (text.find("sounds" ) == 0 && isMod)
    {
        uint32 soundEffects = stoi(text.substr(6));

        for (uint32 i = 0; i < 100; i++)
        {
            bot->PlayDistanceSound(i + soundEffects * 100);
            bot->Say(to_string(i + soundEffects * 100), 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        return true;
    }
    else if (text.find("dsound" ) == 0 && isMod)
    {
        uint32 soundEffect = stoi(text.substr(7));
        bot->PlayDirectSound(soundEffect);
        return true;
    }
    else if (text.find("dsound" ) == 0 && isMod)
    {
        uint32 soundEffect = stoi(text.substr(7));
        bot->PlayDirectSound(soundEffect);
        return true;
    }
    else if (text.find("sound" ) == 0 && isMod)
    {
        uint32 soundEffect = stoi(text.substr(6));
        bot->PlayDistanceSound(soundEffect);
        return true;
    }

    string response = ai->HandleRemoteCommand(text);
    ai->TellPlayer(GetMaster(), response);
    return true;
}


void DebugAction::FakeSpell(uint32 spellId, Unit* truecaster, Unit* caster, ObjectGuid target, list<ObjectGuid> otherTargets, list<ObjectGuid> missTargets, WorldPosition source, WorldPosition dest, bool forceDest)
{
    SpellEntry const* spellInfo = sServerFacade.LookupSpellInfo(spellId);
    {
        uint32 castFlags = CAST_FLAG_UNKNOWN2;

        if (spellInfo && spellInfo->HasAttribute(SPELL_ATTR_USES_RANGED_SLOT))
            castFlags |= CAST_FLAG_AMMO;                        // arrows/bullets visual

        WorldPacket data(SMSG_SPELL_START, (8 + 8 + 4 + 2 + 4));

        data << truecaster->GetPackGUID();                      //truecaster

        if (caster)
            data << caster->GetPackGUID();                          //m_caster->GetPackGUID();     
        else
            data << ObjectGuid();

        data << uint32(spellId);                                // spellId
        data << uint16(castFlags);                              // cast flags
        data << uint32(1000.0f);                                   // delay?

        SpellCastTargets m_targets;

        data << m_targets;

        //projectile info
        if (castFlags & CAST_FLAG_AMMO)
        {
            data << uint32(5996);
            data << uint32(INVTYPE_AMMO);
        }

        if(caster)
            caster->GetMap()->MessageBroadcast(caster, data);
        else
            truecaster->GetMap()->MessageBroadcast(truecaster, data);
    }
    {
        uint32 castFlags = CAST_FLAG_UNKNOWN9;



        if (spellInfo && spellInfo->HasAttribute(SPELL_ATTR_USES_RANGED_SLOT))
            castFlags |= CAST_FLAG_AMMO;                        // arrows/bullets visual
        if (spellInfo && HasPersistentAuraEffect(spellInfo))
            castFlags |= CAST_FLAG_PERSISTENT_AA;

        WorldPacket data(SMSG_SPELL_GO, 53);                    // guess size

        data << truecaster->GetPackGUID();                      //truecaster

        if (caster)
            data << caster->GetPackGUID();                          //m_caster->GetPackGUID();     
        else
            data << ObjectGuid();

        data << uint32(spellId);                                // spellId
        data << uint16(castFlags);                              // cast flags

        //WriteSpellGoTargets
        uint32 hits = otherTargets.size() + (target ? 1 : 0);

        data << uint8(hits);                                      // Hits                   

        if (target)
            data << target;

        //Hit targets here.                   
        for (auto otherTarget : otherTargets)
            data << otherTarget;

        data << (uint8)missTargets.size();                         //miss

        for (auto missTarget : missTargets)
        {
            data << missTarget;

            data << uint8(SPELL_MISS_RESIST);    //Miss condition
            data << uint8(SPELL_MISS_NONE);    //Miss condition
        }

        SpellCastTargets m_targets;

        if ((spellInfo && spellInfo->Targets & TARGET_FLAG_DEST_LOCATION) || forceDest)
            m_targets.setDestination(dest.getX(), dest.getY(), dest.getZ());
        if ((spellInfo && spellInfo->Targets & TARGET_FLAG_SOURCE_LOCATION) || forceDest)
            m_targets.setSource(source.getX(), source.getY(), source.getZ());
        if (!forceDest && target)
            if(!spellInfo || !(spellInfo->Targets & TARGET_FLAG_DEST_LOCATION && spellInfo->Targets & TARGET_FLAG_SOURCE_LOCATION))
                m_targets.setUnitTarget(ai->GetUnit(target));

        data << m_targets;

        //projectile info
        if (castFlags & CAST_FLAG_AMMO)
        {
            data << uint32(5996);
            data << uint32(INVTYPE_AMMO);
        }

        if (caster)
            caster->GetMap()->MessageBroadcast(caster, data);
        else
            truecaster->GetMap()->MessageBroadcast(truecaster, data);
    }
}

void DebugAction::addAura(uint32 spellId, Unit* target)
{
    SpellEntry const* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(spellId);
    if (!spellInfo)
        return;

    if (!IsSpellAppliesAura(spellInfo, (1 << EFFECT_INDEX_0) | (1 << EFFECT_INDEX_1) | (1 << EFFECT_INDEX_2)) &&
        !IsSpellHaveEffect(spellInfo, SPELL_EFFECT_PERSISTENT_AREA_AURA))
    {
        return;
    }

    SpellAuraHolder* holder = CreateSpellAuraHolder(spellInfo, target, target);

    for (uint32 i = 0; i < MAX_EFFECT_INDEX; ++i)
    {
        uint8 eff = spellInfo->Effect[i];
        if (eff >= MAX_SPELL_EFFECTS)
            continue;
        if (IsAreaAuraEffect(eff) ||
            eff == SPELL_EFFECT_APPLY_AURA ||
            eff == SPELL_EFFECT_PERSISTENT_AREA_AURA)
        {
            int32 basePoints = spellInfo->CalculateSimpleValue(SpellEffectIndex(i));
            int32 damage = 0; // no damage cos caster doesnt exist
            Aura* aur = CreateAura(spellInfo, SpellEffectIndex(i), &damage, &basePoints, holder, target);
            holder->AddAura(aur, SpellEffectIndex(i));
        }
    }
    if (!target->AddSpellAuraHolder(holder))
        delete holder;

    return;
}