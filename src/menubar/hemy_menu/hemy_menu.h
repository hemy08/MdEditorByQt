//
// Created by zhaoj on 25-7-9.
//

#ifndef HEMY_MENU_H
#define HEMY_MENU_H
#include "menu_base.h"
#include "common.h"

namespace HemyMenu
{
    // 10. 自定义工具菜单映射
    class HMenuCustom final : public MenuBase {
    public:
        explicit HMenuCustom(QWidget *parent = nullptr);
        ~HMenuCustom() override = default;

        static void onCustomMenuActionHandler(const MenuItemID& itemId, const QString& objName, const QString& url, const QString& qml);
    private:
        void InitialCustomMenuItems();
    };

    // 11. 在线工具菜单映射
    class HMenuOnlineTool final : public MenuBase {
    public:
        explicit HMenuOnlineTool(QWidget *parent = nullptr);
        ~HMenuOnlineTool() override = default;

        static void onOnlineMenuActionHandler(const MenuItemID& itemId, const QString& objName, const QString& url);
    private:
        void InitialOnlineMenuItems();
    };

    // 12. 链接菜单映射
    class HMenuLinks final : public MenuBase {
    public:
        explicit HMenuLinks(QWidget *parent = nullptr);
        ~HMenuLinks() override = default;

        static void onLinksMenuActionHandler(const MenuItemID& itemId, const QString& objName, const QString& url);
    private:
        void InitialLinksMenuItems();
    };
}

#endif //HEMY_MENU_H
