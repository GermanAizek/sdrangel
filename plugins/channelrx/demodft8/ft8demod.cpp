///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2023 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <QTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QThread>
#include <QMutexLocker>

#include "SWGChannelSettings.h"
#include "SWGWorkspaceInfo.h"
#include "SWGFT8DemodSettings.h"
#include "SWGChannelReport.h"
#include "SWGFT8DemodReport.h"

#include "dsp/dspengine.h"
#include "dsp/dspcommands.h"
#include "dsp/devicesamplemimo.h"
#include "device/deviceapi.h"
#include "feature/feature.h"
#include "util/db.h"
#include "maincore.h"

#include "ft8demod.h"

MESSAGE_CLASS_DEFINITION(FT8Demod::MsgConfigureFT8Demod, Message)

const char* const FT8Demod::m_channelIdURI = "sdrangel.channel.ft8demod";
const char* const FT8Demod::m_channelId = "FT8Demod";

FT8Demod::FT8Demod(DeviceAPI *deviceAPI) :
        ChannelAPI(m_channelIdURI, ChannelAPI::StreamSingleSink),
        m_deviceAPI(deviceAPI),
        m_running(false),
        m_spectrumVis(SDR_RX_SCALEF),
        m_basebandSampleRate(0)
{
	setObjectName(m_channelId);

	applySettings(m_settings, true);

    m_deviceAPI->addChannelSink(this);
    m_deviceAPI->addChannelSinkAPI(this);

    m_networkManager = new QNetworkAccessManager();
    QObject::connect(
        m_networkManager,
        &QNetworkAccessManager::finished,
        this,
        &FT8Demod::networkManagerFinished
    );
    QObject::connect(
        this,
        &ChannelAPI::indexInDeviceSetChanged,
        this,
        &FT8Demod::handleIndexInDeviceSetChanged
    );

    start();
}

FT8Demod::~FT8Demod()
{
    QObject::disconnect(
        m_networkManager,
        &QNetworkAccessManager::finished,
        this,
        &FT8Demod::networkManagerFinished
    );
    delete m_networkManager;
	m_deviceAPI->removeChannelSinkAPI(this);
    m_deviceAPI->removeChannelSink(this);

    stop();
}

void FT8Demod::setDeviceAPI(DeviceAPI *deviceAPI)
{
    if (deviceAPI != m_deviceAPI)
    {
        m_deviceAPI->removeChannelSinkAPI(this);
        m_deviceAPI->removeChannelSink(this);
        m_deviceAPI = deviceAPI;
        m_deviceAPI->addChannelSink(this);
        m_deviceAPI->addChannelSinkAPI(this);
    }
}

uint32_t FT8Demod::getNumberOfDeviceStreams() const
{
    return m_deviceAPI->getNbSourceStreams();
}

void FT8Demod::feed(const SampleVector::const_iterator& begin, const SampleVector::const_iterator& end, bool positiveOnly)
{
    (void) positiveOnly;

    if (m_running) {
        m_basebandSink->feed(begin, end);
    }
}

void FT8Demod::start()
{
    QMutexLocker m_lock(&m_mutex);

    if (m_running) {
        return;
    }

    qDebug() << "FT8Demod::start";
    m_thread = new QThread();
    m_basebandSink = new FT8DemodBaseband();
    m_basebandSink->setFifoLabel(QString("%1 [%2:%3]")
        .arg(m_channelId)
        .arg(m_deviceAPI->getDeviceSetIndex())
        .arg(getIndexInDeviceSet())
    );
    m_basebandSink->setSpectrumSink(&m_spectrumVis);
    m_basebandSink->setChannel(this);
    m_basebandSink->setMessageQueueToGUI(getMessageQueueToGUI());
    m_basebandSink->moveToThread(m_thread);

    QObject::connect(
        m_thread,
        &QThread::finished,
        m_basebandSink,
        &QObject::deleteLater
    );
    QObject::connect(
        m_thread,
        &QThread::finished,
        m_thread,
        &QThread::deleteLater
    );

    if (m_basebandSampleRate != 0) {
        m_basebandSink->setBasebandSampleRate(m_basebandSampleRate);
    }

    m_thread->start();

    FT8DemodBaseband::MsgConfigureFT8DemodBaseband *msg = FT8DemodBaseband::MsgConfigureFT8DemodBaseband::create(m_settings, true);
    m_basebandSink->getInputMessageQueue()->push(msg);

    m_running = true;
}

void FT8Demod::stop()
{
    QMutexLocker m_lock(&m_mutex);

    if (!m_running) {
        return;
    }

    qDebug() << "FT8Demod::stop";
    m_running = false;
	m_thread->exit();
	m_thread->wait();
}

