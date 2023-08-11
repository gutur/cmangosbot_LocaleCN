#pragma once

#include "../botpch.h"
#include "PlayerbotMgr.h"
#include "PlayerbotAIBase.h"
#include "strategy/AiObjectContext.h"
#include "strategy/ReactionEngine.h"
#include "strategy/ExternalEventHelper.h"
#include "ChatFilter.h"
#include "PlayerbotSecurity.h"
#include "PlayerbotTextMgr.h"
#include "BotState.h"
#include "PlayerTalentSpec.h"
#include <stack>
#include "strategy/IterateItemsMask.h"

class Player;
class PlayerbotMgr;
class ChatHandler;

using namespace std;
using namespace ai;

bool IsAlliance(uint8 race);

class PlayerbotChatHandler: protected ChatHandler
{
public:
    explicit PlayerbotChatHandler(Player* pMasterPlayer) : ChatHandler(pMasterPlayer->GetSession()) {}
    void sysmessage(string str) { SendSysMessage(str.c_str()); }
    uint32 extractQuestId(string str);
    uint32 extractSpellId(string str)
    {
        char* source = (char*)str.c_str();
        return ExtractSpellIdFromLink(&source);
    }
};

namespace ai
{
    class WorldPosition;
    class GuidPosition;
    class IterateItemsVisitor;
    class FindItemVisitor;

	class MinValueCalculator {
	public:
		MinValueCalculator(float def = 0.0f) {
			param = NULL;
			minValue = def;
		}

	public:
		void probe(float value, void* p) {
			if (!param || minValue >= value) {
				minValue = value;
				param = p;
			}
		}

	public:
		void* param;
		float minValue;
	};
};

enum HealingItemDisplayId
{
   HEALTHSTONE_DISPLAYID = 8026,
   MAJOR_HEALING_POTION = 24152,
   WHIPPER_ROOT_TUBER = 21974,
   NIGHT_DRAGON_BREATH = 21975,
   LIMITED_INVULNERABILITY_POTION = 24213,
   GREATER_DREAMLESS_SLEEP_POTION = 17403,
   SUPERIOR_HEALING_POTION = 15714,
   CRYSTAL_RESTORE = 2516,
   DREAMLESS_SLEEP_POTION = 17403,
   GREATER_HEALING_POTION = 15713,
   HEALING_POTION = 15712,
   LESSER_HEALING_POTION = 15711,
   DISCOLORED_HEALING_POTION = 15736,
   MINOR_HEALING_POTION = 15710,
   VOLATILE_HEALING_POTION = 24212,
   SUPER_HEALING_POTION = 37807,
   CRYSTAL_HEALING_POTION = 47132,
   FEL_REGENERATION_POTION = 37864,
   MAJOR_DREAMLESS_SLEEP_POTION = 37845,
};

enum RoguePoisonDisplayId
{
   DEADLY_POISON_DISPLAYID = 13707,
   CRIPPLING_POISON_DISPLAYID = 13708,
   CRIPPLING_POISON_DISPLAYID_II = 2947,
   MIND_POISON_DISPLAYID = 13709,
   INSTANT_POISON_DISPLAYID = 13710,
#ifdef MANGOSBOT_ZERO
   WOUND_POISON_DISPLAYID = 13708
#else
   WOUND_POISON_DISPLAYID = 37278
#endif
};

enum SharpeningStoneDisplayId
{
   ROUGH_SHARPENING_DISPLAYID = 24673,
   COARSE_SHARPENING_DISPLAYID = 24674,
   HEAVY_SHARPENING_DISPLAYID = 24675,
   SOLID_SHARPENING_DISPLAYID = 24676,
   DENSE_SHARPENING_DISPLAYID = 24677,
   CONSECRATED_SHARPENING_DISPLAYID = 24674,    // will not be used because bot can not know if it will face undead targets
   ELEMENTAL_SHARPENING_DISPLAYID = 21072,
   FEL_SHARPENING_DISPLAYID = 39192,
   ADAMANTITE_SHARPENING_DISPLAYID = 39193,
};

enum WeightStoneDisplayId
{
   ROUGH_WEIGHTSTONE_DISPLAYID = 24683,
   COARSE_WEIGHTSTONE_DISPLAYID = 24684,
   HEAVY_WEIGHTSTONE_DISPLAYID = 24685,
   SOLID_WEIGHTSTONE_DISPLAYID = 24686,
   DENSE_WEIGHTSTONE_DISPLAYID = 24687,
   FEL_WEIGHTSTONE_DISPLAYID = 39548,
   ADAMANTITE_WEIGHTSTONE_DISPLAYID = 39549,
};

