/*** 
 * tracepoint - manage existing trace points
 * - max _at_ matasano.com
 */

// TODO:
// no way to remove entries from table (requires DELETED marker)

#include "tracepoint.h"
#include <ntddk.h>

struct tp_e {
	tp_t tp;
	unsigned char saved[TP_SAVED_LEN];
	unsigned char* tpline;
};

// max # of tracked tpoints, must be prime number
#define MAX_TPOINTS 7919
#define HASH_TBL_SIZE (MAX_TPOINTS * sizeof(struct tp_e))
#define EMPTY(a) (g_pTPHash[a].tp == 0)

struct tp_e *g_pTPHash;
int g_nHash = 0;

KSPIN_LOCK g_lock;
KIRQL g_IRQL;
#define ENTER_CS 	KeAcquireSpinLock(&g_lock, &g_IRQL);
#define LEAVE_CS	KeReleaseSpinLock(&g_lock, g_IRQL);

static unsigned int hash(tp_t key)
{
	return (key % MAX_TPOINTS);
}

static int iforKey(tp_t key)
{
	int s,i;
	s = i = hash(key);
	while((g_pTPHash[i].tp != key) && (!EMPTY(i))) {
		i = (i+1) % MAX_TPOINTS;
		if(i==s) return -1;
	}
	return i;
}

int initTracePoint()
{
    KeInitializeSpinLock(&g_lock);

	g_pTPHash = ExAllocatePoolWithTag(NonPagedPool, HASH_TBL_SIZE, 'crTK');
	if(!g_pTPHash)
	{
		DbgPrint("Can't allocate TPHash table\n");
		return -1;
	}
	RtlZeroMemory(g_pTPHash, HASH_TBL_SIZE);
	return 0;
}

int saveTracePoint(tp_t key, char* tpline)
{
	int i;
	int ret;
	ENTER_CS;
	if((i = iforKey(key))<0) {
		ret = -1;
		goto safe_ret;
	}
	// if key already exists we overwrite, double tracing must be checked upstream
	g_pTPHash[i].tp = key;
	g_pTPHash[i].tpline = tpline;
	RtlCopyMemory(g_pTPHash[i].saved, (void*)key, TP_SAVED_LEN);
	InterlockedIncrement(&g_nHash);
	ret = 0;
safe_ret:
	LEAVE_CS;
	return ret;
}

char* codeforTracePoint(tp_t key)
{
	int i;
	char* ret;

	ENTER_CS;
	if(((i = iforKey(key))<0) || (g_pTPHash[i].tp != key))
	{
		ret = NULL; // key not found
		goto safe_ret;
	}
	ret = g_pTPHash[i].saved;
safe_ret:
	LEAVE_CS;
	return ret;
}

char* tpforTracePoint(tp_t key)
{
	int i;
	char* ret;

	ENTER_CS;
	if(((i = iforKey(key))<0) || (g_pTPHash[i].tp != key))
	{
		ret = NULL; // key not found
		goto safe_ret;
	}
	ret = g_pTPHash[i].tpline;
safe_ret:
	LEAVE_CS;
	return ret;
}

int destroyTracePoint()
{
	ExFreePoolWithTag(g_pTPHash, 'crTK');
	return 0;
}

int countTracePoint()
{
	return g_nHash;
}