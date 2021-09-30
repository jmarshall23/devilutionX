using System;
using System.IO;
using System.Collections.Generic;

public class DataTable
{
	public readonly string fileName;

	class DataRow
	{
		public DataRow(string line)
		{
			tokens = line.Split(',');
		}

		public string[] tokens;
	}

	private List<DataRow> rows = new List<DataRow>();

	public DataTable(string filename)
	{
		string[] lines = File.ReadAllLines(filename);
		this.fileName = filename;

		foreach (string line in lines)
		{
			DataRow row = new DataRow(line);
			rows.Add(row);
		}
	}

	public int NumRows
	{
		get
		{
			return rows.Count - 1; 
		}
	}

	public string GetToken(string rowName, int row)
	{
		for(int i = 0; i < rows[0].tokens.Length; i++)
		{
			if(rows[0].tokens[i] == rowName)
			{
				return rows[row + 1].tokens[i];
			}
		}
		throw new Exception("Invalid row name!");
	}

	public int GetTokenInt(string rowName, int row)
	{
		return int.Parse(GetToken(rowName, row));
	}
}