bool FT8Demod::handleMessage(const Message& cmd)
{
    if (MsgConfigureFT8Demod::match(cmd))
    {
        MsgConfigureFT8Demod& cfg = (MsgConfigureFT8Demod&) cmd;
        qDebug("FT8Demod::handleMessage: MsgConfigureFT8Demod");

        applySettings(cfg.getSettings(), cfg.getForce());

        return true;
    }
    else if (DSPSignalNotification::match(cmd))
    {
        qDebug() << "FT8Demod::handleMessage: DSPSignalNotification";
        DSPSignalNotification& notif = (DSPSignalNotification&) cmd;
        m_basebandSampleRate = notif.getSampleRate();
        // Forward to the sink
        if (m_running) {
            m_basebandSink->getInputMessageQueue()->push(new DSPSignalNotification(notif));
        }
        // Forwatd to GUI if any
        if (getMessageQueueToGUI()) {
            getMessageQueueToGUI()->push(new DSPSignalNotification(notif));
        }

        return true;
    }
    else if (MainCore::MsgChannelDemodQuery::match(cmd))
    {
        qDebug() << "FT8Demod::handleMessage: MsgChannelDemodQuery";
        sendSampleRateToDemodAnalyzer();

        return true;
    }
	else
	{
		return false;
	}
}

void FT8Demod::setCenterFrequency(qint64 frequency)
{
    FT8DemodSettings settings = m_settings;
    settings.m_inputFrequencyOffset = frequency;
    applySettings(settings, false);

    if (m_guiMessageQueue) // forward to GUI if any
    {
        MsgConfigureFT8Demod *msgToGUI = MsgConfigureFT8Demod::create(settings, false);
        m_guiMessageQueue->push(msgToGUI);
    }
}

