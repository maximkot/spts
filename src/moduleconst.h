#ifndef MODULECONST_H
#define MODULECONST_H
#include <QColor>
#include <QString>
#include <QWidget>
#include <QTextCodec>
#include <QTranslator>
#include "modulevar.h"


// Наименование задачи-----------------------------------------------------------------------------
const QString ZadName = "Ведение информации о состоянии ТС, используемых при проведении спасательных работ";
//-------

// Имена файлов------------------------------------------------------------------------------------
const QString ProgName = "r4102_spts";                // Обозначение программы
const QString ProgSetFile = "r4102_spts.cfg";        // Файл настроек проекта и подключений
const QString FormSetFile = "r4102_spts_frm.cfg";   // Файл настроек окон
const QString TablSetFile = "r4102_spts_tbl.cfg";  // Файл настроек таблиц
//-------

// Цвета стандартной цветовой схемы----------------------------------------------------------------
const QColor defaultPohodColor = Qt::lightGray;      // Цвет "по-походному" - светло-серый
const QColor defaultReadyColor = Qt::green;         // Цвет 'Готов' - зеленый
const QColor defaultNotReadyColor = Qt::yellow;    // Цвет 'Не Готов' - желтый
const QColor defaultAvarColor = Qt::red;          // Цвет 'Аварийное состояние' - красный
const QColor defaultSelectColor = Qt::lightGray; // Цвет 'Выделение неактивного компонента' - светло-серый
//-------

 // Наименование типов технических средств----------------------------------------------------------
// аббревиатуры
const QString tsBDD = "БДД";    // Блок двигателей-движителей
const QString tsGVS = "ЖВС";    // Жесткий водолазный скафандр
const QString tsNPA = "НПА";    // Необитаемый подводный аппарат
const QString tsSGA = "СГА";    // Спасательный глубоководный аппарат
const QString tsSJO = "СЖО";    // Система жизнеобеспечения

// полностью
const QString tsBDDfull = "Блок двигателей-движителей";
const QString tsGVSfull = "Жесткий водолазный скафандр";
const QString tsNPAfull = "Необитаемый подводный аппарат";
const QString tsSGAfull = "Спасательный глубоководный аппарат";
const QString tsSGOfull = "Система жизнеобеспечения";


 // Наименование бланков проводимых работ----------------------------------------------------------
// По учету состояния СГА:
const QString blankPredSGA  = "Контрольно-проверочный лист предпогружной проверки СГА"; //к плаванию и выполнению подводных работ";
const QString blankRabSGA   = "Лист учёта событий при работе СГА под водой"; // и нормативные требования к параметрам";
const QString blankPosleSGA = "Контрольно-проверочный лист послепогружной проверки СГА";

// По учету состояния ЖВС:
const QString blankPredGVS  = "Лист предспусковой проверки ЖВС";
const QString blankPredBDD  = "Лист предспусковой проверки БДД";
const QString blankPosleGVS = "Лист послеспусковой проверки ЖВС";
const QString blankPosleBDD = "Лист послеспусковой проверки БДД";
const QString blankParamSGO = "Лист записи показаний СЖО";
const QString blankSpuskGVS = "Лист проверки ЖВС при спуске (подъеме)";

// По учету состояния ТНПА
const QString blankPredNPA  = "Контрольно-проверочный лист предпогружных проверок НПА";
const QString blankPosleNPA = "Контрольно-проверочный лист послепогружных проверок НПА";


// Массив наименований вкладок
const QString TabWidgetTexts[3][4] = {
                                      {"Ведение информации \nо состоянии СГА","Контрольно-проверочный лист\nпредпогружной проверки СГА","Лист учёта событий\nпри работе СГА под водой","Контрольно-проверочный лист\nпослепогружной проверки СГА"},
                                      {"Ведение информации \nо состоянии ЖВС","Листы предспусковой\nпроверки ЖВС и БДД","Лист проверки\nЖВС при спуске (подъеме)","Лист послеспусковой\nпроверки ЖВС и БДД"},
                                      {"Ведение информации \nо состоянии НПА","Контрольно-проверочный лист\nпредпогружных проверок НПА","Журнал\nпопогружений НПА","Контрольно-проверочный лист\nпослепогружных проверок НПА"}
                                     };

