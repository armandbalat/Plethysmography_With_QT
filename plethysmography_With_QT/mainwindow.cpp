#include "mainwindow.h"

using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);

    QObject::connect(ui->pbOpenFile, SIGNAL(clicked()), this,SLOT(openCapture())); // On connecte notre bouton au Slot openDialog
    QObject::connect(ui->pbCloseFile, SIGNAL(clicked()), this,SLOT(closeCapture())); // On connecte notre bouton au Slot openDialog

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
}


void MainWindow::openCapture()
{
    m_cap.open(0);

     if(!m_cap.isOpened())  // Check if we succeeded
     {
         cout << "camera is not open" << endl;
     }
     else
     {
         cout << "camera is open" << endl;
         connect(m_timer, SIGNAL(timeout()), this, SLOT(update_window()));

         m_timer->start(30);

     }
}

void MainWindow::closeCapture()
{
    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(update_window()));
    m_cap.release();
    Mat image = Mat::zeros(m_frame.size(),CV_8UC3);
    m_qtImage = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(m_qtImage));
    ui->label->resize(ui->label->pixmap()->size());
    cout << "camera is closed" << endl;
}

void MainWindow::update_window()
{
    m_cap >> m_frame;
    detectAndDisplay( m_frame);
    /*cvtColor(m_frame, m_frame, CV_BGR2RGB);
    m_qtImage = QImage((const unsigned char*) (m_frame.data), m_frame.cols, m_frame.rows, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(m_qtImage));
    ui->label->resize(ui->label->pixmap()->size());  */
}

void MainWindow::detectAndDisplay(Mat l_frame)
{
    String face_cascade_name = "C:/Users/arman/Documents/ProjetsQT/sans_titre/haarcascade_frontalface_alt.xml";
    CascadeClassifier face_cascade;

    if( !face_cascade.load( face_cascade_name ) )
    {
        cout<<"[Error] cannot load face cascade detection.xml"<<endl;
    }
        else
        {
        std::vector<Rect> faces;
        Mat frame_gray;

          cvtColor( l_frame, frame_gray, CV_BGR2GRAY );
          equalizeHist( frame_gray, frame_gray );  //640x480
          //-- Detect faces
          face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

          for( size_t i = 0; i < faces.size(); i++ )
            {
           Point center( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );
           Point TopFaceLeftCorner( int(center.x - faces[i].width*0.4),int(center.y + faces[i].height*0.7));
           Point BottomRightCorner( int(center.x + faces[i].width*0.4), int(center.y - faces[i].height*0.7 ));

           Point TopFront( int(center.x - faces[i].width*0.27),int(center.y - faces[i].height*0.5));
           Point BottomFront( int(center.x + faces[i].width*0.27), int(center.y - faces[i].height*0.3 ));

           ellipse( l_frame, center, Size( int(faces[i].width*0.01),int(faces[i].height*0.01)), 0, 0, 360, Scalar( 0, 0, 255 ), 4, 8, 0 );
           rectangle(l_frame,TopFaceLeftCorner, BottomRightCorner, cv::Scalar(0, 255, 0),1);

           rectangle(l_frame,BottomFront,TopFront, cv::Scalar(255, 0, 0),1);  //DEFINIR LE FRONT RECT
           //Mat faceROI = frame_gray( faces[i] );   img avec juste les faces
            }
          cvtColor(l_frame, l_frame, CV_BGR2RGB);
          m_qtImage = QImage((const unsigned char*) (l_frame.data), l_frame.cols, l_frame.rows, QImage::Format_RGB888);
          ui->label->setPixmap(QPixmap::fromImage(m_qtImage));
          ui->label->resize(ui->label->pixmap()->size());
          calculatePixelArea(faces);
         }
}

void MainWindow::calculatePixelArea(std::vector<Rect> front)
{
    QRgb* CoulPixel = new QRgb();       // on crée un objet Qrgb
    int FirstRectX;
    int FirstRectY;
    int SecondRectX;
    int SecondRectY;

    double numberOfPixel=0;
    double RedPixelTotal=0;
    double Mean;

    if(front.size()!=0){
        Point center ( int(front[0].x + front[0].width*0.5), int(front[0].y + front[0].height*0.5));
        Point TopFront( int(center.x - front[0].width*0.27),int(center.y - front[0].height*0.5));
        Point BottomFront( int(center.x + front[0].width*0.27), int(center.y - front[0].height*0.3 ));

        FirstRectX=TopFront.x;
        FirstRectY=TopFront.y;
        SecondRectX=BottomFront.x;
        SecondRectY=BottomFront.y;

        for(int j=FirstRectX;j<SecondRectX;j++) // Permet de défiler toute la largeur du rect d'interret
        {

            for(int i=FirstRectY;i<SecondRectY;i++)  // permet de défiler toute la hauteur de l'image pour chaque colone de l'image (for au dessus)
            {

                *CoulPixel = m_qtImage.pixel(i,j);    // notre objet Qrgb recupere la valeur du pixel de position (i,j)
                RedPixelTotal += qRed(*CoulPixel);
                numberOfPixel++;
            }
        }

        Mean=RedPixelTotal/numberOfPixel;
        cout<<"Moyenne : "<<Mean<<endl;
        WriteOnTxt(Mean);
    }
}

void MainWindow::WriteOnTxt(double moyenne)
{
     ofstream myfile;
     myfile.open ("D:/Data4.txt",ios::app);
     myfile << moyenne <<endl;
     myfile.close();
}