void FT8Demod::applySettings(const FT8DemodSettings& settings, bool force)
{
    qDebug() << "FT8Demod::applySettings:"
            << " m_inputFrequencyOffset: " << settings.m_inputFrequencyOffset
            << " m_filterIndex: " << settings.m_filterIndex
            << " [m_spanLog2: " << settings.m_filterBank[settings.m_filterIndex].m_spanLog2
            << " m_rfBandwidth: " << settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth
            << " m_lowCutoff: " << settings.m_filterBank[settings.m_filterIndex].m_lowCutoff
            << " m_fftWindow: " << settings.m_filterBank[settings.m_filterIndex].m_fftWindow << "]"
            << " m_volume: " << settings.m_volume
            << " m_audioBinaual: " << settings.m_audioBinaural
            << " m_audioFlipChannels: " << settings.m_audioFlipChannels
            << " m_dsb: " << settings.m_dsb
            << " m_audioMute: " << settings.m_audioMute
            << " m_agcActive: " << settings.m_agc
            << " m_agcClamping: " << settings.m_agcClamping
            << " m_agcTimeLog2: " << settings.m_agcTimeLog2
            << " agcPowerThreshold: " << settings.m_agcPowerThreshold
            << " agcThresholdGate: " << settings.m_agcThresholdGate
            << " m_audioDeviceName: " << settings.m_audioDeviceName
            << " m_streamIndex: " << settings.m_streamIndex
            << " m_useReverseAPI: " << settings.m_useReverseAPI
            << " m_reverseAPIAddress: " << settings.m_reverseAPIAddress
            << " m_reverseAPIPort: " << settings.m_reverseAPIPort
            << " m_reverseAPIDeviceIndex: " << settings.m_reverseAPIDeviceIndex
            << " m_reverseAPIChannelIndex: " << settings.m_reverseAPIChannelIndex
            << " force: " << force;

    QList<QString> reverseAPIKeys;

    if ((m_settings.m_inputFrequencyOffset != settings.m_inputFrequencyOffset) || force) {
        reverseAPIKeys.append("inputFrequencyOffset");
    }
    if ((m_settings.m_filterIndex != settings.m_filterIndex) || force) {
        reverseAPIKeys.append("filterIndex");
    }
    if ((m_settings.m_filterBank[m_settings.m_filterIndex].m_spanLog2 != settings.m_filterBank[settings.m_filterIndex].m_spanLog2) || force) {
        reverseAPIKeys.append("spanLog2");
    }
    if ((m_settings.m_filterBank[m_settings.m_filterIndex].m_rfBandwidth != settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth) || force) {
        reverseAPIKeys.append("rfBandwidth");
    }
    if ((m_settings.m_filterBank[m_settings.m_filterIndex].m_lowCutoff != settings.m_filterBank[settings.m_filterIndex].m_lowCutoff) || force) {
        reverseAPIKeys.append("lowCutoff");
    }
    if ((m_settings.m_filterBank[m_settings.m_filterIndex].m_fftWindow != settings.m_filterBank[settings.m_filterIndex].m_fftWindow) || force) {
        reverseAPIKeys.append("fftWindow");
    }
    if ((m_settings.m_volume != settings.m_volume) || force) {
        reverseAPIKeys.append("volume");
    }
    if ((m_settings.m_agcTimeLog2 != settings.m_agcTimeLog2) || force) {
        reverseAPIKeys.append("agcTimeLog2");
    }
    if ((m_settings.m_agcPowerThreshold != settings.m_agcPowerThreshold) || force) {
        reverseAPIKeys.append("agcPowerThreshold");
    }
    if ((m_settings.m_agcThresholdGate != settings.m_agcThresholdGate) || force) {
        reverseAPIKeys.append("agcThresholdGate");
    }
    if ((m_settings.m_agcClamping != settings.m_agcClamping) || force) {
        reverseAPIKeys.append("agcClamping");
    }
    if ((settings.m_audioDeviceName != m_settings.m_audioDeviceName) || force) {
        reverseAPIKeys.append("audioDeviceName");
    }
    if ((m_settings.m_audioBinaural != settings.m_audioBinaural) || force) {
        reverseAPIKeys.append("audioBinaural");
    }
    if ((m_settings.m_audioFlipChannels != settings.m_audioFlipChannels) || force) {
        reverseAPIKeys.append("audioFlipChannels");
    }
    if ((m_settings.m_dsb != settings.m_dsb) || force) {
        reverseAPIKeys.append("dsb");
    }
    if ((m_settings.m_audioMute != settings.m_audioMute) || force) {
        reverseAPIKeys.append("audioMute");
    }
    if ((m_settings.m_agc != settings.m_agc) || force) {
        reverseAPIKeys.append("agc");
    }

    if (m_settings.m_streamIndex != settings.m_streamIndex)
    {
        if (m_deviceAPI->getSampleMIMO()) // change of stream is possible for MIMO devices only
        {
            m_deviceAPI->removeChannelSinkAPI(this);
            m_deviceAPI->removeChannelSink(this, m_settings.m_streamIndex);
            m_deviceAPI->addChannelSink(this, settings.m_streamIndex);
            m_deviceAPI->addChannelSinkAPI(this);
        }

        reverseAPIKeys.append("streamIndex");
    }

    if ((settings.m_dsb != m_settings.m_dsb)
     || (settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth != m_settings.m_filterBank[m_settings.m_filterIndex].m_rfBandwidth)
     || (settings.m_filterBank[settings.m_filterIndex].m_lowCutoff != m_settings.m_filterBank[m_settings.m_filterIndex].m_lowCutoff) || force)
    {
        SpectrumSettings spectrumSettings = m_spectrumVis.getSettings();
        spectrumSettings.m_ssb = !settings.m_dsb;
        spectrumSettings.m_usb = (settings.m_filterBank[settings.m_filterIndex].m_lowCutoff < settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth);
        SpectrumVis::MsgConfigureSpectrumVis *msg = SpectrumVis::MsgConfigureSpectrumVis::create(spectrumSettings, false);
        m_spectrumVis.getInputMessageQueue()->push(msg);
    }

    if (m_running)
    {
        FT8DemodBaseband::MsgConfigureFT8DemodBaseband *msg = FT8DemodBaseband::MsgConfigureFT8DemodBaseband::create(settings, force);
        m_basebandSink->getInputMessageQueue()->push(msg);
    }

    if (settings.m_useReverseAPI)
    {
        bool fullUpdate = ((m_settings.m_useReverseAPI != settings.m_useReverseAPI) && settings.m_useReverseAPI) ||
                (m_settings.m_reverseAPIAddress != settings.m_reverseAPIAddress) ||
                (m_settings.m_reverseAPIPort != settings.m_reverseAPIPort) ||
                (m_settings.m_reverseAPIDeviceIndex != settings.m_reverseAPIDeviceIndex) ||
                (m_settings.m_reverseAPIChannelIndex != settings.m_reverseAPIChannelIndex);
        webapiReverseSendSettings(reverseAPIKeys, settings, fullUpdate || force);
    }

    QList<ObjectPipe*> pipes;
    MainCore::instance()->getMessagePipes().getMessagePipes(this, "settings", pipes);

    if (pipes.size() > 0) {
        sendChannelSettings(pipes, reverseAPIKeys, settings, force);
    }

    m_settings = settings;
}

