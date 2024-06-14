#include <windows.h>
#include <math.h>   
#define ID_TIMER 1
#define START_VX 5
#define START_VY 5
#define START_X1 50
#define START_Y 50
#define START_X2 900
#define ACCELERATION 0.2
#define MAX_SPEED 7
#define MIN_SPEED 1
#define CAR_START_SPEED 3
#define ROTATE_ANGEL 10
#define RADIAN 57.2958
#define TIMER_INTERVAL 20

typedef struct cars {
    HBITMAP hBitmap;

    BOOL RIGHT;
    BOOL LEFT;
    BOOL UP;
    BOOL DOWN;
    int rotate_angel;
    double speed;
    int angel;
    int pos;
    double x;
    double y;
    double vx;
    double vy;
    int ax;
    int ay;
    int score;
}carinfo;
typedef struct point {
    int x;
    int y;
    int r;
    int value;
}sPoint;
sPoint pointsArr[5];
XFORM xForm;
// Глобальные переменные
HBITMAP hBitmapBackground;
HBITMAP hBitmapcar1U;
HBITMAP hBitmapcar1;
HBITMAP hBitmapcar1D;
HBITMAP hBitmapcar1L;
HBITMAP hBitmapcar1R;
HBITMAP hBitmapcar2;
carinfo car1, car2;
int xPos = 0;
int yPos = 0;
DWORD starttime;
DWORD timerbefor,timerPtime;
BOOL timer;
static RECT rectTimer, rectScore1, rectScore2;




void DrawCar(HDC hdc, HBITMAP hBitmap, double angle, int x, int y) {
        BITMAP bm;
        GetObject(hBitmap, sizeof(BITMAP), &bm);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP) SelectObject(hdcMem, hBitmap);

        // Создаем матрицу трансформации и вращаем ее под нужным углом
       
        xForm.eM11 = (FLOAT)cos(angle);
        xForm.eM12 = (FLOAT)sin(angle);
        xForm.eM21 = (FLOAT)-sin(angle);
        xForm.eM22 = (FLOAT)cos(angle);
        xForm.eDx = (FLOAT)x;
        xForm.eDy = (FLOAT)y;

        SetGraphicsMode(hdc, GM_ADVANCED);
        ModifyWorldTransform(hdc, &xForm, MWT_LEFTMULTIPLY);

        // Рисуем изображение
        BitBlt(hdc, 0- 14, 0 - 31, bm.bmWidth + 15, bm.bmHeight + 31, hdcMem, 0, 0, SRCCOPY);

        // Возвращаем исходные настройки
        ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
        SetGraphicsMode(hdc, GM_COMPATIBLE);

        SelectObject(hdcMem, hOldBitmap);
        DeleteDC(hdcMem);
            
}

void DrawBackground(HDC hdc) {
    // Отображение изображения
    HDC hdcMem = CreateCompatibleDC(hdc);
  //  HDC hdcMem2 = CreateCompatibleDC(hdc);


    SelectObject(hdcMem, hBitmapBackground);
    /*switch (car1.pos) {
    case 0:
        SelectObject(hdcMem2, hBitmapcar1U);
        break;    
    case 1:
        SelectObject(hdcMem2, hBitmapcar1R);
        break;   
    case 2:
        SelectObject(hdcMem2, hBitmapcar1D);
        break;
    case 3:
        SelectObject(hdcMem2, hBitmapcar1L);
        break;
    }*/
   

    BitBlt(hdc, 0, 0, 1000, 500, hdcMem, 0, 0, SRCCOPY);
   /* switch (car1.pos) {
    case 0:
    case 2:
        BitBlt(hdc, car1.x, car1.y, car1.x + 29, car1.y + 62, hdcMem2, 0, 0, SRCCOPY);
        break;
    case 1:
    case 3:
        BitBlt(hdc, car1.x-20, car1.y+ 20 , car1.x -20 +62, car1.y + 20 + 29, hdcMem2, 0, 0, SRCCOPY);
        break;
    }*/
    
    DeleteDC(hdcMem);
 //   DeleteDC(hdcMem2);

}


void calcSpeed(int i) {
    if(i == 1){
    int temp = car1.angel;
    double arg = temp / RADIAN;
    car1.vy = (100 * car1.speed * sin(arg)) / 100;
    car1.vx = (100 * car1.speed * cos(arg)) / 100;
    if (car1.speed > 4)
        car1.rotate_angel = 3;
    else
        if (car1.speed < 2)
            car1.rotate_angel = 20;
        else
            car1.rotate_angel = 10;
    }
    else {
        int temp = car2.angel;
        double arg = temp / RADIAN;
        car2.vy = (100 * car2.speed * sin(arg)) / 100;
        car2.vx = (100 * car2.speed * cos(arg)) / 100;
        if (car2.speed > 4)
            car2.rotate_angel = 3;
        else
            if (car2.speed < 2)
                car2.rotate_angel = 15;
            else
                car2.rotate_angel = 10;
    }
}

