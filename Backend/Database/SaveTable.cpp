// Copyright 2020 Robert Carneiro, Derek Meer, Matthew Tabak, Eric Lujan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "SaveTable.h"
#include "GTable.h"
#include "maxid.h"

using namespace shmea;

SaveTable::SaveTable(const GString& newDirName, const GString& newName)
{
	clean();
	dname = newDirName;
	name = newName;
}

SaveTable::~SaveTable()
{
	clean();
}

GString SaveTable::getPath() const
{
	if (dname.length() == 0)
		return "";

	if (name.length() == 0)
		return "";

	// Build the path
	return "database/" + dname + "/" + name;
}

int64_t SaveTable::getID() const
{
	return id;
}

GString SaveTable::getName() const
{
	return name;
}

GTable SaveTable::getTable() const
{
	return value;
}

void SaveTable::loadByName()
{
	if (dname.length() == 0)
		return;

	if (name.length() == 0)
		return;

	// Set the contents
	GString fname = getPath();
	GTable newValue(fname, ',', GTable::TYPE_FILE);
	value = newValue;
}

void SaveTable::loadByID(int64_t newID)
{
	/*if (name.length() == 0)
		return;

	// Is the ID valid
	id = newID;
	if (id < 0)
		return;

	// is the id less than the max id?
	if (id > MaxID::getMaxID(name))
		return;

	// Set the contents now that we know the directory name
	loadByName(dname);*/
}

void SaveTable::saveByName(const GTable& newTable) const
{
	if (name.length() == 0)
		return;

	// Update the UID database
	// TODO

	// save the file
	GString fname = getPath();
	newTable.save(fname);
}

void SaveTable::saveByID(const GTable& newTable)
{
	/*if (name.length() == 0)
		return;

	// generate a new id from maxID
	if (id < 0)
		id = MaxID::generateID(name);

	// save the file
	GString fname = getPath();
	newTable.save(fname);*/
}

/*void SaveTable::saveUID(GString nuid)
{
	struct stat info;
	GString dirname = buildDataDir();
	if (dirname.length() > 0)
	{
		if (stat(dirname.c_str(), &info) != 0)
		{
			// make the directory
			int status = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
			if (status < 0)
			{
				printf("[DB] %s mkdir failed\n", dirname.c_str());
				return;
			}
		}
		else if (info.st_mode & S_IFDIR)
		{
			// directory exists
			// do nothing
		}
		else
		{
			// path is not a directory
			printf("[DB] %s is not a directory\n", dirname.c_str());
			return;
		}
	}

	// open file for writing
	GString fname = buildUIDPath(name);
	FILE* fd = fopen(fname.c_str(), "a");
	if (fd != NULL)
	{
		// save the unique id
		printf("[DB] !%s\n", fname.c_str());

		// check if ID already exist
		std::ifstream infile(fname.c_str());
		GStringstream buffer;
		buffer << infile.rdbuf();
		GString content = buffer.str();
		if (content.find(nuid) == -1)
			fprintf(fd, "%s\n", nuid.c_str());

		// close the fd
		fclose(fd);
	}
	else
		printf("[DB] Max ID Save Error\n");
}*/

bool SaveTable::deleteByName()
{
	if (dname.length() == 0)
		return false;

	if (name.length() == 0)
		return false;

	// Set the contents
	GString fname = getPath();
	return (remove(fname.c_str()) == 0);
}

void SaveTable::clean()
{
	id = -1;
	dname = "";
	name = "";
}

void SaveTable::print() const
{
	if (name.length() == 0)
	{
		printf("[DB] Requires a name\n");
		return;
	}

	/*if (!value)
	{
		printf("[DB] -%s\n", getPath().c_str());
		return;
	}*/

	// Print the contents
	printf("File Name: %s\n", name.c_str());
	value.print();
}
