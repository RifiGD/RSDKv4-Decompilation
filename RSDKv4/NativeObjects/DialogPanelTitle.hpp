#ifndef NATIVE_DIALOGPANELTITLE_H
#define NATIVE_DIALOGPANELTITLE_H

struct NativeEntity_DialogPanelTitle : NativeEntityBase {
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
    ushort text[128];
    float textX;
    float textY;
    float scale;
    ushort title[128];
    float titleX;
    float titleY;
    float titleScale;
};

void DialogPanelTitle_Create(void *objPtr);
void DialogPanelTitle_Main(void *objPtr);

#endif