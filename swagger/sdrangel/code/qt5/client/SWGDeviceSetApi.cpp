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

#include "SWGDeviceSetApi.h"
#include "SWGHelpers.h"
#include "SWGModelFactory.h"

#include <QJsonArray>
#include <QJsonDocument>

namespace SWGSDRangel {

SWGDeviceSetApi::SWGDeviceSetApi() {}

SWGDeviceSetApi::~SWGDeviceSetApi() {}

SWGDeviceSetApi::SWGDeviceSetApi(QString host, QString basePath) {
    this->host = host;
    this->basePath = basePath;
}

void
SWGDeviceSetApi::devicesetChannelDelete(qint32 device_set_index, qint32 channel_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/channel/{channelIndex}");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));
    QString channel_indexPathParam("{"); channel_indexPathParam.append("channelIndex").append("}");
    fullPath.replace(channel_indexPathParam, stringValue(channel_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "DELETE");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetChannelDeleteCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetChannelDeleteCallback(HttpRequestWorker * worker) {
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
    SWGChannelSettings* output = static_cast<SWGChannelSettings*>(create(json, QString("SWGChannelSettings")));
    worker->deleteLater();

    emit devicesetChannelDeleteSignal(output);
    emit devicesetChannelDeleteSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetChannelPost(qint32 device_set_index, SWGChannelSettings body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/channel");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


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
            &SWGDeviceSetApi::devicesetChannelPostCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetChannelPostCallback(HttpRequestWorker * worker) {
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

    emit devicesetChannelPostSignal(output);
    emit devicesetChannelPostSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetChannelSettingsGet(qint32 device_set_index, qint32 channel_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/channel/{channelIndex}/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));
    QString channel_indexPathParam("{"); channel_indexPathParam.append("channelIndex").append("}");
    fullPath.replace(channel_indexPathParam, stringValue(channel_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetChannelSettingsGetCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetChannelSettingsGetCallback(HttpRequestWorker * worker) {
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
    SWGChannelSettings* output = static_cast<SWGChannelSettings*>(create(json, QString("SWGChannelSettings")));
    worker->deleteLater();

    emit devicesetChannelSettingsGetSignal(output);
    emit devicesetChannelSettingsGetSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetChannelSettingsPatch(qint32 device_set_index, qint32 channel_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/channel/{channelIndex}/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));
    QString channel_indexPathParam("{"); channel_indexPathParam.append("channelIndex").append("}");
    fullPath.replace(channel_indexPathParam, stringValue(channel_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PATCH");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetChannelSettingsPatchCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetChannelSettingsPatchCallback(HttpRequestWorker * worker) {
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
    SWGChannelSettings* output = static_cast<SWGChannelSettings*>(create(json, QString("SWGChannelSettings")));
    worker->deleteLater();

    emit devicesetChannelSettingsPatchSignal(output);
    emit devicesetChannelSettingsPatchSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetChannelSettingsPut(qint32 device_set_index, qint32 channel_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/channel/{channelIndex}/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));
    QString channel_indexPathParam("{"); channel_indexPathParam.append("channelIndex").append("}");
    fullPath.replace(channel_indexPathParam, stringValue(channel_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "PUT");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetChannelSettingsPutCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetChannelSettingsPutCallback(HttpRequestWorker * worker) {
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
    SWGChannelSettings* output = static_cast<SWGChannelSettings*>(create(json, QString("SWGChannelSettings")));
    worker->deleteLater();

    emit devicesetChannelSettingsPutSignal(output);
    emit devicesetChannelSettingsPutSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDevicePut(qint32 device_set_index, SWGDeviceListItem body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


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
            &SWGDeviceSetApi::devicesetDevicePutCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDevicePutCallback(HttpRequestWorker * worker) {
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
    SWGDeviceListItem* output = static_cast<SWGDeviceListItem*>(create(json, QString("SWGDeviceListItem")));
    worker->deleteLater();

    emit devicesetDevicePutSignal(output);
    emit devicesetDevicePutSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceRunDelete(qint32 device_set_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/run");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "DELETE");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetDeviceRunDeleteCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceRunDeleteCallback(HttpRequestWorker * worker) {
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
    SWGDeviceState* output = static_cast<SWGDeviceState*>(create(json, QString("SWGDeviceState")));
    worker->deleteLater();

    emit devicesetDeviceRunDeleteSignal(output);
    emit devicesetDeviceRunDeleteSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceRunGet(qint32 device_set_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/run");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetDeviceRunGetCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceRunGetCallback(HttpRequestWorker * worker) {
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
    SWGDeviceState* output = static_cast<SWGDeviceState*>(create(json, QString("SWGDeviceState")));
    worker->deleteLater();

    emit devicesetDeviceRunGetSignal(output);
    emit devicesetDeviceRunGetSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceRunPost(qint32 device_set_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/run");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "POST");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetDeviceRunPostCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceRunPostCallback(HttpRequestWorker * worker) {
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
    SWGDeviceState* output = static_cast<SWGDeviceState*>(create(json, QString("SWGDeviceState")));
    worker->deleteLater();

    emit devicesetDeviceRunPostSignal(output);
    emit devicesetDeviceRunPostSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsGet(qint32 device_set_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetDeviceSettingsGetCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsGetCallback(HttpRequestWorker * worker) {
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
    SWGDeviceSettings* output = static_cast<SWGDeviceSettings*>(create(json, QString("SWGDeviceSettings")));
    worker->deleteLater();

    emit devicesetDeviceSettingsGetSignal(output);
    emit devicesetDeviceSettingsGetSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsPatch(qint32 device_set_index, SWGDeviceSettings body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


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
            &SWGDeviceSetApi::devicesetDeviceSettingsPatchCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsPatchCallback(HttpRequestWorker * worker) {
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
    SWGDeviceSettings* output = static_cast<SWGDeviceSettings*>(create(json, QString("SWGDeviceSettings")));
    worker->deleteLater();

    emit devicesetDeviceSettingsPatchSignal(output);
    emit devicesetDeviceSettingsPatchSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsPut(qint32 device_set_index, SWGDeviceSettings body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}/device/settings");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


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
            &SWGDeviceSetApi::devicesetDeviceSettingsPutCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetDeviceSettingsPutCallback(HttpRequestWorker * worker) {
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
    SWGDeviceSettings* output = static_cast<SWGDeviceSettings*>(create(json, QString("SWGDeviceSettings")));
    worker->deleteLater();

    emit devicesetDeviceSettingsPutSignal(output);
    emit devicesetDeviceSettingsPutSignalE(output, error_type, error_str);
}

void
SWGDeviceSetApi::devicesetGet(qint32 device_set_index) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sdrangel/deviceset/{deviceSetIndex}");

    QString device_set_indexPathParam("{"); device_set_indexPathParam.append("deviceSetIndex").append("}");
    fullPath.replace(device_set_indexPathParam, stringValue(device_set_index));


    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &HttpRequestWorker::on_execution_finished,
            this,
            &SWGDeviceSetApi::devicesetGetCallback);

    worker->execute(&input);
}

void
SWGDeviceSetApi::devicesetGetCallback(HttpRequestWorker * worker) {
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
    SWGDeviceSet* output = static_cast<SWGDeviceSet*>(create(json, QString("SWGDeviceSet")));
    worker->deleteLater();

    emit devicesetGetSignal(output);
    emit devicesetGetSignalE(output, error_type, error_str);
}


}
