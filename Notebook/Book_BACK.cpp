#include "Book.h"

Book::Book()
    : m_title( "untitled" ), m_current_page_number( 0 ), m_current_section( 0 ), m_size( 0 )
{
    m_creation_date = Date();
    m_modified_date = Date();
	
	addSection( "default" );
}

Book::Book( std::string title )
    : m_title( title ), m_current_page_number( 0 ),  m_current_section( 0 ),  m_size( 0 )
{
    m_creation_date = Date();
    m_modified_date = Date();
}

Book::~Book()
{
    m_content.clear();
    m_open_pages.clear();
    m_bookmarks.clear();
}

Book* Book::create( std::string title, std::string section_name /* = "default" */ )
{
    Book* new_book = new Book( title );
    new_book->addSection( section_name );

    return new_book;
}

void Book::nextPage()
{
    jumpToPage( m_current_page_number + ONE_PAGE );
}

void Book::previousPage()
{
    jumpToPage( m_current_page_number - ONE_PAGE );
}

void Book::jumpToPage( unsigned int page_number )
{
    // ensure page is clamped relative to book's size
    unsigned int temp_number = clampPageNumber( page_number );

    m_current_page_number = temp_number;

    // time to check if a 'next' page exists, if not one will be created

    //if( temp_number > m_content[ m_current_section ]->getSize() );
    //  insertPage( m_current_page_number + ONE_PAGE );

    // update the viewable page range based on new page number

    //
    m_current_page_number = temp_number;
    // updateViewablePageRange( m_current_page_number );
}

void Book::jumpToSection( std::string name )
{
    for( unsigned int i = 0, j = m_content.size(); i < j; ++i )
        if( m_content[i]->getTitle() == name )
            m_current_section = i;
    m_content[ m_current_section ]->setCurrentPage( 0 );

    m_current_page_number = m_content[ m_current_section ]->getStartPageNumber();

    updateViewablePageRange( m_current_page_number );
}

bool Book::addBookmark( std::string title, unsigned int page_id )
{
    // ensure bookmark doesn't already exist for this page
    if( m_bookmarks.find( page_id ) != m_bookmarks.end() )
        return false;

    // insert page id that bookmark is on... not the greatest solution, but it'll work for now
    m_bookmarks.insert( std::pair< unsigned int, Bookmark* >( page_id, new Bookmark( title ) ) );

    return true;
}

void Book::removeBookmark( unsigned int page_id )
{
    // ensure a bookmark exists for page id

    std::map< unsigned int, Bookmark* >::iterator iter;
    iter = m_bookmarks.find( page_id );

    // ensure bookmark exists
    if( iter == m_bookmarks.end() )
        return;

    // remove bookmark
    m_bookmarks.erase( iter );
}

bool Book::addSection( std::string section_title, unsigned int page_id, unsigned int page_number /* = 0 */ )
{
    // ensure section name doesn't already exist for book
    if( doesSectionExist( section_title ) )
        return false;

	//std::cout << "[Book:111] add section. Section did not already exist." << std::endl;
	
    // subtracting one as Book is "zero based", meaning it sees what we humans
    // see as Page 1 as Page 0.
    int num_sections =  m_content.size();
	
	//std::cout << "[Book addSection] Number of sections currently in book: " << num_sections << std::endl;
    unsigned int index = 0;
    unsigned int page_loc = 0;

    if( page_number == 0 )
        page_number = m_current_page_number;

    if( num_sections > 0 )
    {
        // if num_sections < 0, then there is no sections in book, so we can skip
        // to just inserting the section
        for( index = num_sections; index >= 0; --index )
        {
            if( m_content[ index - ONE_PAGE ]->getStartPageNumber() > page_number )
            {
                // section @ index comes after where new section will be
                // inserted, so move its starting page back 1

                m_content[ index - ONE_PAGE ]->setStartPageNumber( m_content[ index - ONE_PAGE ]->getStartPageNumber() + ONE_PAGE );
            }
            else
            {
                // index is where we'll be inserting the new section, so let's break
                page_loc = m_content[ index - ONE_PAGE ]->getStartPageNumber() + m_content[ index - ONE_PAGE ]->getSize();
                break;
            }
        }
        m_current_section += 1;
    }
	
    m_content.insert( m_content.begin() + index, new Section( section_title, page_loc, std::deque< std::string >( page_id ) ) );
   
	//std::cout << "[Book addSection] Number of sections currently in book: " << m_content.size() << std::endl;
   //m_content[ index ]->insertPage( 0 );

    m_current_page_number = m_content[ index ]->getStartPageNumber();


    return true;
}

bool Book::removeSection( std::string title )
{
    for( unsigned int i = 0, end = m_content.size() - 1; i < end; ++i )
    {
        if( m_content[ i ]->getTitle() == title )
        {
            // clear out section
            std::cout << m_content[i]->getSize();
            for( unsigned int j = m_content.size() - 1; j > i; --j )
                m_content[ j ]->adjustStartPageNumber( ( m_content[i]->getSize() ), Section::SUBTRACT );

            m_content[ i ]->clear();
            m_content.erase( m_content.begin() + i );

           // m_current_section =
            return true;
        }
    }
    return false;
}
/*
Bookmark* Book::getBookmarkForPage( unsigned int page_number )
{
std::map< unsigned int, Bookmark* >::iterator iter;
iter = m_bookmarks.find( page_number );

// ensure bookmark exists
if( iter == m_bookmarks.end() )
return NULL;

return iter->second;
} */

