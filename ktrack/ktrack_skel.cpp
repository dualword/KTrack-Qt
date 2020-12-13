/****************************************************************************
**
** DCOP Skeleton created by dcopidl2cpp from ktrack.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "./ktrack.h"

#include <kdatastream.h>


static const char* const Ktrack_ftable[2][3] = {
    { "ASYNC", "testMethod()", "testMethod()" },
    { 0, 0, 0 }
};

bool Ktrack::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
    if ( fun == Ktrack_ftable[0][1] ) { // void testMethod()
	replyType = Ktrack_ftable[0][0]; 
	testMethod( );
    } else {
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return TRUE;
}

QCStringList Ktrack::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "Ktrack";
    return ifaces;
}

QCStringList Ktrack::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; Ktrack_ftable[i][2]; i++ ) {
	QCString func = Ktrack_ftable[i][0];
	func += ' ';
	func += Ktrack_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


