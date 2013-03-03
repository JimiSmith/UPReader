/*
    A Google Reader client for Ubuntu Phone
    Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <QtXml/QtXml>
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "feedparser.h"
#include "articlelist.h"
#include "article.h"

FeedParser::FeedParser(QObject* parent)
    : QObject(parent)
{

}

FeedParser::~FeedParser()
{

}

void FeedParser::setFeedString(QString atomText)
{
    m_atomText = atomText;
}

void FeedParser::setFeedId(QString id)
{
    m_id = id;
}

/* Feed format
<feed> - top container
 <generator uri="http://www.google.com/reader">Google Reader</generator> - where the feed was generated
 <id>...</id> - id of the feed - more info below
 <title>xkcd</title>
 <subtitle type="html">The blag of the webcomic</subtitle>
 <gr:continuation>CLf-rJa08qgC</gr:continuation> - continuation attribute - this is used to indicate what feeds google should send next
 <link rel="self" href="link/to/this/feed"/>
 <link rel="alternate" href="http://blog.xkcd.com" type="text/html"/> main site where feed came from
 <updated>2011-05-18T21:06:23Z</updated> latest entry in the feed
 <entry gr:crawl-timestamp-msec="1305752783006">...</entry> - represents one story
 </feed>

the <id> tag contains a the id of the feed or story. e.g. tag:google.com,2005:reader/feed/http://blog.xkcd.com/feed/

<entry>

<id gr:original-id="http://blog.xkcd.com/?p=642">tag:google.com,2005:reader/item/e23cbbd18b20b990</id> - as before
<category term="user/00665486963053778909/state/com.google/read" scheme="http://www.google.com/reader/" label="read"/>
there are then a list of categories which hold the items state(unread/read) and other info
<category term="Uncategorized"/> - this is the folder it's in
<title type="html">Answering Ben Stein’s Question</title>
<published>2011-05-18T21:05:53Z</published>
<updated>2011-05-18T21:05:53Z</updated>
<link rel="alternate" href="http://blog.xkcd.com/2011/05/18/answering-ben-steins-question/" type="text/html"/>
- the link to the original story

<content xml:base="http://blog.xkcd.com/" type="html">
-the actual story. mimetype is specified and the original domain so that relative links work

<author>
<name>some name</name>
</author>
<gr:likingUser> - list of all(?) users that liked this post

<source gr:stream-id="feed/http://blog.xkcd.com/feed/"> - the feed that this article belongs to
<id>...</id> - as before
<title type="html">xkcd</title>
<link rel="alternate" href="http://blog.xkcd.com" type="text/html"/>
</source>

</entry>

*/

void FeedParser::parseFeed()
{
    bool error = false;

    QSqlQuery subQuery;
    subQuery.prepare("SELECT id FROM subscriptions WHERE google_id=:google_id");
    subQuery.bindValue(":google_id", m_id);
    int id = 0;
    if (subQuery.exec() && subQuery.next()) {
        id = subQuery.value(0).toInt();
    } else {
        return;
    }

    QSqlDatabase::database().transaction();
    QDomDocument doc;
    int unread = 0;
    if(!doc.setContent(m_atomText)) return;

    QDomElement root = doc.documentElement();
    if(root.tagName() != "feed") return;
    QDomNodeList l = root.childNodes();
    for(int i = 0; i < l.length(); ++i) {
        QDomNode n = l.at(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if(e.tagName() == "gr:continuation") { //the first element we're interested in is the gr:continuation element
//                map->setContinuationToken(e.text());
            } else if(e.tagName() == "entry") { //next is the entry element
                bool entryUnread = parseEntry(e, id, error);
                if (error) {
                    break;
                }
                if (entryUnread) {
                    unread++;
                }
            }
        }
    }
    if (error) {
        QSqlDatabase::database().rollback();
    } else {
        QSqlDatabase::database().commit();
    }

    QSqlQuery dataQuery;
    dataQuery.prepare("UPDATE subscriptions SET unread=:unread, needs_update=0 WHERE google_id=:google_id");
    dataQuery.bindValue(":unread", QString::number(unread));
    dataQuery.bindValue(":google_id", m_id);
    dataQuery.exec();
}

