#include <dirent.h>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>

#include "FileSystem.h"

FileSystem::FileSystem(std::string appRootPath) :
    m_root( appRootPath )
{

}

FileSystem::~FileSystem()
{
}

void FileSystem::checkBookDirectoryIsWellFormed( std::string book_root )
{

	// ensure the book's folder exists exists
    if( !dirExists( book_root ) )
    {
		std::cout << book_root << ": does not exist" << std::endl;
		createDir( book_root );
	}

	// create metadata and page.xml files if they don't exist.
	createFileIfNotExists( book_root + "metadata.xml" );
	createFileIfNotExists( book_root + "phage.xml" );

	// create settings.xml and index.xml files
	//	std::cout << "derp" << app_root << std::endl;
		//std::cout << "book root did not exist" << std::endl;
     //   bfs::create_directories( temp );

	/*// ensure index.xml exists
	temp += "\\index.xml";
	//std::cout << "Temp: " << temp.string() << std::endl;
	if( exists( temp.string() ) )
	{
		std::ofstream( temp.string().c_str() );
	}

    temp = app_root;
    temp += "\\.backups";
    if( !exists( temp.string() ) )
    {
         bfs::create_directories( temp );
    } */
}

bool FileSystem::doesBookExistOnDisk( std::string book_root )
{
   // bfs::path temp( m_root );
    //temp += "\\books\\" + name;

	return ( dirExists( book_root ) && exists( book_root + "\\metadata.xml" ) && exists( book_root + "\\phage.xml" ) );
}

bool FileSystem::createBookOnDisk( std::string book_root )
{
   // if( doesBookExistOnDisk( name ) )
     //   return NULL;

	//bfs::path temp(
    //bfs::path temp( m_root.string() + "\\books\\" + name );

    //bfs::create_directory( book_root );
  //  bfs::create_directory( book_root + "\\pages" );
   // bfs::create_directory( temp.string() + "\\media" );

	createDir( book_root );

	createFileIfNotExists( book_root + DIRECTORY_SEPERATOR + "metadata.xml" );
	createFileIfNotExists( book_root + DIRECTORY_SEPERATOR + "phage.xml" );
	return true;

   // return loadBookFromDisk( name );
}

bool FileSystem::deleteBook( std::string name )
{
    if( !doesBookExistOnDisk( name ) )
        return false;


    bfs::remove_all( m_root.string() + "/books/" + name );

    return true;
}

void FileSystem::modifyPage( std::string book_name, unsigned int page_number, std::deque< Container * > content )
{
   // bfs::path temp( m_root.string() + "\\books\\" + book_name + "\\pages" +  );
}
bool FileSystem::bookDirectoryIsWellFormed( bfs::path path_to_book )
{
    // make sure the book exists!
    if( !bfs::exists( path_to_book ) )
    {
    //    std::cout << "created directory for: " << path_to_book << std::endl;
        bfs::create_directory( path_to_book );
    }

    bfs::path p( path_to_book );
    p += "/index.xml";

    // make sure it contains index.xml file
    if( !bfs::exists( p ) )
    {
        // if index.xml didn't exist, what do we do???
        std::ofstream( p.string().c_str() );
      //  std::cout << "book index.xml did not exist! for: " << path_to_book << std::endl;
        // return false;
    }

    // now ensure it contains a page folder
    p = path_to_book;
    p += "/pages";
    if( !bfs::exists( p ) )
    {
        std::cout << "pages folder did not exist for: " << path_to_book << std::endl;
        bfs::create_directory( p );

        // if( !bfs::create_directory( p ) )
        //    return false;
    }

    // now ensure it contains a media folder
    p = path_to_book;
    p += "/media";
    if( !bfs::exists( p ) )
    {
        std::cout << "media folder did not exist for: " << path_to_book << std::endl;
        if( bfs::create_directory( p ) )
            return false;
    }

    return true;
}
/*
bool FileSystem::registerBook( std::string title )
{
    bfs::path temp_path( m_root );
    temp_path += "\\books\\" + title;

    return bookDirectoryIsWellFormed( temp_path );
}

Book* FileSystem::loadBookIntoMemory( std::string name )
{
    // ensure the book exists
    bfs::path path( m_root );
    path += "\\notes\\" + name;

    if( !bookDirectoryIsWellFormed( path ) )
    {
        std::cout << "the book directory was not formed correctly! Probably index.XML was missing!" << std::endl;
    }

    // book directory is good, lets build an actual book object!
     Book * new_book = new Book( name );

    // read the index.xml file
    std::string index_loc = path.string() + "\\index.xml";

	// xmlParser parser;
	// std::deque< Section * > book_content;
	// book_content = parser.readIndex( index_loc );
	// if( book_content

    // pass the parser location of index file and the new book, it'll fill in some stuff for us
	//new_book->fill( //
    // xmlParser::getInstance()->parseBookIndexFile( index_loc, &new_book );

    // now we have a book with its bookmark map filled as well as its page id/page number bipmap
    // we still need to setup which pages are currently open however

    // since we just loaded it, we know we're about to view page 1, so let's load the pages around it

    // get the range of the pages we need

    // for( unsigned int i = (new_book.getCurrentPage() - 3 )

    // return new_book
} */
/*
// Return true if the folder exists, false otherwise
bool folderExists(const char* folderName) {
    if (_access(folderName, 0) == -1) {
        //File not found
        return false;
    }

    DWORD attr = GetFileAttributes((LPCSTR)folderName);
    if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        // File is not a directory
        return false;
    }

    return true;
} */