void calcpos(int i) {
    //if ((car1.ax > 0) && (car1.ax >= START_AX - 1))
    //    car1.pos = 1;
    //if ((car1.ax < 0) && (-1 * car1.ax >= START_AX - 1))
    //    car1.pos = 3;
    //if ((car1.ay > 0) && (car1.ay >= START_AY - 1))
    //    car1.pos = 2;
    //if ((car1.ay < 0) && (car1.ay >= START_AY - 1))
    //    car1.pos = 0;
    //if (car1.ax > START_AX / 2)
    //    car1.pos = 0;
    //if (car1.ax <= -1 * START_AX / 2)
    //    car1.pos = 2;
    //if (car1.ay > START_AY / 2)
    //    car1.pos = 1;
    //if (car1.ay <= -1 * START_AY / 2)
    //    car1.pos = 3;
    if (i == 1) {
        if (car1.angel >= 45 && car1.angel < 135)
            car1.pos = 1;
        if (car1.angel >= 135 && car1.angel < 225)
            car1.pos = 2;
        if (car1.angel >= 225 && car1.angel < 315)
            car1.pos = 3;
        if (car1.angel >= 315 || car1.angel < 45)
            car1.pos = 0;
        car1.pos = car1.pos + 1;
        car1.pos %= 4;
    }
    else {
        if (car2.angel >= 45 && car2.angel < 135)
            car2.pos = 1;
        if (car2.angel >= 135 && car2.angel < 225)
            car2.pos = 2;
        if (car2.angel >= 225 && car2.angel < 315)
            car2.pos = 3;
        if (car2.angel >= 315 || car2.angel < 45)
            car2.pos = 0;
        car2.pos = car2.pos + 1;
        car2.pos %= 4;
    }
}

void rotateleft(int i) {
   /* if (car1.ax >= 0 && car1.ay >= 0) {
        if (car1.ax > -1 * START_AX - 1)
        car1.ax--;
        if (car1.ay < START_AY + 1)
        car1.ay++;

    }
    if (car1.ax >= 0 && car1.ay < 0) {
        if (car1.ax < START_AX + 1)
        car1.ax++;
        if (car1.ay < START_AY + 1)
        car1.ay++;

    }
    if (car1.ax < 0 && car1.ay < 0) {
        if (car1.ax < START_AX + 1)
        car1.ax++;
        if (car1.ay > -1 * START_AY - 1)
        car1.ay--;

    }
    if (car1.ax < 0 && car1.ay >= 0) {
        if (car1.ax > -1 * START_AX - 1)
        car1.ax--;
        if (car1.ay > -1 * START_AY - 1)
        car1.ay--;

    }*/
    if(i==1){
    car1.angel = (car1.angel - car1.rotate_angel);
    if (car1.angel < 0)
        car1.angel = 360 - car1.angel;
    calcSpeed(1);
    }
    else {
        car2.angel = (car2.angel - car2.rotate_angel);
        if (car2.angel < 0)
            car2.angel = 360 - car2.angel;
        calcSpeed(2);
    }
  //  calcpos();
}

void rotateright(int i) {
    /*if (car1.ax >= 0 && car1.ay >= 0) {
        if (car1.ax < START_AX + 1)
        car1.ax++;
        if (car1.ay > -1 * START_AY - 1)
        car1.ay--;

    }
    if (car1.ax >= 0 && car1.ay < 0) {
        if(car1.ax > -1 * START_AX - 1)
        car1.ax--;
        if(car1.ay > -1 * START_AY - 1)
        car1.ay--;
    }
    if (car1.ax < 0 && car1.ay < 0) {
        if (car1.ax > -1 * START_AX - 1)
        car1.ax--;
        if (car1.ay < START_AY + 1)
        car1.ay++;
    }
    if (car1.ax < 0 && car1.ay >= 0) {
        if (car1.ax < START_AX + 1)
        car1.ax++;
        if(car1.ay < START_AY + 1)
        car1.ay++;
    }*/
    if(i==1){
    car1.angel = (car1.angel + car1.rotate_angel) % 361;
    calcSpeed(1);
   // calcpos();
    }
    else {
        car2.angel = (car2.angel + car2.rotate_angel) % 361;
        calcSpeed(2);
    }
}

