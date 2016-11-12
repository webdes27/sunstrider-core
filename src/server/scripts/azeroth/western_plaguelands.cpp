/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Western_Plaguelands
SD%Complete: 90
SDComment: Quest support: 5097, 5098, 5216, 5219, 5222, 5225, 5229, 5231, 5233, 5235. To obtain Vitreous Focuser (could use more spesifics about gossip items)
SDCategory: Western Plaguelands
EndScriptData */

/* ContentData
npcs_dithers_and_arbington
npc_the_scourge_cauldron
npc_myranda_the_hag
npcs_andorhal_tower
EndContentData */



/*######
## npcs_dithers_and_arbington
######*/

#define GOSSIP_HDA1 "What does the Felstone Field Cauldron need?"
#define GOSSIP_HDA2 "What does the Dalson's Tears Cauldron need?"
#define GOSSIP_HDA3 "What does the Writhing Haunt Cauldron need?"
#define GOSSIP_HDA4 "What does the Gahrron's Withering Cauldron need?"

#define GOSSIP_SDA1 "Thanks, i need a Vitreous Focuser"

bool GossipHello_npcs_dithers_and_arbington(Player *player, Creature *_Creature)
{
    if(_Creature->IsQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->IsVendor())
        player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if(player->GetQuestRewardStatus(5237) || player->GetQuestRewardStatus(5238))
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        player->SEND_GOSSIP_MENU_TEXTID(3985, _Creature->GetGUID());
    }else
        SEND_PREPARED_GOSSIP_MENU(player, _Creature);

    return true;
}

bool GossipSelect_npcs_dithers_and_arbington(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch(action)
    {
        case GOSSIP_ACTION_TRADE:
            player->SEND_VENDORLIST( _Creature->GetGUID() );
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU_TEXTID(3980, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU_TEXTID(3981, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU_TEXTID(3982, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU_TEXTID(3983, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player, 17529, false);
            break;
    }
    return true;
}

/*######
## npc_the_scourge_cauldron
######*/

struct npc_the_scourge_cauldronAI : public ScriptedAI
{
    npc_the_scourge_cauldronAI(Creature *c) : ScriptedAI(c) {}

    void Reset() override {}

    void EnterCombat(Unit* who) override {}

    void DoDie()
    {
        //summoner dies here
        me->DealDamage(me, me->GetHealth(), nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, false);
        //override any database `spawntimesecs` to prevent duplicated summons
        uint32 rTime = me->GetRespawnDelay();
        if( rTime<600 )
            me->SetRespawnDelay(600);
    }

    void MoveInLineOfSight(Unit *who)
    override {
        if (!who || who->GetTypeId() != TYPEID_PLAYER)
            return;

        if(who->GetTypeId() == TYPEID_PLAYER)
        {
            switch(me->GetAreaId())
            {
                case 199:                                   //felstone
                    if( (who->ToPlayer())->GetQuestStatus(5216) == QUEST_STATUS_INCOMPLETE ||
                        (who->ToPlayer())->GetQuestStatus(5229) == QUEST_STATUS_INCOMPLETE )
                    {
                        DoSpawnCreature(11075,0,0,0,me->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
                case 200:                                   //dalson
                    if( (who->ToPlayer())->GetQuestStatus(5219) == QUEST_STATUS_INCOMPLETE ||
                        (who->ToPlayer())->GetQuestStatus(5231) == QUEST_STATUS_INCOMPLETE )
                    {
                        DoSpawnCreature(11077,0,0,0,me->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
                case 201:                                   //gahrron
                    if( (who->ToPlayer())->GetQuestStatus(5225) == QUEST_STATUS_INCOMPLETE ||
                        (who->ToPlayer())->GetQuestStatus(5235) == QUEST_STATUS_INCOMPLETE )
                    {
                        DoSpawnCreature(11078,0,0,0,me->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
                case 202:                                   //writhing
                    if( (who->ToPlayer())->GetQuestStatus(5222) == QUEST_STATUS_INCOMPLETE ||
                        (who->ToPlayer())->GetQuestStatus(5233) == QUEST_STATUS_INCOMPLETE )
                    {
                        DoSpawnCreature(11076,0,0,0,me->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
            }
        }
    }
};
CreatureAI* GetAI_npc_the_scourge_cauldron(Creature *_Creature)
{
    return new npc_the_scourge_cauldronAI (_Creature);
}

/*##########
#npcs_andorhal_tower
##########*/

enum eAndorhalTower
{
    GO_BEACON_TORCH                             = 176093
};

struct npc_andorhal_towerAI : public ScriptedAI
{
    npc_andorhal_towerAI(Creature *c) : ScriptedAI(c) 
    {
        SetCombatMovementAllowed(false);
    }
    
    void EnterCombat(Unit *pWho) override {}

    void MoveInLineOfSight(Unit* pWho)
    override {
        if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (me->FindNearestGameObject(GO_BEACON_TORCH, 10.0f))
            (pWho)->ToPlayer()->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
        
    }
};

CreatureAI* GetAI_npc_andorhal_tower(Creature* pCreature)
{
    return new npc_andorhal_towerAI (pCreature);
}

/*######
## npc_myranda_the_hag
######*/

enum eMyranda
{
    QUEST_SUBTERFUGE        = 5862,
    QUEST_IN_DREAMS         = 5944,
    SPELL_SCARLET_ILLUSION  = 17961
};

#define GOSSIP_ITEM_ILLUSION    "I am ready for the illusion, Myranda."

bool GossipHello_npc_myranda_the_hag(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->IsQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_SUBTERFUGE) == QUEST_STATUS_COMPLETE &&
        !pPlayer->GetQuestRewardStatus(QUEST_IN_DREAMS) && !pPlayer->HasAuraEffect(SPELL_SCARLET_ILLUSION, 0))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ILLUSION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU_TEXTID(4773, pCreature->GetGUID());
        return true;
    }
    else
        SEND_PREPARED_GOSSIP_MENU(pPlayer, pCreature);

    return true;
}

bool GossipSelect_npc_myranda_the_hag(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_SCARLET_ILLUSION, false);
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_western_plaguelands()
{
    OLDScript *newscript;

    newscript = new OLDScript;
    newscript->Name="npcs_dithers_and_arbington";
    newscript->OnGossipHello = &GossipHello_npcs_dithers_and_arbington;
    newscript->OnGossipSelect = &GossipSelect_npcs_dithers_and_arbington;
    sScriptMgr->RegisterOLDScript(newscript);

    newscript = new OLDScript;
    newscript->Name="npc_the_scourge_cauldron";
    newscript->GetAI = &GetAI_npc_the_scourge_cauldron;
    sScriptMgr->RegisterOLDScript(newscript);
    
    newscript = new OLDScript;
    newscript->Name = "npc_andorhal_tower";
    newscript->GetAI = &GetAI_npc_andorhal_tower;
    sScriptMgr->RegisterOLDScript(newscript);
    
    newscript = new OLDScript;
    newscript->Name = "npc_myranda_the_hag";
    newscript->OnGossipHello = &GossipHello_npc_myranda_the_hag;
    newscript->OnGossipSelect = &GossipSelect_npc_myranda_the_hag;
    sScriptMgr->RegisterOLDScript(newscript);
}