// Массив таблиц используемых в QSqlQuery модели ModelSostTS
const QString ModelSostTSTables[3] = {"z18_spts.sostsga","z18_spts.sostgvs","z18_spts.sostnpa"};


// Массив столбцов для таблицы sostsga
const QString SostSGAColumn[5] = {"code","name","codesga","sostts","codepol"};
// Массив столбцов для таблицы sostgvs
const QString SostGVSColumn[7] = {"code","nomgvs","codesostts","codepol","timegot","code_edtime","ed_time"};
// Массив столбцов для таблицы sostnpa
const QString SostNPAColumn[8] = {"code","name","zavnom","codesostts","codepol","timegot","code_edtime","ed_time"};


// Массив таблиц используемых в QSqlQuery модели ModelPogruzh
const QString ModelPogruzhTSTables[3][3] = {
                                         {"z18_spts.p_sgapred","z18_spts.p_sgaspusk","z18_spts.p_sgaposle"},
                                         {"z18_spts.p_gvspred","z18_spts.p_gvsspusk","z18_spts.p_gvsposle"},
                                         {"z18_spts.p_npapred","z18_spts.p_npaspusk","z18_spts.p_npaposle"}
                                        };





 // сокращенные наименования бланков, применяемые в комбобоксах (чтобы влезло)
// для tsSGA
const QString txtComboBoxSGAPred  = "Предпогружная проверка";
const QString txtComboBoxSGARab   = "Работа под водой";
const QString txtComboBoxSGAPosle = "Послепогружная проверка";

// для tsGVS
const QString txtComboBoxGVSPred  = "Предспусковая проверка ЖВС";
const QString txtComboBoxBDDPred  = "Предспусковая проверка БДД";
const QString txtComboBoxGVSPosle = "Послеспусковая проверка ЖВС";
const QString txtComboBoxBDDPosle = "Послеспусковая проверка БДД";
const QString txtComboBoxGVSSpusk = "Лист событий при спуске ЖВС";
const QString txtComboBoxSGOParam = "Параметры показаний СЖО";
const QString txtComboBoxGVSUpDwn = "Проверка ЖВС при спуске (подъеме)";

// для tsNPA
const QString txtComboBoxNPAPered = "Предпогружная проверка";
const QString txtComboBoxNPAPosle = "Послепогружная проверка";
//-------


// Тексты сообщений оператору и пользователю-------------------------------------------------------
const QString msgErrorCreateProgRabDir = "Ошибка при создании рабочего каталога программы";
const QString msgErrorNoStaticDir      = "Отсутствует каталог со статической информацией программы";
const QString msgErrorCreateOutDocDir  = "Ошибка при создании каталога для выходных документов";
const QString msgErrorCreateOutXMLDir  = "Ошибка при создании каталога для выходных XML документов";
const QString msgErrorReadSettings     = "Ошибка при считывании цветовых настроек параметров программы";
const QString msgErrorPlayingSound     = "Аппаратные средства данного компьютера не позволяют воспроизводить звук";
const QString msgErrorCantReadDBParam  = "Не удалось считать из БД параметры необходимые для работы программы";
const QString msgErrorDeleteCurrentRec = "Ошибка при удалении данных";
const QString msgErrorCodeTableTS      = "Не удалось считать из БД параметры необходимые для работы программы/nПриложение будет закрыто";
const QString msgErrorOpenEditRecord   = "Произошла ошибка открытия редактируемой записи!";
const QString msqErrorAddRecord        = "Произошла ошибка при добавлении данных!";

const QString msgAskDeleteCurrentRec   = "Удалить выбранную запись?";

const QString msgWarningEmptyNumPogr   = "Не заполнено поле 'Номер погружения'";
const QString msgWarningEmptyDatePogr  = "Не заполнено поле 'Дата погружения'";
const QString msgWarningEmptyTimeSPogr = "Не заполнено поле 'Время начала работ'";
const QString msgWarningSameRecord     = "Подобная запись уже имеется в таблице!";


//---------------------------------------------------------------------------------------------------
#endif // MODULECONST_H
