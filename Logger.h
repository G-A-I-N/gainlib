/*
 * Logger.h
 *
 *      Author: ville.kankainen@kangain.com
 *      License: free to use, no liability
 */

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



#define MAX_BUF 1024

#define LOGISON // if this is defined, logging is on.
#define LOGTOSOCKET // if this is defined, data is sent to socket instead of file
#define LOGID //if this is enabled, id is generated to a matching object/integer
#define USESEQNUMBERS // if this is defined, instead of time, running sequence number is as timestamp

#define LOGINIT Logger::Instance();
#define LOGSTOP delete Logger::Instance();

#ifdef LOGISON
    #define LOGEVENTSTART(x) Logger::Instance()->EventStart(x);
    #define LOGEVENTSTOP(x) Logger::Instance()->EventStop(x);
    #define LOGFREETEXT(x) Logger::Instance()->FreeText(x);
    #define LOGSTATE(x,y) Logger::Instance()->StateMachine(x,y);
    #define LOGVALUE(x,y) Logger::Instance()->ValueABS(x,y);
    #define FUNCTIONSCOPE TimeToPicEventObj myFuncScope(__FUNCTION__);


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

	#define LOGOBJECTSTART(x,y) LOGOBJECTFUNC(x,y,LOGEVENTSTART)
	#define LOGOBJECTSTOP(x,y)  LOGOBJECTFUNC(x,y,LOGEVENTSTOP)

#else
    /* Empty macros when logging is disabled */
    #define LOGEVENTSTART(x)
    #define LOGEVENTSTOP(x)
    #define LOGFREETEXT(x)
    #define LOGSTATE(x,y)
    #define LOGVALUE(x,y)
	#define FUNCTIONSCOPE
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
	void FreeText(const char* eventName);
	void StateMachine(const char* machineName, const char* stateName);
	void ValueABS(const char* valueName, unsigned int value);
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

