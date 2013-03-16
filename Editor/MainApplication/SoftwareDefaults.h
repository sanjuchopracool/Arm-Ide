#ifndef SOFTWAREDEFAULTS_H
#define SOFTWAREDEFAULTS_H

#include <QObject>

struct SoftwareDefaultsData;
class SoftwareDefaults : public QObject
{
    Q_OBJECT
public:
    explicit SoftwareDefaults(QObject *parent = 0);
    static SoftwareDefaults& instance() ;

    void setDefaultProjectDir(const QString& projectDir);
    QString defaultProjectDir() const;

    void setToolChainPrefix(const QString& prefix);
    QString toolChainPrefix() const;

    void save();
    void load();
    
signals:
    
public slots:
    void modifiedSettings();
private:
    SoftwareDefaultsData* d;
    
};

#endif // SOFTWAREDEFAULTS_H
