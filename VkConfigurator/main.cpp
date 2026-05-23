#include "VkConfigurator.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VkConfigurator window;
    window.show();
    return app.exec();
}
