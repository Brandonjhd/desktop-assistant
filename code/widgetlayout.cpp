#include "widgetlayout.h"
#include <QIcon>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QAbstractItemView>
#include <QSizePolicy>
#include <QDebug>

WidgetLayout::WidgetLayout(QWidget *parent) : QWidget(parent) {
    // 初始化天气组件
    nowlbl = new QLabel(this);
    nowimglbl = new QLabel(this);
    futurelbl_1 = new QLabel(this);
    futurelbl_2 = new QLabel(this);
    futurelbl_3 = new QLabel(this);
    futurelbl_4 = new QLabel(this);
    updatebtn = new QToolButton(this);

    // 设置天气组件尺寸
    nowlbl->setFixedSize(173, 140);
    nowimglbl->setFixedSize(95, 140);
    futurelbl_1->setFixedSize(75, 94);
    futurelbl_2->setFixedSize(75, 94);
    futurelbl_3->setFixedSize(75, 94);
    futurelbl_4->setFixedSize(75, 94);
    updatebtn->setFixedSize(25, 25);

    // 设置天气更新按钮
    QIcon updateIcon(":/img/img/updatebtn.png");
    if (updateIcon.isNull()) {
        updatebtn->setText("更新");
    } else {
        updatebtn->setIcon(updateIcon);
        updatebtn->setIconSize(updatebtn->size());
    }
    updatebtn->setAutoRaise(true);
    updatebtn->setFocusPolicy(Qt::NoFocus);

    // 创建天气布局
    nowlot = new QHBoxLayout();
    nowlot->addWidget(nowlbl);
    nowlot->addWidget(nowimglbl);
    nowlot->addWidget(updatebtn);

    futurelot = new QHBoxLayout();
    futurelot->setSpacing(5);
    futurelot->addWidget(futurelbl_1);
    futurelot->addWidget(futurelbl_2);
    futurelot->addWidget(futurelbl_3);
    futurelot->addWidget(futurelbl_4);

    weatherlot = new QVBoxLayout();
    weatherlot->setSpacing(5);
    weatherlot->addLayout(nowlot);
    weatherlot->addLayout(futurelot);

    // 创建天气框架
    weatherFrame = new QFrame(this);
    weatherFrame->setFixedSize(320, 255);
    weatherFrame->setLayout(weatherlot);

    // 添加天气框架阴影
    QGraphicsDropShadowEffect *weatherShadow = new QGraphicsDropShadowEffect;
    weatherShadow->setBlurRadius(5);
    weatherShadow->setColor(QColor(0, 0, 0, 50));
    weatherShadow->setOffset(2, 2);
    weatherFrame->setGraphicsEffect(weatherShadow);

    // 初始化Tips组件
    tipsimglbl = new QLabel(this);
    tipslbl = new QLabel(this);
    tipslbl->setObjectName("Tipslbl");
    importantlbl = new QLabel(this);
    importantlbl->setObjectName("Tipslbl");
    tipsupdatebtn = new QToolButton(this);
    adddaybtn = new QToolButton(this);

    // 设置Tips组件属性
    tipsimglbl->setFixedSize(320, 180);
    tipslbl->setFixedWidth(220);
    importantlbl->setFixedWidth(220);
    tipslbl->setWordWrap(true);
    importantlbl->setWordWrap(true);
    tipsimglbl->setAlignment(Qt::AlignCenter);
    tipslbl->setAlignment(Qt::AlignCenter);
    importantlbl->setAlignment(Qt::AlignCenter);

    // 设置Tips更新按钮
    tipsupdatebtn->setFixedSize(25, 25);
    QIcon tipsUpdateIcon(":/img/img/updatebtn.png");
    if (tipsUpdateIcon.isNull()) {
        tipsupdatebtn->setText("更新");
    } else {
        tipsupdatebtn->setIcon(tipsUpdateIcon);
        tipsupdatebtn->setIconSize(tipsupdatebtn->size());
    }
    tipsupdatebtn->setAutoRaise(true);
    tipsupdatebtn->setFocusPolicy(Qt::NoFocus);

    // 设置添加重要日按钮
    adddaybtn->setFixedSize(25, 25);
    QIcon addDayIcon(":/img/img/addbtn.png");
    if (addDayIcon.isNull()) {
        adddaybtn->setText("+");
    } else {
        adddaybtn->setIcon(addDayIcon);
        adddaybtn->setIconSize(adddaybtn->size());
    }
    adddaybtn->setAutoRaise(true);

    // 创建Tips框架
    tipsFrame = new QFrame(this);
    tipsFrame->setFixedWidth(320);

    // 添加Tips框架阴影
    QGraphicsDropShadowEffect *tipsShadow = new QGraphicsDropShadowEffect;
    tipsShadow->setBlurRadius(5);
    tipsShadow->setColor(QColor(0, 0, 0, 50));
    tipsShadow->setOffset(2, 2);
    tipsFrame->setGraphicsEffect(tipsShadow);

    // 创建Tips叠加布局
    tipsStack = new QStackedLayout(tipsFrame);
    tipsStack->setStackingMode(QStackedLayout::StackAll);

    // 创建Tips叠加容器
    QWidget *tipsOverlay = new QWidget(this);
    tipsOverlay->setFixedSize(tipsimglbl->size());
    QGridLayout *overlayLot = new QGridLayout(tipsOverlay);
    overlayLot->setContentsMargins(0, 0, 0, 0);

    // 创建文字容器
    QWidget *textBox = new QWidget(tipsOverlay);
    textBox->setObjectName("semiTransparentBox");
    textBox->setFixedWidth(220);
    QVBoxLayout *textLot = new QVBoxLayout(textBox);
    textLot->setContentsMargins(0, 0, 0, 0);
    textLot->addWidget(tipslbl);

    // 创建文字间距
    tipsTextSpacer = new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);
    textLot->addItem(tipsTextSpacer);
    textLot->addWidget(importantlbl);
    textLot->setAlignment(Qt::AlignCenter);

    // 添加到叠加布局
    overlayLot->addWidget(tipsimglbl, 0, 0);
    overlayLot->addWidget(textBox, 0, 0, Qt::AlignCenter);
    textBox->raise();

    // 创建Tips按钮布局
    QHBoxLayout *tipsBtnLot = new QHBoxLayout();
    tipsBtnLot->addSpacing(9);
    tipsBtnLot->addWidget(tipsupdatebtn);
    tipsBtnLot->addStretch();
    tipsBtnLot->addWidget(adddaybtn);
    tipsBtnLot->addSpacing(9);

    // 创建Tips内容容器
    QWidget *tipsContent = new QWidget(tipsFrame);
    tipsVlot = new QVBoxLayout(tipsContent);
    tipsVlot->setContentsMargins(0, 0, 0, 0);
    tipsVlot->addWidget(tipsOverlay);
    tipsVlot->addLayout(tipsBtnLot);
    tipsVlot->addSpacing(5);

    // 添加到Tips栈布局
    tipsStack->addWidget(tipsContent);

    // 初始化备忘录组件
    memoTable = new QListWidget(this);
    memoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    memoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memoTable->setFixedHeight(80);

    addmemobtn = new QToolButton(this);
    addmemobtn->setFixedSize(25, 25);
    QIcon addMemoIcon(":/img/img/addbtn.png");
    if (addMemoIcon.isNull()) {
        addmemobtn->setText("+");
    } else {
        addmemobtn->setIcon(addMemoIcon);
        addmemobtn->setIconSize(addmemobtn->size());
    }
    addmemobtn->setAutoRaise(true);

    // 创建备忘录布局
    memolot = new QVBoxLayout();
    memolot->addWidget(memoTable);

    QHBoxLayout *memoTop = new QHBoxLayout();
    memoTop->addLayout(memolot, 1);
    memoTop->addStretch();
    memoTop->addWidget(addmemobtn);

    // 创建备忘录框架
    memoFrame = new QFrame(this);
    memoFrame->setFixedSize(320, 88);
    memoFrame->setLayout(memoTop);

    // 添加备忘录框架阴影
    QGraphicsDropShadowEffect *memoShadow = new QGraphicsDropShadowEffect;
    memoShadow->setBlurRadius(5);
    memoShadow->setColor(QColor(0, 0, 0, 50));
    memoShadow->setOffset(2, 2);
    memoFrame->setGraphicsEffect(memoShadow);

    // 创建主布局
    mainlot = new QVBoxLayout(this);
    mainlot->setContentsMargins(10, 0, 10, 10);
    mainlot->addWidget(weatherFrame);
    mainlot->addWidget(tipsFrame);
    mainlot->addWidget(memoFrame);
}

