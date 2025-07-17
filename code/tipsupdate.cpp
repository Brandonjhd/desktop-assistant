#include "tipsupdate.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

TipsUpdate::TipsUpdate(QObject *parent) : QObject(parent) {}

TipsUpdate::~TipsUpdate() {}

// 启动获取Tips数据
void TipsUpdate::fetchQuote() {
    emit tipsLoadingStarted();

    imageDone = false;
    quoteDone = false;

    // 随机选择图片索引(1-24)
    int idx = QRandomGenerator::global()->bounded(1, 25);
    if (idx < 1 || idx > 24) {
        idx = 1;
    }

    QString path = QString(":/img/img/tipsimg/%1.jpg").arg(idx);
    tipsPix = QPixmap(path);

    // 图片加载失败时使用默认图片
    if (tipsPix.isNull()) {
        tipsPix = QPixmap(":/img/img/tipsimg/1.jpg");
        if (tipsPix.isNull()) {
            // 如果默认图片也失败，保持为空
        }
    }

    imageDone = true;
    checkAndEmit();

    // 异步获取引言
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &TipsUpdate::handleReply);

    QNetworkRequest request(QUrl("https://uapis.cn/api/say"));
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    manager->get(request);
}

// 处理网络回复
void TipsUpdate::handleReply(QNetworkReply *reply) {
    if (!reply) {
        tipsText = "网络错误: 回复为空";
        quoteDone = true;
        checkAndEmit();
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        tipsText = "网络错误: " + reply->errorString();
        reply->deleteLater();
        quoteDone = true;
        checkAndEmit();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    if (data.isEmpty()) {
        tipsText = "数据为空";
    } else {
        tipsText = QString::fromUtf8(data);
    }

    quoteDone = true;
    checkAndEmit();
}

// 检查并发出完成信号
void TipsUpdate::checkAndEmit() {
    if (imageDone && quoteDone) {
        emit tipsReady(tipsPix, tipsText);
    }
}
