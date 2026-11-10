#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QStandardPaths>
#include <QAccelerometer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , contadorMuestras(0)
{
    ui->setupUi(this);

    auto *pestanas = new QTabWidget(this);

    QWidget *widgetRobot = new QWidget;
    QVBoxLayout *layoutRobot = new QVBoxLayout(widgetRobot);
    layoutRobot->addWidget(ui->centralwidget);
    pestanas->addTab(widgetRobot, "Robot");

    auto *pagTemp = new QWidget;
    m_capaTemp = new QVBoxLayout(pagTemp);
    pestanas->addTab(pagTemp, "Temperatura");

    setCentralWidget(pestanas);

    // 1. Controles básicos
    m_botonLeerTemp = new QPushButton("Leer temperatura");
    m_temperatura = new QLabel("-- °C");
    m_temperatura->setAlignment(Qt::AlignCenter);

    QFont fuenteTemp = m_temperatura->font();
    fuenteTemp.setPointSize(20); // Tamaño ajustado para modo horizontal
    m_temperatura->setFont(fuenteTemp);

    m_chkAutoGuardar = new QCheckBox("Guardar cada 10 s");
    m_timerGuardar = new QTimer(this);

    m_botonCargar = new QPushButton("Cargar historial");
    m_tabla = new QTableWidget(0, 4);
    m_tabla->setHorizontalHeaderLabels({"id", "fecha_hora", "°C", "nota"});
    m_tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabla->horizontalHeader()->setStretchLastSection(true);

    m_total = new QLabel("Total: 0");

    // 2. Agrupamos la parte superior en filas horizontales (Mejora visual)
    QHBoxLayout *fila1 = new QHBoxLayout();
    fila1->addWidget(m_botonLeerTemp);
    fila1->addWidget(m_temperatura);

    QHBoxLayout *fila2 = new QHBoxLayout();
    fila2->addWidget(m_chkAutoGuardar);
    fila2->addWidget(m_botonCargar);
    fila2->addWidget(m_total);

    // 3. Fila de las notas, borrar y exportar
    QHBoxLayout *capaNota = new QHBoxLayout();

    m_id = new QSpinBox();
    m_id->setRange(0, 99999);
    m_id->setSpecialValueText("—");

    m_nota = new QLineEdit();
    m_nota->setPlaceholderText("Nota...");

    m_botonGuardarNota = new QPushButton("Guardar");
    QPushButton *botonBorrar = new QPushButton("Borrar"); // <-- NUEVO BOTÓN
    QPushButton *botonExportarCSV = new QPushButton("Exportar CSV");

    capaNota->addWidget(new QLabel("ID:"));
    capaNota->addWidget(m_id);
    capaNota->addWidget(m_nota);
    capaNota->addWidget(m_botonGuardarNota);
    capaNota->addWidget(botonBorrar); // <-- AGREGADO A LA INTERFAZ
    capaNota->addWidget(botonExportarCSV);

    // 4. Agregamos las filas a la pestaña principal
    m_capaTemp->addLayout(fila1);
    m_capaTemp->addLayout(fila2);
    m_tabla->setMinimumHeight(60);
    m_capaTemp->addWidget(m_tabla, 1); // Expande la tabla
    m_capaTemp->addLayout(capaNota);

    // --- ACCIONES DE LOS BOTONES ---

    connect(m_botonLeerTemp, &QPushButton::clicked, this, [this]() {
        enviarJSON("{\"tipo\":\"leer_dht\"}");
    });

    connect(m_chkAutoGuardar, &QCheckBox::clicked, this, [this](bool checked) {
        if (checked) m_timerGuardar->start(10000);
        else m_timerGuardar->stop();
    });

    connect(m_timerGuardar, &QTimer::timeout, this, [this]() {
        enviarJSON("{\"tipo\":\"leer_dht\"}");
    });

    connect(m_botonCargar, &QPushButton::clicked, this, [this]() {
        QSqlQuery consulta("SELECT id, fecha_hora, temperatura, nota FROM mediciones ORDER BY id DESC LIMIT 50");
        if (!consulta.isActive()) {
            qDebug() << "SELECT falló:" << consulta.lastError().text();
            return;
        }

        m_tabla->setRowCount(0);

        while (consulta.next()) {
            const int fila = m_tabla->rowCount();
            m_tabla->insertRow(fila);
            m_tabla->setItem(fila, 0, new QTableWidgetItem(consulta.value(0).toString()));
            m_tabla->setItem(fila, 1, new QTableWidgetItem(consulta.value(1).toString()));
            m_tabla->setItem(fila, 2, new QTableWidgetItem(QString::number(consulta.value(2).toDouble(), 'f', 1)));
            m_tabla->setItem(fila, 3, new QTableWidgetItem(consulta.value(3).toString()));
        }
        m_tabla->resizeColumnsToContents();

        QSqlQuery total("SELECT COUNT(*) FROM mediciones");
        if (total.next()) {
            m_total->setText("Total: " + total.value(0).toString());
        }
    });

    // Al tocar una celda, copiar el ID al SpinBox
    connect(m_tabla, &QTableWidget::cellClicked, this, [this](int fila, int) {
        m_id->setValue(m_tabla->item(fila, 0)->text().toInt());
    });

    // UPDATE: Botón Guardar nota
    connect(m_botonGuardarNota, &QPushButton::clicked, this, [this]() {
        if (m_id->value() == 0) return;
        QSqlQuery consulta;
        consulta.prepare("UPDATE mediciones SET nota = :nota WHERE id = :id");
        consulta.bindValue(":nota", m_nota->text());
        consulta.bindValue(":id", m_id->value());
        if (consulta.exec()) {
            qDebug() << "Nota guardada. Filas cambiadas:" << consulta.numRowsAffected();
            m_botonCargar->click();
        }
    });

    // DELETE: Botón Borrar Registro
    connect(botonBorrar, &QPushButton::clicked, this, [this]() {
        if (m_id->value() == 0) return;
        QSqlQuery consulta;
        consulta.prepare("DELETE FROM mediciones WHERE id = :id");
        consulta.bindValue(":id", m_id->value());

        if (consulta.exec()) {
            qDebug() << "Registro borrado. Filas afectadas:" << consulta.numRowsAffected();
            m_id->setValue(0);
            m_nota->clear();
            m_botonCargar->click(); // Recarga la tabla para que desaparezca la fila
        }
    });

    // SELECT: Botón Exportar CSV
    connect(botonExportarCSV, &QPushButton::clicked, this, []() {
        QSqlQuery consulta("SELECT id, fecha_hora, temperatura, fuente, nota FROM mediciones");
        QString ruta = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/historial_9493.csv";
        QFile archivo(ruta);

        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream salida(&archivo);
            salida << "id,fecha_hora,temperatura,fuente,nota\n";

            int filas = 0;
            while (consulta.next()) {
                salida << consulta.value(0).toString() << ","
                       << consulta.value(1).toString() << ","
                       << consulta.value(2).toString() << ","
                       << consulta.value(3).toString() << ","
                       << consulta.value(4).toString() << "\n";
                filas++;
            }
            archivo.close();
            qDebug() << "EXTRA EXPORTAR CSV -> Archivo creado en:" << ruta << "con" << filas << "filas.";
        }
    });

    // --- CONFIGURACIÓN DEL RESTO DEL CÓDIGO ---

    ui->btnAdelante->setFocusPolicy(Qt::NoFocus);
    ui->btnAtras->setFocusPolicy(Qt::NoFocus);
    ui->btnIzquierda->setFocusPolicy(Qt::NoFocus);
    ui->btnDerecha->setFocusPolicy(Qt::NoFocus);
    ui->sliderServo->setFocusPolicy(Qt::NoFocus);
    ui->chkTracking->setFocusPolicy(Qt::NoFocus);
    ui->chkFuga->setFocusPolicy(Qt::NoFocus);

    ui->btnAdelante->setAutoRepeat(true);
    ui->btnAtras->setAutoRepeat(true);
    ui->btnIzquierda->setAutoRepeat(true);
    ui->btnDerecha->setAutoRepeat(true);

    ui->btnAdelante->setAutoRepeatInterval(100);
    ui->btnAtras->setAutoRepeatInterval(100);
    ui->btnIzquierda->setAutoRepeatInterval(100);
    ui->btnDerecha->setAutoRepeatInterval(100);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString carpeta = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(carpeta);
    const QString ruta = carpeta + "/temperatura_9493.db";

    db.setDatabaseName(ruta);
    if (!db.open()) {
        qDebug() << "No se abrió la base:" << db.lastError().text();
    } else {
        QSqlQuery consulta;
        if (!consulta.exec("CREATE TABLE IF NOT EXISTS mediciones ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "fecha_hora TEXT NOT NULL, "
                           "temperatura REAL NOT NULL, "
                           "fuente TEXT NOT NULL, "
                           "nota TEXT)")) {
            qDebug() << "No se creó la tabla:" << consulta.lastError().text();
        }
    }

    setFocusPolicy(Qt::StrongFocus);
    if (centralWidget()) centralWidget()->setFocusPolicy(Qt::StrongFocus);

    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Distancia HC-SR04 (cm)");
    chart->setTheme(QChart::ChartThemeDark);

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

    QVBoxLayout *layoutGrafica = new QVBoxLayout(ui->contenedorGrafica);
    layoutGrafica->addWidget(chartView);

    connect(ui->sliderServo, &QSlider::valueChanged, this, &MainWindow::alMoverServo);
    connect(ui->chkTracking, &QCheckBox::clicked, this, &MainWindow::alCambiarModosExtra);
    connect(ui->chkFuga, &QCheckBox::clicked, this, &MainWindow::alCambiarModosExtra);

    socket.open(QUrl("ws://10.131.211.90:81"));
    connect(&socket, &QWebSocket::connected, this, &MainWindow::alConectar);
    connect(&socket, &QWebSocket::disconnected, this, &MainWindow::alDesconectar);
    connect(&socket, &QWebSocket::textMessageReceived, this, &MainWindow::alRecibirMensaje);

    QTimer *cronometro = new QTimer(this);
    connect(cronometro, &QTimer::timeout, this, &MainWindow::loop);
    cronometro->start(1000);

    acelerometro = new QAccelerometer(this);
    connect(acelerometro, &QAccelerometer::readingChanged, this, &MainWindow::leerAcelerometro);
    acelerometro->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::guardarTemperatura(double celsius) {
    const double c1 = qRound(celsius * 10) / 10.0;
    QSqlQuery consulta;
    consulta.prepare("INSERT INTO mediciones (fecha_hora, temperatura, fuente) "
                     "VALUES (:fecha_hora, :temperatura, :fuente)");
    consulta.bindValue(":fecha_hora", QDateTime::currentDateTime().toString(Qt::ISODate));
    consulta.bindValue(":temperatura", c1);
    consulta.bindValue(":fuente", "simulada");

    if (!consulta.exec()) {
        qDebug() << "INSERT falló:" << consulta.lastError().text();
        return;
    }
    m_botonCargar->click();
}

