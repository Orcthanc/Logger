/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Short file to test the logger
 *
 *        Version:  1.0
 *        Created:  03/27/2020 01:07:48 PM
 *       Revision:  none
 *
 *         Author:  Samuel Knoethig (), samuel@knoethig.net
 *
 * =====================================================================================
 */

#include "Logger.hpp"

enum class eChannels {
	channelDefault,
	channelAudio,
};

int main( int argc, char** argv ){
	Logger::Logger<eChannels> log;

	log.enable( eChannels::channelDefault );

	log << eChannels::channelAudio << 3 << "hidden";
	log << eChannels::channelDefault << 4 << "shown";

	log.enable( eChannels::channelAudio );

	log << eChannels::channelAudio << 2 << "shown" << eChannels::channelDefault << 1 << "shown";
	log << eChannels::channelAudio << eChannels::channelDefault << 4 << "shown";

	log.disable( eChannels::channelAudio );
	log << eChannels::channelAudio << 3 << "hidden";
}
