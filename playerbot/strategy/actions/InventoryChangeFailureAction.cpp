#include "botpch.h"
#include "../../playerbot.h"
#include "InventoryChangeFailureAction.h"


using namespace ai;

map<InventoryResult, string> InventoryChangeFailureAction::messages;

bool InventoryChangeFailureAction::Execute(Event& event)
{
    if (!ai->GetMaster())
        return false;

    if (messages.empty())
    {
        messages[EQUIP_ERR_CANT_EQUIP_LEVEL_I] = "我的等级太低.";
        messages[EQUIP_ERR_CANT_EQUIP_SKILL] = "我的技能等级太低.";
        messages[EQUIP_ERR_ITEM_DOESNT_GO_TO_SLOT] = "无效的槽位.";
        messages[EQUIP_ERR_BAG_FULL] = "我的背包已满.";
        messages[EQUIP_ERR_NONEMPTY_BAG_OVER_OTHER_BAG] = "此背包不为空.";
        messages[EQUIP_ERR_CANT_TRADE_EQUIP_BAGS] = "不能交易已装备的背包.";
        messages[EQUIP_ERR_ONLY_AMMO_CAN_GO_HERE] = "无效的槽位(仅弹药需要)";
        messages[EQUIP_ERR_NO_REQUIRED_PROFICIENCY] = "我没有必要的技能.";
        messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE] = "没有可用的装备槽位.";
        messages[EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM] = "我永远不能使用这个物品.";
        messages[EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM2] = "我永远不能使用这个物品.";
        messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE2] = messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE];
        messages[EQUIP_ERR_CANT_EQUIP_WITH_TWOHANDED] = "不能在持有双手武器时装备.";
        messages[EQUIP_ERR_CANT_DUAL_WIELD] = "我不能进行双武器战斗.";
        messages[EQUIP_ERR_ITEM_DOESNT_GO_INTO_BAG] = "此物品无法放入此背包.";
        messages[EQUIP_ERR_ITEM_DOESNT_GO_INTO_BAG2] = "此物品无法放入此背包.";
        messages[EQUIP_ERR_CANT_CARRY_MORE_OF_THIS] = "我无法再携带更多此类物品.";
        messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE3] = messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE];
        messages[EQUIP_ERR_ITEM_CANT_STACK] = "物品无法叠加.";
        messages[EQUIP_ERR_ITEM_CANT_BE_EQUIPPED] = "物品无法装备.";
        messages[EQUIP_ERR_ITEMS_CANT_BE_SWAPPED] = "无法交换这些物品.";
        messages[EQUIP_ERR_SLOT_IS_EMPTY] = "没有装备任何物品.";
        messages[EQUIP_ERR_ITEM_NOT_FOUND] = "找不到该物品.";
        messages[EQUIP_ERR_CANT_DROP_SOULBOUND] = "无法丢弃灵魂绑定物品.";
        messages[EQUIP_ERR_OUT_OF_RANGE] = "我超出范围.";
        messages[EQUIP_ERR_TRIED_TO_SPLIT_MORE_THAN_COUNT] = "无效的拆分数量.";
        messages[EQUIP_ERR_COULDNT_SPLIT_ITEMS] = "无法拆分此物品.";
        messages[EQUIP_ERR_MISSING_REAGENT] = "缺少材料.";
        messages[EQUIP_ERR_NOT_ENOUGH_MONEY] = "金币不足.";
        messages[EQUIP_ERR_NOT_A_BAG] = "这不是一个背包.";
        messages[EQUIP_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS] = "背包不为空.";
        messages[EQUIP_ERR_DONT_OWN_THAT_ITEM] = "这不是我的物品.";
        messages[EQUIP_ERR_CAN_EQUIP_ONLY1_QUIVER] = "只能装备箭袋.";
        messages[EQUIP_ERR_MUST_PURCHASE_THAT_BAG_SLOT] = "我必须购买该槽位.";
        messages[EQUIP_ERR_TOO_FAR_AWAY_FROM_BANK] = "我离银行太远.";
        messages[EQUIP_ERR_ITEM_LOCKED] = "此物品已锁定.";
        messages[EQUIP_ERR_YOU_ARE_STUNNED] = "我被眩晕.";
        messages[EQUIP_ERR_YOU_ARE_DEAD] = "我已死亡.";
        messages[EQUIP_ERR_CANT_DO_RIGHT_NOW] = "我现在无法进行此操作.";
        messages[EQUIP_ERR_INT_BAG_ERROR] = "内部错误.";
        messages[EQUIP_ERR_CAN_EQUIP_ONLY1_BOLT] = "只能装备弩.";
        messages[EQUIP_ERR_CAN_EQUIP_ONLY1_AMMOPOUCH] = "只能装备弹药包.";
        messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED] = "物品无法打包.";
        messages[EQUIP_ERR_EQUIPPED_CANT_BE_WRAPPED] = messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED];
        messages[EQUIP_ERR_WRAPPED_CANT_BE_WRAPPED] = messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED];
        messages[EQUIP_ERR_BOUND_CANT_BE_WRAPPED] = messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED];
        messages[EQUIP_ERR_UNIQUE_CANT_BE_WRAPPED] = messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED];
        messages[EQUIP_ERR_BAGS_CANT_BE_WRAPPED] = messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED];
        messages[EQUIP_ERR_ALREADY_LOOTED] = "已经捡过了.";
        messages[EQUIP_ERR_INVENTORY_FULL] = "我的背包已满.";
        messages[EQUIP_ERR_BANK_FULL] = "我的银行已满.";
        messages[EQUIP_ERR_ITEM_IS_CURRENTLY_SOLD_OUT] = "该物品目前售罄.";
        messages[EQUIP_ERR_BAG_FULL3] = messages[EQUIP_ERR_BANK_FULL];
        messages[EQUIP_ERR_ITEM_NOT_FOUND2] = messages[EQUIP_ERR_ITEM_NOT_FOUND];
        messages[EQUIP_ERR_ITEM_CANT_STACK2] = messages[EQUIP_ERR_ITEM_CANT_STACK];
        messages[EQUIP_ERR_BAG_FULL4] = messages[EQUIP_ERR_BAG_FULL];
        messages[EQUIP_ERR_ITEM_SOLD_OUT] = messages[EQUIP_ERR_ITEM_IS_CURRENTLY_SOLD_OUT];
        messages[EQUIP_ERR_OBJECT_IS_BUSY] = "此对象正在忙碌中.";
        messages[EQUIP_ERR_NOT_IN_COMBAT] = "我当前不在战斗中.";
        messages[EQUIP_ERR_NOT_WHILE_DISARMED] = "无法在被缴械状态下进行此操作.";
        messages[EQUIP_ERR_BAG_FULL6] = messages[EQUIP_ERR_BAG_FULL];
        messages[EQUIP_ERR_CANT_EQUIP_RANK] = "等级不足.";
        messages[EQUIP_ERR_CANT_EQUIP_REPUTATION] = "声望不足.";
        messages[EQUIP_ERR_TOO_MANY_SPECIAL_BAGS] = "特殊背包太多.";
        messages[EQUIP_ERR_LOOT_CANT_LOOT_THAT_NOW] = "现在无法拾取此物品.";
    }

    WorldPacket p(event.getPacket());
    p.rpos(0);
    uint8 err;
    p >> err;
    if (err == EQUIP_ERR_OK)
        return false;

    string msg = messages[(InventoryResult)err];
    if (!msg.empty())
    {
        ai->TellError(msg);
        return true;
    }

    return false;
}
