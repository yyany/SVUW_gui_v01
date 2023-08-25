#ifndef VCL_IMGRESOURCE_H
#define VCL_IMGRESOURCE_H

#include <QObject>

class vcl_ImgResource : public QObject
{
    Q_OBJECT
        public:
                 explicit vcl_ImgResource(QObject *parent = nullptr);

signals:

};

#endif // VCL_IMGRESOURCE_H
