#include "RetroEngine.hpp"

void ShutdownDialogPanel_Create(void *objPtr)
{
    RSDK_THIS(ShutdownDialogPanel);
    self->panelMesh = LoadMesh("Data/Game/Models/Panel.bin", -1);
    SetMeshVertexColors(self->panelMesh, 0x28, 0x5C, 0xB0, 0xFF);
    self->buttonCount = DLGTYPE_YESNO;
}

void ShutdownDialogPanel_Main(void *objPtr)
{
    RSDK_THIS(ShutdownDialogPanel);
    NewRenderState();
    SetRenderBlendMode(RENDER_BLEND_ALPHA);

    switch (self->state) {
        case DIALOGPANEL_STATE_SETUP: {
            self->moreInfoButton = CREATE_ENTITY(PushButton);
            self->buttons[0]     = self->moreInfoButton;
            // self->moreInfoButton->x = -50.0f;
            // self->moreInfoButton->y = -50.0f;
            self->moreInfoButton->scale          = 0.25f;
            self->moreInfoButton->useRenderMatrix = false;
            // self->moreInfoButton->bgColor        = 0x006030;
            self->moreInfoButton->bgColor         = 0x154C79; // dark blue
            self->moreInfoButton->bgColorSelected = 0x1E81B0; // lighter blue
            SetStringToFont(self->moreInfoButton->text, strMoreInfo, FONT_LABEL);

            self->okButton   = CREATE_ENTITY(PushButton);
            self->buttons[1] = self->okButton;
            // self->okButton->x = 60.0f;
            // self->okButton->y = -50.0f;
            self->okButton->scale          = 0.25f;
            self->okButton->useRenderMatrix = false;
            // self->okButton->bgColor        = 0x00A048;
            self->okButton->bgColor         = 0x00A048;
            self->okButton->bgColorSelected = 0xC06000;

            SetStringToFont(self->okButton->text, strOK, FONT_LABEL);

            self->moreInfoButton->x = -90.0f;
            self->okButton->x = 110.0f;
            
            self->moreInfoButton->y = -100.0f;
            self->okButton->y = -100.0f;

            self->scale = 224.0 / (GetTextWidth(self->text, FONT_TEXT, 1.0) + 1.0);
            self->titleScale = 224.0 / (GetTextWidth(self->title, FONT_TEXT, 1.0) + 1.0);
            self->titleScale -= 0.3;

            if (self->scale > 0.4)
                self->scale = 0.4;
            self->textX = GetTextWidth(self->text, FONT_TEXT, self->scale) * -0.5;
            self->textY = GetTextHeight(self->text, FONT_TEXT, self->scale) * 0.5;
            self->textY -= 10.0;

            self->titleX = GetTextWidth(self->title, FONT_TEXT, self->titleScale) * -0.5;
            self->titleY = GetTextHeight(self->title, FONT_TEXT, self->titleScale) * 0.5;
            self->titleY += 30.0;

            self->state = DIALOGPANEL_STATE_ENTER;
        }
        // FallThrough
        case DIALOGPANEL_STATE_ENTER: {
            self->buttonScale += ((0.77 - self->buttonScale) / ((Engine.deltaTime * 60.0) * 8.0));
            if (self->buttonScale > 0.75)
                self->buttonScale = 0.75;
            NewRenderState();
            MatrixScaleXYZF(&self->buttonMatrix, self->buttonScale, self->buttonScale, 1.0);
            MatrixTranslateXYZF(&self->buttonMult, 0.0, 0.0, 160.0);
            MatrixMultiplyF(&self->buttonMatrix, &self->buttonMult);
            SetRenderMatrix(&self->buttonMatrix);
            for (int i = 0; i < self->buttonCount; ++i) memcpy(&self->buttons[i]->renderMatrix, &self->buttonMatrix, sizeof(MatrixF));

            self->stateTimer += Engine.deltaTime;
            if (self->stateTimer > 0.5) {
                self->state      = DIALOGPANEL_STATE_MAIN;
                self->stateTimer = 0.0;
            }
            break;
        }
        case DIALOGPANEL_STATE_MAIN: {
            CheckKeyDown(&keyDown);
            CheckKeyPress(&keyPress);
            SetRenderMatrix(&self->buttonMatrix);
            if (!usePhysicalControls) {
                if (touches < 1) {
                    if (self->buttons[0]->state == 1) {
                        self->buttonSelected = 0;
                        self->state          = DIALOGPANEL_STATE_ACTION;
                        PlaySfxByName("Menu Select", false);
                        self->buttons[0]->state = PUSHBUTTON_STATE_FLASHING;
                    }
                    if (self->buttonCount == DLGTYPE_YESNO && self->buttons[1]->state == 1) {
                        self->buttonSelected = 1;
                        self->state          = DIALOGPANEL_STATE_ACTION;
                        PlaySfxByName("Menu Select", false);
                        self->buttons[1]->state = PUSHBUTTON_STATE_FLASHING;
                    }
                }
                else {
                    if (self->buttonCount == DLGTYPE_OK) {
                        self->buttons[0]->state =
                            CheckTouchRect(0.0, -30.0, (self->buttons[0]->textWidth + (self->buttons[0]->scale * 64.0)) * 0.75, 12.0) >= 0;
                    }
                    else {
                        self->buttons[0]->state =
                            CheckTouchRect(-36.0, -30.0, (self->buttons[0]->textWidth + (self->buttons[0]->scale * 64.0)) * 0.75, 12.0) >= 0;
                        self->buttons[1]->state =
                            CheckTouchRect(36.0, -30.0, (self->buttons[1]->textWidth + (self->buttons[1]->scale * 64.0)) * 0.75, 12.0) >= 0;
                    }
                }
                if (keyDown.left) {
                    usePhysicalControls  = true;
                    self->buttonSelected = 1;
                }
                else if (keyDown.right) {
                    usePhysicalControls  = true;
                    self->buttonSelected = 0;
                }
            }
            else if (touches > 0) {
                usePhysicalControls = false;
            }
            else if (self->buttonCount == DLGTYPE_OK) {
                self->buttonSelected = 0;
                if (keyPress.start || keyPress.A) {
                    self->state = DIALOGPANEL_STATE_ACTION;
                    PlaySfxByName("Menu Select", false);
                    self->buttons[self->buttonSelected]->state = 2;
                }
            }
            else {
                if (keyPress.left) {
                    PlaySfxByName("Menu Move", false);
                    if (--self->buttonSelected < 0)
                        self->buttonSelected = 1;
                }
                if (keyPress.right) {
                    PlaySfxByName("Menu Move", false);
                    if (++self->buttonSelected > 1)
                        self->buttonSelected = 0;
                }
                self->buttons[0]->state                    = 0;
                self->buttons[1]->state                    = 0;
                self->buttons[self->buttonSelected]->state = 1;

                if (keyPress.start || keyPress.A) {
                    self->state = DIALOGPANEL_STATE_ACTION;
                    PlaySfxByName("Menu Select", false);
                    self->buttons[self->buttonSelected]->state = 2;
                }
            }
            if (self->state == 2 && keyPress.B) {
                PlaySfxByName("Menu Back", false);
                self->selection = DLG_NO;
                self->state     = DIALOGPANEL_STATE_EXIT;
            }
            break;
        }
        case DIALOGPANEL_STATE_ACTION:
            SetRenderMatrix(&self->buttonMatrix);
            if (!self->buttons[self->buttonSelected]->state) {
                self->state = DIALOGPANEL_STATE_EXIT;

                self->selection = self->buttonSelected + 1;
                if (self->buttonCount == DLGTYPE_OK)
                    self->selection = DLG_OK;
            }
            break;
        case DIALOGPANEL_STATE_EXIT:
            self->buttonScale = self->buttonScale + ((((self->stateTimer < 0.2) ? 1 : -1) - self->buttonScale) / ((Engine.deltaTime * 60.0) * 8.0));
            if (self->buttonScale < 0.0)
                self->buttonScale = 0.0;
            NewRenderState();
            MatrixScaleXYZF(&self->buttonMatrix, self->buttonScale, self->buttonScale, 1.0);
            MatrixTranslateXYZF(&self->buttonMult, 0.0, 0.0, 160.0);
            MatrixMultiplyF(&self->buttonMatrix, &self->buttonMult);
            SetRenderMatrix(&self->buttonMatrix);
            for (int i = 0; i < self->buttonCount; ++i) memcpy(&self->buttons[i]->renderMatrix, &self->buttonMatrix, sizeof(MatrixF));

            self->stateTimer += Engine.deltaTime;
            if (self->stateTimer > 0.5) {
                for (int i = 0; i < self->buttonCount; ++i) RemoveNativeObject(self->buttons[i]);
                RemoveNativeObject(self);
                return;
            }
            break;
        case DIALOGPANEL_STATE_IDLE: SetRenderMatrix(&self->buttonMatrix); break;
        default: break;
    }
    RenderMesh(self->panelMesh, MESH_COLORS, false);
    RenderText(self->title, FONT_TEXT, self->titleX, self->titleY, 0.0, self->titleScale, 255);
    RenderText(self->text, FONT_TEXT, self->textX, self->textY, 0.0, self->scale, 255);
}