// 设置天气信息
void WidgetLayout::setWeatherInfo(const QString &info, const QPixmap &pix, const QString infos[4]) {
    // 设置当前天气信息
    nowlbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    nowlbl->setText(info);

    // 设置当前天气图标
    if (!pix.isNull()) {
        nowimglbl->setPixmap(pix.scaled(80, 80));
        nowimglbl->setAlignment(Qt::AlignCenter);
    } else {
        nowimglbl->setText("图片错误");
    }

    // 设置未来天气信息
    if (infos) {
        QLabel *arr[4] = {futurelbl_1, futurelbl_2, futurelbl_3, futurelbl_4};
        for (int i = 0; i < 4; ++i) {
            if (!infos[i].isEmpty()) {
                arr[i]->setAlignment(Qt::AlignLeft | Qt::AlignTop);
                arr[i]->setText(infos[i]);
                QFont font = arr[i]->font();
                font.setPointSize(8);
                arr[i]->setFont(font);
            } else {
                arr[i]->setText("数据缺失");
            }
        }
    }
}

// 显示天气加载状态
void WidgetLayout::showLoading() {
    QPixmap load(":/img/img/loading.png");
    if (load.isNull()) {
        nowlbl->setText("加载中...");
        return;
    }

    QLabel *arr[6] = {nowlbl, nowimglbl, futurelbl_1, futurelbl_2, futurelbl_3, futurelbl_4};
    for (auto *l : arr) {
        if (l) {
            l->setPixmap(load.scaled(15, 15));
            l->setAlignment(Qt::AlignCenter);
        }
    }
}

