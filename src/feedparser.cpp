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

#include "feedparser.h"

FeedParser::FeedParser(QObject* parent)
	: QObject(parent)
{

}

FeedParser::~FeedParser()
{

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

QVariantMap FeedParser::parseFeed(QString atom)
{
	QDomDocument doc;
	QVariantMap map;
	QVariantList entries;
	if(!doc.setContent(atom)) return QVariantMap();

	QDomElement root = doc.documentElement();
	if(root.tagName() != "feed") return QVariantMap();
	QDomNodeList l = root.childNodes();
	for(int i = 0; i < l.length(); ++i) {
		QDomNode n = l.at(i);
		QDomElement e = n.toElement();
		if(!e.isNull()) {
			if(e.tagName() == "gr:continuation") { //the first element we're interested in is the gr:continuation element
				map.insert("cont", e.text());
			} else if(e.tagName() == "entry") { //next is the entry element
				QVariantMap entry = parseEntry(e);
				entries.append(entry);
			}
		}
	}
	map.insert("entries", entries);
	return map;
}

QVariantMap FeedParser::parseEntry(QDomElement entry)
{
	QVariantMap map;
	QDomNodeList l = entry.childNodes();
	for(int i = 0; i < l.length(); ++i) {
		QDomNode n = l.at(i);
		QDomElement e = n.toElement();
		if(!e.isNull()) {
			if(e.tagName() == "category") { //the category tag contains the read status
				if(e.attribute("term").contains("/state/com.google/")) { //this will represent one of googles states(e.g. read)
					QStringList l = map.value("state").toStringList();
					QString state = e.attribute("term").split("/").last();
					l.append(state);
					map["state"] = l;
				} else { //tag name?
					map.insert("tag", e.attribute("term"));
				}
			} else if(e.tagName() == "title") {
				map.insert("titleType", e.attribute("type"));
				map.insert("titleText", e.text());
			} else if(e.tagName() == "published") {
				QDateTime t = QDateTime::fromString("yyyy-MM-ddTHH:mm:ssZ"); //2011-05-18T21:05:53Z
				map.insert("published", t);
			} else if(e.tagName() == "updated") {
				QDateTime t = QDateTime::fromString("yyyy-MM-ddTHH:mm:ssZ"); //2011-05-18T21:05:53Z
				map.insert("updated", t);
			} else if(e.tagName() == "link") {
				map.insert("link", e.attribute("href"));
			} else if((e.tagName() == "content") || (e.tagName() == "summary")) { //the fun part
				map.insert("origDomain", e.attribute("xml:base"));
				QString type = e.attribute("type");
				map.insert("contentType", type);
				QString content = e.text();
				if(type == "html" || type == "text") {
					content = unescape(content);
				}
				map.insert("contentText", content);
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
				map.insert("author", author);
			}
		}
	}
	return map;
}

QString FeedParser::unescape(QString s)
{
	return s;
	//N.B!! Always replace the ampersand last
// 	return s.replace("&lt;", "<").replace("&gt;", ">").replace("&amp;", "&");
}

#include "feedparser.moc"
