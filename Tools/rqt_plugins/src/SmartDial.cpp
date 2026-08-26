#include "robot_framework_ros/SmartDial.h"

#include <QWidget>

// Minimal constructor initializing the base class
SmartDial::SmartDial(QObject* parent)
    : QObject(parent),
      dial_(nullptr),
      indicator_(nullptr),
      label_max_value_(nullptr),
      label_min_value_(nullptr),
      button_scale_X2_(nullptr),
      button_div_X2_(nullptr) {}

bool SmartDial::setupUi(QWidget* parentWidget, const QString& smartdial_name, double min_value, double max_value) {
    if (!parentWidget) {
        return false;
    }
    if (smartdial_name == "") {
        fast::rf::Logger::log_error("Invalid Smart Dial Name!");
        return false;
    }
    if (min_value > max_value) {
        fast::rf::Logger::log_error(std::to_string(min_value) + " < " + std::to_string(max_value) + "!");
    }
    max_value_ = max_value;
    min_value_ = min_value;
    smartdial_name_ = smartdial_name;

    // Safely look up children inside the parent widget hierarchy
    dial_ = parentWidget->findChild<QDial*>(generate_dial_name());
    indicator_ = parentWidget->findChild<QLabel*>(generate_indicator_name());
    label_min_value_ = parentWidget->findChild<QLabel*>(generate_min_value_name());
    label_max_value_ = parentWidget->findChild<QLabel*>(generate_max_value_name());
    button_scale_X2_ = parentWidget->findChild<QPushButton*>(generate_button_Scale_X2_name());
    button_div_X2_ = parentWidget->findChild<QPushButton*>(generate_button_Div_X2_name());

    if (is_initialized() == false) {
        return false;
    }

    connect(dial_, &QDial::valueChanged, this, &SmartDial::dial_changed);
    connect(button_scale_X2_, &QPushButton::clicked, this, &SmartDial::button_scale_X2_clicked);
    connect(button_div_X2_, &QPushButton::clicked, this, &SmartDial::button_div_X2_clicked);

    return true;
}
bool SmartDial::is_initialized() {
    if (dial_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_dial_name().toStdString());
        return false;
    }
    if (indicator_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_indicator_name().toStdString());
        return false;
    }
    if (label_max_value_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_max_value_name().toStdString());
        return false;
    }
    if (label_min_value_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_min_value_name().toStdString());
        return false;
    }
    if (button_scale_X2_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_button_Scale_X2_name().toStdString());
        return false;
    }
    if (button_div_X2_ == nullptr) {
        fast::rf::Logger::log_error("Invalid key: " + generate_button_Div_X2_name().toStdString());
        return false;
    }
    return true;
}
void SmartDial::button_scale_X2_clicked() {
    double current_value_ = get_value();
    double max_offset = max_value_ - current_value_;
    printf("x2 old: %f %f %f\n", min_value_, max_value_, current_value_);
    max_value_ = current_value_ + (max_offset * 2.0);
    double min_offset = (current_value_ - min_value_);
    min_value_ = current_value_ - (min_offset * 2.0);
    printf("x2 new: %f %f\n", min_value_, max_value_);
    update();
}
void SmartDial::button_div_X2_clicked() {
    double current_value_ = get_value();
    printf("D2 old: %f %f\n", min_value_, max_value_);
    double max_offset = max_value_ - current_value_;
    max_value_ = current_value_ + (max_offset / 2.0);
    double min_offset = (current_value_ - min_value_);
    min_value_ = current_value_ - (min_offset / 2.0);
    printf("D2 new: %f %f\n", min_value_, max_value_);
    update();
}

void SmartDial::dial_changed(int value) { current_dial_value_ = value; }
double SmartDial::get_value() {
    double v =
        scale_value((double)current_dial_value_, (double)MIN_TICK_MARK, (double)MAX_TICK_MARK, min_value_, max_value_);
    return v;
}
void SmartDial::update() {
    label_max_value_->setText(QString::number(max_value_));
    label_min_value_->setText(QString::number(min_value_));
    indicator_->setText(QString::number(get_value()));
}
double SmartDial::scale_value(double input, double min_input, double max_input, double min_output, double max_output) {
    // m = (y2-y1)/(x2-x1)
    double m = (max_output - min_output) / (max_input - min_input);
    // y -y1 = m(x-x1) --> y = m(x-x1) + y1
    double output = m * (input - min_input) + min_output;
    return output;
}
