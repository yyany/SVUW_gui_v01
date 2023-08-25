#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>

class VideoStreamer : public QObject
{
    Q_OBJECT
        public:
                 explicit VideoStreamer(QObject *parent = nullptr);

signals:

};

#endif // VIDEOSTREAMER_H
