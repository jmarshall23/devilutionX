// datatable.cpp
//

#include "datatable.h"
#include "engine/load_file.hpp"

DataTable *townerDataTable = nullptr;
DataTable *cowTable = nullptr;
DataTable *monsterTable = nullptr;

namespace devilution {
	void InitMonsterTable(void);
}

void DiabloLoadTables(void)
{
	townerDataTable = new DataTable("excel\\towners.txt");
	cowTable = new DataTable("excel\\cows.txt");
	monsterTable = new DataTable("excel\\monsters.txt");

	devilution::InitMonsterTable();
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

	while (index < bufferSize) {

		if (buffer[index] == ',')
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
