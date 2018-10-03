#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_context(nullptr)
{
    /// Vybereme do jakeho platna bude toto okno vykreslovat
    /// V tomto pripade to bude OpenGL platno
    setSurfaceType(QWindow::OpenGLSurface);
}

/// Destruktor pouze slouzi k uvolneni pameti
OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
    delete m_functions;
}

/// Metodu pro vykresleni neimplementujeme, nechame je na konkretniho potomka.
void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

/// Metodu pro inicializaci neimplementujeme, nechame je na konkretniho potomka.
void OpenGLWindow::initialize()
{
}

/// Metodu pro vykresleni neimplementujeme, nechame je na konkretniho potomka.
void OpenGLWindow::render()
{
}

/// Zakladni slot, ktery je volan, kdyz nastane udalost.
/// Volan kdykoliv je vhodne okno prekreslit.
bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    /// Jestlize je okno potreba prekreslit, volej renderNow()
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    /// Zbytek udalosti at obslouzi predek
    default:
        return QWindow::event(event);
    }
}

/// Tato metoda je zavolana hned po predchozi
/// Je okno viditelne?
void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

/// Zakladni kostra mechanismu vykresleni
void OpenGLWindow::renderNow()
{
    /// Jestlize okno neni viditelne, neni potreba nic resit
    if (!isExposed())
        return;

    bool needsInitialize = false;

    /// Pokud dosud neexistuje, vytvori se pomocna trida pro vykreslovani
    /// a preda se ji nastaveni OpenGL
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    /// musim inicializovat krome kontextu take zastupnou tridu, ktera
    /// zpristupni OpenGL ES funkce a predat ji prave vytvoreny kontext
    m_functions = new QOpenGLFunctions(QOpenGLContext::currentContext());

    if (!m_functions) {
        qWarning() << "Could not obtain required OpenGL context version";
        exit(1);
    }

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    /// Zavola se hlavni vykresleni
    render();

    /// a provede se zapis do framebufferu
    m_context->swapBuffers(this);
}
