#ifndef TIPSUPDATE_H
#define TIPSUPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

// Tips更新类
class TipsUpdate : public QObject {
    Q_OBJECT

public:
    // 构造函数
    explicit TipsUpdate(QObject *parent = nullptr);

    // 析构函数
    ~TipsUpdate();

public slots:
    // 启动获取Tips数据
    void fetchQuote();

private slots:
    // 处理网络回复
    void handleReply(QNetworkReply *reply);

private:
    // 检查并发出完成信号
    void checkAndEmit();

    // 网络管理器
    QNetworkAccessManager *manager;

    // 完成标志
    bool imageDone = false;
    bool quoteDone = false;

    // 存储图片和文本
    QPixmap tipsPix;
    QString tipsText;

signals:
    // Tips数据就绪信号
    void tipsReady(const QPixmap &pix, const QString &text);

    // 加载开始信号
    void tipsLoadingStarted();
};

#endif // TIPSUPDATE_H
