#include "eop3dObjectsHelpers.h"
#include "plugData.h"
void* eop3dObjectsHelpers::MakeTextFont(const char* fontName)
{
    return (*(*plugData::data.funcs.MakeTextFont))(fontName);
}

void eop3dObjectsHelpers::DeleteTextFont(void* fontID)
{
    (*(*plugData::data.funcs.DeleteTextFont))(fontID);
}

Text3DDrawable* eop3dObjectsHelpers::MakeText(void* fontID, const char* utf8Text)
{
    return (*(*plugData::data.funcs.MakeText))(fontID, utf8Text);
}

void eop3dObjectsHelpers::ScaleText(Text3DDrawable* text, float scale)
{
    (*(*plugData::data.funcs.ScaleText))(text, scale);
}

void eop3dObjectsHelpers::ChangeColorText(Text3DDrawable* text, int a, int r, int g, int b)
{
    (*(*plugData::data.funcs.ChangeTextColor))(text, a,r,g,b);
}

void eop3dObjectsHelpers::SetCoordsText(Text3DDrawable* text, float x, float y, float z)
{
    (*(*plugData::data.funcs.SetTextDrawingCoords))(text, x,y,z);
}

void eop3dObjectsHelpers::StartDrawingText(Text3DDrawable* text)
{
    (*(*plugData::data.funcs.StartDrawingText))(text);
}

void eop3dObjectsHelpers::StopDrawingText(Text3DDrawable* text)
{
    (*(*plugData::data.funcs.StopDrawingText))(text);
}

void eop3dObjectsHelpers::DrawOnceText(Text3DDrawable* text)
{
    (*(*plugData::data.funcs.DrawingTextOnce))(text);
}
