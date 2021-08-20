#include "QUCoverArtArchiveRequestUrl.h"

#include "QU.h"
#include "QUStringSupport.h"
#include "QUSongSupport.h"
#include "QUSongInterface.h"

#include <QVariant>
#include <QUrlQuery>
#include <QSettings>

QUCoverArtArchiveRequestUrl::QUCoverArtArchiveRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song, bool tryFirst): QURequestUrl(host, properties, song), _tryFirst(tryFirst) {
	initQuery();
}

QString QUCoverArtArchiveRequestUrl::request() const {
	QString result = QString("http://%1/ws/2/release/?%2")
			.arg(host())
			.arg(QString(fixedPercentageEncoding(this->query().toUtf8())));

	song()->log(tr("[coverartarchive - search] ") + result, QU::Help);

	return result;
}

void QUCoverArtArchiveRequestUrl::initQuery() {
	QStringList data;
	QStringList props = properties();
	song()->log(tr("Props: %1").arg(properties().size()),QU::Help);
	for (auto const& part : QUStringSupport::withoutAnyUmlaut(song()->customTag(props.takeFirst())).split(" ", Qt::SkipEmptyParts))
		data << "artist:" + part;
	if (!_tryFirst)
		props.takeFirst();
	for (auto const& part : QUStringSupport::withoutAnyUmlaut(song()->customTag(props.takeFirst())).split(" ", Qt::SkipEmptyParts))
		data << "release:" + part;
	
	QSettings settings;
	data << "status:" + settings.value("coverartarchive/status").toString();
	if (_tryFirst)
		data << "primarytype:" + settings.value("coverartarchive/primary type").toString();
	else
		data << "primarytype:" + settings.value("coverartarchive/secondary type").toString();
	
	QUrlQuery query;
	query.addQueryItem("query", data.join(" AND ").trimmed());
	query.addQueryItem("fmt", "json");
	this->setQuery(query);
}
