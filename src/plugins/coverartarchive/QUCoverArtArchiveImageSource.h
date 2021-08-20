#ifndef QUCOVERARTARCHIVEIMAGESOURCE_H
#define QUCOVERARTARCHIVEIMAGESOURCE_H

#include "QUMultilingualImageSource.h"

class QUCoverArtArchiveImageSource: public QUMultilingualImageSource {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "QUCoverArtArchiveCollector" FILE "coverartarchive.json")

public:
	QUCoverArtArchiveImageSource(QObject *parent = 0);

	virtual ImageType type() const { return CoverImage; }

	virtual QString name() const { return "CoverArtArchive.org"; }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return "Marcus Haehnel"; }
	virtual QString description() const { return tr("Get splendid covers with high resolution for your songs."); }

	virtual QStringList songDataFields() const { return QStringList() << "artist" << "release" << "album"; }
	virtual QStringList customDataFields() const { return QStringList() << "Status" << "Primary Type" << "Secondary Type"; }
	virtual QString help(const QString &field) const;
	virtual QStringList hosts() const;

protected:
	virtual QString registryKey() const { return "coverartarchive"; }
	virtual QString defaultValue(const QString &key) const;
	virtual QMap<QString, QString> translationLocations() const;
	virtual QURemoteImageCollector* imageCollector(QUSongInterface*);
};

#endif // QUCOVERARTARCHIVEIMAGESOURCE_H
