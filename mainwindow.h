#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QTimer>
#include <QKeyEvent>
#include <QtCharts>
#include <QAccelerometer>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QDateTime>
#include <QTableWidget>
#include <QHeaderView>
// --- NUEVO PASO 5 ---
#include <QSpinBox>
#include <QLineEdit>

#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void alConectar();
    void alDesconectar();
    void enviarJSON(const QString &mensaje);
    void loop();
    void alRecibirMensaje(const QString &mensaje);

    // Botones de movimiento
    void on_btnAdelante_clicked();
    void on_btnAtras_clicked();
    void on_btnIzquierda_clicked();
    void on_btnDerecha_clicked();

    void keyPressEvent(QKeyEvent *event);

    // Slots para funcionalidades extra
    void alMoverServo(int angulo);
    void alCambiarModosExtra();
    void leerAcelerometro();

private:
    Ui::MainWindow *ui;
    QWebSocket socket;

    QVBoxLayout *m_capaTemp;
    QPushButton *m_botonLeerTemp;
    QLabel *m_temperatura;

    QCheckBox *m_chkAutoGuardar;
    QTimer *m_timerGuardar;
    void guardarTemperatura(double celsius);

    QTableWidget *m_tabla;
    QPushButton *m_botonCargar;
    QLabel *m_total;

    // --- NUEVO PASO 5: UPDATE ---
    QSpinBox *m_id;
    QLineEdit *m_nota;
    QPushButton *m_botonGuardarNota;
    QPushButton *m_botonExportarCSV;

    // Componentes de la gráfica
    QChart *chart;
    QLineSeries *series;
    QChartView *chartView;
    QValueAxis *ejeX;
    QValueAxis *ejeY;
    int contadorMuestras;

    // Sensor de movimiento para el Extra (Giroscopio)
    QAccelerometer *acelerometro;
};
#endif // MAINWINDOW_H