unsigned int Book::getCurrentPage()
{
    return m_current_page_number;
}

std::deque< Page* > Book::getOpenedPages()
{
    return m_open_pages;
}

void Book::appendPage( std::string page_title )
{
    insertPage( page_title, m_size + 1 );
}

void Book::prependPage( std::string page_title )
{
    insertPage( page_title, 0 );
}

void Book::insertPageAfterCurrent( std::string page_title /* = "untitled" */ )
{
	insertPage( page_title, m_current_page_number + 1 );

	//updateSectionsStartPage();
}
/*
//                        _______...
//                   .-‘”..........``~.
//               ,.-”..................“-.
//             ,/........................”:
//           ,?............................\
//          /..............................,}
//         /...........................,:`^`.}
//        /..........................,:”..../
//      ?...__......................:`...../
//       /__.(...“~-,_.............,:`...../
//      /(_..”~,_....“~,_..........:`...._/
//     {._$;_...”=,_....“-,_...-~/~},.~”/.}
//      ((...*~_....”=-._...“;,./`./”...../
//      \`~,...“~.,..........`...}....../
//      (..`=-,,....`............(...\_,-”
//      /.`~,...`-................\../^\
//       \`~.*-,...................|/...\,__
//,,_     }.>-._\..................|........`=~-,.. 
//...`=~-,_\_...`\,.................\..............
//..........`=~-,,.\,................\.............
//................`:,,..............`\.......__....
//...................`=-,..........,%`>--==``......
//...................._\......_,-%....`\...........
//..................,<`._|_,-&``........`\.........
*/

void Book::insertPage( std::string page_title, unsigned int page_number, SectionInsertionLoc location )
{
    if( page_number == 0 )
        page_number = m_current_page_number;

    // to insert a page, we first need to  compute the local page number
    // in section for the page. To do this will subtract the current section's
    // starting page number from the page number provided
    // this will result in a local page_number for the current section
    ///****Commented out to get rid of warning****
    unsigned int local_page_number = page_number - m_content[ m_current_section ]->getStartPageNumber();

    // next create the page itself
	Page* new_page = Page::create( Page::LINED, page_title );
	

	// next we must assign this page to a section, in the correct location mind you!
	m_content[ m_current_section ]->insertPage(  local_page_number, page_title + new_page->getCreationDate().getFullDateUnderscores() );
	
	// Now each section's start position (page number) in the book needs to be updated
    // afer the current one accordingly...
	updateSectionsStartPage();

	// next, le
	
  //  unsigned int new_page_id = m_content[ m_current_section ]->insertPage( m_content[ m_current_section ]->getCurrentPage() );

  //  Page* new_page = Page::create( Page::LINED, new_page_id, page_title );

   // if( ResourceMapper::getInstance()->registerPage( m_content[ m_current_section]->getTitle(),
  //     page_title + "-" + new_page->getCreationDate().getFullDateUnderscores() ) != -1 )
  //  {
         //page registered successfully!
        // update viewable page range
       // updateViewablePageRange();
 //  }


    // still need to update viewable range
}

void Book::updateSectionsStartPage()
{
	// only do this if we have 2 or more sections in book...
	if( m_content.size() > 1 )
    {
		// loop over all sections starting from the rear
        for( unsigned int index = ( m_content.size() - 1 ); index > m_current_section; --index )
        {
			// if section[i] start page is less than current_page, then the job is done
            if( m_content[ index ]->getStartPageNumber() <= m_current_page_number )
                break;
			// else increment the section's starting point by one page
            m_content[ index ]->setStartPageNumber( m_content[ index ]->getStartPageNumber() + ONE_PAGE );
        }
    }
}


bool Book::doesPageExist( unsigned int page_number )
{
    return ( clampPageNumberInner( page_number ) == page_number );
}

void Book::setId( unsigned int id )
{
    m_unique_id = id;
}

void Book::removePage( unsigned int page_number )
{
    if( page_number == 0 )
        page_number = m_current_page_number - m_content[ m_current_section ]->getStartPageNumber();
    else
        page_number = page_number - m_content[ m_current_section ]->getStartPageNumber();

	//m_content[ m_current_section ]->removePage( page_number );
	/*(
    unsigned int p_id = m_content[ m_current_section ]->getPageId( page_number - m_content[ m_current_section ]->getStartPageNumber() );
    unsigned int local_pg_number = page_number - m_content[ m_current_section ]->getStartPageNumber();

    for( unsigned int i = 0; i < m_open_pages.size(); ++i )
    {

        if( m_open_pages[i]->getId() == p_id )
        {
            m_content[ m_current_section]->deletePage( local_pg_number );
            break;
        }

    }

    m_content[ m_current_section ]->deletePage( page_number ); */

}