QByteArray FT8Demod::serialize() const
{
    return m_settings.serialize();
}

bool FT8Demod::deserialize(const QByteArray& data)
{
    if (m_settings.deserialize(data))
    {
        MsgConfigureFT8Demod *msg = MsgConfigureFT8Demod::create(m_settings, true);
        m_inputMessageQueue.push(msg);
        return true;
    }
    else
    {
        m_settings.resetToDefaults();
        MsgConfigureFT8Demod *msg = MsgConfigureFT8Demod::create(m_settings, true);
        m_inputMessageQueue.push(msg);
        return false;
    }
}

void FT8Demod::sendSampleRateToDemodAnalyzer()
{
    QList<ObjectPipe*> pipes;
    MainCore::instance()->getMessagePipes().getMessagePipes(this, "reportdemod", pipes);

    if (pipes.size() > 0)
    {
        for (const auto& pipe: pipes)
        {
            MessageQueue *messageQueue = qobject_cast<MessageQueue*>(pipe->m_element);

            if (messageQueue)
            {
                MainCore::MsgChannelDemodReport *msg = MainCore::MsgChannelDemodReport::create(
                    this,
                    getAudioSampleRate()
                );
                messageQueue->push(msg);
            }
        }
    }
}

int FT8Demod::webapiSettingsGet(
        SWGSDRangel::SWGChannelSettings& response,
        QString& errorMessage)
{
    (void) errorMessage;
    response.setFt8DemodSettings(new SWGSDRangel::SWGFT8DemodSettings());
    response.getFt8DemodSettings()->init();
    webapiFormatChannelSettings(response, m_settings);
    return 200;
}

int FT8Demod::webapiWorkspaceGet(
        SWGSDRangel::SWGWorkspaceInfo& response,
        QString& errorMessage)
{
    (void) errorMessage;
    response.setIndex(m_settings.m_workspaceIndex);
    return 200;
}

int FT8Demod::webapiSettingsPutPatch(
        bool force,
        const QStringList& channelSettingsKeys,
        SWGSDRangel::SWGChannelSettings& response,
        QString& errorMessage)
{
    (void) errorMessage;
    FT8DemodSettings settings = m_settings;
    webapiUpdateChannelSettings(settings, channelSettingsKeys, response);

    MsgConfigureFT8Demod *msg = MsgConfigureFT8Demod::create(settings, force);
    m_inputMessageQueue.push(msg);

    qDebug("FT8Demod::webapiSettingsPutPatch: forward to GUI: %p", m_guiMessageQueue);
    if (m_guiMessageQueue) // forward to GUI if any
    {
        MsgConfigureFT8Demod *msgToGUI = MsgConfigureFT8Demod::create(settings, force);
        m_guiMessageQueue->push(msgToGUI);
    }

    webapiFormatChannelSettings(response, settings);

    return 200;
}

