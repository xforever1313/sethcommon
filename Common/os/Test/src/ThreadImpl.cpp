#include "ThreadImpl.h"

ThreadImpl::ThreadImpl() :
    m_numberOfRuns(0)
{
}

ThreadImpl::~ThreadImpl()
{
}

void ThreadImpl::run(){
    SkyvoThread::sleep(1500);
    ++m_numberOfRuns;
}
