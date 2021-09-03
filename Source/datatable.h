// datatable.h
//

#include <string>
#include <vector>

class DataColumn {
	friend class DataTable;
public:
	const char *GetValue(int row) const
	{
		return rows[row].c_str();
	}

	const int GetInt(int row) const
	{
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

private:
	char *GrabToken(const char *buffer, int &index, const int bufferSize);
	std::vector<DataColumn> columns;
};

extern DataTable *townerDataTable;
extern DataTable *cowTable;

void DiabloLoadTables(void);
