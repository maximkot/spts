// $Id: tsziinifile.cpp 241 2009-10-30 12:25:41Z griffin $

#include "tsziinifile.h"
#include "fcntl.h"

//#include <iostream>

bool useNomacl=true;

TSziIniFile::TSziIniFile(QString iniFileName)
{
 initialize(iniFileName, QTextCodec::codecForCStrings());
}

TSziIniFile::TSziIniFile(QString iniFileName, QTextCodec * codec)
{
 initialize(iniFileName, codec);
}

TSziIniFile::TSziIniFile(QString iniFileName, const char *codecName)
{
 initialize(iniFileName, QTextCodec::codecForName(codecName));
}

void TSziIniFile::initialize(QString iniFileName, QTextCodec * codec) {
    currentGroup="default";
    this->iniFileName = iniFileName;
    iniCodec = codec;
    loadIniFile();
    pendingChanges=false;
}

TSziIniFile::~TSziIniFile(){
    sync();
}

void TSziIniFile::setValue(QString key, QVariant value)
{
    pendingChanges = true;

    QString group = currentGroup;
    key=key.trimmed();
    if (key.contains('/'))
    {
        group=key.left(key.indexOf('/'));
        key=key.trimmed();
        key=key.mid(key.indexOf('/')+1,key.length()-key.indexOf('/')-1);
    }

    checkGroupExists(group);
    groups[group].insert(key,value);
}

QVariant TSziIniFile::value(QString key, QVariant defaultValue)
{
    QString group = currentGroup;
    key=key.trimmed();
    if (key.contains('/'))
    {
        group=key.left(key.indexOf('/'));
        key=key.trimmed();
        key=key.mid(key.indexOf('/')+1,key.length()-key.indexOf('/')-1);
    }

    if (!groups.contains(group) || !groups[group].contains(key))
    {
        return defaultValue;
    }

    return groups[group].value(key);
}

void TSziIniFile::beginGroup(QString prefix)
{
    currentGroup=prefix;
}

void TSziIniFile::endGroup()
{
    currentGroup="default";
}

void TSziIniFile::checkGroupExists(QString prefix)
{
    if (!groups.contains(prefix)) {
        QHash <QString, QVariant> newgroup;
        groups.insert(prefix, newgroup);
    }
}

QString TSziIniFile::group()
{
    return currentGroup;
}

bool TSziIniFile::sync()
{

if (!pendingChanges) return true;
pendingChanges=false;

#ifdef Q_OS_WIN
    const char * const eol = "\r\n";
#else
    const char eol = '\n';
#endif


bool writeError=false;


    if (!openFileWrite()) return false;

    iniFile->setCodec(iniCodec);

    QHashIterator<QString, QHash<QString,QVariant> > i(groups);
    while (i.hasNext()) {
        i.next();
        QHash<QString,QVariant> curgroup;
        *iniFile << '[' << i.key() << ']' << endl;
        curgroup = i.value();

        QHashIterator<QString,QVariant> i2(curgroup);
        while (i2.hasNext()) {
            i2.next();

            QByteArray block;

            iniEscapedKey(i2.key(), block);
            block += '=';

            const QVariant &value = i2.value();

            if (value.type() == QVariant::StringList
                    || (value.type() == QVariant::List && value.toList().size() != 1)) {
                iniEscapedStringList(variantListToStringList(value.toList()), block, iniFile->codec());
            } else {
                iniEscapedString(variantToString(value), block, value.type() == QVariant::BitArray ? iniFile->codec() : 0);
            }
            block += eol;

            if (iniFile->device()->write(block) == -1) {
                writeError = true;
                break;
            }
        }
    }

    iniFile->flush();
    if (iniFile->status()>0) lastError=1;
    closeFile();
    return true;
}

QString TSziIniFile::fileName()
{
    return iniFileName;
}

void TSziIniFile::setIniCodec(QTextCodec* codec)
{
    iniCodec = codec;
}

void TSziIniFile::setIniCodec(const char* codecName)
{
    setIniCodec(QTextCodec::codecForName(codecName));
}

