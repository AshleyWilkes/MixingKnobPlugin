/*
  ==============================================================================

    ScannedDirectoriesMap.cpp
    Created: 31 Jan 2019 9:34:20pm
    Author:  Ashley

  ==============================================================================
*/

#include "ScannedDirectoriesMap.h"

ScannedDirectoriesMap::PluginsList
ScannedDirectoriesMap::getPluginsListForDirectories(Array<File> directories)
{
	PluginsList result;
	for (auto dir : directories) {
		result.addArray(map_[dir.getFullPathName()]);
	}
	return result;
}

void
ScannedDirectoriesMap::set(File directory, KnownPluginList& foundPlugins)
{
	PluginsList entry;
	for (auto desc : foundPlugins) {
		entry.add(*desc);
	}
	map_.set(directory.getFullPathName(), entry);
}

bool
ScannedDirectoriesMap::contains(File directory) const
{
	return map_.contains(directory.getFullPathName());
}

void
ScannedDirectoriesMap::clear()
{
	map_.clear();
}

Array<File>
ScannedDirectoriesMap::keys() const
{
	Array<File> result;
	for (auto it = map_.begin(); it != map_.end(); ++it) {
		result.add(it.getKey());
	}
	return result;
}

XmlElement
ScannedDirectoriesMap::asXml(const Array<File>& usedDirectories) const
{
	XmlElement result{ "prescan" };
	//this doesn't compile -- ??maybe because HashMap lacks cbegin() and cend()??
	//for (auto/*Iterator*/ pair : map_) {
	//result.addChildElement(createDirectoryXml(pair));
	//}*/
	for (auto dir : usedDirectories) {
		result.addChildElement(createDirectoryXml(dir));
	}
	/*for (Iterator pair = map_.begin(); pair != map_.end(); ++pair) {
		result.addChildElement(createDirectoryXml(pair));
	}*/
	return result;
}

XmlElement *
ScannedDirectoriesMap::createDirectoryXml(File directory) const {
	XmlElement *result = new XmlElement("directory");
	auto dirName = directory.getFullPathName();
	result->setAttribute("name", dirName);
	for (auto plugin : map_[dirName]) {
		result->addChildElement(createPluginXml(plugin));
	}
	return result;
}

XmlElement *
ScannedDirectoriesMap::createPluginXml(PluginDescription pluginDescription) const
{
	XmlElement *result = new XmlElement("plugin");
	result->addChildElement(pluginDescription.createXml());
	return result;
}

void
ScannedDirectoriesMap::fromXml(const XmlElement& xml)
{
	map_.clear();
	for (int i = 0; i < xml.getNumChildElements(); ++i) {
		directoryFromXml(xml.getChildElement(i));
	}
}

void
ScannedDirectoriesMap::directoryFromXml(XmlElement *xml)
{
	//jsem tag directory
	//mam atribut name obsahujici platnou full cestu k existujicimu adresari
	//kazdy child element obsahuje popis jednoho pluginu
	String directoryName = xml->getStringAttribute("name");
	PluginsList pluginsList;
	for (int i = 0; i < xml->getNumChildElements(); ++i) {
		pluginsList.add(pluginFromXml(xml->getChildElement(i)));
	}
	map_.set(directoryName, pluginsList);
}

PluginDescription
ScannedDirectoriesMap::pluginFromXml(XmlElement* xml)
{
	//jsem tag plugin
	//mam prave jednoho childa a ten umi inicializovat PluginDescription
	PluginDescription result;
	XmlElement *pluginXmlPtr = xml->getChildElement(0);
	result.loadFromXml(*pluginXmlPtr);
	return result;
}