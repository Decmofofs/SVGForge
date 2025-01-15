#ifndef SVGRENDERWORKER_H
#define SVGRENDERWORKER_H

#include <QObject>
#include <QPixmap>

class SVGRenderWorker : public QObject {
    Q_OBJECT

public:
    explicit SVGRenderWorker(QObject *parent = nullptr);

signals:
    void renderFinished(const QPixmap &renderedPixmap);

public slots:
    void renderSVG(const QString &filePath);

private:
    QPixmap renderSVGFile(const QString &filePath);
};

#endif // SVGRENDERWORKER_H
