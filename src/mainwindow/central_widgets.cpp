//
// Created by Administrator on 25-6-29.
//

#include "central_widgets.h"

namespace HemyUI {
    QWidget* CentralWidgets::CreateCentralWidgets(QMainWindow *window)
    {
        QWidget*  centralWidget = new QWidget(window);
        centralWidget->setObjectName("central widget");
        return centralWidget;
    }
} // HemyUI