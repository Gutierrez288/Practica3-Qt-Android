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
#include <QVBoxLayout>
#include <QAccelerometer>

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