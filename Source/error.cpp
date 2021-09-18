/**
 * @file error.cpp
 *
 * Implementation of in-game message functions.
 */

#include <deque>

#include "error.h"

#include "control.h"
#include "engine/render/text_render.hpp"
#include "stores.h"
#include "utils/language.h"

#include "datatable.h"

#include "../rhi/image.h"

namespace devilution {

namespace {

std::deque<std::string> DiabloMessages;
std::vector<std::string> TextLines;
uint32_t msgdelay;
int ErrorWindowHeight = 54;
const int LineHeight = 12;
const int LineWidth = 418;

void InitNextLines()
{
	msgdelay = SDL_GetTicks();
	auto message = DiabloMessages.front();

	TextLines.clear();

	char tempstr[1536]; // Longest test is about 768 chars * 2 for unicode
	strcpy(tempstr, message.data());

	WordWrapGameString(tempstr, LineWidth, GameFontSmall, 1);
	const string_view paragraphs = tempstr;

	size_t previous = 0;
	while (true) {
		size_t next = paragraphs.find('\n', previous);
		TextLines.emplace_back(paragraphs.substr(previous, next));
		if (next == std::string::npos)
			break;
		previous = next + 1;
	}

	ErrorWindowHeight = std::max(54, static_cast<int>((TextLines.size() * LineHeight) + 42));
}

} // namespace



void InitDiabloMsg(diablo_message e)
{
	InitDiabloMsg(messagesTable->GetValue("text", (int)e));
}

void InitDiabloMsg(std::string msg)
{
	if (DiabloMessages.size() >= MAX_SEND_STR_LEN)
		return;

	if (std::find(DiabloMessages.begin(), DiabloMessages.end(), msg) != DiabloMessages.end())
		return;

	DiabloMessages.push_back(msg);
	if (DiabloMessages.size() == 1)
		InitNextLines();
}

bool IsDiabloMsgAvailable()
{
	return !DiabloMessages.empty();
}

void CancelCurrentDiabloMsg()
{
	msgdelay = 0;
}

void ClrDiabloMsg()
{
	DiabloMessages.clear();
}

void DrawDiabloMsg(const Surface &out)
{
	int dialogStartY = ((gnScreenHeight - PANEL_HEIGHT) / 2) - (ErrorWindowHeight / 2) + 9;

	pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 101, dialogStartY, 1);
	pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 527, dialogStartY, 4);
	pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 101, dialogStartY + ErrorWindowHeight - 6, 2);
	pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 527, dialogStartY + ErrorWindowHeight - 6, 3);

	//CelDrawTo(out, { PANEL_X + 101, dialogStartY }, *pSTextSlidCels, 1);
	//CelDrawTo(out, { PANEL_X + 527, dialogStartY }, *pSTextSlidCels, 4);
	//CelDrawTo(out, { PANEL_X + 101, dialogStartY + ErrorWindowHeight - 6 }, *pSTextSlidCels, 2);
	//CelDrawTo(out, { PANEL_X + 527, dialogStartY + ErrorWindowHeight - 6 }, *pSTextSlidCels, 3);

	int sx = PANEL_X + 109;
	for (int i = 0; i < 35; i++) {
		pSTextSlidCels->ClipRenderNoLighting(out, sx, dialogStartY, 5);
		pSTextSlidCels->ClipRenderNoLighting(out, sx, dialogStartY + ErrorWindowHeight - 6, 7);

		//CelDrawTo(out, { sx, dialogStartY }, *pSTextSlidCels, 5);
		//CelDrawTo(out, { sx, dialogStartY + ErrorWindowHeight - 6 }, *pSTextSlidCels, 7);
		sx += 12;
	}
	int drawnYborder = 12;
	while ((drawnYborder + 12) < ErrorWindowHeight) {
		//CelDrawTo(out, {  }, *pSTextSlidCels, 6);
		//CelDrawTo(out, { }, *pSTextSlidCels, 8);

		pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 101, dialogStartY + drawnYborder, 6);
		pSTextSlidCels->ClipRenderNoLighting(out, PANEL_X + 527, dialogStartY + drawnYborder, 8);
		drawnYborder += 12;
	}

	DrawHalfTransparentRectTo(out, PANEL_X + 104, dialogStartY - 8, 432, ErrorWindowHeight);

	auto message = DiabloMessages.front();
	int lineNumber = 0;
	for (auto &line : TextLines) {
		DrawString(out, line.c_str(), { { PANEL_X + 109, dialogStartY + 24 + lineNumber * LineHeight }, { LineWidth, LineHeight } }, UiFlags::AlignCenter, 1, LineHeight);
		lineNumber += 1;
	}

	if (msgdelay > 0 && msgdelay <= SDL_GetTicks() - 3500) {
		msgdelay = 0;
	}
	if (msgdelay == 0) {
		DiabloMessages.pop_front();
		if (!DiabloMessages.empty())
			InitNextLines();
	}
}

} // namespace devilution
