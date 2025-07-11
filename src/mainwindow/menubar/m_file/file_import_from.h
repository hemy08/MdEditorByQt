//
// Created by Administrator on 25-7-5.
//

#ifndef HEMY_MENU_FILE_SUB_IMPORT_FROM_H
#define HEMY_MENU_FILE_SUB_IMPORT_FROM_H
#include <QMenu>
#include "common.h"
#include "menu_base.h"

namespace HemyMenu {
class HemyFileSubMenuImport final : public MenuBase {
    public:
        explicit HemyFileSubMenuImport(QWidget *parent = nullptr);
        ~HemyFileSubMenuImport() override = default;

        // 提供访问菜单项的公共接口
        QAction* fromWordAction() const { return actionFromWord; }
        QAction* fromHtmlAction() const { return actionFromHtml; }
        QAction* fromJsonAction() const { return actionFromJson; }
        QAction* fromYamlAction() const { return actionFromYaml; }
        QAction* fromXmlAction() const { return actionFromXml; }
        QAction* fromTxtAction() const { return actionFromTxt; }
    private:
        QAction* actionFromWord = nullptr;
        QAction* actionFromHtml = nullptr;
        QAction* actionFromJson = nullptr;
        QAction* actionFromYaml = nullptr;
        QAction* actionFromXml = nullptr;
        QAction* actionFromTxt = nullptr;

        void CreateFileSubMenuImport();
    };
}

#endif //HEMY_MENU_FILE_SUB_IMPORT_FROM_H
