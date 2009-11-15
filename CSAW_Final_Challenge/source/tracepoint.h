/*** 
 * tracepoint - manage existing trace points
 * - max _at_ matasano.com
 */

typedef unsigned int tp_t;

// number of bytes saved per trace point
#define TP_SAVED_LEN 8

// initialize tracepoint. <1 on error
int initTracePoint();
// save TP_SAVED_LEN code bytes and trampoline for trace point. <1 on error
int saveTracePoint(tp_t, char*);
// return pointer to TP_SAVED_LEN code bytes saved for trace point. NULL if not found.
char* codeforTracePoint(tp_t);
// return pointer to trampoline for trace point. NULL if not found.
char* tpforTracePoint(tp_t);
// destroy tracepoint. <1 on error
int destroyTracePoint();
// number of current trace points;
int countTracePoint();
