#ifndef SVGRENDERWORKER_H
#define SVGRENDERWORKER_H

#include <QObject>
#include <QPixmap>

class SVGRenderWorker : public QObject
{
    Q_OBJECT

public:
    explicit SVGRenderWorker(QObject *parent = nullptr);

    signals:
        void renderFinished(const QPixmap &renderedPixmap); // 渲染完成信号

    public slots:
        void renderSVG(const QString &filePath); // 接收文件路径并开始渲染

private:
    QPixmap renderSVGFile(const QString &filePath); // 渲染具体实现，作为接口
};

#endif // SVGRENDERWORKER_H
