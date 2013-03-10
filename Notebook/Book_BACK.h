#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED

/**
* \file Book.h
* \brief This is the declaration of a Book Object.
* \TODO: Finish updateViewablePageRange().
*/

#include <deque>
#include <map>
#include <algorithm>
#include <utility>
#include <iostream>
#include <math.h>

#include "Section.h"
#include "Bookmark.h"
#include "Date.h"
#include "Page.h"

#define FIVE_PAGES 5

/**
* \class Book
* \brief This represents a book object
*
* \author Leonard Smith
*/
class Book
{
public:

    enum SectionInsertionLoc {
        BEFORE_CURRENT_SECTION = 0,
        AFTER_CURRENT_SECTION = 1,
        BEGINNING_OF_BOOK = 2,
        END_OF_BOOK = 3,
        CURRENT_PAGE = 4
    };

    /**
    * \brief Default Constructor, constructs a blank book.
    */
    Book();

    /**
    * \brief Default Constructor, constructs a book.
    * \param title - the title of the book
    */
    Book( std::string title );

    /**
    * \brief Default Deconstructor, ensures memory is cleared when destroyed
    */
    ~Book();

    static Book* create( std::string title, std::string section_name = "default" );

    /*************************************************
    * Setters
    **************************************************/

    void setCurrentSection( std::string section_name );

    /**
    * \brief updates the modified date object to current time
    */
    void setTitle( std::string new_title );

    void setId( unsigned int id );



    /**
    * \brief updates the modified date object to current time
    */
    void updateModifiedDate();
    /*************************************************
    * Getters
    **************************************************/
    /**
    * \brief returns all the book marks for the book
    * \returns std::map< unsigned int (page number), Bookmark* (ptrs of bookmarks)
    */
    std::map< unsigned int, Bookmark* > getBookmarkList();

    /**
    * \brief gets a bookmark for a single page
    * \param page_number - page number of page
    */
    Bookmark* getBookmarkForPage( unsigned int page_number );

    /**
    * \brief returns the book's creation date object
    * \return Date object
    */
    Date getCreationDate();

    /**
    * \brief returns the book's modified date object
    * \return Date object
    */
    Date getModifiedDate();

    /**
    * \brief returns the current page number
    * \return current page, unsigned integer
    */
    unsigned int getCurrentPage();

    /**
    * \brief returns current section of book
    * \return std::string, title of section
    */
    const std::string getCurrentSection() const;

    /**
    * \brief returns the current pages that are opened in the book, which are 2-3 in either direction of the current page
    * \return std::deque< Page* (ptr of pages )
    */
    std::deque< Page* > getOpenedPages();

    /**
    * \brief returns sections in book
    * \return std::map< unsigned int, std::string > - page number, section title
    */
    std::map< unsigned int, std::string > getSectionListing();

    /**
    * \brief returns size of book
    * \return unsigned int - size of book
    */
    const unsigned int size() const;

    const unsigned int getId() const;

    void editPage( std::deque< Container * > content, unsigned int page_number = 0 );

    /**
    * \brief returns title of the book
    * \return std::string, title of book
    */
    std::string getTitle();

    bool doesPageExist( unsigned int page_number );



    /*************************************************
    * Modifiers
    **************************************************/
    /**
    * \brief adds a bookmark to the book
    * \param title - the title of the bookmark
    * \param page_number (optional) - page number of bookmark
    */
    bool addBookmark( std::string title, unsigned int page_number = 0 );

    /**
    * \brief removes a bookmark from a page
    * \param page_number - the page number bookmark is on
    */
    void removeBookmark( unsigned int page_number );

    /**
    * \brief virtually adds a blank page at the end of book
    */
    void appendPage( std::string page_title = "untitled" );

    /**
    * \brief virtually adds a blank page at the beginning of book
    */
    void prependPage( std::string page_title = "untitled"  );

	void insertPageAfterCurrent( std::string page_title = "untitled" );
	
    /**
    * \brief adds a blank page at a given page number
    * \param page_number - the page number of new page
    */
    void insertPage( std::string page_title = "untitled", unsigned int page_number = 0, SectionInsertionLoc location = CURRENT_PAGE );

    /**
    * \brief Adds a page to book at given page_number with content
    * \param page_number - page number of the page
    * \param content - deque of container ids
    */
    void addPage( unsigned int page_number, std::deque< unsigned int > content );

    /**
    * \brief Adds a new section to the book
    * \param page_number - page number of the page
    * \param title - title of the section
    */
    bool addSection( std::string title, unsigned int page_id = 0, unsigned int page_number = 0 );

    bool removeSection( std::string title );

    /**
    * \brief virtually flips book to a page
    * \param new_page_number - the page to flip too
    */
    void jumpToPage( unsigned int new_page_number );

    const std::deque< Container * > getCurrentPageContainers();

    void jumpToSection( std::string name );

    /**
    * \brief virtually flips book to next page.
    */
    void nextPage();

    /**
    * \brief virtually flips book to previous page
    */
    void previousPage();

    /**
    * \brief removes a page by number
    * \param page_number - page number to remove
    */
    void removePage( unsigned int page_number = 0 );

    /**
    * \brief removes a section by page number
    * \param page_number - the page number to check for section
    */
    void removeSectionByPageNumber( unsigned int page_number );

    /**
    * \brief removes a section by title
    * \param page_number - the page number section is on
    */
    void removeSectionByName( std::string );
	
	void setSize( unsigned  int new_size );

    void print();

    void printBookmarks();

    void printSections();
	
	void fill();
	
	const unsigned int numSections() const;
protected:

private:

	void updateSectionsStartPage();

    /**
    * \brief clamps page number based on the size of book
    * \param page_number - page number to clamp
    * \return unsigned int : 1 if supplied page number was less than <= 0
    *                        book size + 1 if page number is greater than book size
    *                        else it returns the argument if it was good
    */
    unsigned int clampPageNumber( unsigned int page_number );


    /**
    * \brief clamps page number based on the size of book
    * \param page_number - page number to clamp
    * \return unsigned int : 1 if supplied page number was less than <= 0
    *                        book size if page number is greater than book size
    *                        else it returns the argument if it was good
    */
    unsigned int clampPageNumberInner( unsigned int page_number );

     /**
    * \brief clamps page number while wrapping around the book.
    * ... this is the book that never ends... it just goes on and on my friends~~~.
    * \param page_number - page number to clamp
    * \return unsigned int : valid page number
    */
    unsigned int clampPageNumberRoundRobin( unsigned int page_number );

    /**
    * \brief utility function which prints the deque of pages to the CLI
    */
    void updateViewablePageRange( unsigned int focused_page_number, Page* focus_page = NULL );


    bool doesSectionExist( std::string title );

    bool doesSectionExistsForPage();

    unsigned int getRelativePageNumber( unsigned int global_page_number );

private:

     std::string m_title; ///< title of the book

    unsigned int m_current_page_number; ///< current page we are viewing in the book, 0 based

    std::deque< Page * > m_open_pages; /// <current open pages in book

    std::deque< Section * > m_content; ///< Content of the book

    std::map< unsigned int, Bookmark* > m_bookmarks; ///< bookmarks in book

   // std::map< unsigned int, std::string > m_section_titles;



    unsigned int m_current_section_page_number; ///< current page number in section being viewed

    unsigned int m_unique_id; ///< unique id of the book

    unsigned int m_current_section;

    unsigned int m_size; ///< size of the book

    Date m_creation_date; ///< date book was created
    Date m_modified_date; ///< date book was last modified
};

#endif // BOOK_H_INCLUDED
