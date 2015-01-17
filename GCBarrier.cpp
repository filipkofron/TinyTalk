#include <iostream>
#include "GCBarrier.h"
#include "Common.h"

GCBarrier::GCBarrier()
    : gcRunner(std::this_thread::get_id())
{
    allocating = false;
    gcing = false;
}

void GCBarrier::reg()
{
}

void GCBarrier::unreg()
{
}

void GCBarrier::enteringAlloc(bool &runAgain)
{

}

void GCBarrier::leavingAlloc()
{

}

void GCBarrier::enteringGC()
{

}

void GCBarrier::leavingGC()
{

}

void GCBarrier::tryCatchAnyWhere()
{

}

void GCBarrier::enteringBlocking()
{

}

void GCBarrier::leavingBlocking()
{

}