bool TSziIniFile::loadIniFile()
{
    if (!openFileRead()) return false;

    bool atEnd=false;
    QString str, group;
    QStringList strListValue;
    iniFile->setCodec(iniCodec);

    while (!atEnd) {
        str=iniFile->readLine();
        if (iniFile->status()>0) lastError=1;
        str=str.trimmed();
        atEnd=iniFile->atEnd();

        if (str[0]=='#' || str[0]==';') continue;

        if (str[0]=='[') {
            str=str.remove(0,1);
            str=str.remove(str.length()-1,1);

            QByteArray array;
            array.append(str);

            iniUnescapedKey(array,str);

            checkGroupExists(str);
            this->beginGroup(str);
            continue;
        }

        if (str.contains('='))
        {
            QString key, esc_value, value;

            key=str.left(str.indexOf('='));

            key=key.trimmed();
            value = str.mid(str.indexOf('=')+1, str.length()-str.indexOf('=')-1);
            value=value.trimmed();
            //ini

        QString strValue;
        strValue.reserve(value.length());

        QByteArray array;
        array.append(value);

        bool isStringList = iniUnescapedStringList(array, 0, array.length(),
                                                   strValue, strListValue, QTextCodec::codecForCStrings()/*iniFile->codec()*/);
        QVariant variant;
        if (isStringList) {
            variant = stringListToVariantList(strListValue);
        } else {
            variant = stringToVariant(strValue);
        }
            this->setValue(key,variant);

        }
    }//while
    this->endGroup();
    closeFile();
    return true;
}

bool TSziIniFile::openFileRead()
{
    bool ret;
    lastError = 0;    
#ifdef Q_OS_UNIX
    if (useNomacl) {
        sziFile = new TSziFile(iniFileName);
        ret=sziFile->open(QFile::ReadOnly);
        if (!ret) {
            if (sziFile->last_nomacl_error()==-1) lastError=sziFile->error();
	    else lastError=sziFile->last_nomacl_error();
        }

        iniFile = new QTextStream(sziFile);
    }
    else
    {
        ifile = new QFile(iniFileName);
        ret=ifile->open(QFile::ReadOnly);
        if (!ret) {
            lastError=ifile->error();
        }
        iniFile = new QTextStream(ifile);
    }
#else
    ifile = new QFile(iniFileName);
    ret=ifile->open(QFile::ReadOnly);
    if (!ret) {
        lastError=ifile->error();
    }
    iniFile = new QTextStream(ifile);

#endif
    return ret;
}

void TSziIniFile::closeFile()
{
#ifdef Q_OS_UNIX
    if (useNomacl) {
        sziFile->close();
    }
    else
    {
        ifile->close();
    }
#else
    ifile->close();
#endif
    delete iniFile;
}

bool TSziIniFile::openFileWrite()
{
    bool ret;
    lastError = 0;
#ifdef Q_OS_UNIX
    if (useNomacl) {
        sziFile = new TSziFile(iniFileName);
        ret=sziFile->open_creat(QFile::WriteOnly, 00666);
        if (!ret) {
            if (sziFile->last_nomacl_error()==-1) lastError=sziFile->error();
	    else lastError=sziFile->last_nomacl_error();
        }

        sziFile->resize(0);
        iniFile = new QTextStream(sziFile);
    }
    else
    {
        ifile = new QFile(iniFileName);
        ret=ifile->open(QFile::WriteOnly|QFile::Truncate);
        if (!ret) {
            lastError=ifile->error();
        }
        iniFile = new QTextStream(ifile);
    }
#else
    ifile = new QFile(iniFileName);
    ret=ifile->open(QFile::WriteOnly);
    if (!ret) {
        lastError=ifile->error();
    }
    iniFile = new QTextStream(ifile);
#endif
    if (ret) lastError=0;
    return ret;
}

QSettings::Status TSziIniFile::status()
{
    if (lastError==0) return QSettings::NoError;
    else return QSettings::AccessError;
}

QStringList TSziIniFile::variantListToStringList(const QVariantList &l)
{
    QStringList result;
    QVariantList::const_iterator it = l.constBegin();
    for (; it != l.constEnd(); ++it)
        result.append(variantToString(*it));
    return result;
}

QVariant TSziIniFile::stringListToVariantList(const QStringList &l)
{
    QStringList outStringList = l;
    for (int i = 0; i < outStringList.count(); ++i) {
        const QString &str = outStringList.at(i);

        if (str.startsWith(QLatin1Char('@'))) {
            if (str.length() >= 2 && str.at(1) == QLatin1Char('@')) {
                outStringList[i].remove(0, 1);
            } else {
                QVariantList variantList;
                for (int j = 0; j < l.count(); ++j)
                    variantList.append(stringToVariant(l.at(j)));
                return variantList;
            }
        }
    }
    return outStringList;
}

