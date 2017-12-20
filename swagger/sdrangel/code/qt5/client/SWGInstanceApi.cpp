/**
 * SDRangel
 * This is the web REST/JSON API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ (4.3+ in Windows) GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1 and FunCube     ---   Limitations and specifcities:       * In SDRangel GUI version there is no support for channel deletion. As a consequence the call to the API /sdrangel/deviceset/{deviceSetIndex}/channel/{channelIndex} returns with a status code of 501 (not implemented)   * In SDRangel GUI the first Rx device set cannot be deleted. Conversely the server starts with no device sets and its number of device sets can be reduced to zero by as many calls as necessary to /sdrangel/deviceset with DELETE method.   * Stopping instance i.e. /sdrangel with DELETE method is a server only feature. It allows stopping the instance nicely.   * Preset import and export from/to file is a server only feature.   * The following channels are not implemented (status 501 is returned): ATV demodulator, Channel Analyzer, Channel Analyzer NG, LoRa demodulator, TCP source   * The content type returned is always application/json except in the following cases:     * An incorrect URL was specified: this document is returned as text/html with a status 400     * There is no API adapter in the targeted instance: message \"Service not available\" as text/plain is returned with a status 500. This should not happen with released code.      --- 
 *
 * OpenAPI spec version: 4.0.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

#include "SWGInstanceApi.h"
#include "SWGHelpers.h"
#include "SWGModelFactory.h"

#include <QJsonArray>
#include <QJsonDocument>

namespace SWGSDRangel {

SWGInstanceApi::SWGInstanceApi() {}

SWGInstanceApi::~SWGInstanceApi() {}

SWGInstanceApi::SWGInstanceApi(QString host, QString basePath) {
    this->host = host;
    this->basePath = basePath;
}

void
SWGInstanceApi::instanceAudioGet() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/audio");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceAudioGetCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceAudioGetCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGAudioDevices* output = static_cast<SWGAudioDevices*>(create(json, QString("SWGAudioDevices")));
    worker->deleteLater();

    emit instanceAudioGetSignal(output);
    emit instanceAudioGetSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceAudioPatch(SWGAudioDevicesSelect body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/audio");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PATCH");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceAudioPatchCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceAudioPatchCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGAudioDevicesSelect* output = static_cast<SWGAudioDevicesSelect*>(create(json, QString("SWGAudioDevicesSelect")));
    worker->deleteLater();

    emit instanceAudioPatchSignal(output);
    emit instanceAudioPatchSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceChannels(qint32 tx) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/channels");


    if (fullPath.indexOf("?") > 0) 
      fullPath.append("&");
    else 
      fullPath.append("?");
    fullPath.append(QUrl::toPercentEncoding("tx"))
        .append("=")
        .append(QUrl::toPercentEncoding(stringValue(tx)));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceChannelsCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceChannelsCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGInstanceChannelsResponse* output = static_cast<SWGInstanceChannelsResponse*>(create(json, QString("SWGInstanceChannelsResponse")));
    worker->deleteLater();

    emit instanceChannelsSignal(output);
    emit instanceChannelsSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDVSerialPatch(qint32 dvserial) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/dvserial");


    if (fullPath.indexOf("?") > 0) 
      fullPath.append("&");
    else 
      fullPath.append("?");
    fullPath.append(QUrl::toPercentEncoding("dvserial"))
        .append("=")
        .append(QUrl::toPercentEncoding(stringValue(dvserial)));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PATCH");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDVSerialPatchCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDVSerialPatchCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGDVSeralDevices* output = static_cast<SWGDVSeralDevices*>(create(json, QString("SWGDVSeralDevices")));
    worker->deleteLater();

    emit instanceDVSerialPatchSignal(output);
    emit instanceDVSerialPatchSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDelete() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "DELETE");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDeleteCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDeleteCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGInstanceSummaryResponse* output = static_cast<SWGInstanceSummaryResponse*>(create(json, QString("SWGInstanceSummaryResponse")));
    worker->deleteLater();

    emit instanceDeleteSignal(output);
    emit instanceDeleteSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDeviceSetsDelete() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/devicesets");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "DELETE");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDeviceSetsDeleteCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDeviceSetsDeleteCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGSuccessResponse* output = static_cast<SWGSuccessResponse*>(create(json, QString("SWGSuccessResponse")));
    worker->deleteLater();

    emit instanceDeviceSetsDeleteSignal(output);
    emit instanceDeviceSetsDeleteSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDeviceSetsGet() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/devicesets");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDeviceSetsGetCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDeviceSetsGetCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGDeviceSetList* output = static_cast<SWGDeviceSetList*>(create(json, QString("SWGDeviceSetList")));
    worker->deleteLater();

    emit instanceDeviceSetsGetSignal(output);
    emit instanceDeviceSetsGetSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDeviceSetsPost(qint32 tx) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/devicesets");


    if (fullPath.indexOf("?") > 0) 
      fullPath.append("&");
    else 
      fullPath.append("?");
    fullPath.append(QUrl::toPercentEncoding("tx"))
        .append("=")
        .append(QUrl::toPercentEncoding(stringValue(tx)));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "POST");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDeviceSetsPostCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDeviceSetsPostCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGSuccessResponse* output = static_cast<SWGSuccessResponse*>(create(json, QString("SWGSuccessResponse")));
    worker->deleteLater();

    emit instanceDeviceSetsPostSignal(output);
    emit instanceDeviceSetsPostSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceDevices(qint32 tx) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/devices");


    if (fullPath.indexOf("?") > 0) 
      fullPath.append("&");
    else 
      fullPath.append("?");
    fullPath.append(QUrl::toPercentEncoding("tx"))
        .append("=")
        .append(QUrl::toPercentEncoding(stringValue(tx)));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceDevicesCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceDevicesCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGInstanceDevicesResponse* output = static_cast<SWGInstanceDevicesResponse*>(create(json, QString("SWGInstanceDevicesResponse")));
    worker->deleteLater();

    emit instanceDevicesSignal(output);
    emit instanceDevicesSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceLocationGet() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/location");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceLocationGetCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceLocationGetCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGLocationInformation* output = static_cast<SWGLocationInformation*>(create(json, QString("SWGLocationInformation")));
    worker->deleteLater();

    emit instanceLocationGetSignal(output);
    emit instanceLocationGetSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceLocationPut(SWGLocationInformation body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/location");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PUT");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceLocationPutCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceLocationPutCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGLocationInformation* output = static_cast<SWGLocationInformation*>(create(json, QString("SWGLocationInformation")));
    worker->deleteLater();

    emit instanceLocationPutSignal(output);
    emit instanceLocationPutSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceLoggingGet() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/logging");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceLoggingGetCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceLoggingGetCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGLoggingInfo* output = static_cast<SWGLoggingInfo*>(create(json, QString("SWGLoggingInfo")));
    worker->deleteLater();

    emit instanceLoggingGetSignal(output);
    emit instanceLoggingGetSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceLoggingPut(SWGLoggingInfo body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/logging");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PUT");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceLoggingPutCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceLoggingPutCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGLoggingInfo* output = static_cast<SWGLoggingInfo*>(create(json, QString("SWGLoggingInfo")));
    worker->deleteLater();

    emit instanceLoggingPutSignal(output);
    emit instanceLoggingPutSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetDelete(SWGPresetIdentifier body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "DELETE");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetDeleteCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetDeleteCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetDeleteSignal(output);
    emit instancePresetDeleteSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetFilePost(SWGPresetExport body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset/file");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "POST");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetFilePostCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetFilePostCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetFilePostSignal(output);
    emit instancePresetFilePostSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetFilePut(SWGPresetImport body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset/file");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PUT");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetFilePutCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetFilePutCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetFilePutSignal(output);
    emit instancePresetFilePutSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetGet() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetGetCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetGetCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresets* output = static_cast<SWGPresets*>(create(json, QString("SWGPresets")));
    worker->deleteLater();

    emit instancePresetGetSignal(output);
    emit instancePresetGetSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetPatch(SWGPresetTransfer body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PATCH");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetPatchCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetPatchCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetPatchSignal(output);
    emit instancePresetPatchSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetPost(SWGPresetTransfer body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "POST");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetPostCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetPostCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetPostSignal(output);
    emit instancePresetPostSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instancePresetPut(SWGPresetTransfer body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/preset");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PUT");


    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instancePresetPutCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instancePresetPutCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGPresetIdentifier* output = static_cast<SWGPresetIdentifier*>(create(json, QString("SWGPresetIdentifier")));
    worker->deleteLater();

    emit instancePresetPutSignal(output);
    emit instancePresetPutSignalE(output, error_type, error_str);
}

void
SWGInstanceApi::instanceSummary() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel");



    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGInstanceApi::instanceSummaryCallback);

    worker->execute(&input);
}

void
SWGInstanceApi::instanceSummaryCallback(HttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }


    QString json(worker->response);
    SWGInstanceSummaryResponse* output = static_cast<SWGInstanceSummaryResponse*>(create(json, QString("SWGInstanceSummaryResponse")));
    worker->deleteLater();

    emit instanceSummarySignal(output);
    emit instanceSummarySignalE(output, error_type, error_str);
}


}
