#include "QUCoverArtArchiveCollector.h"
#include "QUCoverArtArchiveImageSource.h"
#include "QUCoverArtArchiveRequestUrl.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"

#include <QFile>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

QUCoverArtArchiveCollector::QUCoverArtArchiveCollector(QUSongInterface *song, QUCoverArtArchiveImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUCoverArtArchiveCollector::url() const {
	return new QUCoverArtArchiveRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("artist") << source()->songDataField("release") << source()->songDataField("album"),
			song(), true);
}

static bool urlExists(QString url_string) {
    QUrl url(url_string);
    QTcpSocket socket;
    socket.connectToHost(url.host(), 80);
    if (socket.waitForConnected()) {
        socket.write("HEAD " + url.path().toUtf8() + " HTTP/1.1\r\n"
                     "Host: " + url.host().toUtf8() + "\r\n\r\n");
        if (socket.waitForReadyRead()) {
            QByteArray bytes = socket.readAll();
            if (!bytes.contains("404")) {
                return true;
            }
        }
    }
    return false;
}

void QUCoverArtArchiveCollector::processSearchResults() {
	song()->log(tr("[QUCoverArtArchiveCollector] processSearchResults(), state() = ") + QString::number(state()), QU::Help);

	if (_inFirstQuery)
		_allUrls.clear();
	
	QJsonDocument jd = QJsonDocument::fromJson(buffer()->data());
	for (QJsonValue const& r : jd["releases"].toArray()) {
		QString url = "http://coverartarchive.org/release/" + r["id"].toString() + "/front";
		if (!urlExists(url))
		{
			song()->log(tr("[coverartarchive - cover] Not found: ") + url, QU::Error);
			continue;
		}
    	_allUrls << url;
		if (_allUrls.size() == source()->limit())
			break;
		song()->log(tr("[coverartarchive - song] Found song: ") + r["id"].toString(), QU::Help);
	}

	if (_allUrls.size() < source()->limit() && _inFirstQuery) {
		QUrl u(QUCoverArtArchiveRequestUrl(source()->host(),
			QStringList() << source()->songDataField("artist") << source()->songDataField("release") << source()->songDataField("album"),
			song(), false).request());
		QNetworkRequest request(u);
		request.setRawHeader("User-Agent", "Ultrastar Manager");
		request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
		_inFirstQuery = false;
		manager()->get(request);
		return;
	}

	_inFirstQuery = true;
		
	handleOldDownloads();

	ignoredUrls = qMax(0, _allUrls.size() - source()->limit());

	if(_allUrls.isEmpty()) {
		setState(Idle);
		communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);
	_remainingRequests = _allUrls.size();
	for(int i = 0; i < _allUrls.size() and i < source()->limit(); i++) {
		QNetworkRequest req(QUrl(_allUrls.at(i)));
		req.setRawHeader("User-Agent", "Ultrastar Manager");
		req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
		song()->log(tr("[coverartarchive - result] ") + req.url().toString(), QU::Help);
		manager()->get(req);
	}
}

void QUCoverArtArchiveCollector::processImageResults(QNetworkReply* reply) {
	song()->log(tr("[QUCoverArtArchiveCollector] processImageResults(), state() = ") + QString::number(state()), QU::Help);
	QUrl url = reply->url();
	QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(url.toString()).fileName()));
	if(file) {
		for (auto const& h : reply->rawHeaderPairs())
			song()->log(tr("[QUCoverArtArchiveCollector] header: ") + QString(h.first) + "->" + QString(h.second), QU::Help);

		song()->log(tr("[QUCoverArtArchiveCollector] saving:") + url.toString() + "->" + reply->error(), QU::Help);
		file->write(reply->readAll());
	}
	_remainingRequests--;
	if (_remainingRequests == 0) {
		setState(Idle);
		communicator()->send(tr("Search complete!"));
		communicator()->send(QUCommunicatorInterface::Done);
	}
}
