// Copyright 2014
//   erkki.salonen@tpnet.fi
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
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

#include "mappings.h"
#ifdef STORE_RELEASE
#else

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include "Logger.h"

Logger::Logger() :
	running_number(0),seq_number(0)
{

#ifdef LOGTOSOCKET
#ifdef TARGET_LOGGER_ADDRESS
	//socket
	sockd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockd == -1)
	{
		LOGITIMETOPIC("FAILED to open socket\n");
		// no socket
		//exit(1);
		status = -1;
		return;
	}

	LOGITIMETOPIC("opened socket\n");

	// server address
	server_name.sin_family = AF_INET;
	inet_aton(TARGET_LOGGER_ADDRESS, &server_name.sin_addr);
	server_name.sin_port = htons(TARGET_LOGGER_PORT);

	// connect to server
	status = connect(sockd, (struct sockaddr*)&server_name, sizeof(server_name));
	if(status == -1)
	{
		LOGITIMETOPIC("no connection\n");

		// no connection
	}
	LOGITIMETOPIC("connection ready\n");
#endif
#endif
	gettimeofday(&start, NULL);
}

Logger::~Logger() {
	if(sockd > 0) {
		close(sockd);
	}
}

Logger* Logger::Instance()
{
	static Logger* instance=NULL;
	if(instance == NULL) {
		instance = new Logger();
	}
	return instance;
}

void Logger::EventStart(const char* eventName)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";event;start;[%u]%s",tid,eventName);
	WriteItem(buf);
}

void Logger::EventStop(const char* eventName)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";event;stop;[%u]%s",tid,eventName);
	WriteItem(buf);
}

void Logger::EventStart(const char* eventName,const char* eventName2)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";event;start;[%u]%s%s",tid,eventName,eventName2);
	WriteItem(buf);
}

void Logger::EventStop(const char* eventName,const char* eventName2)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";event;stop;[%u]%s%s",tid,eventName,eventName2);
	WriteItem(buf);
}

void Logger::FreeText(const char* eventName)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";[%u]%s",tid,eventName);
	WriteItem(buf);
}

void Logger::StateMachine(const char* machineName, const char* stateName)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";state;[%u]%s;%s",tid,machineName,stateName);
	WriteItem(buf);
}
void Logger::ValueABS(const char* valueName,int value)
{
	int tid = gettid();

	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), ";valueabs;%d;[%u]%s",value,tid,valueName);
	WriteItem(buf);
}


void Logger::WriteItem(const char* item)
{
#ifdef LOGTOSOCKET
	char buf[MAX_BUF];
	snprintf(buf,sizeof(buf), "%f%s\n",Elapsed(),item);

	if(status == 0){
		write(sockd, buf, strlen(buf));
	} else {
        //printf(&buf[0)]);
    }
#else
	/* Use current native logger port */
	LOGITIMETOPIC(item);
#endif
}

int Logger::id(int pointer)
{
	int return_val = -1;
	for(int n=0,size = array.size(); n < size;n++)
	{
		if(array[n].pointer == pointer)
		{
			return_val = array[n].id;
			array.erase(array.begin() + n);
			return return_val;
		}
	}
	Pair pair;
	pair.pointer = pointer;
	pair.id = running_number++;
	array.push_back(pair);
	return pair.id;
}

inline double Logger::Elapsed()
{
#ifdef USESEQNUMBERS
	return seq_number++;
#else
    double mtime;
    long seconds, useconds;
    struct timeval now;

    gettimeofday(&now, NULL);

    seconds  = now.tv_sec  - start.tv_sec;
    useconds = now.tv_usec - start.tv_usec;

    mtime = (((double)seconds) * 1000.0 + ((double)useconds)/1000.0) + 0.5;

    return mtime/1000.0;
#endif
}
#endif //STORE_RELEASE
