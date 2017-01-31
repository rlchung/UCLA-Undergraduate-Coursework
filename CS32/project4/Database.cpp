#include "Database.h"
#include "Tokenizer.h"
#include "http.h"
#include <iostream>
#include <fstream>

using namespace std;

Database::Database()
{
	rowCount = 0;
}

Database::~Database()
{
	while (!m_fieldIndex.empty())
	{
		delete m_fieldIndex[m_fieldIndex.size()-1];
		m_fieldIndex.pop_back();
	}
}

bool Database::specifySchema(const std::vector < FieldDescriptor>& schema)
{
	//RESET THE DATA BASE AND ALL PRIVATE MEMBERS
	m_schema.clear();
	m_rows.clear();
	m_fieldIndex.clear();

	//Create new schema

	for (unsigned int i = 0; i < schema.size(); i++)
		m_schema.push_back(schema[i]);
	
	//For each index field in the schema, initialize a MultiMap inside m_fieldIndex
	for (unsigned int j = 0; j < m_schema.size(); j++)
	{
		if (m_schema[j].index == it_indexed)
		{
			MultiMap* m1 = new MultiMap();
			m_fieldIndex.push_back(m1);
		}
	}
	
	if (m_fieldIndex.empty())
	{
		m_schema.clear();
		return false;
	}
	return true;
}

bool Database::addRow(const std::vector<std::string> & rowOfData)
{
	//When adding to the multimap, use the m_schema vector along with the rowOfData to indicate whether or not it is indexed
	//for (int whatever)
	//{ if (m_schema[i].index == it_indexed)
	//    add rowOfData[i] to multimap
	//

	if (m_schema.empty())//If the user did not specify a given schema
		return false;
	if (rowOfData.size() != m_schema.size())//if the inputted Row of values does not equal the number of values specified in the Schema
		return false;

	m_rows.push_back(rowOfData);//insert rowOfData into the last spot in the m_rows vector

	int posOfRow = m_rows.size() - 1;//position of the new row that will be mapped in the MultiMap

	for (unsigned int i = 0; i < rowOfData.size(); i++)//For each category in the rowOfData vector
	{
		if (m_schema[i].index == it_indexed)//Because m_schema has the same category of elements as rowOfData, if the given position of m_schema is indexed, add indexed value to
			//the proper MultiMap in m_fieldIndex, using the string in rowOfData position and it's position in m_Rows
			m_fieldIndex[i]->insert(rowOfData[i], posOfRow);
	}

	rowCount++;
	return true;
}

