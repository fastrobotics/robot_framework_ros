#pragma once

#include <Infrastructure/Logger.hpp>
#include <QDebug>
#include <QDial>
#include <QLabel>
#include <QObject>
#include <QPushButton>

class SmartDial : public QObject {
    Q_OBJECT

   public:
    static constexpr int MAX_TICK_MARK = 1024;
    static constexpr int MIN_TICK_MARK = -1024;
    const double ABSOLUTE_MIN = -200.0;
    const double ABSOLUTE_MAX = 200.0;
    // Constructor
    explicit SmartDial(QObject* parent = nullptr);

    // Destructor (Virtual ensures proper cleanup if subclassed)
    virtual ~SmartDial() = default;

    // Dynamically hooks up UI elements from a parent widget container
    bool setupUi(QWidget* parentWidget, const QString& smartdial_name, double min_value, double max_value);

    double get_value();
    double get_min_value() { return min_value_; }
    double get_max_value() { return max_value_; }
    void set_value(double v);
    void update();

   public slots:
    void button_scale_X2_clicked();
    void button_div_X2_clicked();

    void dial_changed(int value);

   private:
    QString generate_dial_name() {
        QString name = smartdial_name_ + "_dial";
        return name;
    }
    QString generate_indicator_name() {
        QString name = smartdial_name_ + "_label_indicator";
        return name;
    }

    QString generate_min_value_name() {
        QString name = smartdial_name_ + "_label_min_value";
        return name;
    }
    QString generate_max_value_name() {
        QString name = smartdial_name_ + "_label_max_value";
        return name;
    }
    QString generate_button_Scale_X2_name() {
        QString name = smartdial_name_ + "_button_ScaleX2";
        return name;
    }
    QString generate_button_Div_X2_name() {
        QString name = smartdial_name_ + "_button_DivX2";
        return name;
    }
    bool is_initialized();
    double scale_value(double input, double min_input, double max_input, double min_output, double max_output);

    QString smartdial_name_;
    // Encapsulated UI components
    QDial* dial_ = nullptr;
    QLabel* indicator_ = nullptr;
    QLabel* label_max_value_ = nullptr;
    QLabel* label_min_value_ = nullptr;
    QPushButton* button_scale_X2_ = nullptr;
    QPushButton* button_div_X2_ = nullptr;

    // Data
    int current_dial_value_{0};
    double min_value_;
    double max_value_;
};
