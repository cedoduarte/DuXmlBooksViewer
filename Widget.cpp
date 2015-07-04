#include "Widget.h"
#include "ui_Widget.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels({"Catálogo", "Datos"});
    QFile booksFile(":/books.xml");
    if (!booksFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", booksFile.errorString());
        return;
    }
    QDomDocument doc;
    doc.setContent(&booksFile);
    auto rootDoc = doc.documentElement().toElement();
    auto rootTree = new QTreeWidgetItem({rootDoc.tagName()});
    ui->treeWidget->addTopLevelItem(rootTree);
    auto ix = rootDoc.firstChild().toElement();
    while (!ix.isNull()) {
        auto book = new QTreeWidgetItem({ix.tagName()});
        auto iy = ix.firstChild().toElement();
        while (!iy.isNull()) {
            auto child = new QTreeWidgetItem({iy.tagName(), iy.toElement().text()});
            book->addChild(child);
            iy = iy.nextSibling().toElement();
        }
        rootTree->addChild(book);
        ix = ix.nextSibling().toElement();
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_quitarPushButton_clicked()
{
    close();
}
