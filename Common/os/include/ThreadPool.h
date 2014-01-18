#ifndef SKY_THREAD_POOL_H
#define SKY_THREAD_POOL_H

/************************
*
* @Project: SkyWrite, a next generation handwriting
* and gesture recognition engine. Based upon the N$
* recognizer by Jacob O. Wobbrock, Ph.D. and Lisa Anthony, Ph.D. 
* And the $1 Unitstorke Recognizer by Baylor Wetzel.
*
* @SkyWrite authors:
*		Leonard Smith
*       Rochester Institute of Technology
*       ljs6852@rit.edu
*
*  		Seth Hendrick
*       Rochester Institute of Technology
*       srh7240@rit.edu
*
* Copyright (C) 2013 Leonard Smith and Seth Hendrick. 
*
* @Skywrite License: Released under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported 
* license. See sw-license.txt for the exact wording of the license. However, a quick summary is listed below.
*
* NOTE: The following summary is by no means a replacement to reading sw-license.txt. You are obligated to fully understand
* the license in its complete legal detail.

* Quick summary of license:
*		You are free to copy, distribute, and transmit SkyWrite, under the following conditions.
*
*			1) You must attribute Leonard Smith and Seth Hendrick in some manner. This could be as little as a brief sentence 
*			   in an about page.
*			2) You may not use this work for commerical purposes.
*			3) You may not alter, transform, or build upon this work.
*		
* The above copyright notice, project summary, author listing, and license details (IE: The entire comment) may not be removed
* and must be provided with the distribution.
*
*************************/

#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "Test_Task.h"

class Threadpool
{
public:
	/// @brief Constructor.
	Threadpool() :
		m_running(false)
	{}
	

	void initAndStart( std::size_t pool_size )
	{
		m_running = true;
		
		for ( std::size_t i = 0; i < pool_size; ++i )
		{
			m_threads.create_thread( boost::bind( &Threadpool::pool_main, this ) ) ;
		}
		
	}

	/// @brief Destructor.
	virtual ~Threadpool()
	{
		// Set running flag to false then notify all threads.
		{
			boost::unique_lock< boost::mutex > lock( m_mutex );
			m_running = false;
			m_condition.notify_all();
		}

		try
		{
			m_threads.join_all();
		}
	
		// Suppress all exceptions.
		catch ( ... ) {}
		
		for(unsigned int i = 0; i < m_tasks.size(); ++i)
		{
			delete m_tasks[i];
			m_tasks[i] = NULL;
		}
		m_tasks.clear();
	}

	/// @brief Add task to the thread pool if a thread is currently available.
	template <typename T, typename F >
	void assignTask( T task, F callback )
	{
		boost::unique_lock< boost::mutex > lock( m_mutex );
		m_tasks.push_back( task );
		m_condition.notify_one();

	}
	
	void stop()
	{
		m_running = false;
	}

private:
	/// @brief Entry point for pool threads.
	void pool_main()
	{
		while( m_running )
		{
			// Wait on condition variable while the task is empty and the pool is
			// still running.
			boost::unique_lock< boost::mutex > lock( m_mutex );
			while ( m_tasks.empty() && m_running )
			{
				m_condition.wait( lock );
			}
			
			// If pool is no longer running, break out.
			if ( !m_running ) break;

			// Copy task locally and remove from the queue.  This is done within
			// its own scope so that the task object is destructed immediately
			// after running the task.  This is useful in the event that the
			// function contains shared_ptr arguments bound via bind.
			{
				Base_Task* task = m_tasks.front();
		
				m_tasks.pop_front();
				lock.unlock();
				
				// Run the task.
				try
				{
					task->execute();
				}
				// Suppress all exceptions.
				catch ( ... ) 
				{
					std::cout << "ERROR RUNNINGTASK!" << std::endl;
				}
		
				delete task;
				task = NULL;
			}
		} // while m_running
	}
  
private:
	std::deque< Base_Task* > m_tasks;
	boost::thread_group m_threads;
	boost::mutex m_mutex;
	boost::condition_variable m_condition;

	bool m_running;
};

#endif