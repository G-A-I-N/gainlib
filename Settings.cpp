// Copyright 2015
//   erkki.salonen@tpnet.fi
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include <map>


#include "Settings.h"

#define SETTINGS_FILE_NAME "appsettings.txt"



// http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

//http://kengine.sourceforge.net/tutorial/g/stdmap-eng.htm
typedef std::pair<std::string, std::string> TStrStrPair;


namespace Gain {



using namespace std;


static Settings * gSettings = NULL;

Settings::Settings() {
	SetSettingsFileName(SETTINGS_FILE_NAME);
}

Settings::~Settings() {

	mStoredValues.clear();
	mfileStream.close();
}

Settings * Settings::GetSettings(void) {

	if(gSettings==NULL) {

		gSettings = new Settings();

	}
	return gSettings;
}

void Settings::SetSettingsFileName(string fileName) {
	mDefaultSettingsFilenName = fileName;
}

int Settings::SetStringValue(string key, string value) {

	int ret = ERROR_SUCCESS;
	mStoredValues[key]=value;
	if (WriteDataFromMapToDisc() == true ) {
		ret = ERROR_SUCCESS;
	}
	else {
		ret = ERROR_FILEACCESS; //
	}
}

int Settings::SetFloatValue(string key, float value) {

	int ret = ERROR_SUCCESS;
	mStoredValues[key]=to_string(value);
	WriteDataFromMapToDisc();
	if (WriteDataFromMapToDisc() == true ) {
		ret = ERROR_SUCCESS;
	}
	else {
		ret = ERROR_FILEACCESS; //
	}
}

int Settings::SetIntValue(string key, int value) {

	int ret = ERROR_SUCCESS;
	mStoredValues[key]=to_string(value);

	LOGFREETEXT(mStoredValues[key].c_str());

	if (WriteDataFromMapToDisc() == true ) {
		ret = ERROR_SUCCESS;
	}
	else {
		ret = ERROR_FILEACCESS; //
	}
}

string Settings::GetLastError() {

	return mLastError;
}

string Settings::GetStringValue(string key, string defaultValue) {

	string ret ;
	if (mStoredValues.size()==0) {
		/* import keys first */
		LoadSettings();
	}

	/* Check if key is found from map. If not, return default value */
	if ( mStoredValues.find(key) != mStoredValues.end() ) {
		ret = mStoredValues.at(key);
	}
	else {

		mStoredValues.insert(TStrStrPair(key,defaultValue));
		ret = mStoredValues.at(key);
	}

	return ret;
}

float Settings::GetFloatValue(string key, float defaultValue) {

	float ret ;
	string value;
	if (mStoredValues.size()==0) {
		/* import keys first */
		LoadSettings();
	}

	/* Check if key is found from map. If not, return default value */
	if ( mStoredValues.find(key) != mStoredValues.end() ) {
		value = mStoredValues.at(key);
	}
	else {
		mStoredValues.insert(TStrStrPair(key,to_string(defaultValue)));
		value = mStoredValues.at(key);
	}


	ret = atof (value.c_str());
	return ret;

}

int Settings::GetIntValue(string key, int defaultValue) {

	int ret ;
	string value;
	if (mStoredValues.size()==0) {
		/* import keys first */
		LoadSettings();
	}

	/* Check if key is found from map. If not, return default value */
	if ( mStoredValues.count(key) ==1 ) {
		value = mStoredValues.at(key);
	}
	else {
		mStoredValues.insert(TStrStrPair(key,to_string(defaultValue)));
		value = mStoredValues.at(key);
	}

	ret = atoi (value.c_str());
	return ret;
}

void Settings::ReadDataToMap() {
	/* Read data into memory */
	assert (mfileStream.is_open()==true);

	string line;
	while ( getline (mfileStream,line) )
	{

		const char iniSeparator = '=';
		std::vector<std::string> x = split(line, iniSeparator);
		string key;
		string value;



		if (x.size()==2) {
			key = x.at(0);
			value = x.at(1);
			mStoredValues.insert(TStrStrPair(key,value));

			LOGFREETEXT(line.c_str());
			LOGFREETEXT(key.c_str());
			LOGFREETEXT(value.c_str());
		}
	}
}

bool Settings::LoadSettings() {
	bool ret = false;
	/* get settings file path from Core */
	/* close previous file if open */
	if (mfileStream.is_open() == true) {
		mfileStream.close();
	}

	string fileNameWithPath = SETTINGS_FILE_NAME;
	if (OpenSettingsFile(fileNameWithPath, true) == true) {
		/* Read data into memory */
		ReadDataToMap();
		/* Close file */
		ret = true;
	} else {
		/* No settings file exists yet */
		ret = false;
	}
	return ret;
}

bool Settings::OpenSettingsFile(string fileName, bool bOpenForRead) {

	bool ret = false;

	if (bOpenForRead==true) {
		mfileStream.open (fileName.c_str(), std::fstream::in);
	}
	else {
		mfileStream.open (fileName.c_str(), std::fstream::out);
	}

	ret =mfileStream.is_open();

	if (ret==false) {
		mLastError = string("Failed to open file ") + fileName;
	}
	else {
		mLastError = string("");
	}

	return ret;
}

bool Settings::WriteDataFromMapToDisc() {

	bool ret = false;
	string fileNameWithPath = SETTINGS_FILE_NAME;

	if (OpenSettingsFile(fileNameWithPath,false)==true) {
		/* File is open */

		for(std::map<string,string>::iterator iter = mStoredValues.begin(); iter != mStoredValues.end(); ++iter)
		{
			string key =  iter->first;
			string value = iter->second;

			mfileStream << key;
			mfileStream << "=";
			mfileStream << value;
			mfileStream << "\n";

		}
		mfileStream.flush();
		mfileStream.close();
		ret = true;
	}

	return ret;
}

} /* namespace Gain */


