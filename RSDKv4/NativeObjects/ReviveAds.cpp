#include "RetroEngine.hpp"

void ReviveAds_Enter()
{
    if (isWaitingAds == true) {
        return;
    }

    bool adAvailable = true;

    if (adAvailable == true){
        isWaitingAds = true;
        stageMode = STAGEMODE_FROZEN;
        // objectEntityList[0].priority = PRIORITY_ALWAYS;
        CREATE_ENTITY(ReviveAds);
        return;
    }
    isWaitingAds = false;
    SetGlobalVariableByName("waitingAds.result", 2);
}

void ReviveAdsWater_Enter()
{
    if (isWaitingWaterAds == true) {
        return;
    }

    bool adAvailable = true;

    if (adAvailable == true){
        isWaitingWaterAds = true;
        stageMode = STAGEMODE_FROZEN;
        CREATE_ENTITY(ReviveAds);
        return;
    }
    isWaitingWaterAds = false;
    SetGlobalVariableByName("waitingAds.water", 2);
}

void ReviveAds_Create(void *objPtr)
{
    PrintLog("StageMode: %d", stageMode);
    RSDK_THIS(ReviveAds);
    self->dialogTitle = CREATE_ENTITY(DialogPanelTitle);
    self->dialogTitle->buttonCount = DLGTYPE_YESNO;
    self->dialogTitle->selection = NULL;
    SetStringToFont(self->dialogTitle->title, strGameOver, FONT_TEXT);
    SetStringToFont(self->dialogTitle->text, strReviveAds, FONT_TEXT);
}

void ReviveAds_Main(void *objPtr)
{
    RSDK_THIS(ReviveAds);
    if (self->dialogTitle->selection == DLG_NO){
        if (isWaitingAds == true){
            SetGlobalVariableByName("waitingAds.result", 2);
            isWaitingAds = false;
        }
        else if (isWaitingWaterAds == true){
            SetGlobalVariableByName("waitingAds.water", 2);
            isWaitingWaterAds = false;
        }
        Engine.gameMode = ENGINE_MAINGAME;
        stageMode = STAGEMODE_NORMAL;

        // self->dialogTitle->buttonCount = NULL;
        // RemoveNativeObject(self->dialogTitle);
        if (self->dialogTitle->state == DIALOGPANEL_STATE_EXIT && self->dialogTitle->stateTimer > 0.49)
            RemoveNativeObject(self);
    }
    else if (self->dialogTitle->selection == DLG_YES){
        if (isWaitingAds == true){
            SetGlobalVariableByName("waitingAds.result", 1);
            isWaitingAds = false;
        }
        else if (isWaitingWaterAds == true){
            SetGlobalVariableByName("waitingAds.water", 1);
            isWaitingWaterAds = false;
        }
        Engine.gameMode = ENGINE_MAINGAME;
        stageMode = STAGEMODE_NORMAL;

        // self->dialogTitle->buttonCount = NULL;
        // RemoveNativeObject(self->dialogTitle);
        if (self->dialogTitle->state == DIALOGPANEL_STATE_EXIT && self->dialogTitle->stateTimer > 0.5)
            RemoveNativeObject(self);
        
    }
}