void FT8Demod::webapiUpdateChannelSettings(
        FT8DemodSettings& settings,
        const QStringList& channelSettingsKeys,
        SWGSDRangel::SWGChannelSettings& response)
{
    if (channelSettingsKeys.contains("inputFrequencyOffset")) {
        settings.m_inputFrequencyOffset = response.getFt8DemodSettings()->getInputFrequencyOffset();
    }
    if (channelSettingsKeys.contains("filterIndex")) {
        settings.m_filterIndex = response.getFt8DemodSettings()->getFilterIndex();
    }
    if (channelSettingsKeys.contains("spanLog2")) {
        settings.m_filterBank[settings.m_filterIndex].m_spanLog2 = response.getFt8DemodSettings()->getSpanLog2();
    }
    if (channelSettingsKeys.contains("rfBandwidth")) {
        settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth = response.getFt8DemodSettings()->getRfBandwidth();
    }
    if (channelSettingsKeys.contains("lowCutoff")) {
        settings.m_filterBank[settings.m_filterIndex].m_lowCutoff = response.getFt8DemodSettings()->getLowCutoff();
    }
    if (channelSettingsKeys.contains("fftWimdow")) {
        settings.m_filterBank[settings.m_filterIndex].m_fftWindow = (FFTWindow::Function) response.getFt8DemodSettings()->getFftWindow();
    }
    if (channelSettingsKeys.contains("volume")) {
        settings.m_volume = response.getFt8DemodSettings()->getVolume();
    }
    if (channelSettingsKeys.contains("audioBinaural")) {
        settings.m_audioBinaural = response.getFt8DemodSettings()->getAudioBinaural() != 0;
    }
    if (channelSettingsKeys.contains("audioFlipChannels")) {
        settings.m_audioFlipChannels = response.getFt8DemodSettings()->getAudioFlipChannels() != 0;
    }
    if (channelSettingsKeys.contains("dsb")) {
        settings.m_dsb = response.getFt8DemodSettings()->getDsb() != 0;
    }
    if (channelSettingsKeys.contains("audioMute")) {
        settings.m_audioMute = response.getFt8DemodSettings()->getAudioMute() != 0;
    }
    if (channelSettingsKeys.contains("agc")) {
        settings.m_agc = response.getFt8DemodSettings()->getAgc() != 0;
    }
    if (channelSettingsKeys.contains("agcClamping")) {
        settings.m_agcClamping = response.getFt8DemodSettings()->getAgcClamping() != 0;
    }
    if (channelSettingsKeys.contains("agcTimeLog2")) {
        settings.m_agcTimeLog2 = response.getFt8DemodSettings()->getAgcTimeLog2();
    }
    if (channelSettingsKeys.contains("agcPowerThreshold")) {
        settings.m_agcPowerThreshold = response.getFt8DemodSettings()->getAgcPowerThreshold();
    }
    if (channelSettingsKeys.contains("agcThresholdGate")) {
        settings.m_agcThresholdGate = response.getFt8DemodSettings()->getAgcThresholdGate();
    }
    if (channelSettingsKeys.contains("rgbColor")) {
        settings.m_rgbColor = response.getFt8DemodSettings()->getRgbColor();
    }
    if (channelSettingsKeys.contains("title")) {
        settings.m_title = *response.getFt8DemodSettings()->getTitle();
    }
    if (channelSettingsKeys.contains("audioDeviceName")) {
        settings.m_audioDeviceName = *response.getFt8DemodSettings()->getAudioDeviceName();
    }
    if (channelSettingsKeys.contains("streamIndex")) {
        settings.m_streamIndex = response.getFt8DemodSettings()->getStreamIndex();
    }
    if (channelSettingsKeys.contains("useReverseAPI")) {
        settings.m_useReverseAPI = response.getFt8DemodSettings()->getUseReverseApi() != 0;
    }
    if (channelSettingsKeys.contains("reverseAPIAddress")) {
        settings.m_reverseAPIAddress = *response.getFt8DemodSettings()->getReverseApiAddress();
    }
    if (channelSettingsKeys.contains("reverseAPIPort")) {
        settings.m_reverseAPIPort = response.getFt8DemodSettings()->getReverseApiPort();
    }
    if (channelSettingsKeys.contains("reverseAPIDeviceIndex")) {
        settings.m_reverseAPIDeviceIndex = response.getFt8DemodSettings()->getReverseApiDeviceIndex();
    }
    if (channelSettingsKeys.contains("reverseAPIChannelIndex")) {
        settings.m_reverseAPIChannelIndex = response.getFt8DemodSettings()->getReverseApiChannelIndex();
    }
    if (settings.m_spectrumGUI && channelSettingsKeys.contains("spectrumConfig")) {
        settings.m_spectrumGUI->updateFrom(channelSettingsKeys, response.getFt8DemodSettings()->getSpectrumConfig());
    }
    if (settings.m_channelMarker && channelSettingsKeys.contains("channelMarker")) {
        settings.m_channelMarker->updateFrom(channelSettingsKeys, response.getFt8DemodSettings()->getChannelMarker());
    }
    if (settings.m_rollupState && channelSettingsKeys.contains("rollupState")) {
        settings.m_rollupState->updateFrom(channelSettingsKeys, response.getFt8DemodSettings()->getRollupState());
    }
}

int FT8Demod::webapiReportGet(
        SWGSDRangel::SWGChannelReport& response,
        QString& errorMessage)
{
    (void) errorMessage;
    response.setFt8DemodReport(new SWGSDRangel::SWGFT8DemodReport());
    response.getFt8DemodReport()->init();
    webapiFormatChannelReport(response);
    return 200;
}

