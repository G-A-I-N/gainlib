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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <gain.h>

namespace Gain {

using namespace std;

#define CORESETTINGS Settings::GetSettings()

class Settings {

public:

	enum errorCodes {
		ERROR_SUCCESS =0,
		ERROR_FILEACCESS
	};

	/* If want have singleton object, use GetSettings() for reference */
	static Settings * GetSettings();

	Settings();

	virtual ~Settings();

	/* Setting file name for settings object. If not set, default name is
	 * used.  */
	void SetSettingsFileName(string fileName);

	/* API's for storing settings. zero indicates success, non zero
	 * is error code.  */
	int SetStringValue(string key,string value);
	int SetFloatValue(string key,float value);
	int SetIntValue(string key,int value);
	long long SetLongLongValue(string key,long long value);

	/* In case setting failed, this can be used retriving the result */
	string GetLastError();

	/* API's for retrieving settings. If key does not found, defaultValue
	 * is set to file and returned as result. */
	string GetStringValue(string key, string defaultValue = string("-") );
	float GetFloatValue(string key, float defaultValue = 0);
	int GetIntValue(string key, int defaultValue = 0);
	long long GetLongLongValue(string key,long long defaultValue = 0);

private:
	bool OpenSettingsFile(string fileName, bool bOpenForRead);
	bool LoadSettings();
	void ReadDataToMap();
	bool WriteDataFromMapToDisc();

	std::map<string,string> 	mStoredValues;
	std::fstream 		mfileStream;
	string				mLastError;
	string				mDefaultSettingsFilenName;
};


} /* namespace Gain */

#endif /* SETTINGS_H_ */