QString TSziIniFile::variantToString(const QVariant &v)
{
    QString result;

    switch (v.type()) {
        case QVariant::Invalid:
            result = QLatin1String("@Invalid()");
            break;

        case QVariant::ByteArray: {
            QByteArray a = v.toByteArray();
            result = QLatin1String("@ByteArray(");
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
            break;
        }

        case QVariant::String:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::KeySequence: {
            result = v.toString();
            if (result.startsWith(QLatin1Char('@')))
                result.prepend(QLatin1Char('@'));
            break;
        }
#ifndef QT_NO_GEOM_VARIANT
        case QVariant::Rect: {
            QRect r = qvariant_cast<QRect>(v);
            result += QLatin1String("@Rect(");
            result += QString::number(r.x());
            result += QLatin1Char(' ');
            result += QString::number(r.y());
            result += QLatin1Char(' ');
            result += QString::number(r.width());
            result += QLatin1Char(' ');
            result += QString::number(r.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Size: {
            QSize s = qvariant_cast<QSize>(v);
            result += QLatin1String("@Size(");
            result += QString::number(s.width());
            result += QLatin1Char(' ');
            result += QString::number(s.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Point: {
            QPoint p = qvariant_cast<QPoint>(v);
            result += QLatin1String("@Point(");
            result += QString::number(p.x());
            result += QLatin1Char(' ');
            result += QString::number(p.y());
            result += QLatin1Char(')');
            break;
        }
#endif // !QT_NO_GEOM_VARIANT

        default: {
#ifndef QT_NO_DATASTREAM
            QByteArray a;
            {
                QDataStream s(&a, QIODevice::WriteOnly);
                s.setVersion(QDataStream::Qt_4_0);
                s << v;
            }

            result = QLatin1String("@Variant(");
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
#else
            Q_ASSERT(!"QSettings: Cannot save custom types without QDataStream support");
#endif
            break;
        }
    }

    return result;
}


QVariant TSziIniFile::stringToVariant(const QString &s)
{
    if (s.startsWith(QLatin1Char('@'))) {
        if (s.endsWith(QLatin1Char(')'))) {
            if (s.startsWith(QLatin1String("@ByteArray("))) {
                return QVariant(s.toLatin1().mid(11, s.size() - 12));
            } else if (s.startsWith(QLatin1String("@Variant("))) {
#ifndef QT_NO_DATASTREAM
                QByteArray a(s.toLatin1().mid(9));
                QDataStream stream(&a, QIODevice::ReadOnly);
                stream.setVersion(QDataStream::Qt_4_0);
                QVariant result;
                stream >> result;
                return result;
#else
                Q_ASSERT(!"QSettings: Cannot load custom types without QDataStream support");
#endif
#ifndef QT_NO_GEOM_VARIANT
            } else if (s.startsWith(QLatin1String("@Rect("))) {
                QStringList args = TSziIniFile::splitArgs(s, 5);
                if (args.size() == 4)
                    return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
            } else if (s.startsWith(QLatin1String("@Size("))) {
                QStringList args = TSziIniFile::splitArgs(s, 5);
                if (args.size() == 2)
                    return QVariant(QSize(args[0].toInt(), args[1].toInt()));
            } else if (s.startsWith(QLatin1String("@Point("))) {
                QStringList args = TSziIniFile::splitArgs(s, 6);
                if (args.size() == 2)
                    return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
#endif
            } else if (s == QLatin1String("@Invalid()")) {
                return QVariant();
            }

        }
        if (s.startsWith(QLatin1String("@@")))
            return QVariant(s.mid(1));
    }

    return QVariant(s);
}

static const char hexDigits[] = "0123456789ABCDEF";

void TSziIniFile::iniEscapedKey(const QString &key, QByteArray &result)
{
    result.reserve(result.length() + key.length() * 3 / 2);
    for (int i = 0; i < key.size(); ++i) {
        uint ch = key.at(i).unicode();

        if (ch == '/') {
            result += '\\';
        } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')
                || ch == '_' || ch == '-' || ch == '.') {
            result += (char)ch;
        } else if (ch <= 0xFF) {
            result += '%';
            result += hexDigits[ch / 16];
            result += hexDigits[ch % 16];
        } else {
            result += "%U";
            QByteArray hexCode;
            for (int i = 0; i < 4; ++i) {
                hexCode.prepend(hexDigits[ch % 16]);
                ch >>= 4;
            }
            result += hexCode;
        }
    }
}

bool TSziIniFile::iniUnescapedKey(const QByteArray &key, QString &result)
{
    bool lowercaseOnly = true;
    int i = 0;
    int to = key.length();
    result.clear();
    result.reserve(key.length());
    while (i < to) {

        int ch = (uchar) key.at(i);

        if (ch == '\\') {
            result += QLatin1Char('/');
            ++i;
            continue;
        }

        if (ch != '%' || i == to - 1) {
            if (uint(ch - 'A') <= 'Z' - 'A') // only for ASCII
                lowercaseOnly = false;
            result += QLatin1Char(ch);
            ++i;
            continue;
        }

        int numDigits = 2;
        int firstDigitPos = i + 1;

        ch = key.at(i + 1);
        if (ch == 'U') {
            ++firstDigitPos;
            numDigits = 4;
        }

        if (firstDigitPos + numDigits > to) {
            result += QLatin1Char('%');
            // ### missing U
            ++i;
            continue;
        }

        bool ok;
        ch = key.mid(firstDigitPos, numDigits).toInt(&ok, 16);
        if (!ok) {
            result += QLatin1Char('%');
            // ### missing U
            ++i;
            continue;
        }

        QChar qch(ch);
        if (qch.isUpper())
            lowercaseOnly = false;
        result += qch;
        i = firstDigitPos + numDigits;
    }
    return lowercaseOnly;
}

void TSziIniFile::iniEscapedString(const QString &str, QByteArray &result, QTextCodec *codec)
{
    bool needsQuotes = false;
    bool escapeNextIfDigit = false;
    int i;
    int startPos = result.size();

    result.reserve(startPos + str.size() * 3 / 2);
    for (i = 0; i < str.size(); ++i) {
        uint ch = str.at(i).unicode();
        if (ch == ';' || ch == ',' || ch == '=')
            needsQuotes = true;

        if (escapeNextIfDigit
                && ((ch >= '0' && ch <= '9')
                    || (ch >= 'a' && ch <= 'f')
                    || (ch >= 'A' && ch <= 'F'))) {
            result += "\\x";
            result += QByteArray::number(ch, 16);
            continue;
        }

        escapeNextIfDigit = false;

        switch (ch) {
        case '\0':
            result += "\\0";
            escapeNextIfDigit = true;
            break;
        case '\a':
            result += "\\a";
            break;
        case '\b':
            result += "\\b";
            break;
        case '\f':
            result += "\\f";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        case '\v':
            result += "\\v";
            break;
        case '"':
        case '\\':
            result += '\\';
            result += (char)ch;
            break;
        default:
            if (ch <= 0x1F || (ch >= 0x7F && !codec)) {
                result += "\\x";
                result += QByteArray::number(ch, 16);
                escapeNextIfDigit = true;
#ifndef QT_NO_TEXTCODEC
            } else if (codec) {
                // slow
                result += codec->fromUnicode(str.at(i));
#endif
            } else {
                result += (char)ch;
            }
        }
    }

    if (needsQuotes
            || (startPos < result.size() && (result.at(startPos) == ' '
                                                || result.at(result.size() - 1) == ' '))) {
        result.insert(startPos, '"');
        result += '"';
    }
}

inline static void iniChopTrailingSpaces(QString &str)
{
    int n = str.size() - 1;
    QChar ch;
    while (n >= 0 && ((ch = str.at(n)) == QLatin1Char(' ') || ch == QLatin1Char('\t')))
        str.truncate(n--);
}

void TSziIniFile::iniEscapedStringList(const QStringList &strs, QByteArray &result, QTextCodec *codec)
{
    if (strs.isEmpty()) {
        /*
            We need to distinguish between empty lists and one-item
            lists that contain an empty string. Ideally, we'd have a
            @EmptyList() symbol but that would break compatibility
            with Qt 4.0. @Invalid() stands for QVariant(), and
            QVariant().toStringList() returns an empty QStringList,
            so we're in good shape.

            ### Qt 5: Use a nicer syntax, e.g. @List, for variant lists
        */
        result += "@Invalid()";
    } else {
        for (int i = 0; i < strs.size(); ++i) {
            if (i != 0)
                result += ", ";
            iniEscapedString(strs.at(i), result, codec);
        }
    }
}

bool TSziIniFile::iniUnescapedStringList(const QByteArray &str, int from, int to,
                                              QString &stringResult, QStringList &stringListResult,
                                              QTextCodec *codec)
{
    static const char escapeCodes[][2] =
    {
        { 'a', '\a' },
        { 'b', '\b' },
        { 'f', '\f' },
        { 'n', '\n' },
        { 'r', '\r' },
        { 't', '\t' },
        { 'v', '\v' },
        { '"', '"' },
        { '?', '?' },
        { '\'', '\'' },
        { '\\', '\\' }
    };
    static const int numEscapeCodes = sizeof(escapeCodes) / sizeof(escapeCodes[0]);

    bool isStringList = false;
    bool inQuotedString = false;
    bool currentValueIsQuoted = false;
    int escapeVal = 0;
    int i = from;
    char ch;

StSkipSpaces:
    while (i < to && ((ch = str.at(i)) == ' ' || ch == '\t'))
        ++i;
    // fallthrough

StNormal:
    while (i < to) {
        switch (str.at(i)) {
        case '\\':
            ++i;
            if (i >= to)
                goto end;

            ch = str.at(i++);
            for (int j = 0; j < numEscapeCodes; ++j) {
                if (ch == escapeCodes[j][0]) {
                    stringResult += QLatin1Char(escapeCodes[j][1]);
                    goto StNormal;
                }
            }

            if (ch == 'x') {
                escapeVal = 0;

                if (i >= to)
                    goto end;

                ch = str.at(i);
                if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
                    goto StHexEscape;
            } else if (ch >= '0' && ch <= '7') {
                escapeVal = ch - '0';
                goto StOctEscape;
            } else if (ch == '\n' || ch == '\r') {
                if (i < to) {
                    char ch2 = str.at(i);
                    // \n, \r, \r\n, and \n\r are legitimate line terminators in INI files
                    if ((ch2 == '\n' || ch2 == '\r') && ch2 != ch)
                        ++i;
                }
            } else {
                // the character is skipped
            }
            break;
        case '"':
            ++i;
            currentValueIsQuoted = true;
            inQuotedString = !inQuotedString;
            if (!inQuotedString)
                goto StSkipSpaces;
            break;
        case ',':
            if (!inQuotedString) {
                if (!currentValueIsQuoted)
                    iniChopTrailingSpaces(stringResult);
                if (!isStringList) {
                    isStringList = true;
                    stringListResult.clear();
                    stringResult.squeeze();
                }
                stringListResult.append(stringResult);
                stringResult.clear();
                currentValueIsQuoted = false;
                ++i;
                goto StSkipSpaces;
            }
            // fallthrough
        default: {
            int j = i + 1;
            while (j < to) {
                ch = str.at(j);
                if (ch == '\\' || ch == '"' || ch == ',')
                    break;
                ++j;
            }

#ifndef QT_NO_TEXTCODEC
            if (codec) {
                stringResult += codec->toUnicode(str.constData() + i, j - i);
            } else
#endif
            {
                int n = stringResult.size();
                stringResult.resize(n + (j - i));
                QChar *resultData = stringResult.data() + n;
                for (int k = i; k < j; ++k)
                    *resultData++ = QLatin1Char(str.at(k));
            }
            i = j;
        }
        }
    }
    goto end;

StHexEscape:
    if (i >= to) {
        stringResult += QChar(escapeVal);
        goto end;
    }

    ch = str.at(i);
    if (ch >= 'a')
        ch -= 'a' - 'A';
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')) {
        escapeVal <<= 4;
        escapeVal += strchr(hexDigits, ch) - hexDigits;
        ++i;
        goto StHexEscape;
    } else {
        stringResult += QChar(escapeVal);
        goto StNormal;
    }

StOctEscape:
    if (i >= to) {
        stringResult += QChar(escapeVal);
        goto end;
    }

    ch = str.at(i);
    if (ch >= '0' && ch <= '7') {
        escapeVal <<= 3;
        escapeVal += ch - '0';
        ++i;
        goto StOctEscape;
    } else {
        stringResult += QChar(escapeVal);
        goto StNormal;
    }

end:
    if (!currentValueIsQuoted)
        iniChopTrailingSpaces(stringResult);
    if (isStringList)
        stringListResult.append(stringResult);
    return isStringList;
}

QStringList TSziIniFile::splitArgs(const QString &s, int idx)
{
    int l = s.length();
    Q_ASSERT(l > 0);
    Q_ASSERT(s.at(idx) == QLatin1Char('('));
    Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

    QStringList result;
    QString item;

    for (++idx; idx < l; ++idx) {
        QChar c = s.at(idx);
        if (c == QLatin1Char(')')) {
            Q_ASSERT(idx == l - 1);
            result.append(item);
        } else if (c == QLatin1Char(' ')) {
            result.append(item);
            item.clear();
        } else {
            item.append(c);
        }
    }

    return result;
}

void TSziIniFile::clearAll()
{
  groups.clear();
  currentGroup="default";
  pendingChanges=true;
}

bool TSziIniFile::reloadIniFile()
{
  clearAll();
  bool res = loadIniFile();
  pendingChanges=false;
  return res;
}

int TSziIniFile::getLastError()
{
  return lastError;
}