void newPoint(int i) {
    pointsArr[i].r = rand() % 15 + 5;
    pointsArr[i].x = rand() % 950 + 25;
    pointsArr[i].y = rand() % 360 + 20;
    pointsArr[i].value = pointsArr[i].r * 10 + rand() % 10;
}

void initPoints() {
    for (int i = 0; i < 5; i++)
    {
        newPoint(i);
    }
}

void checkPoints() {
    for (int i = 0; i <5; i++)
    {
        if((car1.x + 50 > pointsArr[i].x ) && (car1.x - 50 < pointsArr[i].x))
            if ((car1.y + 50 > pointsArr[i].y) && (car1.y - 50 < pointsArr[i].y))
            {
                car1.score += pointsArr[i].value;
                newPoint(i);
            }
        if ((car2.x + 50 > pointsArr[i].x) && (car2.x - 50 < pointsArr[i].x))
            if ((car2.y + 50 > pointsArr[i].y) && (car2.y - 50 < pointsArr[i].y))
            {
                car2.score += pointsArr[i].value;
                newPoint(i);
            }
    }
}

void drawPoints(HDC hdc, int i) {
   
//        Ellipse(hdc, pointsArr[4].x - pointsArr[4].r, pointsArr[4].x - pointsArr[4].r, pointsArr[4].x + pointsArr[4].r, pointsArr[4].x + pointsArr[4].r);
//        Ellipse(hdc, pointsArr[3].x - pointsArr[3].r, pointsArr[3].x - pointsArr[3].r, pointsArr[3].x + pointsArr[3].r, pointsArr[3].x + pointsArr[3].r);
//        Ellipse(hdc, pointsArr[2].x - pointsArr[2].r, pointsArr[2].x - pointsArr[2].r, pointsArr[2].x + pointsArr[2].r, pointsArr[2].x + pointsArr[2].r);
//        Ellipse(hdc, pointsArr[1].x - pointsArr[1].r, pointsArr[1].x - pointsArr[1].r, pointsArr[1].x + pointsArr[1].r, pointsArr[1].x + pointsArr[1].r);
//        Ellipse(hdc, pointsArr[0].x - pointsArr[0].r, pointsArr[0].x - pointsArr[0].r, pointsArr[0].x + pointsArr[0].r, pointsArr[0].x + pointsArr[0].r);
        Ellipse(hdc, pointsArr[i].x - pointsArr[i].r, pointsArr[i].y - pointsArr[i].r, pointsArr[i].x + pointsArr[i].r, pointsArr[i].y + pointsArr[i].r);
    
}
// Обработчик сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int timeleft;
    static HDC hdcBuffer;
    static HBITMAP hbmBuffer;
   
    hdcBuffer = CreateCompatibleDC(NULL);
    hbmBuffer = CreateCompatibleBitmap(GetDC(hwnd), 1000,500);
    SelectObject(hdcBuffer, hbmBuffer);

    static TCHAR szBuffer[100];
    static TCHAR szBufferscore1[100];
    static TCHAR szBufferscore2[100];
        
    switch (uMsg) {
    case WM_CREATE:
        rectTimer.left = 410;
        rectTimer.right = 550;
        rectTimer.top = 5;
        rectTimer.bottom = 20;
        
        rectScore1.top = 5;
        rectScore1.bottom = 20;
        rectScore1.left = 30;
        rectScore1.right = 180;
        
        rectScore2.top = 5;
        rectScore2.bottom = 20;
        rectScore2.left = 1000-180;
        rectScore2.right = 1000-30;

        car1.x = START_X1;
        car1.y = START_Y;

        car2.x = START_X2;
        car2.y = START_Y;

        car1.speed = CAR_START_SPEED;
        car2.speed = CAR_START_SPEED;

        car1.rotate_angel = ROTATE_ANGEL;
        car2.rotate_angel = ROTATE_ANGEL;

        car2.angel += 180;
        SetTimer(hwnd, ID_TIMER, TIMER_INTERVAL, NULL);
        timer = TRUE;   
        initPoints();
        // Загрузка изображения задника 
        hBitmapBackground = (HBITMAP)LoadImage(NULL, L"C:/MYSTUFF/Kursach/images/backgrnd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hBitmapBackground == NULL) {
            MessageBox(hwnd, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }
        car1.hBitmap = (HBITMAP)LoadImage(NULL, L"C:/MYSTUFF/Kursach/images/carU.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (car1.hBitmap == NULL) {
            MessageBox(hwnd, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }
            car2.hBitmap = (HBITMAP)LoadImage(NULL, L"C:/MYSTUFF/Kursach/images/car2U.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (car1.hBitmap == NULL) {
            MessageBox(hwnd, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }
        break;
        
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Рисуем на скрытом буфере
        DrawBackground(hdcBuffer);  
        DrawText(hdcBuffer, szBuffer, -1, &rectTimer, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawText(hdcBuffer, szBufferscore1, -1, &rectScore1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawText(hdcBuffer, szBufferscore2, -1, &rectScore2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawCar(hdcBuffer, car1.hBitmap, (double)(car1.angel + 90) / RADIAN, car1.x, car1.y);
        DrawCar(hdcBuffer, car2.hBitmap, (double)(car2.angel + 90) / RADIAN, car2.x, car2.y);
        for (int i = 0; i < 5; i++) {
            drawPoints(hdcBuffer, i);
        }

        // Копируем скрытый буфер на видимый
        BitBlt(hdc, 0, 0, 1000, 500, hdcBuffer, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
    }
        break;
    case WM_TIMER:
        if (starttime == 0)
            starttime = GetTickCount();
        timeleft = (int)(20 - ((GetTickCount() - starttime - timerPtime) / 1000));
        wsprintf(szBuffer, TEXT("Время: %d"), timeleft);
        wsprintf(szBufferscore1, TEXT("Счет 1: %d"), (int)car1.score);
        wsprintf(szBufferscore2, TEXT("Счет 2: %d"), (int)car2.score);

        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (timer) {
                if (car1.speed < MAX_SPEED)
                    car1.speed += ACCELERATION;
                calcSpeed(1);
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (timer) {
                if (car1.speed > MIN_SPEED)
                    car1.speed -= ACCELERATION;
                calcSpeed(1);
            }
        }

        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (timer) {
                rotateright(1);
                car1.x += car1.vx;
                car1.y += car1.vy;
            }
        }

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (timer) {
                rotateleft(1);
                car1.x += car1.vx;
                car1.y += car1.vy;
            }
        }
        // 2 car
        if (GetAsyncKeyState('W') & 0x8000) {
            if (timer) {
                if (car2.speed < MAX_SPEED)
                    car2.speed += ACCELERATION;
                calcSpeed(2);
            }
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            if (timer) {
                if (car2.speed > MIN_SPEED)
                    car2.speed -= ACCELERATION;
                calcSpeed(2);
            }
        }

        if (GetAsyncKeyState('D') & 0x8000) {
            if (timer) {
                rotateright(2);
                car2.x += car2.vx;
                car2.y += car2.vy;
            }
        }

        if (GetAsyncKeyState('A') & 0x8000) {
            if (timer) {
                rotateleft(2);
                car2.x += car2.vx;
                car2.y += car2.vy;
            }
        }
        //car1
        car1.x += car1.vx;
        if (car1.x > 1000){
        car1.x -= 1000;
        }
        if (car1.x < 0) {
            car1.x += 1000;
        }
        car1.y += car1.vy;
        if (car1.y > 500){
            car1.y -= 500;
        }
        if (car1.y < 0)
            car1.y += 500;
        //car2
        car2.x += car2.vx;
        if (car2.x > 1000) {
            car2.x -= 1000;
        }
        if (car2.x < 0) {
            car2.x += 1000;
        }
        car2.y += car2.vy;
        if (car2.y > 500) {
            car2.y -= 500;
        }
        if (car2.y < 0)
            car2.y += 500;
        checkPoints();
        if (timeleft <= -1) {
            KillTimer(hwnd, ID_TIMER);
            if(car1.score>car2.score)
            MessageBox(hwnd, L"Красный игрок победил!", L"Конец игры", MB_OK );
            else
                if(car1.score<car2.score)
                    MessageBox(hwnd, L"Синий игрок победил!", L"Конец игры", MB_OK );
                else
                    MessageBox(hwnd, L"Ничья", L"Конец игры", MB_OK);
            wchar_t szModule[MAX_PATH];
            GetModuleFileName(NULL, szModule, MAX_PATH);

            // Перезапускаем программу
            ShellExecute(NULL, NULL, szModule, NULL, NULL, SW_SHOWNORMAL);
            PostQuitMessage(0);
        }
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;
    case WM_KEYDOWN:
        // Обработка клавиш для перемещения изображения
            switch (wParam) {
            /*case VK_LEFT:
                car1.pos = 3;
                car1.LEFT = TRUE;
                if (car1.pos == 3)
                {
                    if(car1.DOWN == FALSE && car1.UP == false)
                        car1.x -= 10;
                    if (car1.DOWN == TRUE)
                    {
                        car1.x -= 5;
                        car1.y += 5;
                    }
                    if (car1.UP == TRUE)
                    {
                        car1.x -= 5;
                        car1.y -= 5;
                    }
                }
                else{
                car1.pos = 3;
                car1.LEFT = TRUE;
                }
                break;
            case VK_RIGHT:
                car1.pos = 1;
                car1.RIGHT = TRUE;
                if (car1.pos == 1) {
                    if (car1.DOWN == FALSE && car1.UP == false)
                        car1.x += 10;
                    if (car1.DOWN == TRUE)
                    {
                        car1.x += 5;
                        car1.y += 5;
                    }
                    if (car1.UP == TRUE)
                    {
                        car1.x += 5;
                        car1.y -= 5;
                    }
                }
                else{
                car1.pos = 1;
                car1.RIGHT = TRUE;
                }
                break;
            case VK_UP:
                car1.pos = 0;
                car1.UP = TRUE;
                if (car1.pos == 0) {
                    if(car1.LEFT == FALSE && car1.RIGHT == FALSE)
                    car1.y -= 10;
                    if (car1.LEFT == TRUE)
                    {
                        car1.y -= 5;
                        car1.x -= 5;
                    }
                    if (car1.RIGHT == TRUE)
                    {
                        car1.y -= 5;
                        car1.x += 5;
                    }
                }
                else{
                car1.pos = 0;
                car1.UP = TRUE;
                }
                break;
            case VK_DOWN:
                car1.pos = 2;
                car1.DOWN = TRUE;
                if (car1.pos == 2)
                {
                    if (car1.LEFT == FALSE && car1.RIGHT == FALSE)
                        car1.y += 10;
                    if (car1.LEFT == TRUE)
                    {
                        car1.y += 5;
                        car1.x -= 5;
                    }
                    if (car1.RIGHT == TRUE)
                    {
                        car1.y += 5;
                        car1.x += 5;
                    }
                }
                else{
                car1.pos = 2;
                car1.DOWN = TRUE;
                }
                break;
            }*/
        /*case VK_UP:
            if(timer){
            if(car1.speed < MAX_SPEED)
            car1.speed += ACCELERATION;
            calcSpeed();
            }
            break;
        case VK_DOWN:
            if (timer) {
            if (car1.speed > MIN_SPEED)
                car1.speed -= ACCELERATION;
            calcSpeed();
              }
              break;
        case VK_RIGHT:
            if (timer) {
            rotateright();
            car1.x += car1.vx;
            car1.y += car1.vy;
              }
              break;
        case VK_LEFT:
            if (timer) {
            rotateleft();
            car1.x += car1.vx;
            car1.y += car1.vy;
              }
              break;*/
        case 0x48 :
            KillTimer(hwnd, ID_TIMER);
            timerbefor = GetTickCount();
            MessageBox(NULL, L"Добро пожаловать в игровое приложение гонки!\n Назначения клавиш: \nP - пауза, останавливает игру на время\nH - помощь, напомнить себе основные элементы управления \nESC - закрыть игру\nИгра длиться 20 секунд на поле появляются белые круги, каждый из которых даст очки, собравшему их игроку в зависимости от своего размера. Для упарвления используйте клавиши WASD и стрелки\nУдачи", L"Welcome", MB_OK );
            timerPtime += GetTickCount() - timerbefor;
            SetTimer(hwnd, ID_TIMER, TIMER_INTERVAL, NULL);
            break;
        case 0x50 : // P
            if(timer)
            {   
                timerbefor = GetTickCount();
                KillTimer(hwnd, ID_TIMER);
                timer = FALSE;
            }
            else
            {
                SetTimer(hwnd, ID_TIMER, TIMER_INTERVAL, NULL);
                timer = TRUE;
                timerPtime += GetTickCount() - timerbefor;
            }
            break;
        case VK_ESCAPE:
            KillTimer(hwnd, ID_TIMER);
            DeleteObject(hBitmapBackground);
            PostQuitMessage(0);
            break;
}
        // Принудительное перерисование окна
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;
    
    case WM_DESTROY:
        // Освобождение ресурсов при закрытии окна
        if (hdcBuffer) {
            DeleteDC(hdcBuffer);
            DeleteObject(hbmBuffer);
        }
        KillTimer(hwnd, ID_TIMER);
        DeleteObject(hBitmapBackground);
        DeleteObject(car1.hBitmap);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ImageWindowClass";
    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,
        L"ImageWindowClass",
        L"Racing Club",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 530,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }
   
    // Отображение окна
    ShowWindow(hwnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
