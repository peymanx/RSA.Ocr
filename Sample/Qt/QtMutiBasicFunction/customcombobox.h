#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include "qlineedit.h"
#include "qlistwidget.h"
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>


class customComboBox : public QComboBox {
	Q_OBJECT
public:
	explicit customComboBox(QWidget* parent = nullptr);
	virtual void hidePopup();

	void addItem(const QString& _text, const QVariant& _variant = QVariant());

	void addItems(const QStringList& _text_list);

	QStringList currentText();

	int count() const;

	void SetPlaceHolderText(const QString& _text);

	void ResetSelection();
	void setCurrentText(const QString& _text);

	void setCurrentText(const QStringList& _text_list);


protected:

	virtual bool eventFilter(QObject* watched, QEvent* event);

	virtual void wheelEvent(QWheelEvent* event);

	virtual void keyPressEvent(QKeyEvent* event);

	private slots:

	void stateChange(int _row);
    void itemClicked(int _index);
	void checkAllState(int state);

signals:

	void selectionChange(const QString _data);

private:
	QListWidget* list_widget_;
	QLineEdit*   line_edit_;
	QCheckBox*   checkAll;
	bool         show_flag_;
};

#endif  // CUSTOMCOMBOBOX_H
