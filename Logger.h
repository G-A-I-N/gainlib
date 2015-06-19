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

#ifndef LOGGER_H_
#define LOGGER_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#include "mappings.h" // native log port implementation


#define MAX_BUF 1024

// #define LOGISON // if this is defined, logging is on.  this define comes from "mappings.h"
//#define LOGTOSOCKET // if this is defined, data is sent to socket. Other native logger interface is used
#define LOGID //if this is enabled, id is generated to a matching object/integer
//#define USESEQNUMBERS // if this is defined, instead of time, running sequence number is as timestamp


#ifdef LOGISON
	#define LOGINIT Logger::Instance();
	#define LOGSTOP delete Logger::Instance();
    #define LOGEVENTSTART(x) Logger::Instance()->EventStart(x);
    #define LOGEVENTSTOP(x) Logger::Instance()->EventStop(x);
    #define LOGEVENT2START(x,y) Logger::Instance()->EventStart(x,y);
    #define LOGEVENT2STOP(x,y) Logger::Instance()->EventStop(x,y);
    #define LOGFREETEXT(x) Logger::Instance()->FreeText(x);
	#define LOGFREETEXTWITHKEY(x,y) { string z = string(x) + string(":")+ string(y) ; Logger::Instance()->FreeText(z.c_str()); }
    #define LOGSTATE(x,y) Logger::Instance()->StateMachine(x,y);
    #define LOGVALUE(x,y) Logger::Instance()->ValueABS(x,y);
    #define LOGSCOPE TimeToPicEventObj myFuncScope(__PRETTY_FUNCTION__);
	#define LOGSCOPEINFO(param) scopeTrace myScope(__FUNCTION__ + param);

	#define LOGOBJECTSTART(x,y) LOGOBJECTFUNC(x,y,LOGEVENTSTART)
	#define LOGOBJECTSTOP(x,y)  LOGOBJECTFUNC(x,y,LOGEVENTSTOP)


	#ifdef LOGID
		#define LOGFUNC(y)	snprintf(__temp,sizeof(__temp),"[%04i]",Logger::Instance()->id((int)y))
	#else //logging pointer or what ever integer was given
		#define LOGFUNC(y)	snprintf(__temp,sizeof(__temp),"_%08X",(int)y)
	#endif

	#define LOGOBJECTFUNC(x,y,f) \
		{\
			char __temp[9];\
			const int sizex = sizeof(x);\
			char ___temp[sizex+sizeof(__temp)];\
			memcpy(___temp,x,sizex);\
			LOGFUNC(y);\
			memcpy((___temp+sizex-1),__temp,sizeof(__temp));\
			f(___temp);\
		}

#else
	#define LOGINIT
	#define LOGSTOP
    /* Empty macros when logging is disabled */
    #define LOGEVENTSTART(x)
    #define LOGEVENTSTOP(x)
    #define LOGFREETEXT(x)
	#define LOGFREETEXTWITHKEY(x,y)
    #define LOGSTATE(x,y)
    #define LOGVALUE(x,y)
	#define LOGSCOPE
	#define LOGOBJECTSTART(x,y)
	#define LOGOBJECTSTOP(x,y)
#endif

typedef struct _Pair
{
	int pointer;
	int id;
} Pair;


class Logger {

public:
	static Logger* Instance();

	Logger();
	virtual ~Logger();

public:
	void EventStart(const char* eventName);
	void EventStop(const char* eventName);
	void EventStart(const char* eventName,const char* eventName2);
	void EventStop(const char* eventName,const char* eventName2);
	void FreeText(const char* eventName);
	void StateMachine(const char* machineName, const char* stateName);
	void ValueABS(const char* valueName, int value);
	int id(int pointer);

private:
	void WriteItem(const char* item);
    inline double Elapsed();



private:
	  int sockd;
	  struct sockaddr_in server_name;
	  int status;
	  int running_number;
	  int seq_number;

	  struct timeval start;
	  std::vector<Pair> array;

};

/* Automatic object. Event starts in constructor and destructor it will be stopped. */
class scopeTrace {
public:
	inline scopeTrace(const char *scopeName) :  scopeText(scopeName) {
		LOGEVENTSTART(scopeText.c_str());
	}
	inline ~scopeTrace() {
		LOGEVENTSTOP(scopeText.c_str());
	}
private:
	std::string scopeText;
};


class TimeToPicEventObj
{
public:
    TimeToPicEventObj(const char* name)
    :mName(name)
    {
    	LOGEVENTSTART(name);
    }

    ~TimeToPicEventObj()
    {
    	LOGEVENTSTOP(mName);
    }
private:
    const char* mName;
};





#endif /* LOGGER_H_ */

