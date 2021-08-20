#include "QUCoverArtArchiveImageSource.h"
#include "QUCoverArtArchiveCollector.h"

#include <QCoreApplication>
#include <QString>
#include <QLocale>

QUCoverArtArchiveImageSource::QUCoverArtArchiveImageSource(QObject *parent): QUMultilingualImageSource(parent) {}

QStringList QUCoverArtArchiveImageSource::hosts() const {
	return QStringList() << "www.musicbrainz.org";
}

QURemoteImageCollector* QUCoverArtArchiveImageSource::imageCollector(QUSongInterface *song) {
	return new QUCoverArtArchiveCollector(song, this);
}

QString QUCoverArtArchiveImageSource::help(const QString &field) const {
	if(QString::compare(field, customDataFields().first(), Qt::CaseInsensitive) == 0)
			return tr("Let's you specify the search domain.<br><br>ALL ... all<br>ARTIST ... artist<br>COMPOSER ... composer<br>TITLE ... title<br>UPLOADER ... by uploader<br>ARTISTTITLE ... artist & title");
	else if(QString::compare(field, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return tr("Let's you specify the result order. This helps you to better handle the download limit.<br><br>ARTIST ... by artist<br>COMPOSER ... by composer<br>TITLE ... title<br>DATE ... by date<br>POSTER ... by uploader<br>VIEWS ... by  most viewed<br>RATING ... by rating");
	else
		return "";
}

QString QUCoverArtArchiveImageSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTIST";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "TITLE";
	else if(QString::compare(key, songDataFields().at(2), Qt::CaseInsensitive) == 0)
		return "ALBUM";
	else if(QString::compare(key, customDataFields().first(), Qt::CaseInsensitive) == 0)
		return "official";
	else if(QString::compare(key, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "Single";
	else if(QString::compare(key, customDataFields().at(2), Qt::CaseInsensitive) == 0)
		return "Album";
	else
		return QUMultilingualImageSource::defaultValue(key);
}

QMap<QString, QString> QUCoverArtArchiveImageSource::translationLocations() const {
	return QMap<QString, QString>();
}
