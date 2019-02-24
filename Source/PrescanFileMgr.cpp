/*
  ==============================================================================

    PrescanFileMgr.cpp
    Created: 2 Feb 2019 4:29:09pm
    Author:  Ashley

  ==============================================================================
*/

#include "PrescanFileMgr.h"
#include "MixingKnobPluginProcessor.h"

PrescanFileMgr::PrescanFileMgr(File file, ScannedDirectoriesMap& map, MixingKnobPluginAudioProcessor& processor) :
	file_{ file }, map_{ map }, processor_{ processor } {
	processor.addChangeListener(this);

	File logFile{ file.getSiblingFile("log.txt") };
	logFile.replaceWithText("");
	FileLogger log(logFile, "");
	Logger::setCurrentLogger(&log);
	Logger::writeToLog("reading prescan file");

	XmlDocument xmlDoc{ file };
	auto xmlEle{ xmlDoc.getDocumentElement() };
	try {
		auto error = xmlDoc.getLastParseError();
		if (error.isEmpty()) {
			map_.fromXml(*xmlEle);
		}
		Logger::writeToLog("prescan file loaded");
	}
	catch (std::exception& e) {
		Logger::writeToLog("prescan lofing error");
	}
	delete xmlEle;
}

void
PrescanFileMgr::changeListenerCallback(ChangeBroadcaster* /*changeBroadcaster*/) {
	auto xmlEle = map_.asXml(processor_.getUsedDirectories());
	xmlEle.writeToFile(file_, String());
}