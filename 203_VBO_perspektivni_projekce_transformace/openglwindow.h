#include <QtGui/QWindow>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QOpenGLFunctions>
#include <QtDebug>

/// Trida je potomkem QWindow, protoje schopna vykreslovat na canvas
/// Dale dedi po QOpenGLFunctions, to se vyuziva pro OpenGL ES
/// Tato trida je obecnym predkem jakehokoliv okna pro vykreslovani
/// OpenGL obsahu. Je to sablona ze ktere se vytvori potomek a
/// doimplementuje se jeho funkcionalita.
class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    // metody zdedene po QWindow, slouzi k inicializaci a vykreslovani
    virtual void render(QPainter *painter);
    virtual void render();
    virtual void initialize();

public slots:
    /// Zakladni slot, ktery je vyvolan, kdyz dojde k udalosti prekresleni okna
    void renderNow();

protected:
    // metody zdedene po QWindow, slouzi ke zpracovani udalosti
    bool event(QEvent *event);
    void exposeEvent(QExposeEvent *event);

private:
    /// Ceka se jiz na prekresleni okna?
    bool m_update_pending;
    /// Platno, ktere se pouziva pro vykreslovani
    QOpenGLContext *m_context;

protected:
    /// Trida umoznujici pristup k beznym OpenGL funkcim
    QOpenGLFunctions* m_functions;
};

