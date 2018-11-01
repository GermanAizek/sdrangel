///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
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

#include "util/simpleserializer.h"

#include "device/devicesourceapi.h"
#include "device/devicesinkapi.h"
#include "dsp/dspcommands.h"
#include "dsp/filerecord.h"
#include "dsp/dspengine.h"
#include "soapysdr/devicesoapysdr.h"

#include "soapysdrinputthread.h"
#include "soapysdrinput.h"

SoapySDRInput::SoapySDRInput(DeviceSourceAPI *deviceAPI) :
    m_deviceAPI(deviceAPI),
    m_thread(0),
    m_deviceDescription("SoapySDRInput"),
    m_running(false)
{
    openDevice();
}

SoapySDRInput::~SoapySDRInput()
{
}

void SoapySDRInput::destroy()
{
    delete this;
}

bool SoapySDRInput::openDevice()
{
    if (!m_sampleFifo.setSize(96000 * 4))
    {
        qCritical("SoapySDRInput::openDevice: could not allocate SampleFifo");
        return false;
    }
    else
    {
        qDebug("SoapySDRInput::openDevice: allocated SampleFifo");
    }

    // look for Rx buddies and get reference to the device object
    if (m_deviceAPI->getSourceBuddies().size() > 0) // look source sibling first
    {
        qDebug("SoapySDRInput::openDevice: look in Rx buddies");

        DeviceSourceAPI *sourceBuddy = m_deviceAPI->getSourceBuddies()[0];
        DeviceSoapySDRShared *deviceSoapySDRShared = (DeviceSoapySDRShared*) sourceBuddy->getBuddySharedPtr();

        if (deviceSoapySDRShared == 0)
        {
            qCritical("SoapySDRInput::openDevice: the source buddy shared pointer is null");
            return false;
        }

        SoapySDR::Device *device = deviceSoapySDRShared->m_device;

        if (device == 0)
        {
            qCritical("SoapySDRInput::openDevice: cannot get device pointer from Rx buddy");
            return false;
        }

        m_deviceShared.m_device = device;
        m_deviceShared.m_deviceParams = deviceSoapySDRShared->m_deviceParams;
    }
    // look for Tx buddies and get reference to the device object
    else if (m_deviceAPI->getSinkBuddies().size() > 0) // then sink
    {
        qDebug("SoapySDRInput::openDevice: look in Tx buddies");

        DeviceSinkAPI *sinkBuddy = m_deviceAPI->getSinkBuddies()[0];
        DeviceSoapySDRShared *deviceSoapySDRShared = (DeviceSoapySDRShared*) sinkBuddy->getBuddySharedPtr();

        if (deviceSoapySDRShared == 0)
        {
            qCritical("SoapySDRInput::openDevice: the sink buddy shared pointer is null");
            return false;
        }

        SoapySDR::Device *device = deviceSoapySDRShared->m_device;

        if (device == 0)
        {
            qCritical("SoapySDRInput::openDevice: cannot get device pointer from Tx buddy");
            return false;
        }

        m_deviceShared.m_device = device;
        m_deviceShared.m_deviceParams = deviceSoapySDRShared->m_deviceParams;
    }
    // There are no buddies then create the first SoapySDR device
    else
    {
        qDebug("SoapySDRInput::openDevice: open device here");
        DeviceSoapySDR& deviceSoapySDR = DeviceSoapySDR::instance();
        m_deviceShared.m_device = deviceSoapySDR.openSoapySDR(m_deviceAPI->getSampleSourceSequence());

        if (!m_deviceShared.m_device)
        {
            qCritical("BladeRF2Input::openDevice: cannot open BladeRF2 device");
            return false;
        }

        m_deviceShared.m_deviceParams = new DeviceSoapySDRParams(m_deviceShared.m_device);
    }

    m_deviceShared.m_channel = m_deviceAPI->getItemIndex(); // publicly allocate channel
    m_deviceShared.m_source = this;
    m_deviceAPI->setBuddySharedPtr(&m_deviceShared); // propagate common parameters to API
    return true;
}

void SoapySDRInput::closeDevice()
{
    if (m_deviceShared.m_device == 0) { // was never open
        return;
    }

    if (m_running) {
        stop();
    }

    if (m_thread) { // stills own the thread => transfer to a buddy
        moveThreadToBuddy();
    }

    m_deviceShared.m_channel = -1; // publicly release channel
    m_deviceShared.m_source = 0;

    // No buddies so effectively close the device and delete parameters

    if ((m_deviceAPI->getSinkBuddies().size() == 0) && (m_deviceAPI->getSourceBuddies().size() == 0))
    {
        delete m_deviceShared.m_deviceParams;
        m_deviceShared.m_deviceParams = 0;
        DeviceSoapySDR& deviceSoapySDR = DeviceSoapySDR::instance();
        deviceSoapySDR.closeSoapySdr(m_deviceShared.m_device);
        m_deviceShared.m_device = 0;
    }
}

void SoapySDRInput::getFrequencyRange(uint64_t& min, uint64_t& max)
{
    const DeviceSoapySDRParams::ChannelSettings* channelSettings = m_deviceShared.m_deviceParams->getRxChannelSettings(m_deviceShared.m_channel);

    if (channelSettings && (channelSettings->m_frequencySettings.size() > 0))
    {
        DeviceSoapySDRParams::FrequencySetting freqSettings = channelSettings->m_frequencySettings[0];
        SoapySDR::RangeList rangeList = freqSettings.m_ranges;

        if (rangeList.size() > 0) // TODO: handle multiple ranges
        {
            SoapySDR::Range range = rangeList[0];
            min = range.minimum();
            max = range.maximum();
        }
        else
        {
            min = 0;
            max = 0;
        }
    }
    else
    {
        min = 0;
        max = 0;
    }
}

const SoapySDR::RangeList& SoapySDRInput::getRateRanges()
{
    const DeviceSoapySDRParams::ChannelSettings* channelSettings = m_deviceShared.m_deviceParams->getRxChannelSettings(m_deviceShared.m_channel);
    return channelSettings->m_ratesRanges;
}

void SoapySDRInput::init()
{
}

void SoapySDRInput::moveThreadToBuddy()
{
    const std::vector<DeviceSourceAPI*>& sourceBuddies = m_deviceAPI->getSourceBuddies();
    std::vector<DeviceSourceAPI*>::const_iterator it = sourceBuddies.begin();

    for (; it != sourceBuddies.end(); ++it)
    {
        SoapySDRInput *buddySource = ((DeviceSoapySDRShared*) (*it)->getBuddySharedPtr())->m_source;

        if (buddySource)
        {
            buddySource->setThread(m_thread);
            m_thread = 0;  // zero for others
        }
    }
}

bool SoapySDRInput::start()
{
    return false;
}

void SoapySDRInput::stop()
{
}

QByteArray SoapySDRInput::serialize() const
{
    SimpleSerializer s(1);
    return s.final();
}

bool SoapySDRInput::deserialize(const QByteArray& data __attribute__((unused)))
{
    return false;
}

const QString& SoapySDRInput::getDeviceDescription() const
{
    return m_deviceDescription;
}

int SoapySDRInput::getSampleRate() const
{
    return 0;
}

quint64 SoapySDRInput::getCenterFrequency() const
{
    return 0;
}

void SoapySDRInput::setCenterFrequency(qint64 centerFrequency __attribute__((unused)))
{
}

bool SoapySDRInput::handleMessage(const Message& message __attribute__((unused)))
{
    return false;
}