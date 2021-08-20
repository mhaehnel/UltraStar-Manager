#ifndef QUCOVERARTARCHIVEREQUESTURL_H
#define QUCOVERARTARCHIVEREQUESTURL_H

#include "QURequestUrl.h"

class QUCoverArtArchiveRequestUrl: public QURequestUrl {
public:
	QUCoverArtArchiveRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song, bool tryFirst);
	virtual QString request() const;

private:
	void initQuery();
	bool _tryFirst;
};

#endif // QUCOVERARTARCHIVEREQUESTURL_H