bool FeedParser::parseEntry(QDomElement entry, int id, bool &error)
{
    error = false;
    QHash<QString, QString> data;
    QStringList states;
    bool unread = false;
    QDomNodeList l = entry.childNodes();
    for(int i = 0; i < l.length(); ++i) {
        QDomNode n = l.at(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if(e.tagName() == "category") { //the category tag contains the read status
                if(e.attribute("term").contains("/state/com.google/")) { //this will represent one of googles states(e.g. read)
                    QString state = e.attribute("term").split("/").last();
                    states.append(state);
                } else { //tag name?
                    states.append(e.attribute("term"));
                }
            } else if(e.tagName() == "id") {
                data.insert("google_id", e.text());
            } else if(e.tagName() == "title") {
                data.insert("title", e.text());
            } else if(e.tagName() == "published") {
                QDateTime t = QDateTime::fromString("yyyy-MM-ddTHH:mm:ssZ"); //2011-05-18T21:05:53Z
                data.insert("published", t.toString());
            } else if(e.tagName() == "updated") {
                QDateTime t = QDateTime::fromString("yyyy-MM-ddTHH:mm:ssZ"); //2011-05-18T21:05:53Z
                data.insert("updated", t.toString());
            } else if(e.tagName() == "link") {
                data.insert("link", e.attribute("href"));
            } else if((e.tagName() == "content") || (e.tagName() == "summary")) { //the fun part
                data.insert("article_domain_name", e.attribute("xml:base"));
                QString type = e.attribute("type");
                QString content = e.text();
                if(type == "html" || type == "text") {
                    content = unescape(content);
                }
                data.insert("content", content);
            } else if(e.tagName() == "author") {
                //lets find the name of the author - not interested in the other stuff that can be here as well
                QString author;
                QDomNodeList le = e.childNodes();
                for(int j = 0; j < le.length(); ++j) {
                    QDomNode c = le.at(j);
                    QDomElement ce = c.toElement();
                    if(!ce.isNull()) {
                        if(ce.tagName() == "name") {
                            author = e.text();
                        }
                    }
                }
                data.insert("author", author);
            }
        }
    }
    unread = !states.contains("read");

    QString google_id = data.value("google_id");

    google_id.replace("?", "%3F");
    google_id.replace("=", "%3D");

    QSqlQuery existsQuery;
    existsQuery.prepare("SELECT count(google_id) FROM articles WHERE google_id=:google_id");
    existsQuery.bindValue(":google_id", google_id);
    bool existsRet = existsQuery.exec();

    if (!existsRet) {
        qWarning() << "Error with exists query" << existsQuery.lastError();
        error = true;
        return false;
    }

    bool exists = existsQuery.next() && existsQuery.value(0).toInt() > 0;

    QSqlQuery dataQuery;

    if (exists) {
        dataQuery.prepare("UPDATE articles SET title=:title, link=:link, "
                          "published=:published, updated=:updated, "
                          "article_domain_name=:article_domain_name, author=:author, "
                          "content=:content, unread=:unread, subscription_id=:subscription_id "
                          "WHERE google_id=:google_id");
    } else {
        dataQuery.prepare("INSERT INTO articles (google_id, title, link, published, updated, article_domain_name, author, content, unread, subscription_id) "
                          "VALUES (:google_id, :title, :link, :published, :updated, :article_domain_name, :author, :content, :unread, :subscription_id)");
    }

    dataQuery.bindValue(":google_id", google_id);
    dataQuery.bindValue(":title", data.value("title", ""));
    dataQuery.bindValue(":link", data.value("link", ""));
    dataQuery.bindValue(":published", data.value("published", ""));
    dataQuery.bindValue(":updated", data.value("updated", ""));
    dataQuery.bindValue(":article_domain_name", data.value("article_domain_name", ""));
    dataQuery.bindValue(":author", data.value("author", ""));
    dataQuery.bindValue(":content", data.value("content", ""));
    dataQuery.bindValue(":unread", unread);
    dataQuery.bindValue(":subscription_id", id);

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        qWarning() << "Error with update query" << dataQuery.lastError() << dataQuery.executedQuery();
        error = true;
        return false;
    }

    return unread;
}

QString FeedParser::unescape(QString s)
{
    return s;
    //N.B!! Always replace the ampersand last
    // 	return s.replace("&lt;", "<").replace("&gt;", ">").replace("&amp;", "&");
}

void FeedParser::beginParsing()
{
    parseFeed();
    doneParsing();
}

#include "feedparser.moc"
