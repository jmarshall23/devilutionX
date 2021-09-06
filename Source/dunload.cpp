#include "datatable.h"
#include "dunload.h"
#include "engine/load_file.hpp"
#include <memory>

std::unique_ptr<uint16_t[]> LoadLevelSetPiece(DungeonLevelId levelId)
{
	const char *dungeonPath = lvlSetPiecesTables->GetValue("path", levelId);
	//
	//DataTable *table = new DataTable(dungeonPath);
	//
	//std::unique_ptr<uint16_t[]> buf { new uint16_t[table->NumRows()] };
	//
	//uint16_t *rawbuf = &buf[0];
	//
	//*rawbuf = table->GetInt("x", table->NumRows() - 1) + 1;
	//rawbuf++;
	//*rawbuf = table->GetInt("y", table->NumRows() - 1) + 1;
	//rawbuf++;
	//
	//for (int i = 0; i < table->NumRows(); i++) {
	//	*rawbuf++ = table->GetInt("tilid", i) - 1;
	//}
	//
	//
	//delete table;

	return devilution::LoadFileInMem<uint16_t>(dungeonPath);
}
