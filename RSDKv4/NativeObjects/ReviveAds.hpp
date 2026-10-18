#ifndef NATIVE_REVIVEADS_H
#define NATIVE_REVIVEADS_H

struct NativeEntity_ReviveAds : NativeEntityBase {
    NativeEntity_DialogPanelTitle *dialogTitle;
};

void ReviveAds_Enter();
void ReviveAdsWater_Enter();

void ReviveAds_Create(void *objPtr);
void ReviveAds_Main(void *objPtr);

#endif