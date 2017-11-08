#include "cfinddialog.h"
#include "ui_cfinddialog.h"
#include "settings/csettings.h"

#include <QLineEdit>

#define SETTINGS_SEARCH_EXPRESSION_LIST "Expressions"

#define SETTINGS_REGEX                  "Regex"
#define SETTINGS_BACKWARDS              "SearchBackwards"
#define SETTINGS_CASE_SENSITIVE         "CaseSensitive"
#define SETTINGS_WHOLE_WORDS            "WholeWords"

#define SETTINGS_GEOMETRY               "Geometry"

CFindDialog::CFindDialog(QWidget *parent, const QString& settingsRootCategory) :
	QDialog(parent),
	_settingsRootCategory(settingsRootCategory),
	ui(new Ui::CFindDialog)
{
	ui->setupUi(this);

	ui->_searchText->enableAutoSave(_settingsRootCategory + SETTINGS_SEARCH_EXPRESSION_LIST);

	connect(ui->_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->_btnFind, &QPushButton::clicked, this, &QDialog::accept);
	connect(ui->_btnFind, &QPushButton::clicked, this, &CFindDialog::find);
	connect(ui->_btnFindNext, &QPushButton::clicked, this, &CFindDialog::findNext);

	connect(ui->_searchText, &CHistoryComboBox::itemActivated, ui->_btnFind, &QPushButton::click);

	if (!_settingsRootCategory.isEmpty())
	{
		CSettings s;
		ui->_cbSearchBackwards->setChecked(s.value(_settingsRootCategory + SETTINGS_BACKWARDS).toBool());
		ui->_cbCaseSensitive->setChecked(s.value(_settingsRootCategory + SETTINGS_CASE_SENSITIVE).toBool());
		ui->_cbRegex->setChecked(s.value(_settingsRootCategory + SETTINGS_REGEX).toBool());
		ui->_cbWholeWords->setChecked(s.value(_settingsRootCategory + SETTINGS_WHOLE_WORDS).toBool());
	}
}

CFindDialog::~CFindDialog()
{
	saveSearchSettings();
	delete ui;
}

QString CFindDialog::searchExpression() const
{
	return ui->_searchText->currentText();
}

bool CFindDialog::regex() const
{
	return ui->_cbRegex->isChecked();
}

bool CFindDialog::searchBackwards() const
{
	return ui->_cbSearchBackwards->isChecked();
}

bool CFindDialog::wholeWords() const
{
	return ui->_cbWholeWords->isChecked();
}

bool CFindDialog::caseSensitive() const
{
	return ui->_cbCaseSensitive->isChecked();
}

void CFindDialog::accept()
{
	QDialog::accept();
	saveSearchSettings();
}

void CFindDialog::showEvent(QShowEvent * e)
{
	ui->_searchText->lineEdit()->selectAll();
	ui->_searchText->lineEdit()->setFocus();

	if (!_settingsRootCategory.isEmpty())
		restoreGeometry(CSettings().value(_settingsRootCategory + SETTINGS_GEOMETRY).toByteArray());

	QDialog::showEvent(e);
}

void CFindDialog::closeEvent(QCloseEvent * e)
{
	if (!_settingsRootCategory.isEmpty())
		CSettings().setValue(_settingsRootCategory + SETTINGS_GEOMETRY, saveGeometry());

	QDialog::closeEvent(e);
}

void CFindDialog::saveSearchSettings() const
{
	CSettings s;
	s.setValue(_settingsRootCategory + SETTINGS_BACKWARDS, searchBackwards());
	s.setValue(_settingsRootCategory + SETTINGS_CASE_SENSITIVE, caseSensitive());
	s.setValue(_settingsRootCategory + SETTINGS_REGEX, regex());
	s.setValue(_settingsRootCategory + SETTINGS_WHOLE_WORDS, wholeWords());

}
