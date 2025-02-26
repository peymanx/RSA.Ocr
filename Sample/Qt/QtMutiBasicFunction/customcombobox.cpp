#include "customcombobox.h"
#include "qcombobox.h"
#include "qcoreevent.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include <QtCore/QDebug>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QListWidgetItem>

customComboBox::customComboBox(QWidget *parent) : QComboBox(parent)
{
	list_widget_ = new QListWidget();
	line_edit_ = new QLineEdit();
	//checkAll = new QCheckBox();
	/* ch: �����ı��� | en: Set the text box  */
	/* ch: ��Ϊֻ������Ϊ�������ֻ������ʾѡ�е�ѡ���Ϊ�ı��������Щ 
		| en: Set to read-only because the input box is used only to display the selected options, and is better called a text box */
	line_edit_->setReadOnly(true);
	/* ch: �ѵ�ǰ����װ(��ע��)Ϊ�¼�����������ǰҲ��Ϊ�����������¼�������ͨ���ڹ��캯���н���ע�� 
	| en: Installs (or registers) the current object as an event filter, currently also known as a filter object. Event filters are typically registered in constructors */
	line_edit_->installEventFilter(this);
	/* ch: ���ý�����ʽ����Ϊ������ʽ����ƣ���ʱ������� | en: Set the disabled style because it is not controlled by the style sheet. This is a temporary solution */ 
	line_edit_->setStyleSheet("QLineEdit:disabled{background:rgb(233,233,233);}");
	QListWidgetItem *currentItem = new QListWidgetItem(list_widget_);
	//checkAll->setText("ALL");
	//list_widget_->addItem(currentItem);
	//list_widget_->setItemWidget(currentItem, checkAll);

	this->setModel(list_widget_->model());
	this->setView(list_widget_);
	this->setLineEdit(line_edit_);

	connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &customComboBox::itemClicked);
	/*connect(checkAll, &QCheckBox::released, [=]() mutable {
		bool ischecked = checkAll->isChecked();
		for (int i = 0; i < list_widget_->count(); i++) {
			QCheckBox *check_box = static_cast<QCheckBox *>(list_widget_->itemWidget(list_widget_->item(i)));
			check_box->setChecked(ischecked);
		}
	});*/
}
void customComboBox::checkAllState(int state)
{
	if (Qt::Unchecked == state) {
		//checkAll->setChecked(false);
	}
	else if (Qt::Checked == state) {
		bool isallcheck = true;
		for (int i = 1; i < list_widget_->count(); i++) {
			QCheckBox *check_box = static_cast<QCheckBox *>(list_widget_->itemWidget(list_widget_->item(i)));
			if (check_box->isChecked() == false) {
				isallcheck = false;
			}
		}
		//checkAll->setChecked(isallcheck);
	}
}
void customComboBox::hidePopup()
{
	show_flag_ = false;
	int width = this->width();
	int height = this->height();
	int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
	int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
	if (x >= 0 && x <= width && y >= this->height() && y <= height + this->height()) {
	}
	else {
		QComboBox::hidePopup();
	}
}

void customComboBox::addItem(const QString &_text, const QVariant &_variant)
{
	Q_UNUSED(_variant);
	QListWidgetItem *item = new QListWidgetItem(list_widget_);
	QCheckBox       *checkbox = new QCheckBox(this);
	checkbox->setText(_text);
	list_widget_->addItem(item);
	list_widget_->setItemWidget(item, checkbox);
	connect(checkbox, &QCheckBox::stateChanged, this, &customComboBox::stateChange);
	connect(checkbox, &QCheckBox::stateChanged, this, &customComboBox::checkAllState);
}

void customComboBox::addItems(const QStringList &_text_list)
{
	for (const auto &text_one : _text_list) {
		addItem(text_one);
	}
}