#ifdef MANGOSBOT_ZERO
// m_zero
enum WizardOilDisplayId
{
    MINOR_WIZARD_OIL = 33194,
    LESSER_WIZARD_OIL = 33450,
    BRILLIANT_WIZARD_OIL = 33452,
    WIZARD_OIL = 33451,
    SUPERIOR_WIZARD_OIL = 47904,
    /// Blessed Wizard Oil = 26865,//scourge inv
};
// m_zero
enum ManaOilDisplayId
{
    MINOR_MANA_OIL = 33453,
    LESSER_MANA_OIL = 33454,
    BRILLIANT_MANA_OIL = 33455,
    SUPERIOR_MANA_OIL = 36862,
};
#else
enum WizardOilDisplayId
{
   MINOR_WIZARD_OIL     = 9731,
   LESSER_WIZARD_OIL    = 47903,
   BRILLIANT_WIZARD_OIL = 47901,
   WIZARD_OIL           = 47905,
   SUPERIOR_WIZARD_OIL  = 47904,
  /// Blessed Wizard Oil = 26865,//scourge inv
};

enum ManaOilDisplayId
{
   MINOR_MANA_OIL       = 34492,
   LESSER_MANA_OIL      = 47902,
   BRILLIANT_MANA_OIL   = 41488,
   SUPERIOR_MANA_OIL    = 36862,
};
#endif

enum ShieldWardDisplayId
{
   LESSER_WARD_OFSHIELDING = 38759,
   GREATER_WARD_OFSHIELDING = 38760,
};

enum class BotTypeNumber : uint8
{    
    ACTIVITY_TYPE_NUMBER = 1,
    GROUPER_TYPE_NUMBER = 2,
    GUILDER_TYPE_NUMBER = 3,
    CHATFILTER_NUMBER = 4
};

enum class GrouperType : uint8
{
    SOLO = 0,
    MEMBER = 1,
    LEADER_2 = 2,
    LEADER_3 = 3,
    LEADER_4 = 4,
    LEADER_5 = 5,
#ifndef MANGOSBOT_ZERO
    RAIDER_10 = 10,
    RAIDER_MAX = 25
#else
    RAIDER_20 = 20,
    RAIDER_MAX = 40
#endif
};

enum class GuilderType : uint8
{
    SOLO = 0,
    TINY = 30,
    SMALL = 50,
    MEDIUM = 70,
    LARGE = 120,
    HUGE = 250
};

enum class ActivePiorityType : uint8
{
    IS_REAL_PLAYER = 0,
    HAS_REAL_PLAYER_MASTER = 1,
    IN_GROUP_WITH_REAL_PLAYER = 2,
    IN_INSTANCE = 3,
    VISIBLE_FOR_PLAYER = 4,
    IS_ALWAYS_ACTIVE = 5,
    IN_COMBAT = 6,
    IN_BG_QUEUE = 7,
    IN_LFG = 8,
    NEARBY_PLAYER = 9,
    PLAYER_FRIEND = 10,
    PLAYER_GUILD = 11,
    IN_ACTIVE_AREA = 12,
    IN_ACTIVE_MAP = 13,
    IN_INACTIVE_MAP = 14,
    IN_EMPTY_SERVER = 15,
    MAX_TYPE
};

enum ActivityType
{
    GRIND_ACTIVITY = 1,
    RPG_ACTIVITY = 2,
    TRAVEL_ACTIVITY = 3,
    OUT_OF_PARTY_ACTIVITY = 4,
    PACKET_ACTIVITY = 5,
    DETAILED_MOVE_ACTIVITY = 6,
    PARTY_ACTIVITY = 7,
    REACT_ACTIVITY = 8,
    ALL_ACTIVITY = 9,
    MAX_ACTIVITY_TYPE 
};

enum BotRoles
{
    BOT_ROLE_NONE = 0x00,
    BOT_ROLE_TANK = 0x01,
    BOT_ROLE_HEALER = 0x02,
    BOT_ROLE_DPS = 0x04
};

class PacketHandlingHelper
{
public:
    void AddHandler(uint16 opcode, string handler, bool shouldDelay = false);
    void Handle(ExternalEventHelper &helper);
    void AddPacket(const WorldPacket& packet);

private:
    map<uint16, string> handlers;
    map<uint16, bool> delay;
    stack<WorldPacket> queue;
    std::mutex m_botPacketMutex;
};

