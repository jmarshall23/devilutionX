// datatable.cpp
//

#include "datatable.h"
#include "engine/load_file.hpp"

DataTable *townerDataTable = nullptr;
DataTable *cowTable = nullptr;
DataTable *monsterTable = nullptr;
DataTable *spellTable = nullptr;
DataTable *playerTable = nullptr;
DataTable *missileTable = nullptr;
DataTable *sfxTable = nullptr;
DataTable *musicTable = nullptr;
DataTable *difficultyTable = nullptr;
DataTable *experienceTable = nullptr;
DataTable *heroSpeechTable = nullptr;
DataTable *messagesTable = nullptr;
DataTable *itemsTable = nullptr;
DataTable *textTable = nullptr;
DataTable *DunMonTable = nullptr;
DataTable *uniqueMonsterTable = nullptr;
DataTable *objectTable = nullptr;
DataTable *uniqueItemTable = nullptr;
DataTable *itemPrefixTable = nullptr;
DataTable *itemSuffixTable = nullptr;
DataTable *lvlSetPiecesTables = nullptr;
DataTable *shrineTable = nullptr;
DataTable *cutsceneTable = nullptr;
DataTable *invBeltTable = nullptr;
DataTable *invTable = nullptr;
DataTable *objMapTable = nullptr;
DataTable *warpTownTable = nullptr;

namespace devilution {
	void InitMonsterTable(void);
	void InitSpellDataTable(void);
    void InitMissileDataTable(void);
    void InitSfxDataTable(void);
    void InitItemTable(void);
    void InitPlayerExperienceTable(void);
    void InitTextDataStruct(void);
    void InitObjectDefinitionTable(void);
    void InitShrineTable(void);
	void InitInventoryBeltTable(void);
    void InitInventoryTable(void);
    void InitObjMapTable(void);
}

void DiabloLoadTables(void)
{
	townerDataTable = new DataTable("excel\\towners.txt");
	cowTable = new DataTable("excel\\cows.txt");
	monsterTable = new DataTable("excel\\monsters.txt");
	spellTable = new DataTable("excel\\spells.txt");
	playerTable = new DataTable("excel\\player.txt");
	missileTable = new DataTable("excel\\missile.txt");
	sfxTable = new DataTable("excel\\sfx.txt");
	musicTable = new DataTable("excel\\music.txt");
	difficultyTable = new DataTable("excel\\difficulty.txt");
	experienceTable = new DataTable("excel\\experience.txt");
	heroSpeechTable = new DataTable("excel\\heroSpeech.txt");
	messagesTable = new DataTable("excel\\messages.txt");
	itemsTable = new DataTable("excel\\items.txt");
	textTable = new DataTable("excel\\text.txt");
	DunMonTable = new DataTable("excel\\dunmontbl.txt");
	uniqueMonsterTable = new DataTable("excel\\uniquemon.txt");
	objectTable = new DataTable("excel\\objects.txt");
	uniqueItemTable = new DataTable("excel\\uniqueitems.txt");
	itemPrefixTable = new DataTable("excel\\itemprefix.txt");
	itemSuffixTable = new DataTable("excel\\itemsuffix.txt");
	lvlSetPiecesTables = new DataTable("excel\\lvlsetpieces.txt");
	shrineTable = new DataTable("excel\\shrines.txt");
	cutsceneTable = new DataTable("excel\\cutscenes.txt");
	invBeltTable = new DataTable("excel\\invbelt.txt");
	invTable = new DataTable("excel\\inv.txt");
	objMapTable = new DataTable("excel\\objmap.txt");
	warpTownTable = new DataTable("excel\\warptown.txt");

	devilution::InitMonsterTable();
	devilution::InitSpellDataTable();
	devilution::InitMissileDataTable();
	devilution::InitSfxDataTable();
	devilution::InitPlayerExperienceTable();
	devilution::InitItemTable();
	devilution::InitTextDataStruct();
	devilution::InitObjectDefinitionTable();
	devilution::InitShrineTable();
	devilution::InitInventoryBeltTable();
	devilution::InitInventoryTable();
	devilution::InitObjMapTable();
}

/*
===============
DataTable::DataTable
===============
*/
DataTable::DataTable(const char *path)
{
	int len = devilution::GetFileSize(path);
	if (len <= 0) {
		devilution::app_fatal("Failed to load DataTable %s", path);
	}

	std::unique_ptr<uint8_t[]> buffer = devilution::LoadFileInMem<uint8_t>(path);

	int index = 0;
	int row = 0;
	int id = 0;
	//
	while (index < len) {
		if (buffer[index] == '\n') {
			if (id != columns.size() && row > 0) {
				devilution::app_fatal("Failed to load DataTable %s - entries mismatch on row %d", path, row);
			}
			row++;			
			id = 0;
			index++;
			continue;
		}

		if (buffer[index] == '\r' || buffer[index] == ',') {
			index++;
			continue;
		}

		if (row == 0) {
			DataColumn col;
			col.name = GrabToken((const char *)buffer.get(), index, len);
			columns.push_back(col);
		} else {
			std::string tok = GrabToken((const char *)buffer.get(), index, len);		
			columns[id++].rows.push_back(tok);
		}
	}
}

/*
===============
DataTable::GrabToken
===============
*/
char *DataTable::GrabToken(const char *buffer, int &index, const int bufferSize)
{
	static char token[1024];
	int idx = 0;

	memset(token, 0, sizeof(token));
	bool inQuote = false;
	while (index < bufferSize) {
		if (buffer[index] == '"') {
			inQuote = !inQuote;
			index++;
			continue;
		}

		if (buffer[index] == ',' && !inQuote)
			break;

		if (buffer[index] == '\n')
			break;

		if (buffer[index] == '\r')
			break;

		token[idx++] = buffer[index++];
	}

	return token;
}

/*
===============
DataTable::FindColumn
===============
*/
const DataColumn *DataTable::FindColumn(const char *name) {
	for (int i = 0; i < columns.size(); i++) {
		if (columns[i].name == name) {
			return &columns[i];
		}
	}

	return nullptr;
}