void FT8Demod::webapiFormatChannelSettings(SWGSDRangel::SWGChannelSettings& response, const FT8DemodSettings& settings)
{
    response.getFt8DemodSettings()->setAudioMute(settings.m_audioMute ? 1 : 0);
    response.getFt8DemodSettings()->setInputFrequencyOffset(settings.m_inputFrequencyOffset);
    response.getFt8DemodSettings()->setFilterIndex(settings.m_filterIndex);
    response.getFt8DemodSettings()->setSpanLog2(settings.m_filterBank[settings.m_filterIndex].m_spanLog2);
    response.getFt8DemodSettings()->setRfBandwidth(settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth);
    response.getFt8DemodSettings()->setLowCutoff(settings.m_filterBank[settings.m_filterIndex].m_lowCutoff);
    response.getFt8DemodSettings()->setFftWindow((int) settings.m_filterBank[settings.m_filterIndex].m_fftWindow);
    response.getFt8DemodSettings()->setVolume(settings.m_volume);
    response.getFt8DemodSettings()->setAudioBinaural(settings.m_audioBinaural ? 1 : 0);
    response.getFt8DemodSettings()->setAudioFlipChannels(settings.m_audioFlipChannels ? 1 : 0);
    response.getFt8DemodSettings()->setDsb(settings.m_dsb ? 1 : 0);
    response.getFt8DemodSettings()->setAudioMute(settings.m_audioMute ? 1 : 0);
    response.getFt8DemodSettings()->setAgc(settings.m_agc ? 1 : 0);
    response.getFt8DemodSettings()->setAgcClamping(settings.m_agcClamping ? 1 : 0);
    response.getFt8DemodSettings()->setAgcTimeLog2(settings.m_agcTimeLog2);
    response.getFt8DemodSettings()->setAgcPowerThreshold(settings.m_agcPowerThreshold);
    response.getFt8DemodSettings()->setAgcThresholdGate(settings.m_agcThresholdGate);
    response.getFt8DemodSettings()->setRgbColor(settings.m_rgbColor);

    if (response.getFt8DemodSettings()->getTitle()) {
        *response.getFt8DemodSettings()->getTitle() = settings.m_title;
    } else {
        response.getFt8DemodSettings()->setTitle(new QString(settings.m_title));
    }

    if (response.getFt8DemodSettings()->getAudioDeviceName()) {
        *response.getFt8DemodSettings()->getAudioDeviceName() = settings.m_audioDeviceName;
    } else {
        response.getFt8DemodSettings()->setAudioDeviceName(new QString(settings.m_audioDeviceName));
    }

    response.getFt8DemodSettings()->setStreamIndex(settings.m_streamIndex);
    response.getFt8DemodSettings()->setUseReverseApi(settings.m_useReverseAPI ? 1 : 0);

    if (response.getFt8DemodSettings()->getReverseApiAddress()) {
        *response.getFt8DemodSettings()->getReverseApiAddress() = settings.m_reverseAPIAddress;
    } else {
        response.getFt8DemodSettings()->setReverseApiAddress(new QString(settings.m_reverseAPIAddress));
    }

    response.getFt8DemodSettings()->setReverseApiPort(settings.m_reverseAPIPort);
    response.getFt8DemodSettings()->setReverseApiDeviceIndex(settings.m_reverseAPIDeviceIndex);
    response.getFt8DemodSettings()->setReverseApiChannelIndex(settings.m_reverseAPIChannelIndex);

    if (settings.m_spectrumGUI)
    {
        if (response.getFt8DemodSettings()->getSpectrumConfig())
        {
            settings.m_spectrumGUI->formatTo(response.getFt8DemodSettings()->getSpectrumConfig());
        }
        else
        {
            SWGSDRangel::SWGGLSpectrum *swgGLSpectrum = new SWGSDRangel::SWGGLSpectrum();
            settings.m_spectrumGUI->formatTo(swgGLSpectrum);
            response.getFt8DemodSettings()->setSpectrumConfig(swgGLSpectrum);
        }
    }

    if (settings.m_channelMarker)
    {
        if (response.getFt8DemodSettings()->getChannelMarker())
        {
            settings.m_channelMarker->formatTo(response.getFt8DemodSettings()->getChannelMarker());
        }
        else
        {
            SWGSDRangel::SWGChannelMarker *swgChannelMarker = new SWGSDRangel::SWGChannelMarker();
            settings.m_channelMarker->formatTo(swgChannelMarker);
            response.getFt8DemodSettings()->setChannelMarker(swgChannelMarker);
        }
    }

    if (settings.m_rollupState)
    {
        if (response.getFt8DemodSettings()->getRollupState())
        {
            settings.m_rollupState->formatTo(response.getFt8DemodSettings()->getRollupState());
        }
        else
        {
            SWGSDRangel::SWGRollupState *swgRollupState = new SWGSDRangel::SWGRollupState();
            settings.m_rollupState->formatTo(swgRollupState);
            response.getFt8DemodSettings()->setRollupState(swgRollupState);
        }
    }
}