class ChatCommandHolder
{
public:
    ChatCommandHolder(string command, Player* owner = NULL, uint32 type = CHAT_MSG_WHISPER, time_t time = 0) : command(command), owner(owner), type(type), time(time) {}
    ChatCommandHolder(ChatCommandHolder const& other)
    {
        this->command = other.command;
        this->owner = other.owner;
        this->type = other.type;
        this->time = other.time;
    }

public:
    string GetCommand() { return command; }
    Player* GetOwner() { return owner; }
    uint32 GetType() { return type; }
    time_t GetTime() { return time; }

private:
    string command;
    Player* owner;
    uint32 type;
    time_t time;
};

class PlayerbotAI : public PlayerbotAIBase
{
public:
	PlayerbotAI();
	PlayerbotAI(Player* bot);
	virtual ~PlayerbotAI();

    virtual void UpdateAI(uint32 elapsed, bool minimal = false);

    void HandleCommands();
private:
    void UpdateAIInternal(uint32 elapsed, bool minimal = false) override;

public:	
    static string BotStateToString(BotState state);
	string HandleRemoteCommand(string command);
    void HandleCommand(uint32 type, const string& text, Player& fromPlayer, const uint32 lang = LANG_UNIVERSAL);
    void QueueChatResponse(uint8 msgtype, ObjectGuid guid1, ObjectGuid guid2, std::string message, std::string chanName, std::string name);
	void HandleBotOutgoingPacket(const WorldPacket& packet);
    void HandleMasterIncomingPacket(const WorldPacket& packet);
    void HandleMasterOutgoingPacket(const WorldPacket& packet);
	void HandleTeleportAck();
    void ChangeEngine(BotState type);
    void DoNextAction(bool minimal = false);
    bool CanDoSpecificAction(const string& name, bool isUseful = true, bool isPossible = true);
    virtual bool DoSpecificAction(const string& name, Event event = Event(), bool silent = false);
    void ChangeStrategy(const string& name, BotState type);
    void ClearStrategies(BotState type);
    list<string_view> GetStrategies(BotState type);
    bool ContainsStrategy(StrategyType type);
    bool HasStrategy(const string& name, BotState type);
    template<class T>
    T* GetStrategy(const string& name, BotState type);
    BotState GetState() { return currentState; };
    void ResetStrategies(bool autoLoad = true);
    void ReInitCurrentEngine();
    void Reset(bool full = false);
    bool IsTank(Player* player, bool inGroup = true);
    bool IsHeal(Player* player, bool inGroup = true);
    bool IsRanged(Player* player, bool inGroup = true);
    Creature* GetCreature(ObjectGuid guid);
    Unit* GetUnit(ObjectGuid guid);
    static Unit* GetUnit(CreatureDataPair const* creatureDataPair);
    GameObject* GetGameObject(ObjectGuid guid);
    static GameObject* GetGameObject(GameObjectDataPair const* gameObjectDataPair);
    WorldObject* GetWorldObject(ObjectGuid guid);
    vector<Player*> GetPlayersInGroup();
    bool TellPlayer(Player* player, ostringstream &stream, PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, bool isPrivate = true) { return TellPlayer(player, stream.str(), securityLevel, isPrivate); }
    bool TellPlayer(Player* player, string text, PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, bool isPrivate = true);
    bool TellPlayerNoFacing(Player* player, ostringstream& stream, PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, bool isPrivate = true, bool noRepeat = true) { return TellPlayerNoFacing(player, stream.str(), securityLevel, isPrivate, noRepeat); }
    bool TellPlayerNoFacing(Player* player, string text, PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL, bool isPrivate = true, bool noRepeat = true);
    bool TellError(string text, PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL);
    void SpellInterrupted(uint32 spellid);
    int32 CalculateGlobalCooldown(uint32 spellid);
    void InterruptSpell();
    bool RemoveAura(const std::string& name);
    void RemoveShapeshift();
    void WaitForSpellCast(Spell *spell);
    bool PlaySound(uint32 emote);
    bool PlayEmote(uint32 emote);
    void Ping(float x, float y);
    void Poi(float x, float y, string icon_name = "This way", Player* player = nullptr, uint32 flags = 99, uint32 icon = 6 /* red flag */, uint32 icon_data = 0);
    Item * FindPoison() const;
    Item * FindConsumable(uint32 displayId) const;
    Item * FindBandage() const;
    Item* FindStoneFor(Item* weapon) const;
    Item* FindOilFor(Item* weapon) const;
#ifdef MANGOSBOT_ZERO
    void ImbueItem(Item* item, uint16 targetFlag, ObjectGuid targetGUID);
#else
    void ImbueItem(Item* item, uint32 targetFlag, ObjectGuid targetGUID);
#endif
    void ImbueItem(Item* item, uint8 targetInventorySlot);
    void ImbueItem(Item* item, Unit* target);
    void ImbueItem(Item* item);
    void EnchantItemT(uint32 spellid, uint8 slot, Item* item = nullptr);
    uint32 GetBuffedCount(Player* player, string spellname);

