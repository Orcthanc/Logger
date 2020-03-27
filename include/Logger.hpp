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

namespace Logger {
	/**
	 *	Base struct that does everything
	 */
	template <typename eDebugChannels>
	struct Logger {
		Logger( std::ostream& os = std::cout ): out( os ), enabled_debug(), min_log_level(){}
		
		std::ostream& out;									/**< The ostream to which the messages should be written */	
		std::unordered_set<eDebugChannels> enabled_debug;	/**< All channels that should be written to out. */
		size_t min_log_level;								/**< The loglevel below which messages should be ignored */
	};
}
