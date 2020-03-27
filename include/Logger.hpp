/*
 * =====================================================================================
 *
 *       Filename:  Logger.hpp
 *
 *    Description:  Used to more easily log things
 *
 *        Version:  1.0
 *        Created:  03/27/2020 01:08:34 PM
 *       Revision:  none
 *
 *         Author:  Samuel Knoethig (), samuel@knoethig.net
 *
 * =====================================================================================
 */

#include <ostream>
#include <iostream>
#include <unordered_set>
#include <functional>
#include <chrono>

namespace Logger {
	/*
	 * Forward declares
	 */
	template <typename eDebugChannel>
	struct Logger;

	template <typename eDebugChannel>
	struct LoggerHelper;

	template <typename eDebugChannel>
	struct PartialLoggerHelper;

	/**
	 *	Helper class to allow log << logchannel << message;
	 */
	template <typename eDebugChannel>
	struct LoggerHelper {
		public:
			/**
			 *	Constructor
			 *	@param logger A reference to the logger that created this class
			 *	@param channel The channel on which data was recieved
			 *	@param loglevel The current loglevel
			 *	@param noop True if data should be ignored
			 */
			LoggerHelper( Logger<eDebugChannel>& logger, eDebugChannel channel, size_t loglevel, bool noop ): logger( logger ), channel( channel ), loglevel( loglevel ), noop( noop ){}

			/**
			 *	Writes data
			 *	@param m The data to be written
			 *	@return this
			 */
			template <typename Message>
			std::enable_if_t<!std::is_same_v<eDebugChannel, Message>, LoggerHelper>
			operator<<( Message&& m ){
				if( !noop )
					logger.out << m;
				return *this;
			}

			/**
			 *	Recreates the helper class to acommodate for writing to a new channel
			 *	@param c The new channel
			 *	@return A new helper class instance that expects a new Loglevel
			 */
			template <typename Message>
			std::enable_if_t<std::is_same_v<eDebugChannel, Message>, PartialLoggerHelper<eDebugChannel>>
			operator<<( Message c ){
				return logger << c;
			}

		private:
			Logger<eDebugChannel>& logger;	/**< Reference to the original logger class */
			eDebugChannel channel;			/**< Channel written to */
			size_t loglevel;				/**< Loglevel */
			bool noop;						/**< If true all writes get treated as noop */
	};

	/**
	 *	Helper class...
	 */
	template <typename eDebugChannel>
	struct PartialLoggerHelper {
		public:
			/**
			 *	Constructor
			 *	@param logger A reference to the object that created this object
			 *	@param channel The channel that data is recieved from
			 */
			PartialLoggerHelper( Logger<eDebugChannel>& logger, eDebugChannel channel ): logger( logger ), channel( channel ){}

			/**
			 *	Creates a LoggerHelper and starts printing to the console
			 *	@param l The loglevel that should be used
			 *	@return A new LoggerHelper
			 */
			template <typename LogLevelType>
			std::enable_if_t<std::is_convertible_v<LogLevelType, size_t>, LoggerHelper<eDebugChannel>>
			operator<<( LogLevelType l ){
				bool no_write = static_cast<size_t>( l ) < logger.min_log_level || !logger.enabled_debug.contains( channel );

				if( !no_write ){
					std::time_t now;
					time( &now );
					struct tm * timeinfo = localtime( &now );
					char buf[20];
					strftime( buf, 20, "%F %T", timeinfo );
					logger.out << "\n[" << logger.loglevel_to_string( l ) << "] " << buf << " " << logger.channel_to_string( channel ) << ": ";
				}

				return LoggerHelper<eDebugChannel>( logger, channel, static_cast<size_t>( l ), no_write );
			}

			/**
			 *	Switches the channel to the newly requested one
			 *	@param c The new channel
			 *	@return this
			 */
			template <typename LogLevelType>
			std::enable_if_t<std::is_same_v<std::remove_cvref_t<LogLevelType>, std::remove_cvref_t<eDebugChannel>>, PartialLoggerHelper&> operator<<( LogLevelType c ){
				channel = c;
				return *this;
			}

		private:
			Logger<eDebugChannel>& logger;	/**< A reference to the original class */
			eDebugChannel channel;			/**< The channel on which data will be recieved */
	};

	/**
	 *	Base struct that does everything
	 */
	template <typename eDebugChannel>
	struct Logger {
		/**
		 *	Constructor
		 *	@param os The stream that should be written to (defaults to std::cout)
		 *	@param loglevel_to_string A method that should be used to turn loglevels into strings (Defaults to integer representation)
		 *	@param channel_to_string A method that should be used to turn channels into strings( Defaults to integer representation)
		 */
		Logger( std::ostream& out = std::cout,
			std::function<std::string( eDebugChannel )> channel_to_string =
				[]( eDebugChannel e ){ return std::to_string( static_cast<std::underlying_type_t<eDebugChannel>>( e ));},
			std::function<std::string( size_t )> loglevel_to_string =
				[]( size_t l ){ return std::to_string( l );})
			: out( out ), enabled_debug(), min_log_level(), channel_to_string( channel_to_string ), loglevel_to_string( loglevel_to_string ){}

		/**
		 *	Deconstructor
		 *	Writes a newline
		 */
		~Logger(){
			out << "\n";
		}

		/**
		 *	Creates a new PartialLoggerHelper used to write things
		 *	@param channel The channel on which data will be recieved
		 *	@return A new instance of a PartialLoggerHelper
		 */
		PartialLoggerHelper<eDebugChannel> operator<<( const eDebugChannel& channel ){
			return PartialLoggerHelper<eDebugChannel>( *this, channel );
		}

		/**
		 *	Enables a channel to be passed to out
		 *	@param e The channel that should be enabled
		 */
		void enable( eDebugChannel e ){
			enabled_debug.insert( e );
		}

		/**
		 *	Disables a channel
		 *	@param e The channel that should no longer be written to out
		 */
		void disable( eDebugChannel e ){
			enabled_debug.erase( e );
		}

		std::ostream& out;									/**< The ostream to which the messages should be written */
		std::unordered_set<eDebugChannel> enabled_debug;	/**< All channels that should be written to out. */
		size_t min_log_level;								/**< The loglevel below which messages should be ignored */
		std::function<std::string( eDebugChannel )> channel_to_string;	/**< Used to convert channels to strings */
		std::function<std::string( size_t )> loglevel_to_string;		/**< Used to turn loglevels to strings */
	};
}