bool Database::loadFromURL(std::string url)
{
	string page;

	if (HTTP().get(url, page))//Downloads the webpage into page
		;//DO NOTHING
	else
	{
		cout << "Error fetching content from URL " << url << endl;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	///////////////////READING EACH LINE OF THE WEBPAGE INTO A VECTOR//////////////////
	//////////////////////////////////////////////////////////////////////////////

	vector<string> i_file;
	string read_line;
	string web_delimiter("\n");
	Tokenizer web_line(page, web_delimiter);

	while (web_line.getNextToken(read_line))//read each line until the end of the file
		i_file.push_back(read_line);

	////////////////////////////////////////////////////////
	//TEST THE FIRST LINE TO MAKE SURE IT IS THE SCHEMA
	////////////////////////////////////////////////////////
	vector<string> file_schema;
	string schema_delimiter = ",";
	string file_schema_single;
	Tokenizer t_schema(i_file[0], schema_delimiter);

	while (t_schema.getNextToken(file_schema_single))
		file_schema.push_back(file_schema_single);

	////CREATE A VECTOR OF FieldDescriptor OBJECTS TO USE specifySchema ON////
	vector<FieldDescriptor> input_schema;
	for (unsigned int i = 0; i < file_schema.size(); i++)
	{
		char index_check = file_schema[i].back();//get the last character of the given string
		if (index_check == '*')
		{
			FieldDescriptor sch1;
			sch1.name = file_schema[i].substr(0, file_schema[i].size() - 1);
			sch1.index = it_indexed;
			input_schema.push_back(sch1);
		}
		else
		{
			FieldDescriptor sch2;
			sch2.name = file_schema[i];
			sch2.index = it_none;
			input_schema.push_back(sch2);
		}
	}

	bool schema_continue = specifySchema(input_schema);

	if (!schema_continue)//If the schema is improperly formatted, return false and do nothing
		return false;

	//////////////////////////////////////////////////////////////////////////////
	///////////////////END OF SCHEMA HANDLING//////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	for (unsigned int i = 1; i < i_file.size(); i++)//for each line in the file after the first line, which was a schema line
	{
		vector<string> line_of_fields;
		string delimiter = ",";
		string field;
		Tokenizer t(i_file[i], delimiter);//create a delimiter that break apart the current line in the file

		while (t.getNextToken(field))//put the next token into field to be pushed back into the vector, line_of_fields
			line_of_fields.push_back(field);

		if (line_of_fields.size() != file_schema.size())//If data row has the wrong number of fields, set schema and fieldIndex to empty and return false
		{
			m_schema.clear();
			m_fieldIndex.clear();
			return false;
		}

		addRow(line_of_fields);
	}

	return true;


}

bool Database::loadFromFile(std::string filename)
{
	ifstream infile(filename);

	if (!infile)
	{
		cerr << "Error: Cannot open " << filename << endl;
		m_schema.clear();
		m_rows.clear();
		m_fieldIndex.clear();
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	///////////////////READING THE ENTIRE FILE INTO VECTOR i_file//////////////////
	//////////////////////////////////////////////////////////////////////////////

	vector<string> i_file;
	string read_line;

	while (getline(infile, read_line))//read each line until the end of the file
		i_file.push_back(read_line);

	////////////////////////////////////////////////////////
	//TEST THE FIRST LINE TO MAKE SURE IT IS THE SCHEMA
	////////////////////////////////////////////////////////
	vector<string> file_schema;
	string schema_delimiter = ",";
	string file_schema_single;
	Tokenizer t_schema(i_file[0], schema_delimiter);

	while (t_schema.getNextToken(file_schema_single))
		file_schema.push_back(file_schema_single);

	////CREATE A VECTOR OF FieldDescriptor OBJECTS TO USE specifySchema ON////
	vector<FieldDescriptor> input_schema;
	for (unsigned int i = 0; i < file_schema.size(); i++)
	{
		char index_check = file_schema[i].back();//get the last character of the given string
		if (index_check == '*')
		{
			FieldDescriptor sch1;
			sch1.name = file_schema[i].substr(0, file_schema[i].size() - 1);
			sch1.index = it_indexed;
			input_schema.push_back(sch1);
		}
		else
		{
			FieldDescriptor sch2;
			sch2.name = file_schema[i];
			sch2.index = it_none;
			input_schema.push_back(sch2);
		}
	}
	
	bool schema_continue = specifySchema(input_schema);

	if (!schema_continue)//If the schema is improperly formatted, return false and do nothing
		return false;

	//////////////////////////////////////////////////////////////////////////////
	///////////////////END OF SCHEMA HANDLING//////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	
	for (unsigned int i = 1; i < i_file.size(); i++)//for each line in the file after the first line, which was a schema line
	{
		vector<string> line_of_fields;
		string delimiter = ",";
		string field;
		Tokenizer t(i_file[i], delimiter);//create a delimiter that break apart the current line in the file

		while (t.getNextToken(field))//put the next token into field to be pushed back into the vector, line_of_fields
			line_of_fields.push_back(field);

		if (line_of_fields.size() != file_schema.size())//If data row has the wrong number of fields, set schema and fieldIndex to empty and return false
		{
			m_schema.clear();
			m_fieldIndex.clear();
			return false;
		}
		
		addRow(line_of_fields);
	}

	return true;
}

int Database::getNumRows() const
{
	return rowCount;
}

bool Database::getRow(int rowNum, std::vector<std::string>& row) const
{
	if ((unsigned int) rowNum > m_rows.size() - 1 || rowNum < 0)
		return false;

	for (unsigned int i = 0; i < m_rows[rowNum].size(); i++)//size element is used because it retrieves the number of strings in the requested Row's vector of strings
		row.push_back(m_rows[rowNum][i]);//Retrieve each element in the requested row into row parameter
	return true;
}

int Database::search(const	std::vector<SearchCriterion>&	searchCriteria,
	const	std::vector<SortCriterion>&	sortCriteria,
	std::vector<int>&	results)
{

	/////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////CREATING RESULTS FROM THE SEARCH CRITERIA/////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	for (unsigned int i = 0; i < searchCriteria.size(); i++)
	{
		vector <int> tempResult;
		///////////////////////////HANDLING OF ERROR_RESULT && FINDING POSITION OF FIELDNAME////////////////////////////////////////////////

		if (searchCriteria[i].fieldName == "" || (searchCriteria[i].minValue == "" && searchCriteria[i].maxValue == ""))//If there is no specified search Criteria, return Error
		{
			results.clear();
			return ERROR_RESULT;
		}

		int fieldPos = 0;//will contain the field position of the fieldName

		for (unsigned int j = 0; j < m_schema.size(); j++)//Negligable O() cost
		{
			if (m_schema[j].name == searchCriteria[i].fieldName && m_schema[j].index == it_none)//If the fieldName of the given search criteria is not indexed, return ERROR_RESULT
			{
				results.clear();
				return ERROR_RESULT;
			}

			if (m_schema[j].name == searchCriteria[i].fieldName)
				break;
			++fieldPos;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MultiMap::Iterator it1;

		//Case 1: There is both a minimum bound and a maximum bound, for which which you woud findEqualOrSuccessor from the Minimum bound until you reach past the maximum (break from loop then)
		//
		//Case 2: There is only a minimum bound, for which you would findEqualOrSuccessor from the Minimum bound until the iterator becomes invalid
		//Case 3: THere is only a maximum bound, for which you would findEqualOrPredecessor from the Maximum bound until the iterator becomes invalid

		if (searchCriteria[i].minValue != "" && searchCriteria[i].maxValue == "")//Only a minimum bound
		{
			it1 = m_fieldIndex[fieldPos]->findEqualOrSuccessor(searchCriteria[i].minValue);

			while (it1.valid())
			{
				tempResult.push_back(it1.getValue());//Add found values to the vector
				it1.next();
			}
		}
		else if (searchCriteria[i].minValue == "" && searchCriteria[i].maxValue != "")//Only a maximum bound
		{
			it1 = m_fieldIndex[fieldPos]->findEqualOrPredecessor(searchCriteria[i].maxValue);

			while (it1.valid())
			{
				tempResult.push_back(it1.getValue());//Add found values to the vector
				it1.prev();
			}
		}
		else
		{
			it1 = m_fieldIndex[fieldPos]->findEqualOrSuccessor(searchCriteria[i].minValue);

			while (it1.valid())//while the iterator is valid and the specified key is less than or equal to the maxValue
			{
				if (it1.getKey() >= searchCriteria[i].maxValue)
					break;
				
				tempResult.push_back(it1.getValue());//Add found values to the vector
				it1.next();
			}
		}

		////////////////////////////////////////////////REFINE THE RESULTS VECTOR VIA COMPARISONS TO TEMPRESULTS////////////////////////////////////////////////////
		
		if (i == 0)//If it is the first criteria and the results vector is empty
			for (unsigned int k = 0; k < tempResult.size(); k++)
				results.push_back(tempResult[k]);
	
		else
		{
			vector<int> newResults;//newResults will store the intersection between the old results and the newly found temporary results based on current criteria
			for (unsigned int k = 0; k < tempResult.size(); k++)//Has a cost of O(M)
			{
				for (unsigned int l = 0; l < results.size(); l++)//Negligable cost
				{
					if (tempResult[k] == results[l])//If the given tempResult is equal to a value in Result, add to newResults and break
					{
						newResults.push_back(tempResult[k]);
						break;
					}
				}
			}
			results = newResults;
		}
	}

	return results.size();

	/////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////IMPLEMENTATION OF SORTING/////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
}
