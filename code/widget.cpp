#include "widget.h"
#include "widgetlayout.h"
#include "tipsupdate.h"
#include "weatherupdate.h"
#include "importantdaymanager.h"
#include "memomanager.h"
#include "managedaydialog.h"
#include "managememodialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QIODevice>
#include <QApplication>
#include <QMessageBox>
#include <QFont>
#include <QPainter>
#include <QBrush>
#include <QListWidgetItem>
#include <QPainterPath>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setFixedSize(342, 615);

    // 加载样式表
    QFile styleFile(":/styles/styles.qss");
    if (!styleFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "样式加载错误", "无法加载 styles.qss 文件，使用默认样式。");
    } else {
        QString style = styleFile.readAll();
        qApp->setStyleSheet(style);
        styleFile.close();
    }

    // 初始化UI组件
    ui = new WidgetLayout(this);

    // 初始化功能模块
    tips = new TipsUpdate(this);
    weather = new WeatherUpdate(this);
    dayMgr = new ImportantDayManager(this);
    memoMgr = new MemoManager(this);

    // 创建自定义标题栏
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setObjectName("titleBar");

    // 创建标题栏按钮
    minBtn = new QToolButton(titleBar);
    minBtn->setFixedSize(25, 25);
    minBtn->setIcon(QIcon(":/img/img/minimize.png"));
    minBtn->setIconSize(QSize(15, 15));

    closeBtn = new QToolButton(titleBar);
    closeBtn->setFixedSize(25, 25);
    closeBtn->setIcon(QIcon(":/img/img/close.png"));
    closeBtn->setIconSize(QSize(15, 15));

    // 设置标题栏布局
    QHBoxLayout *titleHLayout = new QHBoxLayout();
    titleHLayout->setContentsMargins(0, 0, 0, 0);
    titleHLayout->addStretch();
    titleHLayout->addWidget(minBtn);
    titleHLayout->addWidget(closeBtn);
    titleHLayout->addSpacing(10);

    QVBoxLayout *titleVLayout = new QVBoxLayout(titleBar);
    titleVLayout->setContentsMargins(0, 0, 0, 0);
    titleVLayout->addSpacing(5);
    titleVLayout->addLayout(titleHLayout);

    // 连接标题栏按钮信号
    connect(minBtn, &QToolButton::clicked, this, &Widget::showMinimized);
    connect(closeBtn, &QToolButton::clicked, this, &Widget::close);

    // 设置主布局
    auto *rootLot = new QVBoxLayout(this);
    rootLot->setContentsMargins(0, 0, 0, 0);
    rootLot->addWidget(titleBar);
    rootLot->addWidget(ui);

    // 显示加载状态
    ui->showLoading();
    weather->fetchWeather();

    ui->showTipsLoading();
    tips->fetchQuote();

    // 刷新数据
    refreshMemo();

    // 连接天气相关信号
    connect(weather, &WeatherUpdate::loadingStarted, ui, &WidgetLayout::showLoading);
    connect(ui->getUpDateBtn(), &QToolButton::clicked, weather, &WeatherUpdate::fetchWeather);
    connect(weather, &WeatherUpdate::weatherReady, ui, &WidgetLayout::setWeatherInfo);

    // 连接Tips相关信号
    connect(tips, &TipsUpdate::tipsLoadingStarted, ui, &WidgetLayout::showTipsLoading);
    connect(ui->getTipsUpDateBtn(), &QToolButton::clicked, tips, &TipsUpdate::fetchQuote);
    connect(tips, &TipsUpdate::tipsReady, this, [=](const QPixmap &pix, const QString &text) {
        ui->setTipsInfo(pix, text);
        refreshDayTip();
    });

    // 连接重要日按钮信号
    if (ui->getAddDayBtn()) {
        connect(ui->getAddDayBtn(), &QToolButton::clicked, this, [=]{
            ManageDayDialog dia(dayMgr, this);
            dia.exec();
            refreshDayTip();
        });
    }

    // 连接备忘录按钮信号
    if (ui->getAddMemoBtn()) {
        connect(ui->getAddMemoBtn(), &QToolButton::clicked, this, [=]{
            ManageMemoDialog dia(memoMgr, this);
            dia.exec();
            refreshMemo();
        });
    }

    // 连接备忘录列表双击信号
    if (ui->getMemoTable()) {
        connect(ui->getMemoTable(), &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
            if (!item) {
                QMessageBox::warning(this, "操作错误", "请先选择一行！");
                return;
            }

            QString title = item->text();
            int id = memoMgr->getIdByTitle(title);
            if (id <= 0) {
                QMessageBox::warning(this, "错误", "无效备忘录 ID！");
                return;
            }

            ManageMemoDialog dia(memoMgr, this, id);
            dia.exec();
            refreshMemo();
        });
    }

    // 设置字体
    QFont font = QApplication::font();
    font.setPointSize(10);
    setFont(font);

    // 设置窗口透明背景
    setAttribute(Qt::WA_TranslucentBackground);
}

Widget::~Widget() {
    delete ui;
    delete tips;
    delete weather;
    delete dayMgr;
    delete memoMgr;
    delete titleBar;
}

// 绘制圆角
void Widget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 添加圆角剪裁路径（恢复原圆角效果，radius 10）
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    // 使用QSS样式绘制背景
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);
}

// 鼠标按下事件
void Widget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->pos().y() < 30) {
        isDragging = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

// 鼠标移动事件
void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

// 鼠标释放事件
void Widget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        event->accept();
    }
}

// 刷新重要日提示
void Widget::refreshDayTip() {
    QString title;
    int diff = 0;

    if (dayMgr->nearestDay(title, diff)) {
        if (diff < 0) {
            diff = 0;
        }
        ui->getImportantLbl()->setText(QString("距离 %1 还有 %2 天").arg(title).arg(diff));
    } else {
        ui->getImportantLbl()->setText("最近有什么计划嘛？");
    }
}

// 刷新备忘录列表
void Widget::refreshMemo() {
    ui->getMemoTable()->clear();

    auto memos = memoMgr->listMemos();
    int row = 0;

    for (const auto &title : memos) {
        if (title.trimmed().isEmpty()) continue;

        ui->getMemoTable()->addItem(title);
        if (++row >= 3) break;
    }

    if (ui->getMemoTable()->count() == 0) {
        ui->getMemoTable()->addItem("暂无备忘录记录");
    }
}