    bool GetSpellRange(string name, float* maxRange, float* minRange = nullptr);
    uint32 GetSpellCastDuration(Spell* spell);

    virtual bool HasAura(string spellName, Unit* player, bool maxStack = false, bool checkIsOwner = false, int maxAmount = -1, bool hasMyAura = false, int minDuration = 0);
    virtual bool HasAnyAuraOf(Unit* player, ...);
    virtual bool HasMyAura(string spellName, Unit* player) { return HasAura(spellName, player, false, false, -1, true); }
    uint8 GetHealthPercent(const Unit& target) const;
    uint8 GetHealthPercent() const;
    uint8 GetManaPercent(const Unit& target) const;
    uint8 GetManaPercent() const;

    virtual bool IsInterruptableSpellCasting(Unit* player, string spell, uint8 effectMask);
    virtual bool HasAuraToDispel(Unit* player, uint32 dispelType);
    bool canDispel(const SpellEntry* entry, uint32 dispelType);

    bool HasSpell(string name) const;
    bool HasSpell(uint32 spellid) const;
    bool HasAura(uint32 spellId, Unit* player, bool checkOwner = false);
    Aura* GetAura(uint32 spellId, Unit* player, bool checkOwner = false);
    Aura* GetAura(std::string spellName, Unit* player, bool checkOwner = false);
    std::vector<Aura*> GetAuras(Unit* player);

    virtual bool CanCastSpell(string name, Unit* target, uint8 effectMask, Item* itemTarget = NULL, bool ignoreRange = false);
    bool CanCastSpell(uint32 spellid, Unit* target, uint8 effectMask, bool checkHasSpell = true, Item* itemTarget = NULL, bool ignoreRange = false);
    bool CanCastSpell(uint32 spellid, GameObject* goTarget, uint8 effectMask, bool checkHasSpell = true, bool ignoreRange = false);
    bool CanCastSpell(uint32 spellid, float x, float y, float z, uint8 effectMask, bool checkHasSpell = true, Item* itemTarget = NULL, bool ignoreRange = false);
    bool CanCastVehicleSpell(uint32 spellid, Unit* target);

    virtual bool CastSpell(string name, Unit* target, Item* itemTarget = NULL, bool waitForSpell = true, uint32* outSpellDuration = NULL, bool canUseReagentCheat = true);
    bool CastSpell(uint32 spellId, Unit* target, Item* itemTarget = NULL, bool waitForSpell = true, uint32* outSpellDuration = NULL, bool canUseReagentCheat = true);
    bool CastSpell(uint32 spellId, float x, float y, float z, Item* itemTarget = NULL, bool waitForSpell = true, uint32* outSpellDuration = NULL, bool canUseReagentCheat = true);
    bool CastVehicleSpell(uint32 spellId, Unit* target);
    bool CastVehicleSpell(uint32 spellId, float x, float y, float z);

    bool IsInVehicle(bool canControl = false, bool canCast = false, bool canAttack = false, bool canTurn = false, bool fixed = false);

    uint32 GetEquipGearScore(Player* player, bool withBags, bool withBank);
    uint32 GetEquipStatsValue(Player* player);
    bool HasSkill(SkillType skill);
    bool IsAllowedCommand(string text);
    float GetRange(string type);