bool FileSystem::dirExists(const std::string dir_path )
{
	// windows
	#if (defined _WIN32 || defined __WIN64)
	//DWORD attr = GetFileAttributes((LPCSTR)dir_path.c_str());
	if (!CreateDirectory(dir_path.c_str(), NULL) &&
		ERROR_ALREADY_EXISTS == GetLastError() )

	// linux/unix
	#else
	DIR* dir = opendir( dir_path.c_str() );
	//\todo fix this for unix
	if( true)//ENONENT == errno )
	#endif
	{

		std::cout << "Directory '" << dir_path << "' exists!" << std::endl;
		return true;
	}
	// if ENOENT == errno, then file did not exist
	else
	{
		#if __linux__  || __linux
		closedir( dir );
		#endif
		std::cout << "Directory did not exist!" << std::endl;
	}
	return false;
}

bool FileSystem::createDir( const std::string dir_path )
{
	#if (defined _WIN32 || defined __WIN64)
	if (CreateDirectory(dir_path.c_str(), NULL) == 0)
	{
	#else
	///\todo Linux
	if (true)
	{
	#endif
		return true;
    }
	return false;
}

// Returns false on success, true on error
bool FileSystem::createDirRecursively( const std::string dir_path )
{
	///\todo this method was commented out so the program could compile on linux
	/**
	// ensure dir doesn't already exist
	if( dirExists( dir_path ) )
		return false;

    std::list<std::string> folderLevels;
    char* c_str = (char*)dir_path.c_str();

    // Point to end of the string
    char* strPtr = &c_str[strlen(c_str) - 1];

    // Create a list of the folders which do not currently exist
    do {
        if (dirExists(c_str)) {
            break;
        }
        // Break off the last folder name, store in folderLevels list
        do {
            strPtr--;
        } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
        folderLevels.push_front(std::string(strPtr + 1));
        strPtr[1] = 0;
    } while (strPtr >= c_str);

    if (_chdir(c_str)) {
        return true;
    }

    // Create the folders iteratively
    for (std::list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
		#if (defined _WIN32 || defined __WIN64)
        if (CreateDirectory(it->c_str(), NULL) == 0) {
		#else
			// *nix here
		#endif
            return true;
        }
        _chdir(it->c_str());
    }
    */

    return false;
}

bool FileSystem::createFile( std::string path )
{
	try
	{
		return( std::ofstream( path.c_str() ) );
	}
	catch( ... )
	{
		return false;
	}
}

void FileSystem::createFileIfNotExists( std::string file_path )
{
	if( !exists( file_path ) )
		std::ofstream( file_path.c_str() );
}

bool FileSystem::exists( std::string path )
{
	std::ifstream file( path.c_str() );

	return(file);
}


/*

  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
} */
