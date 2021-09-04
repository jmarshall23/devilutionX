// datatable.h
//

#include <string>
#include <vector>

class DataColumn {
	friend class DataTable;
public:
	const char *GetValue(int row) const
	{
		if (rows[row] == "nullptr")
			return nullptr;

		return rows[row].c_str();
	}

	const int GetInt(int row) const
	{
		if (rows[row] == "TRUE" || rows[row] == "true")
			return 1;

		if (rows[row] == "FALSE" || rows[row] == "false")
			return 0;

		return atoi(rows[row].c_str());
	}

	std::string name;
	std::vector<std::string> rows;
};

class DataTable {
public:
	DataTable(const char *path);

	const DataColumn *FindColumn(const char *name);

	const char *GetValue(const char *rowName, int index)
	{
		return FindColumn(rowName)->GetValue(index);
	}
	int GetInt(const char *rowName, int index)
	{
		return FindColumn(rowName)->GetInt(index);
	}

	int NumRows() const
	{
		return columns[0].rows.size();
	}

private:
	char *GrabToken(const char *buffer, int &index, const int bufferSize);
	std::vector<DataColumn> columns;
};

extern DataTable *townerDataTable;
extern DataTable *cowTable;
extern DataTable *monsterTable;
extern DataTable *spellTable;
extern DataTable *playerTable;
extern DataTable *missileTable;
extern DataTable *sfxTable;
extern DataTable *musicTable;
extern DataTable *difficultyTable;
extern DataTable *experienceTable;
extern DataTable *heroSpeechTable;
extern DataTable *messagesTable;
extern DataTable *itemsTable;
extern DataTable *textTable;
extern DataTable *DunMonTable;

void DiabloLoadTables(void);

__forceinline std::vector<std::string> split(const std::string &s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos) {
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}
