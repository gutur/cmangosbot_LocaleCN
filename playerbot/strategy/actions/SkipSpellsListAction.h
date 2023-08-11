#pragma once
#include "../../LootObjectStack.h"
#include "GenericActions.h"

namespace ai
{
    class SkipSpellsListAction : public ChatCommandAction
    {
    public:
        SkipSpellsListAction(PlayerbotAI* ai) : ChatCommandAction(ai, "skip spells list") {}
        virtual bool Execute(Event& event) override;

#ifdef GenerateBotHelp
        virtual string GetHelpName() { return "skip spells list"; } //Must equal iternal name
        virtual string GetHelpDescription()
        {
            return "This chat command gives control over the list of spells bots aren't allowed to cast.\n"
                "Examples:\n"
                "ss ? : List the ignored spells.\n"
                "ss [spell name] : Never cast this spell.\n"
                "ss -[spell name] : Remove this spell from the ignored spell list.\n";
        }
        virtual vector<string> GetUsedActions() { return {}; }
        virtual vector<string> GetUsedValues() { return { "skip spells list" }; }
#endif 

    private:
        std::vector<string> ParseSpells(const string& text);
    };
}
