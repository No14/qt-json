#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "../json.h"

using Json::Object;
using Json::Array;

QString readFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        return QString();
    } else {
        QTextStream in(&f);
        return in.readAll();
    }
}

void printResult(const Object &result)
{
    qDebug() << "encoding:" << result.value("encoding");
    qDebug() << "plugins:";

    Array plugins = result["plug-ins"].toList();
    foreach(QVariant plugin, plugins) {
        qDebug() << "  -" << plugin.toString();
    }

    Object indent = result["indent"].toMap();
    qDebug() << "length:" << indent["length"].toInt();
    qDebug() << "use_space:" << indent["use_space"].toBool();
}

void printJson(const Object &result)
{
    qDebug() << Json::stringify(result);
}

int main(int argc, char **argv)
{
    QString json = readFile("example.json");
    if (json.isEmpty()) {
        qFatal("Could not read JSON file!");
        return 1;
    }

    bool ok;
    Object result = Json::parse(json, ok).toMap();
    if (!ok) {
        qFatal("An error occurred during parsing");
        return 1;
    }

    printResult(result);

    // Add extra object to result which contains an array of keywords and a ratio.
    Array keywords = Array() << "json" << "qt" << "parser";
    Object extra;
    extra["ratio"] = 3.35;
    extra["keywords"] = keywords;
    result["extra"] = extra;

    printJson(result);

    return 0;
}
