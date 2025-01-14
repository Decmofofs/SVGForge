#include "MainMenuWidget.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent)

{
    // 初始化按钮
    openFileButton = new QPushButton("打开 SVG 文件", this);

    // 布局管理
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(openFileButton);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    // 连接按钮点击信号到槽函数
    connect(openFileButton, &QPushButton::clicked, this, &MainMenuWidget::openFileDialog);
}

void MainMenuWidget::openFileDialog()
{
    // 打开文件资源管理器并过滤文件类型
    QString filePath = QFileDialog::getOpenFileName(this, "选择 SVG 文件", "", "SVG Files (*.svg)");
    if (!filePath.isEmpty()) {
        emit fileSelected(filePath); // 发出文件选择信号
    }
}
