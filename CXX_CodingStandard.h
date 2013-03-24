#ifndef CXX_CODING_STANDARD_H					//Guards should have a '_' between each word, and an 'H' at the end.  Some IDE's put an _INCLUDED at the end.  This is fine.
#define CXX_CODING_STANDARD_H

#include <systemFile>							//#includes go next.  It is recommended that libraries go first.  Put in alphabetical order

#include "MyFile.h"							//User files go next.  Put in alphabetical order

//DO NOT PUT ANY "USING" STATEMENTS IN THE .h FILE

/**
 * \class CXX_CodingStandard    								-The class name (if any)
 * \brief A sample class so we know what the coding standard is	-brief class description
 * \details Lots of details go here								-A longer description (only if needed)
 * \author                                                      -The author name
 */ 
class CXX_CodingStandard : public BaseClass {  //A class that implements a base class

	//First, list all of the public attributes of a class.
	public: //Public, private, and protected should be tabbed in
	
        //Enums come first
        enum someEnum{
            A = 10,
            B,
            C
        };
        
        //Structs come next.
        typedef struct someStruct{
            int a;
            int b;
        }someStruct_t;
    
		///Constructors come next.  No comments are needed, as its obvious what they do
		CXX_CodingStandard(int x);
	
        ///Destructors come next.  No comment needed, as its obvious what it does.
        ///Always make the destructor virtual
        ///THOU SHALT ALWAYS WRITE A DESTRUCTOR NO MATTER HOW TRIVIAL
		virtual ~CXX_CodingStandard();
	
        ///No comment needed, as its obvious what this method does.
		void setX(int newX);
		
        ///Again, no  comment needed, as its obvious what this methood does.
        ///Getters probably can be const, so make them so.
		int getX() const;
        
        static int MAXIMUM_X;  ///Only constants can go in the public section.  All other variables need to go in protected or private.  Instead of camelCase, put a '_' where spaces go
		
	protected:
		//Any protected methods go here
	
	private: //Private methods go next
		///If there is no reason for there to be a default construction, make the default constructor private
		CXX_CodingStandard();
        
		//Everything else goes here
		int m_x;  ///<A value                              //For data members, use this (<) before comment so doxygen can get it.  Put an 'm_' in front of each member of the class
		
}; //Don't forget the semicolon!

#endif //CXX_CODING_STANDARD_H		-Guard name at the end

/**
 * Note that there aren't a lot of comments.  This is on purpose.  A comment should only add important information.  If a method needs a comment, it probably needs to be renamed.
 * for example:
 * void incX(); needs a comment since I don't know what inc does
 * void incrementX(); does not need a comment since I understand what it does now.
*/

///.cpp file---------------------------------------------------------------------------------------------------

//The following should go in the .cpp file, NOT the .h file (unless its a template)

 #include "CXX_CodingStandard.h"  //#includes go first, again put in alphabetical order
 
 //It is highly recommended that you not use using statements.  Use them sparingly.
 
//First, any static data members should be initialized here, for example:
int CXX_CodingStandard::maximumX = 3;
 
//Next come the implementation of the methods.  Comments are not needed before methods
 
CXX_CodingStandard::CXX_CodingStandard(int x) :   //Put the colon on the same line as the constructor
	m_x(x)                     //On the next line, tab over and initalize all the data members
                             //If there are more, put it on a new line
{	//This is the only spot where the curly goes on the next line
}

CXX_CodingStandard::~CXX_CodingStandard(){  //Note the curly brace is on the same line
}

void CXX_CodingStandard::setX(int newX){
	m_x = newX;
}

int CXX_CodingStandard::getX() const{  //Don't forget the const
	return m_x;
}

///Unit Testing-------------------------------------------------------------
/**
 * \file PageTest.cpp                    //File name
 * \brief Tests the Page class           //Description
 *
 * \author Seth Hendrick                 //Author
 */
 
#include <boost/test/unit_test.hpp>     //First, include any system files.
#include <string>

#define private public                  //Do this if you need access to private members.  Do this AFTER the including the system files
#define protected public                //Do this if you need access to protected members

#include "containers/Test/DummyContainer.h" 
#include "Page.h"                       //Include any user files next.  Put in alphabetical order
#include "PageMetaData.h"

//Next, put any constants used by the test.  Note, due to limitations of boost, these names must be unique throughout the tests.  If there is a value that is across multiple tests, put in a testHelper.h file, and include the file in both tests.
Date baseCreationDate;
Date baseModifiedDate;
PageMetaData dummyPageData("some title", PageMetaData::GRAPH, baseCreationDate, baseModifiedDate, "some location");

//Now its for the actual tests.
///\brief tests the copy constructor    //Make a description of what the test does
BOOST_AUTO_TEST_CASE(Page_copyTest){  //Note the value in the ().  The first part, before the '_' is the class being tested.  The next value is a name of the test being done.  This is done so we know which test fails.
    DummyContainer *cont1 = new DummyContainer (13, 14, 15, 16, 17);
    Textbox *cont2 = new Textbox (2, "font", "text", 18, 19, 20, 21, 22);
    Picture *cont3 = new Picture ("location", 18, 19, 20, 21, 22);
    std::deque <Container *> clist;
    clist.push_back(cont1);
    clist.push_back(cont2);
    clist.push_back(cont3);

    Page *uut = new Page(new PageMetaData(dummyPageData), clist);
    Page *uut2 = new Page((*uut));
    uut->setTitle("Hello!");
    //Ensure the pages are separate    //Put ensure statements where needed.
    BOOST_CHECK_EQUAL(uut2->getTitle(), "some title");
    delete uut;
    //Ensure a copy of pageMetaData was made.  This shouldn't segfault
    BOOST_CHECK_EQUAL(uut2->getTitle(), "some title");
    delete uut2;
}

/**
* Remeber, Unit tests need 100% coverage.  Do not be afraid to use #ifdef UNIT_TEST in your classes to get this.
*/

///Other tips---------------------------------------------------------------
//For for loops, use ++i, NOT i++.  ++i is more efficient
for (int i = 0; i < 3; ++i){
}

//Also, notice the '3'.  This is a magic number.  There should be a constant somewhere the defines this.
for (int i = 0; i < maxNumLoops; ++i){
}

//If you are doing a .size() in the STL, it is unsigned.  Be sure to make i an unsigned int, or a warning will be raised
for (unsigned int i = 0; i < myVector.size(); ++i){
}

//Warnings are considered errors, you have been warned!