void Book::editPage( std::deque< Container * > content, unsigned int page_number /* = 0 */ )
{
    if( page_number == 0 )
        page_number = m_current_page_number;

  //  m_open_pages[ 0 ]->setContent( content );
}

const std::deque< Container * > Book::getCurrentPageContainers()
{
    return m_open_pages[ 0 ]->getContent();
}

unsigned int Book::clampPageNumber( unsigned int page_number )
{
    if( page_number < 1 || m_size == 0 )
        return 0; // return first page

    if( page_number > m_size )
        return m_size + ONE_PAGE; // return 1 more than book size

    return page_number; // return original argument
}

unsigned int Book::clampPageNumberInner( unsigned int page_number )
{
    if( page_number < 0 || m_size == 0 )
        return 0; // return first page

    if( page_number > m_size )
        return m_size; // return 1 more than book size

    return page_number; // return original argument
}

unsigned int Book::clampPageNumberRoundRobin( unsigned int page_number )
{
    if( m_size < page_number )
        return ( page_number - m_size );
    else if( page_number < 0 )
        return ( page_number + m_size );

    return page_number;
}

// 3 pages in each direction around current page
void Book::updateViewablePageRange( unsigned int focused_page_number, Page* focus_page )
{
    m_current_page_number = focused_page_number;
    // if no page was passed, then we'll assume
    // std::deque< Page* > new_active_pages;

    /*   unsigned int lower_bound = clampPageNumberRoundRobin( m_current_page_number - 3 );
    unsigned int upper_bound = clampPageNumberRoundRobin( m_current_page_number + 3 );

    for( unsigned int i = lower_bound; lower_bound <= upper_bound; ++lower_bound )
    if( lower_bound > m_size )
    lower_bound = clampPageNumberRoundRobin( lower_bound );

    if( focus_page == NULL )
    {
    } */

    // build a new deque with the new current page in its center
    /*   std::deque< Page * > new_pages;

    new_pages.resize( 7 ); // this shouldn't be a magic number!

    // clamp bound so we stay within bounds
    // Do we want to roll back between the front and end of book instead if we
    // go out of bounds????
    unsigned int lower_bound = clampPageNumber( m_current_page_number - 3 );
    unsigned int upper_bound = clampPageNumber( m_current_page_number + 3 ); */

    // ensure bounds are good
}

void Book::setSize( unsigned int new_size )
{
	m_size = new_size;
}

const unsigned int Book::size() const
{
	return m_size;
}


void Book::fill()
{

}

void Book::print()
{
    /* int count = 1;
    for( std::deque< unsigned int >::const_iterator left_iter = m_pages.begin(), iend = m_pages.end();
    left_iter != iend; ++left_iter )
    {
    // left_iter->first  : key  : int
    // left_iter->second : data : std::string

    std::cout << "page: " << count++ << " id: " << *left_iter << std::endl;
    } */
}

void Book::printBookmarks()
{
    /*int count = 1;
    for( std::map< unsigned int, Bookmark* >::const_iterator left_iter = m_bookmarks.begin(), iend = m_bookmarks.end();
    left_iter != iend; ++left_iter )
    {
    // left_iter->first  : key  : int
    // left_iter->second : data : std::string

    std::cout << "page: " << count++ << " id: " << left_iter->first << " title: " << left_iter->second->getTitle() << std::endl;
    } */
}

void Book::printSections()
{
    std::cout << std::endl << "Print sections for book: " << m_title << std::endl << std::endl;
    for( unsigned int i = 0; i < m_content.size(); ++i )
    {
        std::cout << "index: " << i << std::endl;
        m_content[i]->print();
    }
}

void Book::setCurrentSection( std::string section_name )
{
    for( unsigned int i = 0, end = m_content.size(); i < end; i++ )
        if( m_content[i]->getTitle() == section_name )
        {
            m_current_section = i;
            break;
        }
}

const std::string Book::getCurrentSection() const
{
    return m_content[ m_current_section ]->getTitle();
}

bool Book::doesSectionExist( std::string title )
{
    std::deque< Section * >::const_iterator iter_end = m_content.end();
    for( std::deque< Section * >::const_iterator iter = m_content.begin(); iter != iter_end; ++iter )
        if( (*iter)->getTitle() == title )
            return true;
    return false;
}

bool Book::doesSectionExistsForPage()
{
    for( unsigned int i = 0, j = m_content.size(); i < j; ++i )
        if( m_content[i]->getStartPageNumber() == ( m_current_page_number ) )
            return true;
    return false;
}

void Book::updateModifiedDate()
{
    m_modified_date = Date();
}

Date Book::getModifiedDate()
{
    return m_modified_date;
}

Date Book::getCreationDate()
{
    return m_creation_date;
}

std::string Book::getTitle()
{
    return m_title;
}

const unsigned int Book::getId() const
{
    return m_unique_id;
}

const unsigned int Book::numSections() const
{
	return m_content.size();
}