QStringList customComboBox::currentText()
{
	QStringList text_list;
	if (!line_edit_->text().isEmpty()) {
		/* ch: ��;Ϊ�ָ����ָ��ַ��� | en: By means of; Split the string for delimiters */
		text_list = line_edit_->text().split(';');
	}
	return text_list;
}

int customComboBox::count() const
{
	int count = list_widget_->count() - 1;
	if (count < 0) {
		count = 0;
	}
	return count;
}

void customComboBox::SetPlaceHolderText(const QString &_text)
{
	line_edit_->setPlaceholderText(_text);
}

void customComboBox::ResetSelection()
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++) {
		/* ch: ��ȡ��Ӧλ�õ�QWidget���� | en: Gets the QWidget object for the corresponding location */
		QWidget   *widget = list_widget_->itemWidget(list_widget_->item(i));
		/* ch: ��QWidget����ת���ɶ�Ӧ������ | en: Converts the QWidget object to the corresponding type */
		QCheckBox *check_box = static_cast<QCheckBox *>(widget);
		check_box->setChecked(false);
	}
}

void customComboBox::setCurrentText(const QString &_text)
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++) {
		/* ch: ��ȡ��Ӧλ�õ�QWidget���� | en: Gets the QWidget object for the corresponding location */
		QWidget   *widget = list_widget_->itemWidget(list_widget_->item(i));
		/* ch: ��QWidget����ת���ɶ�Ӧ������ | en: Converts the QWidget object to the corresponding type */
		QCheckBox *check_box = static_cast<QCheckBox *>(widget);
		if (_text.compare(check_box->text()))
			check_box->setChecked(true);
	}
}

void customComboBox::setCurrentText(const QStringList &_text_list)
{
	int count = list_widget_->count();
	for (int i = 1; i < count; i++) {
		/* ch: ��ȡ��Ӧλ�õ�QWidget���� | en: Gets the QWidget object for the corresponding location */
		QWidget   *widget = list_widget_->itemWidget(list_widget_->item(i));
		/* ch: ��QWidget����ת���ɶ�Ӧ������ | en: Converts the QWidget object to the corresponding type */
		QCheckBox *check_box = static_cast<QCheckBox *>(widget);
		if (_text_list.contains(check_box->text()))
			check_box->setChecked(true);
	}
}

bool customComboBox::eventFilter(QObject *watched, QEvent *event)
{
	/* ch: ���õ�������Ҳ���Ե��������� | en: Settings Click the input box to also pop up a drop-down box */
	if (watched == line_edit_ && event->type() == QEvent::MouseButtonRelease && this->isEnabled()) {
		showPopup();
		return true;
	}
	return false;
}

void customComboBox::wheelEvent(QWheelEvent *event)
{
	/* ch: ����QComboBoxĬ�ϵĹ����¼� | en: Disable QComboBox's default wheel event */
	Q_UNUSED(event);
}

void customComboBox::keyPressEvent(QKeyEvent *event)
{
	QComboBox::keyPressEvent(event);
}

void customComboBox::stateChange(int _row)
{
	Q_UNUSED(_row);
	QString selected_data("");
	int     count = list_widget_->count();
	for (int i = 0; i < count; i++) {
		QWidget   *widget = list_widget_->itemWidget(list_widget_->item(i));
		QCheckBox *check_box = static_cast<QCheckBox *>(widget);
		if (check_box->isChecked()) {
			selected_data.append(check_box->text()).append(";");
		}
	}
	selected_data.chop(1);
	if (!selected_data.isEmpty()) {
		line_edit_->setText(selected_data);
	}
	else {
		line_edit_->clear();
	}

	line_edit_->setToolTip(selected_data);
	emit selectionChange(selected_data);
}

void customComboBox::itemClicked(int _index)
{
	if (_index != 0) {
		QCheckBox *check_box = static_cast<QCheckBox *>(list_widget_->itemWidget(list_widget_->item(_index)));
		check_box->setChecked(!check_box->isChecked());
	}
}
