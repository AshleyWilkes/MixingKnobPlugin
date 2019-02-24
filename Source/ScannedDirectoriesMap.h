/*
  ==============================================================================

    ScannedDirectoriesMap.h
    Created: 31 Jan 2019 9:34:20pm
    Author:  Ashley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ScannedDirectoriesMap {
public:
	using PluginsList = Array<PluginDescription>;
	using Map = HashMap<String, PluginsList>;
	using Iterator = Map::Iterator;

	PluginsList getPluginsListForDirectories(Array<File> directories);

	void set(File directory, KnownPluginList& foundPlugins);

	bool contains(File directory) const;

	void clear();

	Array<File> keys() const;

	XmlElement asXml(const Array<File>& usedDirectories) const;
	void fromXml(const XmlElement& xml);
private:
	Map map_;

	//XmlElement* createDirectoryXml(Iterator pair);
	XmlElement* createDirectoryXml(File directory) const;
	XmlElement* createPluginXml(PluginDescription pluginDescription) const;

	void directoryFromXml(XmlElement* xml);
	PluginDescription pluginFromXml(XmlElement* xml);
};