    static ReputationRank GetFactionReaction(FactionTemplateEntry const* thisTemplate, FactionTemplateEntry const* otherTemplate);
    static bool friendToAlliance(FactionTemplateEntry const* templateEntry) { return GetFactionReaction(templateEntry, sFactionTemplateStore.LookupEntry(1)) >= REP_NEUTRAL; }
    static bool friendToHorde(FactionTemplateEntry const* templateEntry) { return GetFactionReaction(templateEntry, sFactionTemplateStore.LookupEntry(2)) >= REP_NEUTRAL; }
    bool IsFriendlyTo(FactionTemplateEntry const* templateEntry) { return GetFactionReaction(bot->GetFactionTemplateEntry(), templateEntry) >= REP_NEUTRAL; }
    bool IsFriendlyTo(uint32 faction) { return GetFactionReaction(bot->GetFactionTemplateEntry(), sFactionTemplateStore.LookupEntry(faction)) >= REP_NEUTRAL; }
    static bool AddAura(Unit* unit, uint32 spellId);
    ReputationRank getReaction(FactionTemplateEntry const* factionTemplate) { return GetFactionReaction(bot->GetFactionTemplateEntry(), factionTemplate);}

    void InventoryIterateItems(IterateItemsVisitor* visitor, IterateItemsMask mask);
    void InventoryTellItems(Player* player, map<uint32, int> items, map<uint32, bool> soulbound);
    void InventoryTellItem(Player* player, ItemPrototype const* proto, int count, bool soulbound);
    list<Item*> InventoryParseItems(string text, IterateItemsMask mask);
    uint32 InventoryGetItemCount(FindItemVisitor* visitor, IterateItemsMask mask);
    string InventoryParseOutfitName(string outfit);
    ItemIds InventoryParseOutfitItems(string outfit);
    ItemIds InventoryFindOutfitItems(string name);

    void AccelerateRespawn(Creature* creature, float accelMod = 0);
    void AccelerateRespawn(ObjectGuid guid, float accelMod = 0) { Creature* creature = GetCreature(guid); if (creature) AccelerateRespawn(creature,accelMod); }

private:
    void InventoryIterateItemsInBags(IterateItemsVisitor* visitor);
    void InventoryIterateItemsInEquip(IterateItemsVisitor* visitor);   
    void InventoryIterateItemsInBank(IterateItemsVisitor* visitor);
    void InventoryIterateItemsInBuyBack(IterateItemsVisitor* visitor);

private:
    void _fillGearScoreData(Player *player, Item* item, std::vector<uint32>* gearScore, uint32& twoHandScore);
    bool IsTellAllowed(PlayerbotSecurityLevel securityLevel = PlayerbotSecurityLevel::PLAYERBOT_SECURITY_ALLOW_ALL);

public:
	Player* GetBot() { return bot; }
    Player* GetMaster() { return master; }

    //Checks if the bot is really a player. Players always have themselves as master.
    bool IsRealPlayer() { return bot->GetSession()->GetRemoteAddress() != "disconnected/bot"; }
    bool IsRealPlayer(Unit* unit) { return unit->IsPlayer() && ((Player*)unit)->GetSession()->GetRemoteAddress() != "disconnected/bot"; }
    bool IsSelfMaster() { return master ? (master == bot) : false; }
    //Bot has a master that is a player.
    bool HasRealPlayerMaster() { return master && (!master->GetPlayerbotAI() || master->GetPlayerbotAI()->IsRealPlayer()); } 
    //Bot has a master that is actively playing.
    bool HasActivePlayerMaster() { return master && !master->GetPlayerbotAI(); }
    //Checks if the bot is summoned as alt of a player
    bool IsAlt() { return HasRealPlayerMaster() && !sRandomPlayerbotMgr.IsRandomBot(bot); }
    //Get the group leader or the master of the bot.
    Player* GetGroupMaster() { return bot->InBattleGround() ? master : bot->GetGroup() ? (sObjectMgr.GetPlayer(bot->GetGroup()->GetLeaderGuid()) ? sObjectMgr.GetPlayer(bot->GetGroup()->GetLeaderGuid()) : master) : master; }

    //Check if player is safe to use.
    bool IsSafe(Player* player) { return player && player->GetMapId() == bot->GetMapId() && player->GetInstanceId() == bot->GetInstanceId() && !player->IsBeingTeleported(); }
    bool IsSafe(WorldObject* obj) { return obj && obj->GetMapId() == bot->GetMapId() && obj->GetInstanceId() == bot->GetInstanceId() && (!obj->IsPlayer() || !((Player*)obj)->IsBeingTeleported()); }

    //Returns a semi-random (cycling) number that is fixed for each bot.
    uint32 GetFixedBotNumer(BotTypeNumber typeNumber, uint32 maxNum = 100, float cyclePerMin = 1); 