void FT8Demod::webapiFormatChannelReport(SWGSDRangel::SWGChannelReport& response)
{
    double magsqAvg, magsqPeak;
    int nbMagsqSamples;
    getMagSqLevels(magsqAvg, magsqPeak, nbMagsqSamples);

    response.getFt8DemodReport()->setChannelPowerDb(CalcDb::dbPower(magsqAvg));

    if (m_running)
    {
        response.getFt8DemodReport()->setSquelch(m_basebandSink->getAudioActive() ? 1 : 0);
        response.getFt8DemodReport()->setAudioSampleRate(m_basebandSink->getAudioSampleRate());
        response.getFt8DemodReport()->setChannelSampleRate(m_basebandSink->getChannelSampleRate());
    }
}

void FT8Demod::webapiReverseSendSettings(QList<QString>& channelSettingsKeys, const FT8DemodSettings& settings, bool force)
{
    SWGSDRangel::SWGChannelSettings *swgChannelSettings = new SWGSDRangel::SWGChannelSettings();
    webapiFormatChannelSettings(channelSettingsKeys, swgChannelSettings, settings, force);

    QString channelSettingsURL = QString("http://%1:%2/sdrangel/deviceset/%3/channel/%4/settings")
            .arg(settings.m_reverseAPIAddress)
            .arg(settings.m_reverseAPIPort)
            .arg(settings.m_reverseAPIDeviceIndex)
            .arg(settings.m_reverseAPIChannelIndex);
    m_networkRequest.setUrl(QUrl(channelSettingsURL));
    m_networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QBuffer *buffer = new QBuffer();
    buffer->open((QBuffer::ReadWrite));
    buffer->write(swgChannelSettings->asJson().toUtf8());
    buffer->seek(0);

    // Always use PATCH to avoid passing reverse API settings
    QNetworkReply *reply = m_networkManager->sendCustomRequest(m_networkRequest, "PATCH", buffer);
    buffer->setParent(reply);

    delete swgChannelSettings;
}

void FT8Demod::sendChannelSettings(
    const QList<ObjectPipe*>& pipes,
    QList<QString>& channelSettingsKeys,
    const FT8DemodSettings& settings,
    bool force)
{
    qDebug("FT8Demod::sendChannelSettings: %d pipes", pipes.size());

    for (const auto& pipe : pipes)
    {
        MessageQueue *messageQueue = qobject_cast<MessageQueue*>(pipe->m_element);

        if (messageQueue)
        {
            SWGSDRangel::SWGChannelSettings *swgChannelSettings = new SWGSDRangel::SWGChannelSettings();
            webapiFormatChannelSettings(channelSettingsKeys, swgChannelSettings, settings, force);
            MainCore::MsgChannelSettings *msg = MainCore::MsgChannelSettings::create(
                this,
                channelSettingsKeys,
                swgChannelSettings,
                force
            );
            messageQueue->push(msg);
        }
    }
}

