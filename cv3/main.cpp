#include <QtGui/QGuiApplication>
#include "trianglewindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize( 24 );
    format.setMajorVersion( 3 );
    format.setMinorVersion( 1 );
    format.setSamples(4);
    format.setProfile( QSurfaceFormat::CoreProfile );

    TriangleWindow window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    return app.exec();
}