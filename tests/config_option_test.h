#ifndef CONFIG_OPTION_TEST_H
#define CONFIG_OPTION_TEST_H

#include <QObject>

class ConfigOptionTest : public QObject
{
    Q_OBJECT
private slots:
    void cliOverridesEnv();
};

#endif // CONFIG_OPTION_TEST_H