    GrouperType GetGrouperType();
    GuilderType GetGuilderType();
    bool HasPlayerNearby(WorldPosition pos, float range);
    bool HasPlayerNearby(float range = sPlayerbotAIConfig.reactDistance);
    bool HasManyPlayersNearby(uint32 trigerrValue = 20, float range = sPlayerbotAIConfig.sightDistance);

    ActivePiorityType GetPriorityType();
    pair<uint32,uint32> GetPriorityBracket(ActivePiorityType type);
    bool AllowActive(ActivityType activityType);
    bool AllowActivity(ActivityType activityType = ALL_ACTIVITY, bool checkNow = false);

    bool HasCheat(BotCheatMask mask) { return ((uint32)mask & (uint32)cheatMask) != 0 || ((uint32)mask & (uint32)sPlayerbotAIConfig.botCheatMask) != 0; }
    BotCheatMask GetCheat() { return cheatMask; }
    void SetCheat(BotCheatMask mask) { cheatMask = mask; }

    void SetMaster(Player* master) { this->master = master; }
    AiObjectContext* GetAiObjectContext() { return aiObjectContext; }
    void SetAiObjectContext(AiObjectContext* aiObjectContext) { this->aiObjectContext = aiObjectContext; }
    ChatHelper* GetChatHelper() { return &chatHelper; }
    bool IsOpposing(Player* player);
    static bool IsOpposing(uint8 race1, uint8 race2);
    PlayerbotSecurity* GetSecurity() { return &security; }

    Position GetJumpDestination() { return jumpDestination; }
    void SetJumpDestination(Position pos) { jumpDestination = pos; }
    void ResetJumpDestination() { jumpDestination = Position(); }

    bool CanMove();
    void StopMoving();
    bool IsInRealGuild();
    void SetPlayerFriend(bool isFriend) {isPlayerFriend = isFriend;}
    bool IsPlayerFriend() { return isPlayerFriend; }
    bool HasPlayerRelation();

    bool IsStateActive(BotState state) const;
    time_t GetCombatStartTime() const;

    void OnCombatStarted();
    void OnCombatEnded();
    void OnDeath();
    void OnResurrected();
    
    void SetActionDuration(const Action* action);
    void SetActionDuration(uint32 duration);

    const Action* GetLastExecutedAction(BotState state) const;

    bool IsImmuneToSpell(uint32 spellId) const;

    void SetMoveToTransport(bool flag = true) { isMovingToTransport = flag; }
    bool GetMoveToTransport() { return isMovingToTransport; }


    void SetShouldLogOut(bool val = true) { shouldLogOut = val; }
    bool GetShouldLogOut() { return shouldLogOut; }

    PlayerTalentSpec GetTalentSpec();
    void UpdateTalentSpec(PlayerTalentSpec spec = PlayerTalentSpec::TALENT_SPEC_INVALID);
    
private:
    bool UpdateAIReaction(uint32 elapsed, bool minimal, bool isStunned);
    void UpdateFaceTarget(uint32 elapsed, bool minimal);

protected:
	Player* bot;
	Player* master;
	uint32 accountId;
    AiObjectContext* aiObjectContext;
    Engine* currentEngine;
    ReactionEngine* reactionEngine;
    Engine* engines[(uint8)BotState::BOT_STATE_ALL];
    BotState currentState;
    ChatHelper chatHelper;
    queue<ChatCommandHolder> chatCommands;
    queue<ChatQueuedReply> chatReplies;
    PacketHandlingHelper botOutgoingPacketHandlers;
    PacketHandlingHelper masterIncomingPacketHandlers;
    PacketHandlingHelper masterOutgoingPacketHandlers;
    CompositeChatFilter chatFilter;
    PlayerbotSecurity security;
    map<string, time_t> whispers;
    pair<ChatMsg, time_t> currentChat;
    static set<string> unsecuredCommands;
    bool allowActive[MAX_ACTIVITY_TYPE];
    time_t allowActiveCheckTimer[MAX_ACTIVITY_TYPE];
    bool inCombat = false;
    bool isMoving = false;
    bool isWaiting = false;
    BotCheatMask cheatMask = BotCheatMask::none;
    Position jumpDestination = Position();
    uint32 faceTargetUpdateDelay;
    bool isPlayerFriend = false;
    bool isMovingToTransport = false;
    bool shouldLogOut = false;
};

template<typename T>
T* PlayerbotAI::GetStrategy(const string& name, BotState type)
{
    return  dynamic_cast<T*>(engines[(uint8)type]->GetStrategy(name));
}