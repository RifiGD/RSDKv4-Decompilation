#ifndef NATIVE_SHUTDOWNDIALOGPANEL_H
#define NATIVE_SHUTDOWNDIALOGPANEL_H

struct NativeEntity_ShutdownDialogPanel : NativeEntityBase {
    DialogPanelStates state;
    int buttonCount;
    float stateTimer;
    int unused;
    float buttonScale;
    MeshInfo *panelMesh;
    MatrixF buttonMatrix;
    MatrixF buttonMult;
    NativeEntity_PushButton *buttons[2];
    int buttonSelected;
    int selection;
    ushort text[256];
    float textX;
    float textY;
    float scale;
    ushort title[256];
    float titleX;
    float titleY;
    float titleScale;
    NativeEntity_PushButton *moreInfoButton;
    NativeEntity_PushButton *okButton;
};

void ShutdownDialogPanel_Create(void *objPtr);
void ShutdownDialogPanel_Main(void *objPtr);
#endif