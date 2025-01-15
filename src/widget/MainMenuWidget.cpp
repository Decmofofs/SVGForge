#include "MainMenuWidget.h"
#include <QSvgRenderer>
#include "GlobalData.h"
#include <QMessageBox>
#include <QLabel>
MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent) {

    QFile file(":/resources/buttons/OpenFolder.svg");
    if (!file.exists()) {
        qDebug() << "Resource file not found: " << file.fileName();
    }
    else {
        qDebug() << "Resource file found: " << file.fileName();
    }

    openFileButton = new QPushButton( this);
    openFileButton->setFixedSize(120,120);
    openFileButton->setStyleSheet(
        "QPushButton {"

        "    background: transparent;"
        "    background-image: url(:/resources/buttons/OpenFolder.svg);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(215,219,229,0.2);"
        "    background-image: url(:/resources/buttons/OpenFolder.svg);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "}"
    );

    openFileButton->setToolTip("打开 SVG 文件");

    exportFileButton = new QPushButton(this);
    exportFileButton->setFixedSize(120, 120);
    exportFileButton->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    background-image: url(:/resources/buttons/Export.svg);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(215,219,229,0.2);"
        "    background-image: url(:/resources/buttons/Export.svg);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "}"
    );

    exportFileButton->setToolTip("导出 PNG 文件");

    scaleSlider = new QSlider(Qt::Horizontal, this);
    scaleSlider->setRange(1, 8);
    scaleSlider->setValue(static_cast<int>(scale)*100/50);
    scaleLabel = new QLabel("缩放比例: 100%", this);



    QGridLayout *sliderLayout = new QGridLayout();
    sliderLayout->addWidget(scaleLabel, 0,0, Qt::AlignCenter);
    sliderLayout->addWidget(scaleSlider, 1, 0, Qt::AlignCenter);
    sliderLayout->setSpacing(0);
    sliderLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(openFileButton, Qt::AlignTop);
    layout->addLayout(sliderLayout);
    layout->addWidget(exportFileButton, Qt::AlignBottom);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(120);


    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);

    connect(openFileButton, &QPushButton::clicked, this, &MainMenuWidget::openFileDialog);
    connect(exportFileButton, &QPushButton::clicked, this, &MainMenuWidget::exportFileDialog);
    connect(scaleSlider, &QSlider::valueChanged, this, &MainMenuWidget::scaleChanged);
}

void MainMenuWidget::openFileDialog() {

    QString filePath = QFileDialog::getOpenFileName(this, "选择 SVG 文件", "", "SVG Files (*.svg)");
    if (!filePath.isEmpty()) {
        emit fileSelected(filePath);
    }
}

void MainMenuWidget::exportFileDialog() {

    if (renderFinished == 0) {
        QMessageBox::warning(this, "警告", "请先打开一个 SVG 文件并等待渲染完成", QMessageBox::Ok);
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "导出 PNG 文件", "test.png", "PNG Files (*.png)");
    if (!filePath.isEmpty()) {
        if (globalPix->save(filePath, "PNG")) {
            QMessageBox::information(this, "成功", "导出成功", QMessageBox::Ok);
        } else {
            QMessageBox::warning(this, "失败", "导出失败", QMessageBox::Ok);
        }
    }
}

void MainMenuWidget::scaleChanged(int value) {
    scale = value*50/100.0;
    scaleLabel->setText(QString("缩放比例: %1%").arg(value * 50));
}