#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QStandardPaths>
#include <QAccelerometer> // <--- Librería del Extra agregada

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , contadorMuestras(0)
{
    ui->setupUi(this);
    // Quitar el foco del teclado a todos los elementos para que el MainWindow siempre lo tenga
    ui->btnAdelante->setFocusPolicy(Qt::NoFocus);
    ui->btnAtras->setFocusPolicy(Qt::NoFocus);
    ui->btnIzquierda->setFocusPolicy(Qt::NoFocus);
    ui->btnDerecha->setFocusPolicy(Qt::NoFocus);
    ui->sliderServo->setFocusPolicy(Qt::NoFocus);
    ui->chkTracking->setFocusPolicy(Qt::NoFocus);
    ui->chkFuga->setFocusPolicy(Qt::NoFocus);

    // === NUEVO: ACTIVAR MODO "MANTENER PRESIONADO" PARA LOS BOTONES TÁCTILES ===
    ui->btnAdelante->setAutoRepeat(true);
    ui->btnAtras->setAutoRepeat(true);
    ui->btnIzquierda->setAutoRepeat(true);
    ui->btnDerecha->setAutoRepeat(true);

    // El comando se enviará continuamente cada 100 ms mientras el dedo toque la pantalla
    ui->btnAdelante->setAutoRepeatInterval(100);
    ui->btnAtras->setAutoRepeatInterval(100);
    ui->btnIzquierda->setAutoRepeatInterval(100);
    ui->btnDerecha->setAutoRepeatInterval(100);
    // ============================================================================

    // 1. Cargar el driver de SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 2. Pedirle a Android la ruta segura para guardar archivos
    QString ruta = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Asegurarnos de que la carpeta exista en la memoria del celular
    QDir dir(ruta);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    db.setDatabaseName(ruta + "/robot_datos.db");

    // 3. Abrir la base de datos y verificar
    if(db.open()) {
        qDebug() << "¡Éxito! SQLite conectado en Android:" << ruta;
    } else {
        qDebug() << "Error al conectar SQLite:" << db.lastError().text();
    }

    setFocusPolicy(Qt::StrongFocus);
    if (centralWidget()) centralWidget()->setFocusPolicy(Qt::StrongFocus);

    // 1. Configuración de la Gráfica de Distancia
    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Distancia HC-SR04 (cm)");
    chart->setTheme(QChart::ChartThemeDark); // Combina con tu diseño

    ejeX = new QValueAxis();
    ejeX->setRange(0, 50);
    ejeX->setTitleText("Muestras");
    chart->addAxis(ejeX, Qt::AlignBottom);
    series->attachAxis(ejeX);

    ejeY = new QValueAxis();
    ejeY->setRange(0, 80);
    ejeY->setTitleText("Distancia (cm)");
    chart->addAxis(ejeY, Qt::AlignLeft);
    series->attachAxis(ejeY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Inyectamos la gráfica en tu contenedor vacío de Qt Designer
    QVBoxLayout *layoutGrafica = new QVBoxLayout(ui->contenedorGrafica);
    layoutGrafica->addWidget(chartView);

    // 2. Conectar controles de UI creados en Designer
    connect(ui->sliderServo, &QSlider::valueChanged, this, &MainWindow::alMoverServo);
    connect(ui->chkTracking, &QCheckBox::clicked, this, &MainWindow::alCambiarModosExtra);
    connect(ui->chkFuga, &QCheckBox::clicked, this, &MainWindow::alCambiarModosExtra);

    // 3. Conexión WebSocket (Asegúrate de que la IP sea la de tu ESP32)
    socket.open(QUrl("ws://10.131.211.90:81"));
    connect(&socket, &QWebSocket::connected, this, &MainWindow::alConectar);
    connect(&socket, &QWebSocket::disconnected, this, &MainWindow::alDesconectar);
    connect(&socket, &QWebSocket::textMessageReceived, this, &MainWindow::alRecibirMensaje);

    QTimer *cronometro = new QTimer(this);
    connect(cronometro, &QTimer::timeout, this, &MainWindow::loop);
    cronometro->start(1000);

    // === INICIALIZACIÓN DEL EXTRA: ACELERÓMETRO ===
    acelerometro = new QAccelerometer(this);
    connect(acelerometro, &QAccelerometer::readingChanged, this, &MainWindow::leerAcelerometro);
    acelerometro->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loop() {
    if(socket.state() != QAbstractSocket::ConnectedState) return;
    socket.sendTextMessage("{\"tipo\":\"latido\"}");
}

void MainWindow::alConectar() {
    ui->statusbar->showMessage("Conectado | Óscar Daniel Gutiérrez Barba");
}

void MainWindow::alDesconectar() {
    ui->statusbar->showMessage("Desconectado");
}

void MainWindow::enviarJSON(const QString &mensaje) {
    if(socket.state() != QAbstractSocket::ConnectedState) return;
    socket.sendTextMessage(mensaje);
}

// === FUNCIONALIDAD EXTRA Y SERVO ===

void MainWindow::alMoverServo(int angulo) {
    ui->lblServo->setText(QString("Control Manual Servo: %1°").arg(angulo));
    // Solo permitimos moverlo manual si el Tracking está apagado
    if(!ui->chkTracking->isChecked()){
        QString msg = QString("{\"tipo\":\"servo\", \"angulo\":%1}").arg(angulo);
        enviarJSON(msg);
    }
}

void MainWindow::alCambiarModosExtra() {
    bool fugaOn = ui->chkFuga->isChecked();
    bool trackOn = ui->chkTracking->isChecked();

    // Inhabilitar slider visualmente si el radar automático toma el control
    ui->sliderServo->setEnabled(!trackOn);

    QString msg = QString("{\"tipo\":\"config_extra\", \"fuga\":%1, \"tracking\":%2}")
                      .arg(fugaOn ? "true" : "false")
                      .arg(trackOn ? "true" : "false");
    enviarJSON(msg);
}

void MainWindow::alRecibirMensaje(const QString &mensaje) {
    QJsonDocument doc = QJsonDocument::fromJson(mensaje.toUtf8());
    QJsonObject obj = doc.object();

    if(obj["tipo"].toString() == "telemetria") {
        if(obj["ok"].toBool()) {
            double dist = obj["dist"].toDouble();

            // Renderizar la distancia en la gráfica
            series->append(contadorMuestras, dist);

            // Efecto de ventana deslizante
            if(contadorMuestras > 50) {
                ejeX->setRange(contadorMuestras - 50, contadorMuestras);
            }
            contadorMuestras++;
        }
    }
}

// === BOTONES DE MOVIMIENTO ===

void MainWindow::on_btnAdelante_clicked() { enviarJSON("{\"tipo\":\"mover\", \"accion\":\"derecha\", \"ms\": 100}"); }
void MainWindow::on_btnAtras_clicked() { enviarJSON("{\"tipo\":\"mover\", \"accion\":\"izquierda\", \"ms\": 100}"); }
void MainWindow::on_btnIzquierda_clicked() { enviarJSON("{\"tipo\":\"mover\", \"accion\":\"avanzar\", \"ms\": 100}"); }
void MainWindow::on_btnDerecha_clicked() { enviarJSON("{\"tipo\":\"mover\", \"accion\":\"retroceder\", \"ms\": 100}"); }

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:    on_btnAdelante_clicked(); break;
    case Qt::Key_Down:  on_btnAtras_clicked(); break;
    case Qt::Key_Left:  on_btnIzquierda_clicked();   break;
    case Qt::Key_Right: on_btnDerecha_clicked(); break;
    default: QWidget::keyPressEvent(event); break;
    }
}

// === LÓGICA DEL EXTRA (GIROSCOPIO INVERTIDO PARA FORMATO HORIZONTAL) ===
void MainWindow::leerAcelerometro() {
    qreal x = acelerometro->reading()->x();
    qreal y = acelerometro->reading()->y();

    // Se invirtieron los signos (< y >) para que el movimiento se sienta natural
    if (y < -3.0) {
        enviarJSON("{\"tipo\":\"mover\", \"accion\":\"avanzar\", \"ms\": 40}");
    } else if (y > 3.0) {
        enviarJSON("{\"tipo\":\"mover\", \"accion\":\"retroceder\", \"ms\": 40}");
    } else if (x < -3.0) {
        enviarJSON("{\"tipo\":\"mover\", \"accion\":\"derecha\", \"ms\": 100}");
    } else if (x > 3.0) {
        enviarJSON("{\"tipo\":\"mover\", \"accion\":\"izquierda\", \"ms\": 100}");
    }
}