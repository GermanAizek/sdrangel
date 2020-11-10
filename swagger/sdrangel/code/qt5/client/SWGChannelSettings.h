/**
 * SDRangel
 * This is the web REST/JSON API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ (4.3+ in Windows) GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1, USRP and FunCube    ---   Limitations and specifcities:    * In SDRangel GUI the first Rx device set cannot be deleted. Conversely the server starts with no device sets and its number of device sets can be reduced to zero by as many calls as necessary to /sdrangel/deviceset with DELETE method.   * Preset import and export from/to file is a server only feature.   * Device set focus is a GUI only feature.   * The following channels are not implemented (status 501 is returned): ATV and DATV demodulators, Channel Analyzer NG, LoRa demodulator   * The device settings and report structures contains only the sub-structure corresponding to the device type. The DeviceSettings and DeviceReport structures documented here shows all of them but only one will be or should be present at a time   * The channel settings and report structures contains only the sub-structure corresponding to the channel type. The ChannelSettings and ChannelReport structures documented here shows all of them but only one will be or should be present at a time    --- 
 *
 * OpenAPI spec version: 4.15.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

/*
 * SWGChannelSettings.h
 *
 * Base channel settings. Only the channel settings corresponding to the channel specified in the channelType field is or should be present.
 */

#ifndef SWGChannelSettings_H_
#define SWGChannelSettings_H_

#include <QJsonObject>


#include "SWGADSBDemodSettings.h"
#include "SWGAMDemodSettings.h"
#include "SWGAMModSettings.h"
#include "SWGATVDemodSettings.h"
#include "SWGATVModSettings.h"
#include "SWGBFMDemodSettings.h"
#include "SWGChannelAnalyzerSettings.h"
#include "SWGChirpChatDemodSettings.h"
#include "SWGChirpChatModSettings.h"
#include "SWGDATVDemodSettings.h"
#include "SWGDSDDemodSettings.h"
#include "SWGFileSinkSettings.h"
#include "SWGFileSourceSettings.h"
#include "SWGFreeDVDemodSettings.h"
#include "SWGFreeDVModSettings.h"
#include "SWGFreqTrackerSettings.h"
#include "SWGIEEE_802_15_4_ModSettings.h"
#include "SWGLocalSinkSettings.h"
#include "SWGLocalSourceSettings.h"
#include "SWGNFMDemodSettings.h"
#include "SWGNFMModSettings.h"
#include "SWGPacketModSettings.h"
#include "SWGRemoteSinkSettings.h"
#include "SWGRemoteSourceSettings.h"
#include "SWGSSBDemodSettings.h"
#include "SWGSSBModSettings.h"
#include "SWGSigMFFileSinkSettings.h"
#include "SWGUDPSinkSettings.h"
#include "SWGUDPSourceSettings.h"
#include "SWGWFMDemodSettings.h"
#include "SWGWFMModSettings.h"
#include <QString>

#include "SWGObject.h"
#include "export.h"

namespace SWGSDRangel {

class SWG_API SWGChannelSettings: public SWGObject {
public:
    SWGChannelSettings();
    SWGChannelSettings(QString* json);
    virtual ~SWGChannelSettings();
    void init();
    void cleanup();

    virtual QString asJson () override;
    virtual QJsonObject* asJsonObject() override;
    virtual void fromJsonObject(QJsonObject &json) override;
    virtual SWGChannelSettings* fromJson(QString &jsonString) override;

    QString* getChannelType();
    void setChannelType(QString* channel_type);

    qint32 getDirection();
    void setDirection(qint32 direction);

    qint32 getOriginatorDeviceSetIndex();
    void setOriginatorDeviceSetIndex(qint32 originator_device_set_index);

    qint32 getOriginatorChannelIndex();
    void setOriginatorChannelIndex(qint32 originator_channel_index);

    SWGADSBDemodSettings* getAdsbDemodSettings();
    void setAdsbDemodSettings(SWGADSBDemodSettings* adsb_demod_settings);

    SWGAMDemodSettings* getAmDemodSettings();
    void setAmDemodSettings(SWGAMDemodSettings* am_demod_settings);

    SWGAMModSettings* getAmModSettings();
    void setAmModSettings(SWGAMModSettings* am_mod_settings);