void MainWindow::loop() {
    if(socket.state() != QAbstractSocket::ConnectedState) return;
    socket.sendTextMessage("{\"tipo\":\"latido\"}");
}

void MainWindow::alConectar() {
    ui->statusbar->showMessage("Conectado | Óscar Daniel Gutiérrez Barba");
    m_botonCargar->click();
}

void MainWindow::alDesconectar() {
    ui->statusbar->showMessage("Desconectado");
    m_chkAutoGuardar->setChecked(false);
    m_timerGuardar->stop();
}

void MainWindow::enviarJSON(const QString &mensaje) {
    if(socket.state() != QAbstractSocket::ConnectedState) return;
    socket.sendTextMessage(mensaje);
}

void MainWindow::alMoverServo(int angulo) {
    ui->lblServo->setText(QString("Control Manual Servo: %1°").arg(angulo));
    if(!ui->chkTracking->isChecked()){
        QString msg = QString("{\"tipo\":\"servo\", \"angulo\":%1}").arg(angulo);
        enviarJSON(msg);
    }
}

void MainWindow::alCambiarModosExtra() {
    bool fugaOn = ui->chkFuga->isChecked();
    bool trackOn = ui->chkTracking->isChecked();

    ui->sliderServo->setEnabled(!trackOn);

    QString msg = QString("{\"tipo\":\"config_extra\", \"fuga\":%1, \"tracking\":%2}")
                      .arg(fugaOn ? "true" : "false")
                      .arg(trackOn ? "true" : "false");
    enviarJSON(msg);
}

void MainWindow::alRecibirMensaje(const QString &mensaje) {
    QJsonDocument doc = QJsonDocument::fromJson(mensaje.toUtf8());
    QJsonObject obj = doc.object();
    QString tipoMsg = obj["tipo"].toString();

    if(tipoMsg == "telemetria") {
        if(obj["ok"].toBool()) {
            double dist = obj["dist"].toDouble();
            series->append(contadorMuestras, dist);
            if(contadorMuestras > 50) {
                ejeX->setRange(contadorMuestras - 50, contadorMuestras);
            }
            contadorMuestras++;
        }
    }
    else if (tipoMsg == "dht_lectura") {
        QJsonValue t = obj.value("temperatura");

        if (!obj.value("dht_ok").toBool() || !t.isDouble()) {
            return;
        }

        m_temperatura->setText(QString::number(t.toDouble(), 'f', 1) + " °C");
        guardarTemperatura(t.toDouble());
    }
}

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

void MainWindow::leerAcelerometro() {
    qreal x = acelerometro->reading()->x();
    qreal y = acelerometro->reading()->y();

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