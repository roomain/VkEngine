#include "VkConfigurator.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/VkConfigurator/resources/vulkanConfIcon.png"));
    VkConfigurator window;
    window.show();
    return app.exec();
}
