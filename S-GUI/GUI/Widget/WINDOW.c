#include "WINDOW.h"

/* Window�Ի溯�� */
static void _WINDOW_Paint(WM_hWin hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    GUI_COLOR Color;
    GUI_RECT Rect;
    WINDOW_Obj *pObj = hWin;

    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    
    /* ������� */
    Color = pObj->Widget.Skin.EdgeColor[0];
    GUI_DrawTailorRect(x0, y0, xSize, ySize, Color);
    /* �����п� */
    Color = pObj->Widget.Skin.EdgeColor[1];
    GUI_VertTailorLine(x0 + 1, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_VertTailorLine(x0 + xSize - 2, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_HoriTailorLine(x0 + 1, y0  + ySize - 2, xSize - 2, Color);
    /* �����ڿ� */
    Color = pObj->Widget.Skin.EdgeColor[2];
    GUI_DrawTailorRect(x0 + 2, y0 + pObj->CaptionHeight + 1,
                 xSize - 4, ySize - pObj->CaptionHeight - 3, 
                 Color);

    /* ���Ʊ����� */
    Color = pObj->Widget.Skin.CaptionColor[0];
    GUI_FillTailorRect(x0 + 1, y0 + 1, xSize - 2,
                 pObj->CaptionHeight / 2, Color);
    Color = pObj->Widget.Skin.CaptionColor[1];
    GUI_FillTailorRect(x0 + 1, y0 + pObj->CaptionHeight / 2 + 1,
                 xSize - 2, pObj->CaptionHeight / 2, Color);

    /* �����ڲ����� */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillTailorRect(x0 + 3, y0 + pObj->CaptionHeight + 2,
                 xSize - 6, ySize - pObj->CaptionHeight - 5, Color);
    /* ���Ʊ��� */
    Color = pObj->Widget.Skin.FontColor[0];
    GUI_DspStringCurRectMiddle(x0 + 1, y0 + 1, xSize - 2,
                               pObj->CaptionHeight,
                               pObj->Title, Color, 
                               WIDGET_GetFont(pObj));
}

/* WINDOW�ؼ��Զ��ص����� */
static void _WINDOW_Callback(WM_MESSAGE *pMsg)
{
    i_16 dX, dY;
    
    /* ����Ƿ�ΪWINDOW�ؼ� */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_WINDOW)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            _WINDOW_Paint(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            WM_SetActiveMainWindow(pMsg->hWin);
            break;
        case WM_TP_PRESS:
            dX = GUI_GetTouchPad_MoveX();
            dY = GUI_GetTouchPad_MoveY();
            WM_MoveWindow(pMsg->hWin, dX, dY);
            break;
        case WM_TP_LEAVE:
            dX = GUI_GetTouchPad_MoveX();
            dY = GUI_GetTouchPad_MoveY();
            WM_MoveWindow(pMsg->hWin, dX, dY);
            break;
    }
    /* ִ���û��ص����� */
    ((WINDOW_Obj*)pMsg->hWin)->UserCb(pMsg);
}

/*
 *�������ڿؼ�
 *x0:WINDOW�ؼ�����������(����ڸ�����)
 *y0:WINDOW�ؼ�����������(����ڸ�����)
 *xSize:WINDOW�ؼ���ˮƽ����
 *ySize:WINDOW�ؼ�����ֱ�߶�
 *hParent:�����ھ��
 *Id:����ID
 *Flag:����״̬
 *cb:�û��ص�����ָ��
 */
WM_hWin WINDOW_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag,
                      WM_CALLBACK *cb)
{
    WINDOW_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_WINDOW, Id, _WINDOW_Callback,
                                  sizeof(WINDOW_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    pObj->CaptionHeight = WINDOW_DEF_CAPHEIGHT;  //�������߶�
    /* �����û��� */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0 + 3;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0 +
                                   pObj->CaptionHeight + 2;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x1 - 3;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y1 - 3;
    /* ��ɫ */
    pObj->Widget.Skin.CaptionColor[0] = WINDOW_CAPTION_UPC;   //�������ϰ벿��
    pObj->Widget.Skin.CaptionColor[1] = WINDOW_CAPTION_DOWNC; //�������°벿��
    pObj->Widget.Skin.EdgeColor[0] = STD_WIN_RIM_OUTC;  //����
    pObj->Widget.Skin.EdgeColor[1] = STD_WIN_RIM_MIDC;  //����
    pObj->Widget.Skin.EdgeColor[2] = STD_WIN_RIM_INC;   //����
    pObj->Widget.Skin.BackColor[0] = WINDOW_BODY_BKC;   //��ɫ
    pObj->Widget.Skin.FontColor[0] = WINDOW_TITLE_COLOR;
    pObj->Widget.Skin.FontColor[1] = WINDOW_FONT_COLOR;
    pObj->UserCb = cb;
    WINDOW_SetTitle(pObj, "");      //���ó�ʼ�ַ���
    WINDOW_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

/* WINDOW���ñ��� */
u_8 WINDOW_SetTitle(WM_hWin hWin, const char *str)
{
    /* ����Ƿ�ΪWINDOW�ؼ� */
    if (WM_CheckWindowSign(hWin, WIDGET_WINDOW)) {
        return 1;
    }
    ((WINDOW_Obj*)hWin)->Title = (char*)str;
    return 0;
}

/* WINDOW�������� */
u_8 WINDOW_SetFont(WM_hWin hWin, GUI_FontType Font)
{
    /* ����Ƿ�ΪWINDOW�ؼ� */
    if (WM_CheckWindowSign(hWin, WIDGET_WINDOW)) {
        return 1;
    }
    WIDGET_SetFont(hWin, Font);
    return 0;
}

/* WINDOW����Ϊ͸������ */
void WINDOW_SetAllAlpha(WM_hWin hWin, u_8 Alpha)
{
    GUI_RECT Rect;
    
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    Rect = WM_GetWindowAreaRect(hWin);
    hWin = WM_GetParentHandle(hWin);
    WM_InvalidateRect(hWin, &Rect);  //��������ʧЧ
}