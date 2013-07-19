#ifndef SKY_BASE_TASK_H
#define SKY_BASE_TASK_H

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

#include "Test_Task.h"
#include <iostream>

class Base_Task
{
public:

	Base_Task() 
	{}
	
	virtual ~Base_Task()
	{}

	virtual void execute()
	{
		std::cout << "Derp" << std::endl;
		// first cast task to its dervied 
		//task.run();
	}
	
	void setId( unsigned int id )
	{
		m_id = id;
	}
	
	unsigned int getID()
	{
		return m_id;
	}
	
private:
	unsigned int m_id;
};

#endif