// 设置Tips信息
void WidgetLayout::setTipsInfo(const QPixmap &pix, const QString &text) {
    // 设置Tips图片
    if (pix.isNull()) {
        tipsimglbl->setText("图片无效");
    } else {
        QPixmap scaled = pix.scaled(tipsimglbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        tipsimglbl->setPixmap(scaled);
        tipsimglbl->setObjectName("tipsImage");
    }

    // 设置Tips文本
    if (text.isEmpty()) {
        tipslbl->setText("文本为空");
    } else {
        tipslbl->setText(text);
    }

    // 恢复文字间距
    setTipsTextSpacing(5);
}

// 显示Tips加载状态
void WidgetLayout::showTipsLoading() {
    QPixmap load(":/img/img/loading.png");
    if (load.isNull()) {
        tipsimglbl->setText("加载中...");
    } else {
        tipsimglbl->setPixmap(load.scaled(15, 15));
        tipsimglbl->setAlignment(Qt::AlignCenter);
    }

    // 清空文字内容
    tipslbl->clear();
    importantlbl->clear();

    // 调整间距
    setTipsTextSpacing(45);
}

// 设置Tips文字间距
void WidgetLayout::setTipsTextSpacing(int spacing) {
    if (spacing < 0) {
        qWarning() << "间距值无效，使用默认值0";
        spacing = 0;
    }

    // 动态调整间距
    tipsTextSpacer->changeSize(0, spacing, QSizePolicy::Minimum, QSizePolicy::Fixed);

    // 强制更新布局
    layout()->invalidate();
}