void FT8Demod::webapiFormatChannelSettings(
        QList<QString>& channelSettingsKeys,
        SWGSDRangel::SWGChannelSettings *swgChannelSettings,
        const FT8DemodSettings& settings,
        bool force
)
{
    swgChannelSettings->setDirection(0); // Single sink (Rx)
    swgChannelSettings->setOriginatorChannelIndex(getIndexInDeviceSet());
    swgChannelSettings->setOriginatorDeviceSetIndex(getDeviceSetIndex());
    swgChannelSettings->setChannelType(new QString(m_channelId));
    swgChannelSettings->setFt8DemodSettings(new SWGSDRangel::SWGFT8DemodSettings());
    SWGSDRangel::SWGFT8DemodSettings *swgFT8DemodSettings = swgChannelSettings->getFt8DemodSettings();

    // transfer data that has been modified. When force is on transfer all data except reverse API data

    if (channelSettingsKeys.contains("inputFrequencyOffset") || force) {
        swgFT8DemodSettings->setInputFrequencyOffset(settings.m_inputFrequencyOffset);
    }
    if (channelSettingsKeys.contains("filteIndex") || force) {
        swgFT8DemodSettings->setFilterIndex(settings.m_filterIndex);
    }
    if (channelSettingsKeys.contains("spanLog2") || force) {
        swgFT8DemodSettings->setSpanLog2(settings.m_filterBank[settings.m_filterIndex].m_spanLog2);
    }
    if (channelSettingsKeys.contains("rfBandwidth") || force) {
        swgFT8DemodSettings->setRfBandwidth(settings.m_filterBank[settings.m_filterIndex].m_rfBandwidth);
    }
    if (channelSettingsKeys.contains("lowCutoff") || force) {
        swgFT8DemodSettings->setLowCutoff(settings.m_filterBank[settings.m_filterIndex].m_lowCutoff);
    }
    if (channelSettingsKeys.contains("fftWindow") || force) {
        swgFT8DemodSettings->setLowCutoff(settings.m_filterBank[settings.m_filterIndex].m_fftWindow);
    }
    if (channelSettingsKeys.contains("volume") || force) {
        swgFT8DemodSettings->setVolume(settings.m_volume);
    }
    if (channelSettingsKeys.contains("audioBinaural") || force) {
        swgFT8DemodSettings->setAudioBinaural(settings.m_audioBinaural ? 1 : 0);
    }
    if (channelSettingsKeys.contains("audioFlipChannels") || force) {
        swgFT8DemodSettings->setAudioFlipChannels(settings.m_audioFlipChannels ? 1 : 0);
    }
    if (channelSettingsKeys.contains("dsb") || force) {
        swgFT8DemodSettings->setDsb(settings.m_dsb ? 1 : 0);
    }
    if (channelSettingsKeys.contains("audioMute") || force) {
        swgFT8DemodSettings->setAudioMute(settings.m_audioMute ? 1 : 0);
    }
    if (channelSettingsKeys.contains("agc") || force) {
        swgFT8DemodSettings->setAgc(settings.m_agc ? 1 : 0);
    }
    if (channelSettingsKeys.contains("agcClamping") || force) {
        swgFT8DemodSettings->setAgcClamping(settings.m_agcClamping ? 1 : 0);
    }
    if (channelSettingsKeys.contains("agcTimeLog2") || force) {
        swgFT8DemodSettings->setAgcTimeLog2(settings.m_agcTimeLog2);
    }
    if (channelSettingsKeys.contains("agcPowerThreshold") || force) {
        swgFT8DemodSettings->setAgcPowerThreshold(settings.m_agcPowerThreshold);
    }
    if (channelSettingsKeys.contains("agcThresholdGate") || force) {
        swgFT8DemodSettings->setAgcThresholdGate(settings.m_agcThresholdGate);
    }
    if (channelSettingsKeys.contains("rgbColor") || force) {
        swgFT8DemodSettings->setRgbColor(settings.m_rgbColor);
    }
    if (channelSettingsKeys.contains("title") || force) {
        swgFT8DemodSettings->setTitle(new QString(settings.m_title));
    }
    if (channelSettingsKeys.contains("audioDeviceName") || force) {
        swgFT8DemodSettings->setAudioDeviceName(new QString(settings.m_audioDeviceName));
    }
    if (channelSettingsKeys.contains("streamIndex") || force) {
        swgFT8DemodSettings->setStreamIndex(settings.m_streamIndex);
    }

    if (settings.m_spectrumGUI && (channelSettingsKeys.contains("spectrunConfig") || force))
    {
        SWGSDRangel::SWGGLSpectrum *swgGLSpectrum = new SWGSDRangel::SWGGLSpectrum();
        settings.m_spectrumGUI->formatTo(swgGLSpectrum);
        swgFT8DemodSettings->setSpectrumConfig(swgGLSpectrum);
    }

    if (settings.m_channelMarker && (channelSettingsKeys.contains("channelMarker") || force))
    {
        SWGSDRangel::SWGChannelMarker *swgChannelMarker = new SWGSDRangel::SWGChannelMarker();
        settings.m_channelMarker->formatTo(swgChannelMarker);
        swgFT8DemodSettings->setChannelMarker(swgChannelMarker);
    }

    if (settings.m_rollupState && (channelSettingsKeys.contains("rollupState") || force))
    {
        SWGSDRangel::SWGRollupState *swgRolllupState = new SWGSDRangel::SWGRollupState();
        settings.m_rollupState->formatTo(swgRolllupState);
        swgFT8DemodSettings->setRollupState(swgRolllupState);
    }
}

void FT8Demod::networkManagerFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError replyError = reply->error();

    if (replyError)
    {
        qWarning() << "FT8Demod::networkManagerFinished:"
                << " error(" << (int) replyError
                << "): " << replyError
                << ": " << reply->errorString();
    }
    else
    {
        QString answer = reply->readAll();
        answer.chop(1); // remove last \n
        qDebug("FT8Demod::networkManagerFinished: reply:\n%s", answer.toStdString().c_str());
    }

    reply->deleteLater();
}

void FT8Demod::handleIndexInDeviceSetChanged(int index)
{
    if (!m_running || (index < 0)) {
        return;
    }

    QString fifoLabel = QString("%1 [%2:%3]")
        .arg(m_channelId)
        .arg(m_deviceAPI->getDeviceSetIndex())
        .arg(index);
    m_basebandSink->setFifoLabel(fifoLabel);
    m_basebandSink->setAudioFifoLabel(fifoLabel);
}