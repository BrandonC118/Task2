#pragma once
#include <iostream>
#include <exception>
using namespace std;

struct DDLNotfoundException : public std::exception
{
	const char * what() const throw ()
	{
		return "DDL not found exception";
	}
};

struct InvalidSocketException : public std::exception
{
	const char * what() const throw ()
	{
		return "Invalid Socket exception";
	}
};

struct FailedToBindException : public std::exception
{
	const char * what() const throw ()
	{
		return "Unable to bind exception";
	}
};

struct UnableToListenOnSocketException : public std::exception
{
	const char * what() const throw ()
	{
		return "Unable to listen on socket exception";
	}
};

struct FailedToRecieveException : public std::exception
{
	const char * what() const throw ()
	{
		return "Failed to recieve message exception";
	}
};

struct FailedToSendException : public std::exception
{
	const char * what() const throw ()
	{
		return "Failed to send message exception";
	}
};

struct FailedToAcceptViaSocketException : public std::exception
{
	const char * what() const throw ()
	{
		return "Failed to accept with given socket exception";
	}
};

struct FailedToConnectToServerException : public std::exception
{
	const char * what() const throw ()
	{
		return "Failed to Connect to server with given socket exception";
	}
};