    SWGATVDemodSettings* getAtvDemodSettings();
    void setAtvDemodSettings(SWGATVDemodSettings* atv_demod_settings);

    SWGATVModSettings* getAtvModSettings();
    void setAtvModSettings(SWGATVModSettings* atv_mod_settings);

    SWGBFMDemodSettings* getBfmDemodSettings();
    void setBfmDemodSettings(SWGBFMDemodSettings* bfm_demod_settings);

    SWGChannelAnalyzerSettings* getChannelAnalyzerSettings();
    void setChannelAnalyzerSettings(SWGChannelAnalyzerSettings* channel_analyzer_settings);

    SWGChirpChatDemodSettings* getChirpChatDemodSettings();
    void setChirpChatDemodSettings(SWGChirpChatDemodSettings* chirp_chat_demod_settings);

    SWGChirpChatModSettings* getChirpChatModSettings();
    void setChirpChatModSettings(SWGChirpChatModSettings* chirp_chat_mod_settings);

    SWGDATVDemodSettings* getDatvDemodSettings();
    void setDatvDemodSettings(SWGDATVDemodSettings* datv_demod_settings);

    SWGDSDDemodSettings* getDsdDemodSettings();
    void setDsdDemodSettings(SWGDSDDemodSettings* dsd_demod_settings);

    SWGFileSinkSettings* getFileSinkSettings();
    void setFileSinkSettings(SWGFileSinkSettings* file_sink_settings);

    SWGFileSourceSettings* getFileSourceSettings();
    void setFileSourceSettings(SWGFileSourceSettings* file_source_settings);

    SWGFreeDVDemodSettings* getFreeDvDemodSettings();
    void setFreeDvDemodSettings(SWGFreeDVDemodSettings* free_dv_demod_settings);

    SWGFreeDVModSettings* getFreeDvModSettings();
    void setFreeDvModSettings(SWGFreeDVModSettings* free_dv_mod_settings);

    SWGFreqTrackerSettings* getFreqTrackerSettings();
    void setFreqTrackerSettings(SWGFreqTrackerSettings* freq_tracker_settings);

    SWGIEEE_802_15_4_ModSettings* getIeee802154ModSettings();
    void setIeee802154ModSettings(SWGIEEE_802_15_4_ModSettings* ieee_802_15_4_mod_settings);

    SWGNFMDemodSettings* getNfmDemodSettings();
    void setNfmDemodSettings(SWGNFMDemodSettings* nfm_demod_settings);

    SWGNFMModSettings* getNfmModSettings();
    void setNfmModSettings(SWGNFMModSettings* nfm_mod_settings);

    SWGLocalSinkSettings* getLocalSinkSettings();
    void setLocalSinkSettings(SWGLocalSinkSettings* local_sink_settings);

    SWGLocalSourceSettings* getLocalSourceSettings();
    void setLocalSourceSettings(SWGLocalSourceSettings* local_source_settings);

    SWGPacketModSettings* getPacketModSettings();
    void setPacketModSettings(SWGPacketModSettings* packet_mod_settings);

    SWGRemoteSinkSettings* getRemoteSinkSettings();
    void setRemoteSinkSettings(SWGRemoteSinkSettings* remote_sink_settings);

    SWGRemoteSourceSettings* getRemoteSourceSettings();
    void setRemoteSourceSettings(SWGRemoteSourceSettings* remote_source_settings);

    SWGSigMFFileSinkSettings* getSigMfFileSinkSettings();
    void setSigMfFileSinkSettings(SWGSigMFFileSinkSettings* sig_mf_file_sink_settings);

    SWGSSBModSettings* getSsbModSettings();
    void setSsbModSettings(SWGSSBModSettings* ssb_mod_settings);

    SWGSSBDemodSettings* getSsbDemodSettings();
    void setSsbDemodSettings(SWGSSBDemodSettings* ssb_demod_settings);

    SWGUDPSourceSettings* getUdpSourceSettings();
    void setUdpSourceSettings(SWGUDPSourceSettings* udp_source_settings);

    SWGUDPSinkSettings* getUdpSinkSettings();
    void setUdpSinkSettings(SWGUDPSinkSettings* udp_sink_settings);

