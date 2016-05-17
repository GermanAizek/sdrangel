///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2015 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_BLADERFPLUGIN_H
#define INCLUDE_BLADERFPLUGIN_H

#include <QObject>
#include "plugin/plugininterface.h"

class PluginAPI;

#define BLADERF_DEVICE_TYPE_ID "sdrangel.samplesource.bladerf"

class BlderfPlugin : public QObject, public PluginInterface {
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
	Q_PLUGIN_METADATA(IID BLADERF_DEVICE_TYPE_ID)

public:
	explicit BlderfPlugin(QObject* parent = NULL);

	const PluginDescriptor& getPluginDescriptor() const;
	void initPlugin(PluginAPI* pluginAPI);

	virtual SampleSourceDevices enumSampleSources();
	virtual PluginGUI* createSampleSourcePluginGUI(const QString& sourceId, QWidget **widget, DeviceAPI *deviceAPI);

	static const QString m_deviceTypeID;

private:
	static const PluginDescriptor m_pluginDescriptor;
};

#endif // INCLUDE_BLADERFPLUGIN_H
