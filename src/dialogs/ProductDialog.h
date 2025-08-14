#pragma once

#include <QDialog>
#include "model/Product.h"

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;

class ProductDialog : public QDialog {
	Q_OBJECT
public:
	explicit ProductDialog(QWidget *parent = nullptr);
	void setProduct(const Product &product);
	Product product() const;

private:
	void setupUi();

private:
	QLineEdit *m_name{nullptr};
	QLineEdit *m_desc{nullptr};
	QDoubleSpinBox *m_price{nullptr};
	QSpinBox *m_stock{nullptr};
	Product m_current;
};