    SWGWFMDemodSettings* getWfmDemodSettings();
    void setWfmDemodSettings(SWGWFMDemodSettings* wfm_demod_settings);

    SWGWFMModSettings* getWfmModSettings();
    void setWfmModSettings(SWGWFMModSettings* wfm_mod_settings);


    virtual bool isSet() override;

private:
    QString* channel_type;
    bool m_channel_type_isSet;

    qint32 direction;
    bool m_direction_isSet;

    qint32 originator_device_set_index;
    bool m_originator_device_set_index_isSet;

    qint32 originator_channel_index;
    bool m_originator_channel_index_isSet;

    SWGADSBDemodSettings* adsb_demod_settings;
    bool m_adsb_demod_settings_isSet;

    SWGAMDemodSettings* am_demod_settings;
    bool m_am_demod_settings_isSet;

    SWGAMModSettings* am_mod_settings;
    bool m_am_mod_settings_isSet;

    SWGATVDemodSettings* atv_demod_settings;
    bool m_atv_demod_settings_isSet;

    SWGATVModSettings* atv_mod_settings;
    bool m_atv_mod_settings_isSet;

    SWGBFMDemodSettings* bfm_demod_settings;
    bool m_bfm_demod_settings_isSet;

    SWGChannelAnalyzerSettings* channel_analyzer_settings;
    bool m_channel_analyzer_settings_isSet;

    SWGChirpChatDemodSettings* chirp_chat_demod_settings;
    bool m_chirp_chat_demod_settings_isSet;

    SWGChirpChatModSettings* chirp_chat_mod_settings;
    bool m_chirp_chat_mod_settings_isSet;

    SWGDATVDemodSettings* datv_demod_settings;
    bool m_datv_demod_settings_isSet;

    SWGDSDDemodSettings* dsd_demod_settings;
    bool m_dsd_demod_settings_isSet;

    SWGFileSinkSettings* file_sink_settings;
    bool m_file_sink_settings_isSet;

    SWGFileSourceSettings* file_source_settings;
    bool m_file_source_settings_isSet;

    SWGFreeDVDemodSettings* free_dv_demod_settings;
    bool m_free_dv_demod_settings_isSet;

    SWGFreeDVModSettings* free_dv_mod_settings;
    bool m_free_dv_mod_settings_isSet;

    SWGFreqTrackerSettings* freq_tracker_settings;
    bool m_freq_tracker_settings_isSet;

    SWGIEEE_802_15_4_ModSettings* ieee_802_15_4_mod_settings;
    bool m_ieee_802_15_4_mod_settings_isSet;

    SWGNFMDemodSettings* nfm_demod_settings;
    bool m_nfm_demod_settings_isSet;

    SWGNFMModSettings* nfm_mod_settings;
    bool m_nfm_mod_settings_isSet;

    SWGLocalSinkSettings* local_sink_settings;
    bool m_local_sink_settings_isSet;

    SWGLocalSourceSettings* local_source_settings;
    bool m_local_source_settings_isSet;

    SWGPacketModSettings* packet_mod_settings;
    bool m_packet_mod_settings_isSet;

    SWGRemoteSinkSettings* remote_sink_settings;
    bool m_remote_sink_settings_isSet;

    SWGRemoteSourceSettings* remote_source_settings;
    bool m_remote_source_settings_isSet;

    SWGSigMFFileSinkSettings* sig_mf_file_sink_settings;
    bool m_sig_mf_file_sink_settings_isSet;

    SWGSSBModSettings* ssb_mod_settings;
    bool m_ssb_mod_settings_isSet;

    SWGSSBDemodSettings* ssb_demod_settings;
    bool m_ssb_demod_settings_isSet;

    SWGUDPSourceSettings* udp_source_settings;
    bool m_udp_source_settings_isSet;

    SWGUDPSinkSettings* udp_sink_settings;
    bool m_udp_sink_settings_isSet;

    SWGWFMDemodSettings* wfm_demod_settings;
    bool m_wfm_demod_settings_isSet;

    SWGWFMModSettings* wfm_mod_settings;
    bool m_wfm_mod_settings_isSet;

};

}

#endif /* SWGChannelSettings_H_ */
