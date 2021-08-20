#ifndef QUCOVERARTARCHIVECOLLECTOR_H
#define QUCOVERARTARCHIVECOLLECTOR_H

#include "QUHttpCollector.h"

class QUCoverArtArchiveImageSource;

class QUCoverArtArchiveCollector: public QUHttpCollector {
	Q_OBJECT

public:
	QUCoverArtArchiveCollector(QUSongInterface *song, QUCoverArtArchiveImageSource *source = 0);

protected:
	virtual QURequestUrl* url() const;
	virtual void processSearchResults();
	virtual void processImageResults(QNetworkReply* reply);
private:
	int _remainingRequests = 0;
	bool _inFirstQuery = true;
	QStringList _allUrls;
};

#endif // QUCOVERARTARCHIVECOLLECTOR_H
