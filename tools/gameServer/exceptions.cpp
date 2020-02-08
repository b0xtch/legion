/**
 * This file holds custom exceptions
**/
#include <stdexcept>

struct SessionLimitExceeded : public std::exception
{
	const char * what () const throw ()
    {
    	return "Session Limit exceeded";
    }
};

struct SessionNotFound : public std::exception
{
	const char * what () const throw ()
    {
    	return "Session Not Found";
    }
};


struct ConnectionNotFound : public std::exception
{
	const char * what () const throw ()
    {
    	return "Connection Not Found";
    }
};

struct CouldNotSendMessage : public std::exception
{
	const char * what () const throw ()
    {
    	return "Could Not Send Message";
    }
};

struct SERVERLIMITREACHED : public std::exception
{
	const char * what () const throw ()
    {
    	return "Could not create a new session. Server limit reached.";
    }
};

