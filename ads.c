#ifndef UNICODE
#define UNICODE
#endif

#include <math.h>
#include <windows.h>
#include <commctrl.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "ads_constants.h"

#pragma comment(lib, "winmm.lib")

WAVEHDR header1;
WAVEHDR header2;
HWAVEOUT hWaveOut = 0;
WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 44100, 44100*2, 2, 16, 0 };

__int16 buffer1[BUFFERSIZE];
__int16 buffer2[BUFFERSIZE];

int currentBuffer = 1;


//File IO-------------------------
FILE *pFile;
char myString[16]; 
int myInt1;
int myInt2;

int output_size;
wchar_t *wstring;
int size;

char keywords[4];
char ch;
int doBreak = 0;

//--------------------------------

//MIDI----------------------------
int midiStarted = 0;
int midiDeviceNumber = 0;
HMIDIIN device;
int flag;
int activeMIDIDevices;

int doStartMIDI = 0;

//--------------------------------

double sinOsc[30] = {1.0, 0.5, 0.33333, 0.25, 0.2, 0.1667, 0.1429, 0.125, 0.1111, 0.1, 0.1, 0.3, 0.0, 0.0, 0.2, 0.1667, 0.0, 0.0, 0.0, 0.0,  0.1, 0.3, 0.0, 0.0, 0.2, 0.1667, 0.0, 0.0, 0.0, 0.0};
double oscSum = 1;

double wavTable[10000];
void fillWaveTable(int waveForm);

int volume = 2000;

int tone; 
int state = 0;
int i = 0;
int sel = 0;
int toneLength = 2;
int toneShift = 0;
int timeShift = 0;
int timeShiftMax = 4;

int bpm=120;
int beatres=8;

int ticker = 0;
int playOn = 0;
int smallTicker = 0;

int paramAttack = 141; //300;
int paramDecay = 600;
int paramSustain = 100;
int paramRelease = 189;//450;

double paramLFOfreq = 10.0;
double paramLFOamp = 4.0;
int vibratoOn = 0;

int mouseX;
int mouseY;


struct csFIFO{
  __int16 data[CSFIFOSIZE];
  int indexZero;
};

struct csFIFO echoEffect;
void csFIFO_Add(_int16 newData, struct csFIFO *t);
_int16 csFIFO_Peek(int atIndex, struct csFIFO *t);


struct delayLine{
  _int16 data[DELAYMAX];
  int index;
  int length;
};

double delayProcess(_int16 input, struct delayLine *t);
void delayInit(int theLength, struct delayLine *t);

struct delayLine delay1;
struct delayLine delay2;
struct delayLine delay3;
struct delayLine delay4;
struct delayLine delay5;
struct delayLine delay6;
struct delayLine delay7;
struct delayLine delay8;
struct delayLine delay9;
struct delayLine delay10;

int echoActive = 0;

_int16 echoVar = 0;

_int16 echoArray[44100];
int echoIndex = 0;
int echoLength = 1;

_int16 echoArray2[44100];
int echoIndex2 = 0;
int echoLength2 = 1;

//echoParams------------------------------------------

                    //0-9 early bounce, 10-19 bounce
double bounce[20] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.25, 0.20, 0.15, 0.10,
                     0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20};

double genOut[10] = {0.0, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.0};

double feedbackMix = 0.7;

int delayMult = 10000;

int currentPreset = 0;
void chooseEchoPreset(int choice);

//-----------------------------------------------

struct soundNode{
  double frequency;
  double phaseAngle;
  double lfoPhase;
  int id;

  double attack;
  double decay;
  double release;

  int releasedTrue;

  struct soundNode *next;
};

struct soundNode *soundlistHead, *soundlistZero;
struct soundNode *sLoopPointer;

int nPresets = 0;

struct presetNode{
  wchar_t *name;
  int paramAttack;
  int paramDecay;
  int paramSustain;
  int paramRelease;
  int sinOsc[30];
  int id;
  struct presetNode *next;
};

struct presetNode *presetlistHead, *presetlistZero;
struct presetNode *pLoopPointer;

//playlist in a linked list-form
struct plNode {
  int tone;
  int timeMark;
  int length;
  struct plNode *next;
};

struct plNode *playlistHead, *playlistZero;

struct plNode *plLoopPointer;


RECT waveDisplay;
RECT adsrDisplay;

void soundlistInsertAfter(double v1, double v2, int v3, struct soundNode *t);
void soundlistDeleteNext(struct soundNode *t);
void emptySoundlist(void);

void presetInsertAfter(wchar_t *v1, int v2, struct presetNode *t);
void presetDeleteNext(struct presetNode *t);

void playlistInsertAfter(int v1, int v2, int v3, struct plNode *t);
void playlistDeleteNext(struct plNode *t);
void emptyPlaylist(void);

void gaussian(double *v1, double *v2);

double getFrequency(int tone);
double envelopeADSR(double a, double d, double r);


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSaveProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndMIDIProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSeqProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndRevProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


void fillBuffer(__int16 buffer[]);
void theGraphics(HWND hwnd);
void theSeqGraphics(HWND hwnd);
void theRevGraphics(HWND hwnd);


HINSTANCE thisInstance;

HWND hwnd2; //input
WNDCLASSW wc2;
int saveWindowActive = 0;
POINT currentPos;

HWND hwnd3; //input
WNDCLASSW wc3;
int midiWindowActive = 0;

HWND hwnd4; //sequencer
WNDCLASSW wc4;
int seqWindowActive = 0;

HWND hwnd5; //reverb
WNDCLASSW wc5;
int revWindowActive = 0;


HWND hwndList;

BYTE ANDmaskIcon[] = {0xFF, 0xFF, 0xFF, 0xFF,   // line 1 
                      0xFF, 0xFF, 0xC3, 0xFF,   // line 2 
                      0xFF, 0xFF, 0x00, 0xFF,   // line 3 
                      0xFF, 0xFE, 0x00, 0x7F,   // line 4 
 
                      0xFF, 0xFC, 0x00, 0x1F,   // line 5 
                      0xFF, 0xF8, 0x00, 0x0F,   // line 6 
                      0xFF, 0xF8, 0x00, 0x0F,   // line 7 
                      0xFF, 0xF0, 0x00, 0x07,   // line 8 
 
                      0xFF, 0xF0, 0x00, 0x03,   // line 9 
                      0xFF, 0xE0, 0x00, 0x03,   // line 10 
                      0xFF, 0xE0, 0x00, 0x01,   // line 11 
                      0xFF, 0xE0, 0x00, 0x01,   // line 12 
 
                      0xFF, 0xF0, 0x00, 0x01,   // line 13 
                      0xFF, 0xF0, 0x00, 0x00,   // line 14 
                      0xFF, 0xF8, 0x00, 0x00,   // line 15 
                      0xFF, 0xFC, 0x00, 0x00,   // line 16 
 
                      0xFF, 0xFF, 0x00, 0x00,   // line 17 
                      0xFF, 0xFF, 0x80, 0x00,   // line 18 
                      0xFF, 0xFF, 0xE0, 0x00,   // line 19 
                      0xFF, 0xFF, 0xE0, 0x01,   // line 20 
 
                      0xFF, 0xFF, 0xF0, 0x01,   // line 21 
                      0xFF, 0xFF, 0xF0, 0x01,   // line 22 
                      0xFF, 0xFF, 0xF0, 0x03,   // line 23 
                      0xFF, 0xFF, 0xE0, 0x03,   // line 24 
 
                      0xFF, 0xFF, 0xE0, 0x07,   // line 25 
                      0xFF, 0xFF, 0xC0, 0x0F,   // line 26 
                      0xFF, 0xFF, 0xC0, 0x0F,   // line 27 
                      0xFF, 0xFF, 0x80, 0x1F,   // line 28 
 
                      0xFF, 0xFF, 0x00, 0x7F,   // line 29 
                      0xFF, 0xFC, 0x00, 0xFF,   // line 30 
                      0xFF, 0xF8, 0x03, 0xFF,   // line 31 
                      0xFF, 0xFC, 0x3F, 0xFF};  // line 32

BYTE XORmaskIcon[] = {0x00, 0x00, 0x00, 0x00,   // line 1 
                      0x00, 0x00, 0x00, 0x00,   // line 2 
                      0x00, 0x00, 0x00, 0x00,   // line 3 
                      0x00, 0x00, 0x00, 0x00,   // line 4 
 
                      0x00, 0x00, 0x00, 0x00,   // line 5 
                      0x00, 0x00, 0x00, 0x00,   // line 6 
                      0x00, 0x00, 0x00, 0x00,   // line 7 
                      0x00, 0x00, 0x38, 0x00,   // line 8 
 
                      0x00, 0x00, 0x7C, 0x00,   // line 9 
                      0x00, 0x00, 0x7C, 0x00,   // line 10 
                      0x00, 0x00, 0x7C, 0x00,   // line 11 
                      0x00, 0x00, 0x38, 0x00,   // line 12 
 
                      0x00, 0x00, 0x00, 0x00,   // line 13 
                      0x00, 0x00, 0x00, 0x00,   // line 14 
                      0x00, 0x00, 0x00, 0x00,   // line 15 
                      0x00, 0x00, 0x00, 0x00,   // line 16 
 
                      0x00, 0x00, 0x00, 0x00,   // line 17 
                      0x00, 0x00, 0x00, 0x00,   // line 18 
                      0x00, 0x00, 0x00, 0x00,   // line 19 
                      0x00, 0x00, 0x00, 0x00,   // line 20 
 
                      0x00, 0x00, 0x00, 0x00,   // line 21 
                      0x00, 0x00, 0x00, 0x00,   // line 22 
                      0x00, 0x00, 0x00, 0x00,   // line 23 
                      0x00, 0x00, 0x00, 0x00,   // line 24 
 
                      0x00, 0x00, 0x00, 0x00,   // line 25 
                      0x00, 0x00, 0x00, 0x00,   // line 26 
                      0x00, 0x00, 0x00, 0x00,   // line 27 
                      0x00, 0x00, 0x00, 0x00,   // line 28 
 
                      0x00, 0x00, 0x00, 0x00,   // line 29 
                      0x00, 0x00, 0x00, 0x00,   // line 30 
                      0x00, 0x00, 0x00, 0x00,   // line 31 
                      0x00, 0x00, 0x00, 0x00};  // line 32 

time_t t;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, int nCmdShow) {
    
    MSG  msg;    
    HWND hwnd;
    WNDCLASSW wc; 



    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.lpszClassName = L"Sinewave audio test";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpszMenuName  = NULL;
    wc.lpfnWndProc   = WindowProc;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = CreateIcon(NULL, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon); //LoadIcon(hInstance, IDI_APPLICATION);

    RegisterClassW(&wc);

    hwnd = CreateWindowW(wc.lpszClassName, L"Additive Synthesizer",
                WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX | WS_VISIBLE, 500, 300, 895, 475, NULL, NULL, hInstance, NULL);  

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    static HWND hwndTrack1;
    static HWND hwndTrack2;
    static HWND hwndTrack3;
    static HWND hwndTrack4;
    static HWND hwndTrack5;
    static HWND hwndTrack6;
    static HWND hwndTrack7;
    static HWND hwndTrack8;
    static HWND hwndTrack9;
    static HWND hwndTrack10;


    static HWND hwndTrackVolume;

    static HWND hwndTrackAttack;
    static HWND hwndTrackDecay;
    static HWND hwndTrackSustain;
    static HWND hwndTrackRelease;

    static HWND hwndTrackLFOfreq;
    static HWND hwndTrackLFOamp;


    static HWND hwndButton1;
    static HWND hwndButton2;
    static HWND hwndButton3;

    static HWND hWndComboBox1;

    RECT area;
   // static HWND hwndList; //use global instead to be able to reach from save sound window

    if ((doStartMIDI > 0)&&(midiStarted < 1)){
        //midiDeviceNumber = 1;  //this we get from the startMIDI-window

        //open the given device
        flag = midiInOpen(&device, midiDeviceNumber, (DWORD_PTR)hwnd,0, CALLBACK_WINDOW);
        if (!flag == MMSYSERR_NOERROR){
            MessageBoxW(NULL, L"Error opening MIDI Input.", L"Error", MB_OK);
        }
        else{
            flag = midiInStart(device);
            if (!flag == MMSYSERR_NOERROR){
                MessageBoxW(NULL, L"Error starting MIDI recording.", L"Error", MB_OK);
                midiInReset(device);
                midiInClose(device);
            }
            else{
                midiStarted = 1;
            }
        }
        doStartMIDI = 0;
    }

    switch(msg) {
        case WM_CREATE:
            srand((unsigned) time(&t)); //random number generator initialized

            //initialize echobuffer
            for(i = 0; i<CSFIFOSIZE; i++){
              echoEffect.data[i] = 0;
            }
            echoEffect.indexZero = 0;

            //initialize linked list-playlist
            playlistHead = (struct plNode *)malloc(sizeof *playlistHead);
            playlistZero = (struct plNode *)malloc(sizeof *playlistZero);


            playlistHead->next = playlistZero;
            playlistHead->tone = -1;
            playlistHead->timeMark = -1;
            playlistHead->length = -1;

            playlistZero->tone = -1;
            playlistZero->timeMark = -1;
            playlistZero->length = -1;
            playlistZero->next = playlistZero;


            //initialize 'linked list'-soundlist
            soundlistHead = (struct soundNode *)malloc(sizeof *soundlistHead);
            soundlistZero = (struct soundNode *)malloc(sizeof *soundlistZero);

            soundlistHead->next = soundlistZero;
            soundlistHead->frequency = 0.0;
            soundlistHead->phaseAngle = 0.0;
            soundlistHead->id = -1;
            
            soundlistZero->next = soundlistZero;
            soundlistZero->frequency = 0.0;
            soundlistZero->phaseAngle = 0.0;
            soundlistZero->id = -2;

            fillWaveTable(SINE_WAVE);

            fillBuffer(buffer1);
            fillBuffer(buffer2);

            waveOutOpen(&hWaveOut, WAVE_MAPPER, (WAVEFORMATEX *)&wfx, (DWORD_PTR)hwnd, 0, CALLBACK_WINDOW);

            header1 = (WAVEHDR){ (LPSTR)buffer1, sizeof(buffer1), 0, 0, 0, 0, 0, 0};
            waveOutPrepareHeader(hWaveOut, &header1, sizeof(WAVEHDR));

            header2 = (WAVEHDR){ (LPSTR)buffer2, sizeof(buffer2), 0, 0, 0, 0, 0, 0};
            waveOutPrepareHeader(hWaveOut, &header2, sizeof(WAVEHDR));

            waveOutWrite(hWaveOut, &header1, sizeof(WAVEHDR));
            waveOutWrite(hWaveOut, &header2, sizeof(WAVEHDR));





            hwndTrack1 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack1, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack1, TBM_SETPOS, TRUE, (int)(sinOsc[0]*1000)); 

            hwndTrack2 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+1*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack2, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack2, TBM_SETPOS, TRUE, (int)(sinOsc[1]*1000)); 

            hwndTrack3 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+2*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack3, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack3, TBM_SETPOS, TRUE, (int)(sinOsc[2]*1000)); 

            hwndTrack4 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+3*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack4, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack4, TBM_SETPOS, TRUE, (int)(sinOsc[3]*1000)); 


            hwndTrack5 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+4*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack5, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack5, TBM_SETPOS, TRUE, (int)(sinOsc[4]*1000)); 

            hwndTrack6 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+5*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack6, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack6, TBM_SETPOS, TRUE, (int)(sinOsc[5]*1000)); 

            hwndTrack7 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+6*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack7, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack7, TBM_SETPOS, TRUE, (int)(sinOsc[6]*1000)); 

            hwndTrack8 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+7*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack8, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack8, TBM_SETPOS, TRUE, (int)(sinOsc[7]*1000)); 

            hwndTrack9 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+8*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack9, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack9, TBM_SETPOS, TRUE, (int)(sinOsc[8]*1000)); 

            hwndTrack10 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      STB_X, STB_Y+9*30, 300, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrack10, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrack10, TBM_SETPOS, TRUE, (int)(sinOsc[9]*1000)); 

            hwndButton1 = CreateWindow( 
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"1-10",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_RADIOBUTTON |  WS_GROUP ,  // Styles 
                STB_X+10,         // x position 
                STB_Y-20,         // y position 
                60,        // Button width
                20,        // Button height
                hwnd,     // Parent window
                (HMENU)1,       // No menu.
                NULL,  //(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), 
                NULL);      // Pointer not needed.

            hwndButton2 = CreateWindow( 
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"11-20",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_RADIOBUTTON |  WS_GROUP ,  // Styles 
                STB_X+70,         // x position 
                STB_Y-20,         // y position 
                60,        // Button width
                20,        // Button height
                hwnd,     // Parent window
                (HMENU)2,       // No menu.
                NULL,  //(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), 
                NULL);      // Pointer not needed.

            hwndButton3 = CreateWindow( 
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"21-30",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_RADIOBUTTON |  WS_GROUP ,  // Styles 
                STB_X+140,         // x position 
                STB_Y-20,         // y position 
                60,        // Button width
                20,        // Button height
                hwnd,     // Parent window
                (HMENU)3,       // No menu.
                NULL,  //(HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE), 
                NULL);      // Pointer not needed.


            SendMessageW(hwndButton1, BM_SETCHECK , BST_CHECKED, 0);
            SendMessageW(hwndButton2, BM_SETCHECK , BST_UNCHECKED, 0);
            SendMessageW(hwndButton3, BM_SETCHECK , BST_UNCHECKED, 0);

            hWndComboBox1 = CreateWindow(WC_COMBOBOX, TEXT(""), 
                CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                STB_X+8, 20, 150, 500, hwnd, NULL, NULL,
                NULL);

            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Sine");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Square");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Sawtooth");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"N2 wave");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"N3 wave");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Organ");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"N-1/2 wave");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"3 steps stairs");

            SendMessage(hWndComboBox1, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

            hwndTrackVolume = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      15, STB_Y, 30, 300, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackVolume, TBM_SETRANGE,  FALSE, MAKELONG(0, 5000)); 
            SendMessageW(hwndTrackVolume, TBM_SETPOS, TRUE, 5000-2000); 

            hwndTrackAttack = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      410 + ADSR_TRANSL_X, 330 + ADSR_TRANSL_Y, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackAttack, TBM_SETRANGE,  FALSE, MAKELONG(0, 500)); 
            SendMessageW(hwndTrackAttack, TBM_SETPOS, TRUE, 600-paramAttack); 

            hwndTrackDecay = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      410+30 + ADSR_TRANSL_X, 330 + ADSR_TRANSL_Y, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDecay, TBM_SETRANGE,  FALSE, MAKELONG(0, 500)); 
            SendMessageW(hwndTrackDecay, TBM_SETPOS, TRUE, 600-paramDecay); 

            hwndTrackSustain = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      410+2*30 + ADSR_TRANSL_X, 330 + ADSR_TRANSL_Y, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackSustain, TBM_SETRANGE,  FALSE, MAKELONG(0, 100)); 
            SendMessageW(hwndTrackSustain, TBM_SETPOS, TRUE, 100-paramSustain); 

            hwndTrackRelease = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      410+3*30 + ADSR_TRANSL_X, 330 + ADSR_TRANSL_Y, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackRelease, TBM_SETRANGE,  FALSE, MAKELONG(0, 500)); 
            SendMessageW(hwndTrackRelease, TBM_SETPOS, TRUE, 600-paramRelease); 


            waveDisplay.left = 400;
            waveDisplay.top = 100-30;
            waveDisplay.right = 400+300;
            waveDisplay.bottom = 100+200-30;
           
            adsrDisplay.left = 540 + ADSR_TRANSL_X;
            adsrDisplay.top = 330 + ADSR_TRANSL_Y;
            adsrDisplay.right = 540+160 + ADSR_TRANSL_X;
            adsrDisplay.bottom = 330+90 + ADSR_TRANSL_Y;


            hwndList = CreateWindowW(WC_LISTBOXW , NULL, WS_CHILD 
                | WS_VISIBLE | LBS_NOTIFY |  WS_VSCROLL |  WS_BORDER,
                720, 60, 150, 120, hwnd, 
                (HMENU) IDC_LIST, NULL, NULL);

            //SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)L"Turboman");
            //SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)L"Dunderkarlsson");
            //SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)L"Heinz");

            presetlistHead = (struct presetNode *)malloc(sizeof *presetlistHead);
            presetlistZero = (struct presetNode *)malloc(sizeof *presetlistZero);

            presetlistHead->next = presetlistZero;
            presetlistZero->next = presetlistZero;

            presetlistHead->id = -1;
            presetlistZero->id = -1;


            keywords[0] = 0;
            keywords[1] = 0;
            keywords[2] = 0;
            keywords[3] = 0;


            pFile = fopen("presets.txt", "r");
            if (pFile != NULL){
            ch = ' ';
            doBreak = 0;
            while(1){
                ch = fgetc(pFile);
                if (ch == EOF){
                    break;
                }

                keywords[3] = keywords[2];
                keywords[2] = keywords[1];
                keywords[1] = keywords[0];
                keywords[0] = ch;

                if ((keywords[3] == '\n')&&(keywords[2] == 'N')&&(keywords[1] == 'E')&&(keywords[0] == 'W')){
                   if (fscanf(pFile, "%s", myString) ==EOF){
                      break;
                   }

                   //convert string to unicode (wchar_t)
                   output_size = MultiByteToWideChar(CP_ACP,0,myString,-1,NULL,0);
                   wstring = malloc(output_size * sizeof(wchar_t));
                   size = MultiByteToWideChar(CP_ACP,0,myString,-1,wstring,output_size);

                   presetInsertAfter(wstring, nPresets, presetlistHead);
                   presetlistHead->next->id = nPresets;
                   SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)presetlistHead->next->name);
                   nPresets++;
                   free(wstring);
                }
                else if ((keywords[3] == '\n')&&(keywords[2] == 'S')&&(keywords[1] == 'I')&&(keywords[0] == 'N')){
                   if (fscanf(pFile, "%d", &myInt1) == EOF){
                      break;
                   }
                   if (fscanf(pFile, "%d", &myInt2) == EOF){
                      break;
                   }
                   if((myInt1 > 0)&&(myInt1 < 31)&&(myInt2 > -1)&&(myInt2 < 1001)){
                      presetlistHead->next->sinOsc[myInt1-1] = myInt2;
                   }
                }
                else if ((keywords[3] == '\n')&&(keywords[2] == 'E')&&(keywords[1] == 'A')&&(keywords[0] == 'T')){
                   if (fscanf(pFile, "%d", &myInt1) == EOF){
                      break;
                   }
                   if((myInt1 > 99)&&(myInt1 < 601)){
                      presetlistHead->next->paramAttack = myInt1;
                   }
                }
                else if ((keywords[3] == '\n')&&(keywords[2] == 'E')&&(keywords[1] == 'D')&&(keywords[0] == 'E')){
                   if (fscanf(pFile, "%d", &myInt1) == EOF){
                      break;
                   }
                   if((myInt1 > 99)&&(myInt1 < 601)){
                      presetlistHead->next->paramDecay = myInt1;
                   }
                }
                else if ((keywords[3] == '\n')&&(keywords[2] == 'E')&&(keywords[1] == 'S')&&(keywords[0] == 'U')){
                   if (fscanf(pFile, "%d", &myInt1) == EOF){
                      break;
                   }
                   if((myInt1 > -1)&&(myInt1 < 101)){
                      presetlistHead->next->paramSustain = myInt1;
                   }
                }
                else if ((keywords[3] == '\n')&&(keywords[2] == 'E')&&(keywords[1] == 'R')&&(keywords[0] == 'E')){
                   if (fscanf(pFile, "%d", &myInt1) == EOF){
                      break;
                   }
                   if((myInt1 > 99)&&(myInt1 < 601)){
                      presetlistHead->next->paramRelease = myInt1;
                   }
                }
            }
            fclose(pFile);
            }
//MessageBoxW(NULL, L"plingplong", L"Success!", MB_OK);
           
            //add stuff in preset-linked-list to the displayd-on-screen list.
            //pLoopPointer = presetlistHead;
            //while(pLoopPointer->next->next != pLoopPointer->next){
            //   SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)pLoopPointer->next->name);
            //   pLoopPointer = pLoopPointer->next;
            //}

            CreateWindowW(L"Button", L"Store", WS_CHILD | WS_VISIBLE | SS_LEFT,
                         720, 175, 75, 25, hwnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);

            CreateWindowW(L"Button", L"MIDI", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          795, 175, 75, 25, hwnd, (HMENU)ID_BUTTON_MIDIIN, NULL, NULL);

            CreateWindowW(L"Button", L"Sequence", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          720, 200, 75, 25, hwnd, (HMENU)ID_BUTTON_SEQUENCE, NULL, NULL);

            CreateWindowW(L"Button", L"Reverb", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          795, 200, 75, 25, hwnd, (HMENU)ID_BUTTON_REVERB, NULL, NULL);

            CreateWindowW(L"Button", L"Vibrato", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          795, 390, 75, 25, hwnd, (HMENU)ID_BUTTON_VIBRATO, NULL, NULL);

            CreateWindowW(L"Button", L"About", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          720, 390, 75, 25, hwnd, (HMENU)ID_BUTTON_ABOUT, NULL, NULL);


            hwndTrackLFOfreq = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      12+795, 250+40, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            hwndTrackLFOamp = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      12+795+30, 250+40, 30, 100, hwnd, 
                                      (HMENU) 3, NULL, NULL);


            SendMessageW(hwndTrackLFOfreq, TBM_SETRANGE,  FALSE, MAKELONG(0, 200)); 
            SendMessageW(hwndTrackLFOamp, TBM_SETRANGE,  FALSE, MAKELONG(0, 80)); 
            SendMessageW(hwndTrackLFOfreq, TBM_SETPOS, TRUE, 200-(int)(10*paramLFOfreq)); 
            SendMessageW(hwndTrackLFOamp, TBM_SETPOS, TRUE, 80-(int)(10*paramLFOamp)); 

	    break;

        case MM_WOM_DONE:
            if (currentBuffer == 1){
                fillBuffer(buffer1);
                currentBuffer = 2;
                waveOutWrite(hWaveOut, &header1, sizeof(WAVEHDR));
            }
            else if (currentBuffer == 2){
                fillBuffer(buffer2);
                currentBuffer=1;
                waveOutWrite(hWaveOut, &header2, sizeof(WAVEHDR));
            }

            break;

        case MM_MIM_DATA:
            tone = (int)HIBYTE(LOWORD(lParam))-9;                                    //input tone

            if (LOBYTE(LOWORD(lParam)) == 0x90) {                                    //MIDI key down
                soundlistInsertAfter(getFrequency(tone), 0.0, tone, soundlistHead);  
            }
            else if(LOBYTE(LOWORD(lParam)) == 0x80){                                 //MIDI key up

                sLoopPointer = soundlistHead;
                while(sLoopPointer->next->next != sLoopPointer->next){
                    if (sLoopPointer->next->id == tone){
                        //soundlistDeleteNext(sLoopPointer); //if not using release
                        sLoopPointer->next->releasedTrue = 1;
                        break;
                    }
                    sLoopPointer = sLoopPointer->next;
                } 

            }

            break;
        case WM_MOUSEMOVE:
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);

            break;

        case WM_HSCROLL:

            if (state == 0){
                sinOsc[0] = 0.001*SendMessageW(hwndTrack1, TBM_GETPOS,  0, 0); 
                sinOsc[1] = 0.001*SendMessageW(hwndTrack2, TBM_GETPOS,  0, 0); 
                sinOsc[2] = 0.001*SendMessageW(hwndTrack3, TBM_GETPOS,  0, 0); 
                sinOsc[3] = 0.001*SendMessageW(hwndTrack4, TBM_GETPOS,  0, 0); 
                sinOsc[4] = 0.001*SendMessageW(hwndTrack5, TBM_GETPOS,  0, 0); 
                sinOsc[5] = 0.001*SendMessageW(hwndTrack6, TBM_GETPOS,  0, 0); 
                sinOsc[6] = 0.001*SendMessageW(hwndTrack7, TBM_GETPOS,  0, 0); 
                sinOsc[7] = 0.001*SendMessageW(hwndTrack8, TBM_GETPOS,  0, 0); 
                sinOsc[8] = 0.001*SendMessageW(hwndTrack9, TBM_GETPOS,  0, 0); 
                sinOsc[9] = 0.001*SendMessageW(hwndTrack10, TBM_GETPOS,  0, 0); 
            }
            else if (state == 1){
                sinOsc[10] = 0.001*SendMessageW(hwndTrack1, TBM_GETPOS,  0, 0); 
                sinOsc[11] = 0.001*SendMessageW(hwndTrack2, TBM_GETPOS,  0, 0); 
                sinOsc[12] = 0.001*SendMessageW(hwndTrack3, TBM_GETPOS,  0, 0); 
                sinOsc[13] = 0.001*SendMessageW(hwndTrack4, TBM_GETPOS,  0, 0); 
                sinOsc[14] = 0.001*SendMessageW(hwndTrack5, TBM_GETPOS,  0, 0); 
                sinOsc[15] = 0.001*SendMessageW(hwndTrack6, TBM_GETPOS,  0, 0); 
                sinOsc[16] = 0.001*SendMessageW(hwndTrack7, TBM_GETPOS,  0, 0); 
                sinOsc[17] = 0.001*SendMessageW(hwndTrack8, TBM_GETPOS,  0, 0); 
                sinOsc[18] = 0.001*SendMessageW(hwndTrack9, TBM_GETPOS,  0, 0); 
                sinOsc[19] = 0.001*SendMessageW(hwndTrack10, TBM_GETPOS,  0, 0); 
            }
            else if (state == 2){
                sinOsc[20] = 0.001*SendMessageW(hwndTrack1, TBM_GETPOS,  0, 0); 
                sinOsc[21] = 0.001*SendMessageW(hwndTrack2, TBM_GETPOS,  0, 0); 
                sinOsc[22] = 0.001*SendMessageW(hwndTrack3, TBM_GETPOS,  0, 0); 
                sinOsc[23] = 0.001*SendMessageW(hwndTrack4, TBM_GETPOS,  0, 0); 
                sinOsc[24] = 0.001*SendMessageW(hwndTrack5, TBM_GETPOS,  0, 0); 
                sinOsc[25] = 0.001*SendMessageW(hwndTrack6, TBM_GETPOS,  0, 0); 
                sinOsc[26] = 0.001*SendMessageW(hwndTrack7, TBM_GETPOS,  0, 0); 
                sinOsc[27] = 0.001*SendMessageW(hwndTrack8, TBM_GETPOS,  0, 0); 
                sinOsc[28] = 0.001*SendMessageW(hwndTrack9, TBM_GETPOS,  0, 0); 
                sinOsc[29] = 0.001*SendMessageW(hwndTrack10, TBM_GETPOS,  0, 0); 
            }

            fillWaveTable(0);
            if (LOWORD(wParam) == SB_THUMBPOSITION){
                InvalidateRect(hwnd, &waveDisplay, TRUE);
            }

            break;

        case WM_VSCROLL:
            if ((mouseX>100)&&(mouseX<750)){
                paramAttack = 100+500-SendMessageW(hwndTrackAttack, TBM_GETPOS,  0, 0); 
                paramDecay = 100+500-SendMessageW(hwndTrackDecay, TBM_GETPOS,  0, 0); 
                paramSustain = 100-SendMessageW(hwndTrackSustain, TBM_GETPOS,  0, 0); 
                paramRelease = 100+500-SendMessageW(hwndTrackRelease, TBM_GETPOS,  0, 0); 
                if (LOWORD(wParam) == SB_THUMBPOSITION){
                    InvalidateRect(hwnd, &adsrDisplay, TRUE);
                }
            }
            else{
                volume = 5000-SendMessageW(hwndTrackVolume, TBM_GETPOS,  0, 0);
                paramLFOfreq = 20.0 - 0.1*SendMessageW(hwndTrackLFOfreq, TBM_GETPOS,  0, 0);
                paramLFOamp = 8.0 - 0.1*SendMessageW(hwndTrackLFOamp, TBM_GETPOS,  0, 0);
            } 
            break;

        case WM_COMMAND:
            if (HIWORD(wParam) == CBN_SELCHANGE){
                i = SendMessage(hWndComboBox1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                if(i==0){
                    fillWaveTable(SINE_WAVE);
                }
                else if(i==1){
                    fillWaveTable(SQUARE_WAVE);
                }
                else if(i==2){
                    fillWaveTable(SAWTOOTH_WAVE);
                }
                else if(i==3){
                    fillWaveTable(N2_WAVE);
                }
                else if(i==4){
                    fillWaveTable(N3_WAVE);
                }
                else if(i==5){
                    fillWaveTable(ORGAN_WAVE);
                }
                else if(i==6){
                    fillWaveTable(N05_WAVE);
                }
                else if(i==7){
                    fillWaveTable(STAIRS_3STEPS);
                }
                InvalidateRect(hwnd, &waveDisplay, TRUE);
                SendMessageW(hwnd, WM_COMMAND, MAKEWPARAM(state+1,0),0); //redraw trackbars
            }
            else if (LOWORD(wParam) == 1){
                SendMessageW(hwndButton1, BM_SETCHECK , BST_CHECKED, 0);
                SendMessageW(hwndButton2, BM_SETCHECK , BST_UNCHECKED, 0);
                SendMessageW(hwndButton3, BM_SETCHECK , BST_UNCHECKED, 0);

                SendMessageW(hwndTrack1, TBM_SETPOS, TRUE, (int)(sinOsc[0]*1000));
                SendMessageW(hwndTrack2, TBM_SETPOS, TRUE, (int)(sinOsc[1]*1000));
                SendMessageW(hwndTrack3, TBM_SETPOS, TRUE, (int)(sinOsc[2]*1000));
                SendMessageW(hwndTrack4, TBM_SETPOS, TRUE, (int)(sinOsc[3]*1000));
                SendMessageW(hwndTrack5, TBM_SETPOS, TRUE, (int)(sinOsc[4]*1000));
                SendMessageW(hwndTrack6, TBM_SETPOS, TRUE, (int)(sinOsc[5]*1000));
                SendMessageW(hwndTrack7, TBM_SETPOS, TRUE, (int)(sinOsc[6]*1000));
                SendMessageW(hwndTrack8, TBM_SETPOS, TRUE, (int)(sinOsc[7]*1000));
                SendMessageW(hwndTrack9, TBM_SETPOS, TRUE, (int)(sinOsc[8]*1000));
                SendMessageW(hwndTrack10, TBM_SETPOS, TRUE, (int)(sinOsc[9]*1000));

                state = 0;
            }
            else if(LOWORD(wParam) == 2){
                SendMessageW(hwndButton1, BM_SETCHECK , BST_UNCHECKED, 0);
                SendMessageW(hwndButton2, BM_SETCHECK , BST_CHECKED, 0);
                SendMessageW(hwndButton3, BM_SETCHECK , BST_UNCHECKED, 0);

                SendMessageW(hwndTrack1, TBM_SETPOS, TRUE, (int)(sinOsc[10]*1000));
                SendMessageW(hwndTrack2, TBM_SETPOS, TRUE, (int)(sinOsc[11]*1000));
                SendMessageW(hwndTrack3, TBM_SETPOS, TRUE, (int)(sinOsc[12]*1000));
                SendMessageW(hwndTrack4, TBM_SETPOS, TRUE, (int)(sinOsc[13]*1000));
                SendMessageW(hwndTrack5, TBM_SETPOS, TRUE, (int)(sinOsc[14]*1000));
                SendMessageW(hwndTrack6, TBM_SETPOS, TRUE, (int)(sinOsc[15]*1000));
                SendMessageW(hwndTrack7, TBM_SETPOS, TRUE, (int)(sinOsc[16]*1000));
                SendMessageW(hwndTrack8, TBM_SETPOS, TRUE, (int)(sinOsc[17]*1000));
                SendMessageW(hwndTrack9, TBM_SETPOS, TRUE, (int)(sinOsc[18]*1000));
                SendMessageW(hwndTrack10, TBM_SETPOS, TRUE, (int)(sinOsc[19]*1000));
 
                state = 1;
            }
            else if(LOWORD(wParam) == 3){
                SendMessageW(hwndButton1, BM_SETCHECK , BST_UNCHECKED, 0);
                SendMessageW(hwndButton2, BM_SETCHECK , BST_UNCHECKED, 0);
                SendMessageW(hwndButton3, BM_SETCHECK , BST_CHECKED, 0);

                SendMessageW(hwndTrack1, TBM_SETPOS, TRUE, (int)(sinOsc[20]*1000));
                SendMessageW(hwndTrack2, TBM_SETPOS, TRUE, (int)(sinOsc[21]*1000));
                SendMessageW(hwndTrack3, TBM_SETPOS, TRUE, (int)(sinOsc[22]*1000));
                SendMessageW(hwndTrack4, TBM_SETPOS, TRUE, (int)(sinOsc[23]*1000));
                SendMessageW(hwndTrack5, TBM_SETPOS, TRUE, (int)(sinOsc[24]*1000));
                SendMessageW(hwndTrack6, TBM_SETPOS, TRUE, (int)(sinOsc[25]*1000));
                SendMessageW(hwndTrack7, TBM_SETPOS, TRUE, (int)(sinOsc[26]*1000));
                SendMessageW(hwndTrack8, TBM_SETPOS, TRUE, (int)(sinOsc[27]*1000));
                SendMessageW(hwndTrack9, TBM_SETPOS, TRUE, (int)(sinOsc[28]*1000));
                SendMessageW(hwndTrack10, TBM_SETPOS, TRUE, (int)(sinOsc[29]*1000));
 
                state = 2;
            }

            if (LOWORD(wParam) == IDC_LIST) {        
                if (HIWORD(wParam) == LBN_SELCHANGE) {                   
                   sel = (int) SendMessageW(hwndList, LB_GETCURSEL, 0, 0);

                   pLoopPointer = presetlistHead;
                   while(pLoopPointer->next->next != pLoopPointer->next){
                      if(sel == pLoopPointer->next->id){
                          for(i = 0; i<30; i++){
                              sinOsc[i] = 0.001*(pLoopPointer->next->sinOsc[i]);
                          }
                          paramAttack = pLoopPointer->next->paramAttack;
                          paramDecay = pLoopPointer->next->paramDecay;
                          paramSustain = pLoopPointer->next->paramSustain;
                          paramRelease = pLoopPointer->next->paramRelease;

                          //if (pLoopPointer->next->id == 2){
                          //  MessageBoxW(NULL, L"plingplong", L"Success!", MB_OK);
                          //}
                          break;
                      }
                      pLoopPointer = pLoopPointer->next;
                   }
                   fillWaveTable(0);						 //update sound
                   InvalidateRect(hwnd, &waveDisplay, TRUE); 			//redraw wave display
                   SendMessageW(hwnd, WM_COMMAND, MAKEWPARAM(state+1,0),0);	 //redraw sine trackbars
                   InvalidateRect(hwnd, &adsrDisplay, TRUE); 			//redraw adsr display
                   SendMessageW(hwndTrackAttack, TBM_SETPOS, TRUE, 600-paramAttack);
                   SendMessageW(hwndTrackDecay, TBM_SETPOS, TRUE, 600-paramDecay);
                   SendMessageW(hwndTrackSustain, TBM_SETPOS, TRUE, 100-paramSustain);
                   SendMessageW(hwndTrackRelease, TBM_SETPOS, TRUE, 600-paramRelease);

                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_SAVE){  
                if (saveWindowActive<1){
                    wc2.style         = CS_HREDRAW | CS_VREDRAW;
                    wc2.cbClsExtra    = 0;
                    wc2.cbWndExtra    = 0;
                    wc2.lpszClassName = L"PyrrosSynthExport";
                    wc2.hInstance     = thisInstance;
                    wc2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
                    wc2.lpszMenuName  = NULL;
                    wc2.lpfnWndProc   = WndSaveProc;
                    wc2.hCursor       = LoadCursor(NULL, IDC_ARROW);
                    wc2.hIcon         = CreateIcon(NULL, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon); //LoadIcon(NULL, IDI_APPLICATION);
                    RegisterClassW(&wc2);

                    GetCursorPos(&currentPos); //get (absolute) mouse coordinates
                    hwnd2 = CreateWindowW(wc2.lpszClassName, L"Enter Name",
                                WS_SYSMENU | WS_VISIBLE, 
                                currentPos.x-75, currentPos.y-50, 150, 100, NULL, NULL, thisInstance, NULL);  
//                                  WS_OVERLAPPEDWINDOW^ WS_THICKFRAME ^WS_MAXIMIZEBOX ^WS_MINIMIZEBOX ^WS_OVERLAPPED 

                    ShowWindow(hwnd2, SW_SHOW);
                    UpdateWindow(hwnd2);
                    saveWindowActive = 1;
                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_MIDIIN){  
                if ((midiWindowActive<1)&&(midiStarted < 1)){
                    wc3.style         = CS_HREDRAW | CS_VREDRAW;
                    wc3.cbClsExtra    = 0;
                    wc3.cbWndExtra    = 0;
                    wc3.lpszClassName = L"midiWindow";
                    wc3.hInstance     = thisInstance;
                    wc3.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
                    wc3.lpszMenuName  = NULL;
                    wc3.lpfnWndProc   = WndMIDIProc;
                    wc3.hCursor       = LoadCursor(NULL, IDC_ARROW);
                    wc3.hIcon         = CreateIcon(NULL, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon); 
                    RegisterClassW(&wc3);

                    GetCursorPos(&currentPos); //get (absolute) mouse coordinates
                    hwnd3 = CreateWindowW(wc3.lpszClassName, L"Select MIDI device",
                                WS_SYSMENU | WS_VISIBLE, 
                                currentPos.x-75, currentPos.y-50, 200, 300, NULL, NULL, thisInstance, NULL);  
//                                  WS_OVERLAPPEDWINDOW^ WS_THICKFRAME ^WS_MAXIMIZEBOX ^WS_MINIMIZEBOX ^WS_OVERLAPPED 

                    ShowWindow(hwnd3, SW_SHOW);
                    UpdateWindow(hwnd3);
                    midiWindowActive = 1;
                }
                else{
                    MessageBoxW(NULL, L"A MIDI IN device is already active. Restart the program to change MIDI IN device.", L"Error", MB_OK);
                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_SEQUENCE){  
                if (seqWindowActive<1){
                    wc4.style         = CS_HREDRAW | CS_VREDRAW;
                    wc4.cbClsExtra    = 0;
                    wc4.cbWndExtra    = 0;
                    wc4.lpszClassName = L"theSequencer";
                    wc4.hInstance     = thisInstance;
                    wc4.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
                    wc4.lpszMenuName  = NULL;
                    wc4.lpfnWndProc   = WndSeqProc;
                    wc4.hCursor       = LoadCursor(NULL, IDC_ARROW);
                    wc4.hIcon         = CreateIcon(NULL, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon); 
                    RegisterClassW(&wc4);

                    GetCursorPos(&currentPos); //get (absolute) mouse coordinates
                    hwnd4 = CreateWindowW(wc4.lpszClassName, L"Sequencer",
                                WS_SYSMENU | WS_VISIBLE, 
                                currentPos.x-75, currentPos.y-50, 700, 400, NULL, NULL, thisInstance, NULL);  

                    ShowWindow(hwnd4, SW_SHOW);
                    UpdateWindow(hwnd4);
                    seqWindowActive = 1;
                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_REVERB){  
                //echoActive = (echoActive+1)%2;
                if (revWindowActive<1){
                    wc5.style         = CS_HREDRAW | CS_VREDRAW;
                    wc5.cbClsExtra    = 0;
                    wc5.cbWndExtra    = 0;
                    wc5.lpszClassName = L"theReverb";
                    wc5.hInstance     = thisInstance;
                    wc5.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
                    wc5.lpszMenuName  = NULL;
                    wc5.lpfnWndProc   = WndRevProc;
                    wc5.hCursor       = LoadCursor(NULL, IDC_ARROW);
                    wc5.hIcon         = CreateIcon(NULL, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon); 
                    RegisterClassW(&wc5);

                    GetCursorPos(&currentPos); //get (absolute) mouse coordinates
                    hwnd5 = CreateWindowW(wc5.lpszClassName, L"Reverb",
                                WS_SYSMENU | WS_VISIBLE, 
                                currentPos.x-75, currentPos.y-50, REV_WINDOWSIZE_X, REV_WINDOWSIZE_Y, NULL, NULL, thisInstance, NULL);  

                    ShowWindow(hwnd5, SW_SHOW);
                    UpdateWindow(hwnd5);
                    revWindowActive = 1;
                }

            }
            else if (LOWORD(wParam) == ID_BUTTON_VIBRATO){  
                vibratoOn = (vibratoOn+1)%2;
            }
            else if (LOWORD(wParam) == ID_BUTTON_ABOUT){
                MessageBoxW(NULL, L"Additive synthesizer.\n\nCreated in MMXXIV.\n\n", L"About", MB_OK);
            }

            break;
        case WM_PAINT:
            theGraphics(hwnd);
            break;
        case WM_DESTROY:
            waveOutUnprepareHeader(hWaveOut, &header1, sizeof(WAVEHDR));
            waveOutUnprepareHeader(hWaveOut, &header2, sizeof(WAVEHDR));
            waveOutClose(hWaveOut);

            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void fillBuffer(__int16 buffer[]){
    double thisPhaseAngle = 0.0;
    double thisFrequency = 0.0;

    double thisAttack = 0.0;
    double thisDecay = 0.0;
    double thisRelease = 0.0;

    int i = 0;
    int j = 0;
    int k = 0;

    int temp = 0;

    double tempRnd1 = 0.0;
    double tempRnd2 = 0.0;

    double tempEcho = 0.0;
    double outEcho = 0.0;
    int depthEcho = 2;
    int currentDepth = 0;
    double echoGen[20];


    double feedbacks[10];

    double genKeeps[10];


    double currentFeedback[10];
    double currentBounce[10];

    static int firstTime = 1;

    double thisLfoPhase = 0.0;

    for (i = 0; i < BUFFERSIZE; i++){
       buffer[i] = 0;
    }


    for (i = 0; i < BUFFERSIZE; i++){
       if (playOn){
            smallTicker += 1;
            if (smallTicker == (int)(44100*60/((double)bpm*beatres))){

                plLoopPointer = playlistHead;
                while(plLoopPointer->next != plLoopPointer){
                    if (plLoopPointer->timeMark == ticker){                                                                  
                       //soundFactoryModify(loopPointer->tone, SF_TURN_ON, &theFactory);
                       soundlistInsertAfter(getFrequency(plLoopPointer->tone), 0.0, plLoopPointer->tone, soundlistHead); 
                    }
                    else if(plLoopPointer->timeMark + plLoopPointer->length == ticker){
                       //soundFactoryModify(loopPointer->tone, SF_TURN_OFF, &theFactory);
                        sLoopPointer = soundlistHead;
                        while(sLoopPointer->next->next != sLoopPointer->next){
                            if (sLoopPointer->next->id == plLoopPointer->tone){
                                //soundlistDeleteNext(sLoopPointer); //if not using release
                                sLoopPointer->next->releasedTrue = 1;
                                break;
                            }
                            sLoopPointer = sLoopPointer->next;
                        } 
                    } 
                    plLoopPointer = plLoopPointer->next;
                }

                ticker += 1;

                smallTicker = 0;
                if (ticker == (4+timeShiftMax)*16){ //64
                    ticker = 0;
                    sLoopPointer = soundlistHead;
                    while(sLoopPointer->next->next != sLoopPointer->next){
                        sLoopPointer->next->releasedTrue = 1;
                        sLoopPointer = sLoopPointer->next;
                    }
                }
            }
       }

       sLoopPointer = soundlistHead;
       while(sLoopPointer->next->next != sLoopPointer->next){
          thisFrequency = sLoopPointer->next->frequency;
          thisPhaseAngle = sLoopPointer->next->phaseAngle;
          thisAttack = sLoopPointer->next->attack;
          thisDecay = sLoopPointer->next->decay;
          thisRelease = sLoopPointer->next->release;
          if(vibratoOn>0){
              thisLfoPhase = sLoopPointer->next->lfoPhase;
          }
          else{
              thisLfoPhase = 0.0;
          }


           //buffer[i] += 3000*sin(phaseAngle);
           buffer[i] += volume*envelopeADSR(thisAttack, thisDecay, thisRelease)*wavTable[(int)(thisPhaseAngle*10000/(2*PI))];


           thisPhaseAngle += 2*PI*(thisFrequency+paramLFOamp*sin(thisLfoPhase))/(44100);
           if (thisPhaseAngle > (2*PI)){
               thisPhaseAngle -= 2*PI;
           }
           if (vibratoOn>0){
              thisLfoPhase += 2*PI*paramLFOfreq/(44100);
              if (thisLfoPhase > (2*PI)){
                  thisLfoPhase -= 2*PI;
              }
           }


            if (thisAttack<1){
                thisAttack += 1/pow(10,0.01*paramAttack);
                if (thisAttack>1){
                    thisAttack = 1.0;
                }
            }
            else if ((thisDecay>0.01*paramSustain)&&(sLoopPointer->next->releasedTrue == 0)){
                thisDecay -= 1/pow(10,0.01*paramDecay);
                if (thisDecay < 0.01*paramSustain){
                    thisDecay = 0.01*paramSustain;
                }
            }

            if (sLoopPointer->next->releasedTrue > 0){
                thisRelease -= 1.0/pow(10,0.01*paramRelease);
                if (thisRelease < 0){
                    thisRelease = 0.0;
                }
            }
       

          sLoopPointer->next->attack = thisAttack;
          sLoopPointer->next->decay = thisDecay;
          sLoopPointer->next->release = thisRelease;


          sLoopPointer->next->phaseAngle = thisPhaseAngle;
          sLoopPointer->next->lfoPhase = thisLfoPhase;

          if (thisRelease < 0.0001){
              soundlistDeleteNext(sLoopPointer);
          }
          else{
              sLoopPointer = sLoopPointer->next;
          }
       }



       if (firstTime>0){
           delayInit(delayMult*1, &delay1);
           delayInit(delayMult*2, &delay2);
           delayInit(delayMult*3, &delay3);
           delayInit(delayMult*4, &delay4);
           delayInit(delayMult*5, &delay5);
           delayInit(delayMult*6, &delay6);
           delayInit(delayMult*7, &delay7);
           delayInit(delayMult*8, &delay8);
           delayInit(delayMult*9, &delay9);
           delayInit(delayMult*10, &delay10);

           firstTime = 0;
       }

       if(echoActive>0){


/*
           bounce[0] = 2*0.02;
           bounce[1] = 2*0.04;
           bounce[2] = 2*0.06;
           bounce[3] = 2*0.08;
           bounce[4] = 2*0.10;
           bounce[5] = 2*0.12;
           bounce[6] = 2*0.14;
           bounce[7] = 2*0.10;
           bounce[8] = 2*0.06;
           bounce[9] = 2*0.02;
*/
           echoVar = 0.0;
           tempEcho = 0.0;
           outEcho = 0.0;
           depthEcho = 8;



/*
           bounce[0] = 0.10;
           bounce[1] = 0.20;
           bounce[2] = 0.30;
           bounce[3] = 0.20;
           bounce[4] = 0.10;
           bounce[5] = 0.0;
           bounce[6] = 0.0;
           bounce[7] = 0.0;
           bounce[8] = 0.0;
           bounce[9] = 0.0;
*/

           genKeeps[0] = 0.0;
           genKeeps[1] = 0.0;
           genKeeps[2] = 0.0;
           genKeeps[3] = 0.0;
           genKeeps[4] = 0.0;
           genKeeps[5] = 0.0;
           genKeeps[6] = 0.0;
           genKeeps[7] = 0.0;
           genKeeps[8] = 0.0;
           genKeeps[9] = 0.0;




           echoVar += (exp(bounce[0])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay1);
           echoVar += (exp(bounce[1])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay2);
           echoVar += (exp(bounce[2])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay3);
           echoVar += (exp(bounce[3])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay4);
           echoVar += (exp(bounce[4])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay5);
           echoVar += (exp(bounce[5])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay6);
           echoVar += (exp(bounce[6])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay7);
           echoVar += (exp(bounce[7])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay8);
           echoVar += (exp(bounce[8])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay9);
           echoVar += (exp(bounce[9])-1)/(exp(1)-1)*delayProcess(buffer[i], &delay10);

           outEcho += genOut[0]*echoVar;

           
           for(j = 0; j < depthEcho; j++){
               //echoGen[j] = echoVar;

               tempEcho = 0.0;


               //for(k=0; k<j+1; k++){
               //    tempEcho += echoGen[j-k]*genKeeps[k];
               //}        


               tempEcho += (exp(bounce[10])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay1);
               tempEcho += (exp(bounce[11])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay2);
               tempEcho += (exp(bounce[12])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay3);
               tempEcho += (exp(bounce[13])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay4);
               tempEcho += (exp(bounce[14])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay5);
               tempEcho += (exp(bounce[15])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay6);
               tempEcho += (exp(bounce[16])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay7);
               tempEcho += (exp(bounce[17])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay8);
               tempEcho += (exp(bounce[18])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay9);
               tempEcho += (exp(bounce[19])-1)/(exp(1)-1)*delayProcess(feedbackMix*echoVar, &delay10);

               outEcho += genOut[j+1]*tempEcho;
               
               echoVar = tempEcho;
           }

           buffer[i] += outEcho;
       }

    }

    if (firstTime>0){
        firstTime = 0;
    }

}

double getFrequency(int tone){
  float temp = 1.0;
  int tonfactor = 0;
  int k;
  double res = 0.0;

  tonfactor = tone/12;

  k = tone%12;

  if (k==0)
    temp = 1.0;
  else if (k==1)
    temp = 1.0595;
  else if (k==2)
    temp = 1.1225;
  else if (k==3)
    temp = 1.1892;
  else if (k==4)
    temp = 1.2599;
  else if (k==5)
    temp = 1.3348;
  else if (k==6)
    temp = 1.4142;
  else if (k==7)
    temp = 1.4983;
  else if (k==8)
    temp = 1.5874;
  else if (k==9)
    temp = 1.6818;
  else if (k==10)
    temp = 1.7818;
  else if (k==11)
    temp = 1.8877;
  else if (k==12)
    temp = 2.0;

  res = 55*temp*pow(2.0,tonfactor)*0.25;
  return res;
}

void fillWaveTable(int waveForm){  //resolution is 10000 data points for 1 period 
   int i = 0;
   int j = 0;
   double phaseAngle = 0.0;

   if(waveForm == SQUARE_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 1.0/(i+1);
           if((i+1)%2==0){
               sinOsc[i] = 0.0;
           }
       }
   }
   else if(waveForm == SAWTOOTH_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 1.0/(i+1);
       }
   }
   else if(waveForm == SINE_WAVE){
       sinOsc[0] = 1.0;
       for (i = 1; i<30; i++){
           sinOsc[i] = 0.0;
       }
   }
   else if(waveForm == N2_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 1.0/((i+1)*(i+1));
       }
   }
   else if(waveForm == N3_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 1.0/((i+1)*(i+1)*(i+1));
       }
   }
   else if(waveForm == ORGAN_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 0.0;
       }
       sinOsc[0] = 0.5;
       sinOsc[1] = 0.5;
       sinOsc[3] = 0.5;
       sinOsc[7] = 0.5;
       sinOsc[15] = 0.5;
   }
   else if(waveForm == N05_WAVE){
       for (i = 0; i<30; i++){
           sinOsc[i] = 1.0/(sqrt(i+1));
       }
   }
   else if(waveForm == STAIRS_3STEPS){
       for (i = 0; i<30; i++){
           if((i+1)%3==0){
               sinOsc[i] = 0.0;
           }
           else{
               sinOsc[i] = 1.0/(i+1);
           }
       }
   }



   for (i=0;i<10000;i++){
       wavTable[i] = 0.0;
       for(j = 0; j < 30; j++){
           wavTable[i] += sinOsc[j]*sin((j+1)*phaseAngle);
       }
       phaseAngle += 2*PI/10000.0;
   }
}

double envelopeADSR(double a, double d, double r){
    double retvalue;

    //retvalue = (exp(d)-1)/(exp(1)-1)*(exp(r)-1)/(exp(1)-1)*(exp(a)-1)/(exp(1)-1);

    retvalue = (exp(d)-1)/(exp(1)-1); //apply decayM
    retvalue = retvalue*(exp(a)-1)/(exp(1)-1); //apply attack
    retvalue = retvalue*(exp(r)-1)/(exp(1)-1); //apply release

    return retvalue;

}



void theGraphics(HWND hwnd){
    int i = 0;
    double wavTabMax = 0.0;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
//  HGDIOBJ original = NULL;
//  original = SelectObject(hdc,GetStockObject(DC_PEN));
//  SetDCPenColor(hdc, RGB(0,150,0));

    HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));

    HPEN hPenBlack = CreatePen(PS_SOLID, 1, RGB(0,0,0));

    HPEN hPenRed = CreatePen(PS_SOLID, 1, RGB(255,0,0));
    HPEN hPenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));


    HPEN hPenWhite = CreatePen(PS_SOLID, 1, RGB(255,255,255));
    HPEN hPenGrey = CreatePen(PS_SOLID, 1, RGB(128,128,128));
    HPEN hPenGreyDark = CreatePen(PS_SOLID, 1, RGB(64,64,64));
    HPEN hPenGreyBright = CreatePen(PS_SOLID, 1, RGB(192,192,192));

    HBRUSH hBrushYellow1 = CreateSolidBrush(RGB(255, 255, 255));
    HPEN hPenYellow1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));



    //Draw Waveform window---------------------------------------
    SelectObject(hdc, hPenYellow1);
    SelectObject(hdc, hBrushYellow1);

    Rectangle(hdc, waveDisplay.left, waveDisplay.top, waveDisplay.right, waveDisplay.bottom);



    wavTabMax = 0.0;
    for(i = 0; i<10000; i++){
        if (abs(wavTable[i]) > wavTabMax){
            wavTabMax = wavTable[i];
        }
    }
    oscSum = 0.0;
    for (i= 0; i<30; i++){
        oscSum += sinOsc[i];
    }

    SelectObject(hdc, hPenRed);

    MoveToEx(hdc, 400, 200-30, NULL);
    for(i = 0; i<10000; i++){
        LineTo(hdc, 400+(int)(i*300/10000), 200+(int)(100.0/oscSum*wavTable[i])-30);
    }
    LineTo(hdc,400+300, 200-30);


    //shadows
    SelectObject(hdc, hPenGreyDark);
    MoveToEx(hdc, waveDisplay.left-1, waveDisplay.bottom-1, NULL);
    LineTo(hdc, waveDisplay.left-1, waveDisplay.top-1);
    LineTo(hdc, waveDisplay.right, waveDisplay.top-1);

    SelectObject(hdc, hPenGrey);
    MoveToEx(hdc, waveDisplay.left-2, waveDisplay.bottom, NULL);
    LineTo(hdc, waveDisplay.left-2, waveDisplay.top-2);
    LineTo(hdc, waveDisplay.right+1, waveDisplay.top-2);

    SelectObject(hdc, hPenGreyBright);
    MoveToEx(hdc, waveDisplay.left-1, waveDisplay.bottom, NULL);
    LineTo(hdc, waveDisplay.right, waveDisplay.bottom);
    LineTo(hdc, waveDisplay.right, waveDisplay.top-2);

    SelectObject(hdc, hPenWhite);
    MoveToEx(hdc, waveDisplay.left-2, waveDisplay.bottom+1, NULL);
    LineTo(hdc, waveDisplay.right+1, waveDisplay.bottom+1);
    LineTo(hdc, waveDisplay.right+1, waveDisplay.top-3);


    //Draw ADSR-envelope window--------------------------------------
    SelectObject(hdc, hPenYellow1);
    SelectObject(hdc, hBrushYellow1);

    Rectangle(hdc, 540 + ADSR_TRANSL_X, 330 + ADSR_TRANSL_Y, 540+160 + ADSR_TRANSL_X, 330+90 + ADSR_TRANSL_Y);
    //R-trackbar: 410+3*30, 330, 30, 100

    SelectObject(hdc, hPenBlue);
    MoveToEx(hdc, ADSR_TRANSL_X + 540, ADSR_TRANSL_Y + 330+89, NULL);
    LineTo(hdc, ADSR_TRANSL_X + 540+(int)(1.06*(paramAttack-100)/10.0), ADSR_TRANSL_Y + 330);
    LineTo(hdc, ADSR_TRANSL_X + 540+(int)(1.06*(paramAttack-100)/10.0)+(int)(1.06*(paramDecay-100)/10.0), ADSR_TRANSL_Y + 330+90-(int)(0.9*paramSustain));
    LineTo(hdc, ADSR_TRANSL_X + 540+(int)(1.06*(paramAttack-100)/10.0)+(int)(1.06*(paramDecay-100)/10.0)+(int)(1.06*(paramRelease-100)/10.0), ADSR_TRANSL_Y + 330+90);

    MoveToEx(hdc, ADSR_TRANSL_X + 1+540, ADSR_TRANSL_Y + 330+89, NULL);
    LineTo(hdc, ADSR_TRANSL_X + 1+540+(int)(1.06*(paramAttack-100)/10.0), ADSR_TRANSL_Y + 330);
    LineTo(hdc, ADSR_TRANSL_X + 1+540+(int)(1.06*(paramAttack-100)/10.0)+(int)(1.06*(paramDecay-100)/10.0), ADSR_TRANSL_Y + 330+90-(int)(0.9*paramSustain));
    LineTo(hdc, ADSR_TRANSL_X + 1+540+(int)(1.06*(paramAttack-100)/10.0)+(int)(1.06*(paramDecay-100)/10.0)+(int)(1.06*(paramRelease-100)/10.0), ADSR_TRANSL_Y + 330+90);


    //shadows
    SelectObject(hdc, hPenGreyDark);
    MoveToEx(hdc, adsrDisplay.left-1, adsrDisplay.bottom-1, NULL);
    LineTo(hdc, adsrDisplay.left-1, adsrDisplay.top-1);
    LineTo(hdc, adsrDisplay.right, adsrDisplay.top-1);

    SelectObject(hdc, hPenGrey);
    MoveToEx(hdc, adsrDisplay.left-2, adsrDisplay.bottom, NULL);
    LineTo(hdc, adsrDisplay.left-2, adsrDisplay.top-2);
    LineTo(hdc, adsrDisplay.right+1, adsrDisplay.top-2);

    SelectObject(hdc, hPenGreyBright);
    MoveToEx(hdc, adsrDisplay.left-1, adsrDisplay.bottom, NULL);
    LineTo(hdc, adsrDisplay.right, adsrDisplay.bottom);
    LineTo(hdc, adsrDisplay.right, adsrDisplay.top-2);

    SelectObject(hdc, hPenWhite);
    MoveToEx(hdc, waveDisplay.left-2, adsrDisplay.bottom+1, NULL);
    LineTo(hdc, waveDisplay.right+1, adsrDisplay.bottom+1);
    LineTo(hdc, waveDisplay.right+1, adsrDisplay.top-3);






    //Black section lines-----------------------------------------
    //around adsr
    SelectObject(hdc, hPenBlack);
    MoveToEx(hdc, ADSR_TRANSL_X + 400, ADSR_TRANSL_Y + 320, NULL);
    LineTo(hdc, ADSR_TRANSL_X + 540+160+10, ADSR_TRANSL_Y + 320);
    LineTo(hdc, ADSR_TRANSL_X + 540+160+10, ADSR_TRANSL_Y + 320+110);
    LineTo(hdc, ADSR_TRANSL_X + 400, ADSR_TRANSL_Y + 320+110);
    LineTo(hdc, ADSR_TRANSL_X + 400, ADSR_TRANSL_Y + 320);

    //other
    SelectObject(hdc, hPenBlack);
    MoveToEx(hdc, 540+160+10, 70+200+10, NULL);
    LineTo(hdc, 540+160+10, 60);
    //LineTo(hdc, 375, 90);
    LineTo(hdc, 375, 60);
    LineTo(hdc, 200, 60);

    MoveToEx(hdc, 65, 60, NULL);
    LineTo(hdc, 55, 60);
    LineTo(hdc, 55, 370);
    LineTo(hdc, 375, 370);
    LineTo(hdc, 375, 70+200+10);
    LineTo(hdc, 540+160+10, 70+200+10);




    //Clean up---------------------------------------------------
    DeleteObject(hBrushBlack);
    DeleteObject(hPenBlack);
    DeleteObject(hPenRed);
    DeleteObject(hPenBlue);

    DeleteObject(hPenWhite);
    DeleteObject(hPenGrey);
    DeleteObject(hPenGreyBright);
    DeleteObject(hPenGreyDark);


    DeleteObject(hBrushYellow1);
    DeleteObject(hPenYellow1);


    EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WndSaveProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    WCHAR editInputBuffer[20]; 
    char editCharInputBuffer[20];
    WORD cchText;
    int textLength;

    static HWND hwndEditFileName;


    int nBeats;

    switch(uMsg){

        case WM_CREATE:
            hwndEditFileName = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT , 3, 5, 127, 20, 
                                      hwnd,(HMENU) ID_EDIT_FILENAME,(HINSTANCE) GetWindowLong(hwnd2, GWL_HINSTANCE), NULL); 
//| ES_AUTOVSCROLL

            //CreateWindowW(L"Static", L"File Name: ", WS_CHILD | WS_VISIBLE | SS_LEFT,
            //              10, 10, 100, 20, hwnd, (HMENU) 1, NULL, NULL);

            CreateWindowW(L"Button", L"Enter", WS_CHILD | WS_VISIBLE | SS_LEFT | BS_DEFPUSHBUTTON,
                          40, 30, 60, 25, hwnd, (HMENU)ID_BUTTON_EXPORTNOW, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_EXPORTNOW) {
               //cchText = (WORD)SendDlgItemMessage(hwnd2, ID_EDIT_FILENAME,EM_LINELENGTH,(WPARAM)0,(LPARAM)0);
               //*((LPWORD)editInputBuffer) = cchText;
               //SendDlgItemMessage(hwnd2, ID_EDIT_FILENAME, EM_GETLINE, (WPARAM)0, (LPARAM)editInputBuffer);
               //editInputBuffer[cchText] = 0;


                cchText = (WORD)SendDlgItemMessage(hwnd2, ID_EDIT_FILENAME,EM_LINELENGTH,(WPARAM)0,(LPARAM)0);
                wstring = malloc(cchText * sizeof(wchar_t));
                *((LPWORD)wstring) = cchText;
                SendDlgItemMessage(hwnd2, ID_EDIT_FILENAME, EM_GETLINE, (WPARAM)0, (LPARAM)wstring);
                wstring[cchText] = 0;


                presetInsertAfter(wstring, nPresets, presetlistHead);
                presetlistHead->next->id = nPresets;                
                nPresets++;
                for(i = 0; i<30; i++){
                    presetlistHead->next->sinOsc[i] = (int)(1000*sinOsc[i]);
                }
                presetlistHead->next->paramAttack = paramAttack;
                presetlistHead->next->paramDecay = paramDecay;
                presetlistHead->next->paramSustain = paramSustain;
                presetlistHead->next->paramRelease = paramRelease;
                pFile = fopen("presets.txt", "a");
                fprintf(pFile, "\nNEW %ws\n", wstring);
                for(i = 0; i<30; i++){
                    if(presetlistHead->next->sinOsc[i] > 0){
                       fprintf(pFile, "SIN %d %d\n", i+1, presetlistHead->next->sinOsc[i]);
                    }
                }
                fprintf(pFile, "EAT %d\n", paramAttack);
                fprintf(pFile, "EDE %d\n", paramDecay);
                fprintf(pFile, "ESU %d\n", paramSustain);
                fprintf(pFile, "ERE %d\n", paramRelease);

                fclose(pFile);
                //free(wstring);

                SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)(presetlistHead->next->name));

                ShowWindow(hwnd2, SW_HIDE);
                UpdateWindow(hwnd2);
                saveWindowActive = 0;
            }
            break;

        case WM_DESTROY:
                saveWindowActive = 0;
            break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}



LRESULT CALLBACK WndMIDIProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndList2;
    MIDIINCAPS theInfo;

    switch(msg) {
        case WM_CREATE:
            hwndList2 = CreateWindowW(WC_LISTBOXW , NULL, WS_CHILD 
                | WS_VISIBLE | LBS_NOTIFY |  WS_VSCROLL |  WS_BORDER,
                0, 0, 185, 250, hwnd, 
                (HMENU) IDC_LIST2, NULL, NULL);

            //SendMessageW(hwndList2, LB_ADDSTRING, 0, (LPARAM)L"Turboman");


            activeMIDIDevices = midiInGetNumDevs();
            for (i = 0; i < activeMIDIDevices; i++){
                midiInGetDevCaps(i, &theInfo, sizeof(MIDIINCAPS));
                SendMessageW(hwndList2, LB_ADDSTRING, 0, (LPARAM)theInfo.szPname);
            }
            CreateWindowW(L"Button", L"Select", WS_CHILD | WS_VISIBLE | SS_LEFT | BS_DEFPUSHBUTTON,
                          0, 240, 92, 22, hwnd, (HMENU)ID_BUTTON_MIDISELECT, NULL, NULL);

            CreateWindowW(L"Button", L"Cancel", WS_CHILD | WS_VISIBLE | SS_LEFT | BS_DEFPUSHBUTTON,
                          92, 240, 92, 22, hwnd, (HMENU)ID_BUTTON_MIDICANCEL, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_MIDICANCEL){
                midiWindowActive = 0;
                ShowWindow(hwnd3, SW_HIDE);
                UpdateWindow(hwnd3);
            } 
            else if(LOWORD(wParam) == ID_BUTTON_MIDISELECT){
                if (activeMIDIDevices > 0){
                    i = SendMessageW(hwndList2, LB_GETCURSEL, 0, 0);
                    if(i == LB_ERR){
                        MessageBoxW(NULL, L"No MIDI IN device selected.", L"Error", MB_OK);
                    }
                    else{
                        midiDeviceNumber = i;
                        doStartMIDI = 1;
                        midiWindowActive = 0;
                        ShowWindow(hwnd3, SW_HIDE);
                        UpdateWindow(hwnd3);
                    }
                }
                else{
                    MessageBoxW(NULL, L"No active MIDI IN devices found.", L"Error", MB_OK);
                }
            }
            break;
        case WM_DESTROY:
            midiWindowActive = 0;
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}  



LRESULT CALLBACK WndSeqProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    WCHAR editInputBuffer[20]; 
    char editCharInputBuffer[20];
    WORD cchText;
    int textLength;

    WCHAR buffer[20]; 

    static HWND hwndEditFileName;
    static HWND labelToneLength;
    static HWND labelBPM;
    static HWND labelTimeShiftMax;
    static HWND labelTimeShift;
    static HWND labelTimeShift2;

    RECT r;
    RECT area;

    int nBeats;

    switch(uMsg){

        case WM_CREATE:
            SetTimer(hwnd, ID_TIMER1,(int)(BUFFERSIZE*1000/(44100.0)), (TIMERPROC)NULL);

            CreateWindowW(L"Button", L"Play", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          40, 335, 60, 20, hwnd, (HMENU)ID_BUTTON_PLAY, NULL, NULL);


            CreateWindowW(L"Button", L"-", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          100, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TONELENGTH_DEC, NULL, NULL);
            CreateWindowW(L"Button", L"+", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          140, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TONELENGTH_INC, NULL, NULL);

            labelToneLength = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
                  120, 335, 20, 20, hwnd, (HMENU) 1, NULL, NULL);
            _itow(toneLength, buffer,10);
            SetWindowTextW(labelToneLength, buffer);



            CreateWindowW(L"Button", L"-", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          160, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_BPM_DEC, NULL, NULL);
            CreateWindowW(L"Button", L"+", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          220, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_BPM_INC, NULL, NULL);

            labelBPM = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
                  180, 335, 40, 20, hwnd, (HMENU) 1, NULL, NULL);
            _itow(bpm, buffer,10);
            SetWindowTextW(labelBPM, buffer);

            CreateWindowW(L"Button", L"\\/", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          655, 300, 20, 20, hwnd, (HMENU)ID_BUTTON_TONE_INC, NULL, NULL);
            CreateWindowW(L"Button", L"/\\", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          655, 280, 20, 20, hwnd, (HMENU)ID_BUTTON_TONE_DEC, NULL, NULL);

            CreateWindowW(L"Button", L">", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          630, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TIME_INC, NULL, NULL);
            CreateWindowW(L"Button", L"<", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          610, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TIME_DEC, NULL, NULL);



            CreateWindowW(L"Button", L"-", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          240, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TIMEMAX_DEC, NULL, NULL);
            CreateWindowW(L"Button", L"+", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          280, 335, 20, 20, hwnd, (HMENU)ID_BUTTON_TIMEMAX_INC, NULL, NULL);

            labelTimeShiftMax = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
                  260, 335, 20, 20, hwnd, (HMENU) 1, NULL, NULL);
            _itow(timeShiftMax+4, buffer,10);
            SetWindowTextW(labelTimeShiftMax, buffer);



            labelTimeShift = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
                  655, 335, 20, 20, hwnd, (HMENU) 1, NULL, NULL);

            _itow(timeShift+4, buffer,10);
            SetWindowTextW(labelTimeShift, buffer);

            labelTimeShift2 = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
                  5, 335, 20, 20, hwnd, (HMENU) 1, NULL, NULL);

            _itow(timeShift+1, buffer,10);
            SetWindowTextW(labelTimeShift2, buffer);


            //playlistInsertAfter(16, 0, 2, playlistHead);


            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_PLAY){
                if(playOn>0){
                    emptySoundlist();
                }
                playOn = (playOn+1)%2;
            }
            else if (LOWORD(wParam) == ID_BUTTON_TONELENGTH_DEC){
                if (toneLength > 1){
                    toneLength--;
                    _itow(toneLength, buffer,10);
                    SetWindowTextW(labelToneLength, buffer);
                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_TONELENGTH_INC){
                    toneLength++;
                    _itow(toneLength, buffer,10);
                    SetWindowTextW(labelToneLength, buffer);
            }
            else if (LOWORD(wParam) == ID_BUTTON_BPM_DEC){
                if (bpm > 1){
                    bpm--;
                    _itow(bpm, buffer,10);
                    SetWindowTextW(labelBPM, buffer);
                }
            }
            else if (LOWORD(wParam) == ID_BUTTON_BPM_INC){
                    bpm++;
                    _itow(bpm, buffer,10);
                    SetWindowTextW(labelBPM, buffer);
            }
            else if(LOWORD(wParam) == ID_BUTTON_TONE_INC){
                toneShift += 3;
                InvalidateRect( hwnd4, NULL, TRUE );
            }
            else if(LOWORD(wParam) == ID_BUTTON_TONE_DEC){
                toneShift -= 3;
                InvalidateRect( hwnd4, NULL, TRUE );
            }
            else if(LOWORD(wParam) == ID_BUTTON_TIME_INC){
                if(timeShift<timeShiftMax){
                    timeShift += 1;
                    InvalidateRect( hwnd4, NULL, TRUE );
                    _itow(timeShift+4, buffer,10);
                    SetWindowTextW(labelTimeShift, buffer);
                    _itow(timeShift+1, buffer,10);
                    SetWindowTextW(labelTimeShift2, buffer);
                }
            }
            else if(LOWORD(wParam) == ID_BUTTON_TIME_DEC){
                if(timeShift>0){
                    timeShift -= 1;
                    InvalidateRect( hwnd4, NULL, TRUE );
                    _itow(timeShift+4, buffer,10);
                    SetWindowTextW(labelTimeShift, buffer);
                    _itow(timeShift+1, buffer,10);
                    SetWindowTextW(labelTimeShift2, buffer);
                }
            }
            else if(LOWORD(wParam) == ID_BUTTON_TIMEMAX_DEC){
                if((timeShiftMax>0)&&(playOn <1)){
                    timeShiftMax -= 1;
                    _itow(timeShiftMax+4, buffer,10);
                     SetWindowTextW(labelTimeShiftMax, buffer);
                }
            }
            else if(LOWORD(wParam) == ID_BUTTON_TIMEMAX_INC){
                if(playOn <1){
                    timeShiftMax += 1;
                    _itow(timeShiftMax+4, buffer,10);
                     SetWindowTextW(labelTimeShiftMax, buffer);
                }
            }
            break;
	case WM_TIMER:
	    if (wParam == ID_TIMER1){
                area.top = 322;
                area.bottom = 332;
                //area.left = 10+ticker*10-2;
                //area.right = 20+ticker*10+2;
                area.left = 10-2;
                area.right = 20+640+2;
                InvalidateRect( hwnd4, &area, TRUE );
            }
            break;
        case WM_LBUTTONUP:
            if ((LOWORD(lParam)>10)&&(LOWORD(lParam)<650)&&(HIWORD(lParam)>322)&&(HIWORD(lParam)<333)){ //change ticker time
                r.top = 320-2;
                r.bottom = 335+2;
                r.left = 10-5;
                r.right = 650+5;
                ticker = (LOWORD(lParam)-10)/10+timeShift*16;
                InvalidateRect( hwnd, &r, TRUE );
            }
            else if ((LOWORD(lParam)>10)&&(LOWORD(lParam)<650)&&(HIWORD(lParam)>10)&&(HIWORD(lParam)<320)){ //add tones to playlist
                playlistInsertAfter(TONE_OFFSET-(HIWORD(lParam)-10)/10-toneShift, (LOWORD(lParam)-10)/10+timeShift*16, toneLength, playlistHead);
                r.top = 10+10*((HIWORD(lParam)-10)/10)-2;
                r.bottom = 20+10*((HIWORD(lParam)-10)/10)+2;
                r.left = 10 + 10*((LOWORD(lParam)-10)/10)-2;
                r.right = 10 + 10*(toneLength + (LOWORD(lParam)-10)/10)+2;
                InvalidateRect( hwnd, &r, TRUE );
            }
            break;
        case WM_RBUTTONUP:
            if ((LOWORD(lParam)>10)&&(LOWORD(lParam)<650)&&(HIWORD(lParam)>10)&&(HIWORD(lParam)<320)){ //delete tones from playlist

                plLoopPointer = playlistHead;
                while(plLoopPointer->next->next != plLoopPointer->next){
                    if((plLoopPointer->next->tone == TONE_OFFSET-(HIWORD(lParam)-10)/10-toneShift)&&((plLoopPointer->next->timeMark -timeShift*16 <1+ (LOWORD(lParam)-10)/10))&&((plLoopPointer->next->timeMark - timeShift*16 + plLoopPointer->next->length +1>   (LOWORD(lParam)-10)/10))){
                        r.top = 10+10*(TONE_OFFSET - plLoopPointer->next->tone - toneShift)-2;
                        r.bottom = 20+10*(TONE_OFFSET - plLoopPointer->next->tone - toneShift)+2;
                        r.left = 10 + 10*(plLoopPointer->next->timeMark+toneShift*16)-2;
                        r.right = 10 + 10*(plLoopPointer->next->timeMark+toneShift*16)+10*(plLoopPointer->next->length)+2;
                        InvalidateRect( hwnd, NULL, TRUE );
                        playlistDeleteNext(plLoopPointer);
                        break;
                    }
                    else{
                        plLoopPointer = plLoopPointer->next;
                    }
                }
                
            }
            break;


        case WM_PAINT:
            theSeqGraphics(hwnd);
            break;
        case WM_DESTROY:
                seqWindowActive = 0;
            break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}


void theSeqGraphics(HWND hwnd){
    int i = 0;
    int j = 0;
    RECT r;
    RECT r2;
    int smallTickerLength;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hBrushGreyBright = CreateSolidBrush(RGB(192, 192, 192));
    HBRUSH hBrushBlue = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH hBrushBlueBright = CreateSolidBrush(RGB(230, 230, 255));
    HBRUSH hBrushBlueBrighter = CreateSolidBrush(RGB(240, 240, 255));
    HBRUSH hBrushBlueMedium = CreateSolidBrush(RGB(150, 150, 255));
    HBRUSH hBrushGreenBright = CreateSolidBrush(RGB(230, 255, 230));

    HPEN hPenBlack = CreatePen(PS_SOLID, 1, RGB(0,0,0));

    HPEN hPenRed = CreatePen(PS_SOLID, 1, RGB(255,0,0));
    HPEN hPenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
    HPEN hPenBlueBright = CreatePen(PS_SOLID, 1, RGB(230, 230, 255));
    HPEN hPenBlueMedium = CreatePen(PS_SOLID, 1, RGB(150, 150, 255));
    HPEN hPenBlueBrighter = CreatePen(PS_SOLID, 1, RGB(240, 240, 255));
    HPEN hPenGreenBright = CreatePen(PS_SOLID, 1, RGB(230, 255, 230));

    HPEN hPenWhite = CreatePen(PS_SOLID, 1, RGB(255,255,255));
    HPEN hPenGrey = CreatePen(PS_SOLID, 1, RGB(128,128,128));
    HPEN hPenGreyDark = CreatePen(PS_SOLID, 1, RGB(64,64,64));
    HPEN hPenGreyBright = CreatePen(PS_SOLID, 1, RGB(192,192,192));

    HBRUSH hBrushYellow1 = CreateSolidBrush(RGB(255, 255, 255));
    HPEN hPenYellow1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));


    //shadows

    r2.top = 10;
    r2.bottom = 320;
    r2.left = 10;
    r2.right = 650;

    r.top = r2.top;
    r.bottom = r2.bottom;
    r.left = r2.left;
    r.right = r2.right;
   
    SelectObject(hdc, hPenYellow1);
    SelectObject(hdc, hBrushYellow1);

    Rectangle(hdc, r.left, r.top, r.right, r.bottom);


    SelectObject(hdc, hPenGreyDark);
    MoveToEx(hdc, r.left-1, r.bottom-1, NULL);
    LineTo(hdc, r.left-1, r.top-1);
    LineTo(hdc, r.right, r.top-1);

    SelectObject(hdc, hPenGrey);
    MoveToEx(hdc, r.left-2, r.bottom, NULL);
    LineTo(hdc, r.left-2, r.top-2);
    LineTo(hdc, r.right+1, r.top-2);

    SelectObject(hdc, hPenGreyBright);
    MoveToEx(hdc, r.left-1, r.bottom, NULL);
    LineTo(hdc, r.right, r.bottom);
    LineTo(hdc, r.right, r.top-2);

    SelectObject(hdc, hPenWhite);
    MoveToEx(hdc, r.left-2, r.bottom+1, NULL);
    LineTo(hdc, r.right+1, r.bottom+1);
    LineTo(hdc, r.right+1, r.top-3);

    //piano keys
    SelectObject(hdc, hPenGreyBright);
    SelectObject(hdc, hBrushGreyBright);

    for(i=0; r.top+(i-toneShift)*10+10-1200<r.bottom+1; i++){
       if(r.top+(i-toneShift)*10+10-1200>r.top+10-1){
           j = i%12;

           if(j==4){
              SelectObject(hdc, hPenBlueBright);
              SelectObject(hdc, hBrushBlueBright);
              Rectangle(hdc, r.left, r.top+(i-toneShift)*10-1200, r.right, r.top+(i-toneShift)*10+10-1200);
              SelectObject(hdc, hPenGreyBright);
              SelectObject(hdc, hBrushGreyBright);
           }
           if(i==16+120){
              SelectObject(hdc, hPenGreenBright);
              SelectObject(hdc, hBrushGreenBright);
              Rectangle(hdc, r.left, r.top+(i-toneShift)*10-1200, r.right, r.top+(i-toneShift)*10+10-1200);
              SelectObject(hdc, hPenGreyBright);
              SelectObject(hdc, hBrushGreyBright);
           }
           if((j == 1)||(j == 3)||(j == 6)||(j == 8)||(j == 10)){ //black keys
              Rectangle(hdc, r.left, r.top+(i-toneShift)*10-1200, r.right, r.top+(i-toneShift)*10+10-1200);
           }
           else if((j == 4)||(j==11)){
              MoveToEx(hdc, r.left, r.top+(i-toneShift)*10+10-1200, NULL); //between two white keys
              LineTo(hdc, r.right, r.top+(i-toneShift)*10+10-1200);
           }
       }

    }

    SelectObject(hdc, hPenGreyBright);
    for(i=1; i<16; i++){
        MoveToEx(hdc, (int)(r.left+i*(r.right-r.left)/16.0), r.top, NULL); 
        LineTo(hdc, (int)(r.left+i*(r.right-r.left)/16.0), r.bottom);
    }
    SelectObject(hdc, hPenGreyDark);
    for(i=1; i<4; i++){
        MoveToEx(hdc, (int)(r.left+i*(r.right-r.left)/4.0), r.top, NULL); 
        LineTo(hdc, (int)(r.left+i*(r.right-r.left)/4.0), r.bottom);
    }


    //shadows ticker background
    r.top = 326;
    r.bottom = 329;
    r.left = 10;
    r.right = 650;

    SelectObject(hdc, hPenBlueBright);
    SelectObject(hdc, hBrushBlueBright);

    Rectangle(hdc, r.left, r.top, r.right, r.bottom );



    SelectObject(hdc, hPenGreyDark);
    MoveToEx(hdc, r.left-1, r.bottom-1, NULL);
    LineTo(hdc, r.left-1, r.top-1);
    LineTo(hdc, r.right, r.top-1);

    SelectObject(hdc, hPenGrey);
    MoveToEx(hdc, r.left-2, r.bottom, NULL);
    LineTo(hdc, r.left-2, r.top-2);
    LineTo(hdc, r.right+1, r.top-2);

    SelectObject(hdc, hPenGreyBright);
    MoveToEx(hdc, r.left-1, r.bottom, NULL);
    LineTo(hdc, r.right, r.bottom);
    LineTo(hdc, r.right, r.top-2);

    SelectObject(hdc, hPenWhite);
    MoveToEx(hdc, r.left-2, r.bottom+1, NULL);
    LineTo(hdc, r.right+1, r.bottom+1);
    LineTo(hdc, r.right+1, r.top-3);




    //shadows ticker
    r.top = 325;
    r.bottom = 330;
    r.left = 10+ticker*(int)((r2.right-r2.left)/64.0) -timeShift*160;
    r.right = 20+ticker*(int)((r2.right-r2.left)/64.0)-timeShift*160;

    if((r.left > 10-1)&&(r.right < 650-1)){ //only draw if ticker is within the range currently displayed

        if(playOn){
            smallTickerLength = (int)(44100*60/((double)bpm*beatres));
            r.left = 10+ticker*10+smallTicker/smallTickerLength-timeShift*160;
            r.right = 20+ticker*10+smallTicker/smallTickerLength-timeShift*160;
        }

        SelectObject(hdc, hPenYellow1);
        SelectObject(hdc, hBrushYellow1);

        Rectangle(hdc, r.left, r.top, r.right, r.bottom );


        SelectObject(hdc, hPenWhite);
        MoveToEx(hdc, r.left-1, r.bottom-1, NULL);
        LineTo(hdc, r.left-1, r.top-1);
        LineTo(hdc, r.right, r.top-1);

        SelectObject(hdc, hPenGreyBright);
        MoveToEx(hdc, r.left-2, r.bottom, NULL);
        LineTo(hdc, r.left-2, r.top-2);
        LineTo(hdc, r.right+1, r.top-2);

        SelectObject(hdc, hPenGrey);
        MoveToEx(hdc, r.left-1, r.bottom, NULL);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, r.right, r.top-2);

        SelectObject(hdc, hPenGreyDark);
        MoveToEx(hdc, r.left-2, r.bottom+1, NULL);
        LineTo(hdc, r.right+1, r.bottom+1);
        LineTo(hdc, r.right+1, r.top-3);
    }

    //shadow tones

    //Draw the tones in the playlist
    plLoopPointer = playlistHead;
    while(plLoopPointer->next != plLoopPointer){
        if (plLoopPointer != playlistHead){

            r.top = r2.top + (TONE_OFFSET - plLoopPointer->tone - toneShift)*10 + 2;
            r.bottom = 10 + r2.top + (TONE_OFFSET - plLoopPointer->tone - toneShift)*10-2;
            r.left = r2.left + plLoopPointer->timeMark*10 - timeShift*160 + 2;
            r.right = r2.left + (plLoopPointer->timeMark + plLoopPointer->length)*10 - timeShift*160 - 2;

            if((r.top > 10-1)&&(r.bottom < 320+1)&&(r.left > 10-1)&&(r.right < 650-1)){ //only draw if tones are within the range currently displayed
                SelectObject(hdc, hPenBlueMedium);
                SelectObject(hdc, hBrushBlueMedium);

                Rectangle(hdc, r.left, r.top, r.right, r.bottom );

                SelectObject(hdc, hPenWhite);
                MoveToEx(hdc, r.left-1, r.bottom-1, NULL);
                LineTo(hdc, r.left-1, r.top-1);
                LineTo(hdc, r.right, r.top-1);

                SelectObject(hdc, hPenGreyBright);
                MoveToEx(hdc, r.left-2, r.bottom, NULL);
                LineTo(hdc, r.left-2, r.top-2);
                LineTo(hdc, r.right+1, r.top-2);

                SelectObject(hdc, hPenGrey);
                MoveToEx(hdc, r.left-1, r.bottom, NULL);
                LineTo(hdc, r.right, r.bottom);
                LineTo(hdc, r.right, r.top-2);

                SelectObject(hdc, hPenGreyDark);
                MoveToEx(hdc, r.left-2, r.bottom+1, NULL);
                LineTo(hdc, r.right+1, r.bottom+1);
                LineTo(hdc, r.right+1, r.top-3);
            }

        }
        plLoopPointer = plLoopPointer->next;
    }




    //Clean up---------------------------------------------------
    DeleteObject(hBrushBlack);
    DeleteObject(hBrushGreyBright);
    DeleteObject(hPenBlack);
    DeleteObject(hPenRed);
    DeleteObject(hPenBlue);

    DeleteObject(hPenWhite);
    DeleteObject(hPenGrey);
    DeleteObject(hPenGreyBright);
    DeleteObject(hPenGreyDark);


    DeleteObject(hBrushYellow1);
    DeleteObject(hPenYellow1);

    DeleteObject(hBrushBlue);

    DeleteObject(hPenBlueBright);
    DeleteObject(hBrushBlueBright);
    DeleteObject(hPenBlueMedium);
    DeleteObject(hBrushBlueMedium);
    DeleteObject(hBrushBlueBrighter);
    DeleteObject(hPenBlueBrighter);

    DeleteObject(hBrushGreenBright);
    DeleteObject(hPenGreenBright);

    EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WndRevProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    static HWND hwndTrackEarlyBounce1;
    static HWND hwndTrackEarlyBounce2;
    static HWND hwndTrackEarlyBounce3;
    static HWND hwndTrackEarlyBounce4;
    static HWND hwndTrackEarlyBounce5;
    static HWND hwndTrackEarlyBounce6;
    static HWND hwndTrackEarlyBounce7;
    static HWND hwndTrackEarlyBounce8;
    static HWND hwndTrackEarlyBounce9;
    static HWND hwndTrackEarlyBounce10;

    static HWND hwndTrackBounce1;
    static HWND hwndTrackBounce2;
    static HWND hwndTrackBounce3;
    static HWND hwndTrackBounce4;
    static HWND hwndTrackBounce5;
    static HWND hwndTrackBounce6;
    static HWND hwndTrackBounce7;
    static HWND hwndTrackBounce8;
    static HWND hwndTrackBounce9;
    static HWND hwndTrackBounce10;

    static HWND hwndTrackDepth1;
    static HWND hwndTrackDepth2;
    static HWND hwndTrackDepth3;
    static HWND hwndTrackDepth4;
    static HWND hwndTrackDepth5;
    static HWND hwndTrackDepth6;
    static HWND hwndTrackDepth7;
    static HWND hwndTrackDepth8;
    static HWND hwndTrackDepth9;

    static HWND hwndTrackSpread;
    static HWND hwndTrackFeedback;

    static HWND tempLabel;
    static HWND hWndComboBox1;

    WCHAR buffer[20]; 
    RECT area;

    switch(uMsg) {
        case WM_CREATE:


            hwndTrackEarlyBounce1 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce1, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce1, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[0])); 


            hwndTrackEarlyBounce2 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+1*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce2, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce2, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[1])); 


            hwndTrackEarlyBounce3 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+2*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce3, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce3, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[2])); 


            hwndTrackEarlyBounce4 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+3*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce4, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce4, TBM_SETPOS, TRUE,  1000 - (int)(1000*bounce[3])); 


            hwndTrackEarlyBounce5 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+4*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce5, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce5, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[4])); 

            hwndTrackEarlyBounce6 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+5*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce6, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce6, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[5])); 


            hwndTrackEarlyBounce7 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+6*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce7, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce7, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[6])); 


            hwndTrackEarlyBounce8 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+7*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce8, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce8, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[7])); 


            hwndTrackEarlyBounce9 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+8*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce9, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce9, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[8])); 


            hwndTrackEarlyBounce10 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_EARLY_BOUNCE_X+9*30, REV_EARLY_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackEarlyBounce10, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackEarlyBounce10, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[9])); 


            //--------------------------------------------------------------------------------------------------------------


            hwndTrackBounce1 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce1, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce1, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[10]));


            hwndTrackBounce2 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 1*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce2, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce2, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[11]));


            hwndTrackBounce3 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 2*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce3, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce3, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[12]));


            hwndTrackBounce4 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 3*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce4, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce4, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[13])); 


            hwndTrackBounce5 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 4*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce5, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce5, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[14])); 


            hwndTrackBounce6 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 5*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce6, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce6, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[15]));


            hwndTrackBounce7 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 6*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce7, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce7, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[16])); 


            hwndTrackBounce8 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 7*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce8, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce8, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[17])); 


            hwndTrackBounce9 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 8*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce9, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce9, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[18])); 


            hwndTrackBounce10 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_BOUNCE_X + 9*30, REV_BOUNCE_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackBounce10, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackBounce10, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[19]));

            //--------------------------------------------------------------------------------------------------

            hwndTrackDepth1 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 0*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth1, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth1, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[0])); 

            hwndTrackDepth2 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 1*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth2, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth2, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[1])); 

            hwndTrackDepth3 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 2*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth3, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth3, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[2]));

            hwndTrackDepth4 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 3*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth4, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth4, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[3]));

            hwndTrackDepth5 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 4*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth5, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth5, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[4]));

            hwndTrackDepth6 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 5*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth6, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth6, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[5]));

            hwndTrackDepth7 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 6*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth7, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth7, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[6]));

            hwndTrackDepth8 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 7*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth8, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth8, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[7]));

            hwndTrackDepth9 = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_NOTICKS ,
                                      REV_DEPTH_X + 8*30, REV_DEPTH_Y, 30, 200, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackDepth9, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackDepth9, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[8]));


            //------------------------------------------------------------------------------------------

            hwndTrackFeedback = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      400, 350, 200, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackFeedback, TBM_SETRANGE,  FALSE, MAKELONG(0, 1000)); 
            SendMessageW(hwndTrackFeedback, TBM_SETPOS, TRUE, (int)(1000*feedbackMix));

            hwndTrackSpread = CreateWindowW(TRACKBAR_CLASSW , L"Trackbar Control", 
                                      WS_CHILD | WS_VISIBLE | TBS_NOTICKS ,
                                      400, 410, 200, 30, hwnd, 
                                      (HMENU) 3, NULL, NULL);

            SendMessageW(hwndTrackSpread, TBM_SETRANGE,  FALSE, MAKELONG(0, 10000)); 
            SendMessageW(hwndTrackSpread, TBM_SETPOS, TRUE, delayMult); 

            //------------------------------------------------------------------------------------------

            CreateWindowW(L"Button", L"ON/OFF", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          500, 278, 60, 26, hwnd, (HMENU)ID_BUTTON_REV_ONOFF, NULL, NULL);

            CreateWindowW(L"Button", L"Info out", WS_CHILD | WS_VISIBLE | SS_LEFT,
                          400, 278, 60, 26, hwnd, (HMENU)ID_BUTTON_REV_INFOOUT, NULL, NULL);

           // CreateWindowW(L"Button", L"Reset", WS_CHILD | WS_VISIBLE | SS_LEFT,
           //               REV_WINDOWSIZE_X-220, 470, 60, 25, hwnd, (HMENU)ID_BUTTON_REV_SETSTANDARD, NULL, NULL);


            //------------------------------------------------------------------------------------------
            CreateWindowW(L"Static", L"First order reflections", WS_CHILD | WS_VISIBLE | SS_LEFT,
                  REV_EARLY_BOUNCE_X+70 , REV_EARLY_BOUNCE_Y-20 , 140, 20, hwnd, (HMENU) 1, NULL, NULL);

            CreateWindowW(L"Static", L"Other reflections", WS_CHILD | WS_VISIBLE | SS_LEFT,
                  REV_BOUNCE_X+85, REV_BOUNCE_Y-20, 110, 20, hwnd, (HMENU) 1, NULL, NULL);

            CreateWindowW(L"Static", L"Depth Out", WS_CHILD | WS_VISIBLE | SS_LEFT,
                  REV_DEPTH_X+95, REV_DEPTH_Y-20, 70, 20, hwnd, (HMENU) 1, NULL, NULL);

            for(i=0;i<10;i++){
                tempLabel = CreateWindowW(L"Static", L"1", WS_CHILD | WS_VISIBLE | SS_CENTER,
                      REV_EARLY_BOUNCE_X+1+i*30, REV_EARLY_BOUNCE_Y+198, 20, 20, hwnd, (HMENU) 1, NULL, NULL);
                _itow(i+1, buffer,10);
                SetWindowTextW( tempLabel, buffer);
            }

            for(i=0;i<10;i++){
                tempLabel = CreateWindowW(L"Static", L"1", WS_CHILD | WS_VISIBLE | SS_CENTER,
                      REV_BOUNCE_X+1+i*30, REV_BOUNCE_Y+198, 20, 20, hwnd, (HMENU) 1, NULL, NULL);
                _itow(i+1, buffer,10);
                SetWindowTextW( tempLabel, buffer);
            }
            for(i=0;i<9;i++){
                tempLabel = CreateWindowW(L"Static", L"1", WS_CHILD | WS_VISIBLE | SS_CENTER,
                      REV_DEPTH_X+1+i*30, REV_DEPTH_Y+198, 20, 20, hwnd, (HMENU) 1, NULL, NULL);
                _itow(i+1, buffer,10);
                SetWindowTextW( tempLabel, buffer);
            }

            CreateWindowW(L"Static", L"Feedback", WS_CHILD | WS_VISIBLE | SS_LEFT,
                  420, 330, 65, 20, hwnd, (HMENU) 1, NULL, NULL);

            CreateWindowW(L"Static", L"Spread", WS_CHILD | WS_VISIBLE | SS_LEFT,
                  420, 390, 45, 20, hwnd, (HMENU) 1, NULL, NULL);
            
            //-----------------------------------------------------------------------


            hWndComboBox1 = CreateWindow(WC_COMBOBOX, TEXT(""), 
                CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                400, 460, 200, 150, hwnd, NULL, NULL,
                NULL);

            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Large room");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Wide hall");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Room");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Metalic tube");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Small tube");
            SendMessage(hWndComboBox1,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM)L"Echoes");

            SendMessage(hWndComboBox1, CB_SETCURSEL, (WPARAM)currentPreset, (LPARAM)0);

            break;
        case WM_COMMAND:
            if (HIWORD(wParam) == CBN_SELCHANGE){
                currentPreset = SendMessage(hWndComboBox1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                chooseEchoPreset(currentPreset);
                SendMessageW(hwndTrackEarlyBounce1, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[0])); 
                SendMessageW(hwndTrackEarlyBounce2, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[1])); 
                SendMessageW(hwndTrackEarlyBounce3, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[2])); 
                SendMessageW(hwndTrackEarlyBounce4, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[3])); 
                SendMessageW(hwndTrackEarlyBounce5, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[4])); 
                SendMessageW(hwndTrackEarlyBounce6, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[5])); 
                SendMessageW(hwndTrackEarlyBounce7, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[6])); 
                SendMessageW(hwndTrackEarlyBounce8, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[7])); 
                SendMessageW(hwndTrackEarlyBounce9, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[8])); 
                SendMessageW(hwndTrackEarlyBounce10, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[9])); 
                SendMessageW(hwndTrackBounce1, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[10]));
                SendMessageW(hwndTrackBounce2, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[11]));
                SendMessageW(hwndTrackBounce3, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[12]));
                SendMessageW(hwndTrackBounce4, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[13]));
                SendMessageW(hwndTrackBounce5, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[14]));
                SendMessageW(hwndTrackBounce6, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[15]));
                SendMessageW(hwndTrackBounce7, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[16]));
                SendMessageW(hwndTrackBounce8, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[17]));
                SendMessageW(hwndTrackBounce9, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[18]));
                SendMessageW(hwndTrackBounce10, TBM_SETPOS, TRUE, 1000 - (int)(1000*bounce[19]));
                SendMessageW(hwndTrackDepth1, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[0])); 
                SendMessageW(hwndTrackDepth2, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[1])); 
                SendMessageW(hwndTrackDepth3, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[2])); 
                SendMessageW(hwndTrackDepth4, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[3])); 
                SendMessageW(hwndTrackDepth5, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[4])); 
                SendMessageW(hwndTrackDepth6, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[5])); 
                SendMessageW(hwndTrackDepth7, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[6])); 
                SendMessageW(hwndTrackDepth8, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[7])); 
                SendMessageW(hwndTrackDepth9, TBM_SETPOS, TRUE, 1000 - (int)(1000*genOut[8])); 
                SendMessageW(hwndTrackFeedback, TBM_SETPOS, TRUE, (int)(1000*feedbackMix));
                SendMessageW(hwndTrackSpread, TBM_SETPOS, TRUE, delayMult); 
            }
            else if (LOWORD(wParam) == ID_BUTTON_REV_ONOFF){
                echoActive = (echoActive+1)%2;
                area.top = 280;
                area.bottom = 280+22;
                area.left = 470;
                area.right = 470+22;
                InvalidateRect( hwnd, &area, TRUE );
            }
            else if(LOWORD(wParam) == ID_BUTTON_REV_INFOOUT){
                pFile = fopen("reverbInfoOut.txt", "a");
                fprintf(pFile, "\nNew InfoOut--------------------------------\n");
                for(i=0; i<20; i++){
                    fprintf(pFile, "    bounce[%d] = %f;\n", i, bounce[i]);
                }
                for(i=0; i<9; i++){
                    fprintf(pFile, "    genOut[%d] = %f;\n", i, genOut[i]);
                }
                fprintf(pFile, "    feedbackMix = %f;\n", feedbackMix);
                fprintf(pFile, "    delayMult = %d;\n", delayMult);
                fprintf(pFile, "    delay1.length = delayMult*1;\n");
                fprintf(pFile, "    delay2.length = delayMult*2;\n");
                fprintf(pFile, "    delay3.length = delayMult*3;\n");
                fprintf(pFile, "    delay4.length = delayMult*4;\n");
                fprintf(pFile, "    delay5.length = delayMult*5;\n");
                fprintf(pFile, "    delay6.length = delayMult*6;\n");
                fprintf(pFile, "    delay7.length = delayMult*7;\n");
                fprintf(pFile, "    delay8.length = delayMult*8;\n");
                fprintf(pFile, "    delay9.length = delayMult*9;\n");
                fprintf(pFile, "    delay10.length = delayMult*10;\n");
                fprintf(pFile, "    delay1.index = 0;\n");
                fprintf(pFile, "    delay2.index = 0;\n");
                fprintf(pFile, "    delay3.index = 0;\n");
                fprintf(pFile, "    delay4.index = 0;\n");
                fprintf(pFile, "    delay5.index = 0;\n");
                fprintf(pFile, "    delay6.index = 0;\n");
                fprintf(pFile, "    delay7.index = 0;\n");
                fprintf(pFile, "    delay8.index = 0;\n");
                fprintf(pFile, "    delay9.index = 0;\n");
                fprintf(pFile, "    delay10.index = 0;\n\n");
                fclose(pFile);
                MessageBoxW(NULL, L"Wrote reverb settings to file 'reverbInfoOut.txt'.", L"Success!", MB_OK);
            }
            break;
        case WM_MOUSEMOVE:
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            break;
        case WM_VSCROLL:
            bounce[0] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce1, TBM_GETPOS,  0, 0));
            bounce[1] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce2, TBM_GETPOS,  0, 0));
            bounce[2] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce3, TBM_GETPOS,  0, 0));
            bounce[3] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce4, TBM_GETPOS,  0, 0));
            bounce[4] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce5, TBM_GETPOS,  0, 0));
            bounce[5] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce6, TBM_GETPOS,  0, 0));
            bounce[6] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce7, TBM_GETPOS,  0, 0));
            bounce[7] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce8, TBM_GETPOS,  0, 0));
            bounce[8] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce9, TBM_GETPOS,  0, 0));
            bounce[9] = 0.001*(1000-SendMessageW(hwndTrackEarlyBounce10, TBM_GETPOS,  0, 0));
            bounce[10] = 0.001*(1000-SendMessageW(hwndTrackBounce1, TBM_GETPOS,  0, 0));
            bounce[11] = 0.001*(1000-SendMessageW(hwndTrackBounce2, TBM_GETPOS,  0, 0));
            bounce[12] = 0.001*(1000-SendMessageW(hwndTrackBounce3, TBM_GETPOS,  0, 0));
            bounce[13] = 0.001*(1000-SendMessageW(hwndTrackBounce4, TBM_GETPOS,  0, 0));
            bounce[14] = 0.001*(1000-SendMessageW(hwndTrackBounce5, TBM_GETPOS,  0, 0));
            bounce[15] = 0.001*(1000-SendMessageW(hwndTrackBounce6, TBM_GETPOS,  0, 0));
            bounce[16] = 0.001*(1000-SendMessageW(hwndTrackBounce7, TBM_GETPOS,  0, 0));
            bounce[17] = 0.001*(1000-SendMessageW(hwndTrackBounce8, TBM_GETPOS,  0, 0));
            bounce[18] = 0.001*(1000-SendMessageW(hwndTrackBounce9, TBM_GETPOS,  0, 0));
            bounce[19] = 0.001*(1000-SendMessageW(hwndTrackBounce10, TBM_GETPOS,  0, 0));
            genOut[0] = 0.001*(1000-SendMessageW(hwndTrackDepth1, TBM_GETPOS,  0, 0));
            genOut[1] = 0.001*(1000-SendMessageW(hwndTrackDepth2, TBM_GETPOS,  0, 0));
            genOut[2] = 0.001*(1000-SendMessageW(hwndTrackDepth3, TBM_GETPOS,  0, 0));
            genOut[3] = 0.001*(1000-SendMessageW(hwndTrackDepth4, TBM_GETPOS,  0, 0));
            genOut[4] = 0.001*(1000-SendMessageW(hwndTrackDepth5, TBM_GETPOS,  0, 0));
            genOut[5] = 0.001*(1000-SendMessageW(hwndTrackDepth6, TBM_GETPOS,  0, 0));
            genOut[6] = 0.001*(1000-SendMessageW(hwndTrackDepth7, TBM_GETPOS,  0, 0));
            genOut[7] = 0.001*(1000-SendMessageW(hwndTrackDepth8, TBM_GETPOS,  0, 0));
            genOut[8] = 0.001*(1000-SendMessageW(hwndTrackDepth9, TBM_GETPOS,  0, 0));
            break;

        case WM_HSCROLL:
            feedbackMix = 0.001*(SendMessageW(hwndTrackFeedback, TBM_GETPOS,  0, 0)); 
            if(mouseY>400){
                delayMult = SendMessageW(hwndTrackSpread, TBM_GETPOS,  0, 0);

                delay1.length = delayMult*1;
                delay2.length = delayMult*2;
                delay3.length = delayMult*3;
                delay4.length = delayMult*4;
                delay5.length = delayMult*5;
                delay6.length = delayMult*6;
                delay7.length = delayMult*7;
                delay8.length = delayMult*8;
                delay9.length = delayMult*9;
                delay10.length = delayMult*10;

                delay1.index = 0;
                delay2.index = 0;
                delay3.index = 0;
                delay4.index = 0;
                delay5.index = 0;
                delay6.index = 0;
                delay7.index = 0;
                delay8.index = 0;
                delay9.index = 0;
                delay10.index = 0;
            }
            break;

        case WM_PAINT:
            theRevGraphics(hwnd);
            break;
        case WM_DESTROY:
            revWindowActive = 0;
            break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);

}


void theRevGraphics(HWND hwnd){
    int i = 0;
    int j = 0;
    RECT r;
    //RECT r2;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hBrushGreen = CreateSolidBrush(RGB(0, 230, 0));
    HBRUSH hBrushRed = CreateSolidBrush(RGB(230, 0, 0));

    HPEN hPenBlack = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HPEN hPenGreen = CreatePen(PS_SOLID, 1, RGB(0,230,0));
    HPEN hPenRed = CreatePen(PS_SOLID, 1, RGB(230,0,0));

    HPEN hPenWhite = CreatePen(PS_SOLID, 1, RGB(255,255,255));
    HPEN hPenGrey = CreatePen(PS_SOLID, 1, RGB(128,128,128));
    HPEN hPenGreyDark = CreatePen(PS_SOLID, 1, RGB(64,64,64));
    HPEN hPenGreyBright = CreatePen(PS_SOLID, 1, RGB(192,192,192));

    //shadows onoff-light-----------------------------------------------------
    r.top = 280;
    r.bottom = 280+22;
    r.left = 470;
    r.right = 470+22;

    SelectObject(hdc, hPenBlack);
    SelectObject(hdc, hBrushBlack);
    Rectangle(hdc, r.left, r.top, r.right, r.bottom );

    SelectObject(hdc,GetStockObject(DC_PEN));

    if(echoActive>0){
        for(i=0;i<23;i++){
            for(j=0;j<23;j++){
                SetPixel(hdc, r.left+i,r.top+j,RGB(255-min(255,(int)(sqrt((i-11.0)*(i-11.0)+(j-11.0)*(j-11.0))*250.0/(15.56-5))),0,0));
            }
        }
    }
    else{
        for(i=0;i<22;i++){
            for(j=0;j<22;j++){
                SetPixel(hdc, r.left+i,r.top+j,RGB(100-min(100,(int)(sqrt((i-11.0)*(i-11.0)+(j-11.0)*(j-11.0))*250.0/(15.56-5))),0,0));
            }
        }
    }
    SelectObject(hdc, hPenGreyDark);
    MoveToEx(hdc, r.left-1, r.bottom-1, NULL);
    LineTo(hdc, r.left-1, r.top-1);
    LineTo(hdc, r.right, r.top-1);

    SelectObject(hdc, hPenGrey);
    MoveToEx(hdc, r.left-2, r.bottom, NULL);
    LineTo(hdc, r.left-2, r.top-2);
    LineTo(hdc, r.right+1, r.top-2);

    SelectObject(hdc, hPenGreyBright);
    MoveToEx(hdc, r.left-1, r.bottom, NULL);
    LineTo(hdc, r.right, r.bottom);
    LineTo(hdc, r.right, r.top-2);

    SelectObject(hdc, hPenWhite);
    MoveToEx(hdc, r.left-2, r.bottom+1, NULL);
    LineTo(hdc, r.right+1, r.bottom+1);
    LineTo(hdc, r.right+1, r.top-3);
    //----------------------------------------------------------------------

    SelectObject(hdc, hPenBlack);
    MoveToEx(hdc, REV_EARLY_BOUNCE_X+60, REV_EARLY_BOUNCE_Y-12, NULL);
    LineTo(hdc, REV_EARLY_BOUNCE_X-12, REV_EARLY_BOUNCE_Y-12);
    LineTo(hdc, REV_EARLY_BOUNCE_X-12, REV_EARLY_BOUNCE_Y+195+25);
    LineTo(hdc, REV_EARLY_BOUNCE_X+305, REV_EARLY_BOUNCE_Y+195+25);
    LineTo(hdc, REV_EARLY_BOUNCE_X+305, REV_EARLY_BOUNCE_Y-12);
    LineTo(hdc, REV_EARLY_BOUNCE_X+215, REV_EARLY_BOUNCE_Y-12);


    MoveToEx(hdc, REV_BOUNCE_X+75, REV_BOUNCE_Y-12, NULL);
    LineTo(hdc, REV_BOUNCE_X-12, REV_BOUNCE_Y-12);
    LineTo(hdc, REV_BOUNCE_X-12, REV_BOUNCE_Y+195+25);
    LineTo(hdc, REV_BOUNCE_X+305, REV_BOUNCE_Y+195+25);
    LineTo(hdc, REV_BOUNCE_X+305, REV_BOUNCE_Y-12);
    LineTo(hdc, REV_BOUNCE_X+200, REV_BOUNCE_Y-12);



    MoveToEx(hdc, REV_DEPTH_X+85, REV_DEPTH_Y-12, NULL);
    LineTo(hdc, REV_DEPTH_X-12, REV_DEPTH_Y-12);
    LineTo(hdc, REV_DEPTH_X-12, REV_DEPTH_Y+195+25);
    LineTo(hdc, REV_DEPTH_X+305-30, REV_DEPTH_Y+195+25);
    LineTo(hdc, REV_DEPTH_X+305-30, REV_DEPTH_Y-12);
    LineTo(hdc, REV_DEPTH_X+167, REV_DEPTH_Y-12);


    //feedback
    MoveToEx(hdc, 400+15, 350-12, NULL);
    LineTo(hdc, 400-12, 350-12);
    LineTo(hdc, 400-12, 350+30+2);
    LineTo(hdc, 400+200+12, 350+30+2);
    LineTo(hdc, 400+200+12, 350-12);
    LineTo(hdc, 400+85, 350-12);

    //spread
    MoveToEx(hdc, 400+15, 410-12, NULL);
    LineTo(hdc, 400-12, 410-12);
    LineTo(hdc, 400-12, 410+30+2);
    LineTo(hdc, 400+200+12, 410+30+2);
    LineTo(hdc, 400+200+12, 410-12);
    LineTo(hdc, 400+70, 410-12);


    DeleteObject(hBrushBlack);
    DeleteObject(hBrushGreen);
    DeleteObject(hBrushRed);
    DeleteObject(hPenGreen);
    DeleteObject(hPenBlack);
    DeleteObject(hPenRed);

    DeleteObject(hPenWhite);
    DeleteObject(hPenGrey);
    DeleteObject(hPenGreyDark);
    DeleteObject(hPenGreyBright);

    EndPaint(hwnd, &ps);
}





void gaussian(double *v1, double *v2){ //Box-Muller based computation of normally distributed random values N(0,1)
    double u1 = 0.0;
    double u2 = 0.0;

    u1 = 0.0001*(rand()%10000);
    u2 = 0.0001*(rand()%10000);

    *v1 = sqrt(-2*log(u1))*cos(2*PI*u2);
    *v2 = sqrt(-2*log(u2))*sin(2*PI*u2);
}

void chooseEchoPreset(int choice){
  if(choice == 0){
    bounce[0] = 0.050000;
    bounce[1] = 0.100000;
    bounce[2] = 0.150000;
    bounce[3] = 0.200000;
    bounce[4] = 0.250000;
    bounce[5] = 0.300000;
    bounce[6] = 0.250000;
    bounce[7] = 0.200000;
    bounce[8] = 0.150000;
    bounce[9] = 0.100000;
    bounce[10] = 0.200000;
    bounce[11] = 0.200000;
    bounce[12] = 0.200000;
    bounce[13] = 0.200000;
    bounce[14] = 0.200000;
    bounce[15] = 0.200000;
    bounce[16] = 0.200000;
    bounce[17] = 0.200000;
    bounce[18] = 0.200000;
    bounce[19] = 0.200000;
    genOut[0] = 0.000000;
    genOut[1] = 0.000000;
    genOut[2] = 0.100000;
    genOut[3] = 0.200000;
    genOut[4] = 0.300000;
    genOut[5] = 0.400000;
    genOut[6] = 0.500000;
    genOut[7] = 0.600000;
    genOut[8] = 0.700000;
    feedbackMix = 0.700000;
    delayMult = 10000;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
  else if(choice == 1){
    bounce[0] = 0.050000;
    bounce[1] = 0.100000;
    bounce[2] = 0.150000;
    bounce[3] = 0.200000;
    bounce[4] = 0.250000;
    bounce[5] = 0.300000;
    bounce[6] = 0.250000;
    bounce[7] = 0.200000;
    bounce[8] = 0.150000;
    bounce[9] = 0.100000;
    bounce[10] = 0.087000;
    bounce[11] = 0.116000;
    bounce[12] = 0.173000;
    bounce[13] = 0.214000;
    bounce[14] = 0.266000;
    bounce[15] = 0.312000;
    bounce[16] = 0.266000;
    bounce[17] = 0.220000;
    bounce[18] = 0.168000;
    bounce[19] = 0.116000;
    genOut[0] = 0.000000;
    genOut[1] = 0.000000;
    genOut[2] = 0.100000;
    genOut[3] = 0.200000;
    genOut[4] = 0.300000;
    genOut[5] = 0.400000;
    genOut[6] = 0.500000;
    genOut[7] = 0.600000;
    genOut[8] = 0.700000;
    feedbackMix = 0.844000;
    delayMult = 10000;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
  else if(choice == 2){
    bounce[0] = 0.092000;
    bounce[1] = 0.116000;
    bounce[2] = 0.168000;
    bounce[3] = 0.173000;
    bounce[4] = 0.254000;
    bounce[5] = 0.225000;
    bounce[6] = 0.150000;
    bounce[7] = 0.075000;
    bounce[8] = 0.075000;
    bounce[9] = 0.000000;
    bounce[10] = 0.422000;
    bounce[11] = 0.341000;
    bounce[12] = 0.301000;
    bounce[13] = 0.289000;
    bounce[14] = 0.208000;
    bounce[15] = 0.191000;
    bounce[16] = 0.150000;
    bounce[17] = 0.156000;
    bounce[18] = 0.104000;
    bounce[19] = 0.098000;
    genOut[0] = 0.000000;
    genOut[1] = 0.087000;
    genOut[2] = 0.156000;
    genOut[3] = 0.200000;
    genOut[4] = 0.191000;
    genOut[5] = 0.110000;
    genOut[6] = 0.040000;
    genOut[7] = 0.000000;
    genOut[8] = 0.000000;
    feedbackMix = 0.590000;
    delayMult = 10000;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
  else if(choice == 3){
    bounce[0] = 0.561000;
    bounce[1] = 0.451000;
    bounce[2] = 0.370000;
    bounce[3] = 0.329000;
    bounce[4] = 0.214000;
    bounce[5] = 0.139000;
    bounce[6] = 0.092000;
    bounce[7] = 0.064000;
    bounce[8] = 0.046000;
    bounce[9] = 0.046000;
    bounce[10] = 0.561000;
    bounce[11] = 0.416000;
    bounce[12] = 0.277000;
    bounce[13] = 0.208000;
    bounce[14] = 0.145000;
    bounce[15] = 0.133000;
    bounce[16] = 0.110000;
    bounce[17] = 0.087000;
    bounce[18] = 0.092000;
    bounce[19] = 0.087000;
    genOut[0] = 0.543000;
    genOut[1] = 0.474000;
    genOut[2] = 0.318000;
    genOut[3] = 0.179000;
    genOut[4] = 0.116000;
    genOut[5] = 0.087000;
    genOut[6] = 0.064000;
    genOut[7] = 0.064000;
    genOut[8] = 0.052000;
    feedbackMix = 0.642000;
    delayMult = 2659;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
  else if(choice == 4){
    bounce[0] = 0.050000;
    bounce[1] = 0.100000;
    bounce[2] = 0.150000;
    bounce[3] = 0.200000;
    bounce[4] = 0.250000;
    bounce[5] = 0.300000;
    bounce[6] = 0.250000;
    bounce[7] = 0.200000;
    bounce[8] = 0.150000;
    bounce[9] = 0.100000;
    bounce[10] = 0.200000;
    bounce[11] = 0.200000;
    bounce[12] = 0.200000;
    bounce[13] = 0.200000;
    bounce[14] = 0.200000;
    bounce[15] = 0.200000;
    bounce[16] = 0.200000;
    bounce[17] = 0.200000;
    bounce[18] = 0.191000;
    bounce[19] = 0.133000;
    genOut[0] = 0.283000;
    genOut[1] = 0.214000;
    genOut[2] = 0.249000;
    genOut[3] = 0.200000;
    genOut[4] = 0.300000;
    genOut[5] = 0.335000;
    genOut[6] = 0.364000;
    genOut[7] = 0.428000;
    genOut[8] = 0.347000;
    feedbackMix = 0.780000;
    delayMult = 2370;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
  else if(choice == 5){
    bounce[0] = 0.584000;
    bounce[1] = 0.000000;
    bounce[2] = 0.584000;
    bounce[3] = 0.000000;
    bounce[4] = 0.543000;
    bounce[5] = 0.000000;
    bounce[6] = 0.549000;
    bounce[7] = 0.000000;
    bounce[8] = 0.514000;
    bounce[9] = 0.000000;
    bounce[10] = 0.671000;
    bounce[11] = 0.514000;
    bounce[12] = 0.457000;
    bounce[13] = 0.295000;
    bounce[14] = 0.121000;
    bounce[15] = 0.150000;
    bounce[16] = 0.104000;
    bounce[17] = 0.092000;
    bounce[18] = 0.058000;
    bounce[19] = 0.000000;
    genOut[0] = 0.497000;
    genOut[1] = 0.329000;
    genOut[2] = 0.191000;
    genOut[3] = 0.104000;
    genOut[4] = 0.150000;
    genOut[5] = 0.098000;
    genOut[6] = 0.081000;
    genOut[7] = 0.023000;
    genOut[8] = 0.000000;
    feedbackMix = 0.532000;
    delayMult = 10000;
    delay1.length = delayMult*1;
    delay2.length = delayMult*2;
    delay3.length = delayMult*3;
    delay4.length = delayMult*4;
    delay5.length = delayMult*5;
    delay6.length = delayMult*6;
    delay7.length = delayMult*7;
    delay8.length = delayMult*8;
    delay9.length = delayMult*9;
    delay10.length = delayMult*10;
    delay1.index = 0;
    delay2.index = 0;
    delay3.index = 0;
    delay4.index = 0;
    delay5.index = 0;
    delay6.index = 0;
    delay7.index = 0;
    delay8.index = 0;
    delay9.index = 0;
    delay10.index = 0;
  }
}


//linked-list functions------------------------------------------------------
void soundlistInsertAfter(double v1, double v2, int v3, struct soundNode *t){
    struct soundNode *x;
    x = (struct soundNode *)malloc(sizeof *x);
    x->frequency = v1;
    x->phaseAngle = v2;
    x->id = v3;
    
    x->lfoPhase = 0.0;

    x->attack = 0.0;
    x->decay = 1.0;
    x->release = 1.0;

    x->releasedTrue = 0;

    x->next = t->next;
    t->next = x;
}

void soundlistDeleteNext(struct soundNode *t){
    struct soundNode *x;
    x = t->next;
    t->next = t->next->next;
    free(x);
}

void emptySoundlist(void){
    sLoopPointer = soundlistHead;
    while(sLoopPointer->next->next != sLoopPointer->next){
        soundlistDeleteNext(sLoopPointer);
    }
}


void presetInsertAfter(wchar_t *v1, int v2, struct presetNode *t){
    int i = 0;
    struct presetNode *x;
    x = (struct presetNode *)malloc(sizeof *x);

    x->name = v1;

    x->paramAttack = 100;
    x->paramDecay = 100;
    x->paramSustain = 100;
    x->paramRelease = 100;

    for (i = 0; i<30; i++){
        x->sinOsc[i] = 0;
    }

    x->id = v2;

    x->next = t->next;
    t->next = x;
}

void presetDeleteNext(struct presetNode *t){
    struct presetNode *x;
    x = t->next;
    t->next = t->next->next;
    free(x);
}



void playlistInsertAfter(int v1, int v2, int v3, struct plNode *t){
    struct plNode *x;
    x = (struct plNode *)malloc(sizeof *x);
    x->tone = v1;
    x->timeMark = v2;
    x->length = v3;
    x->next = t->next;

    t->next = x;
}

void playlistDeleteNext(struct plNode *t){
    struct plNode *x;
    x = t->next;
    t->next = t->next->next;
    free(x);
}


void emptyPlaylist(void){
    plLoopPointer = playlistHead;
    while(plLoopPointer->next->next != plLoopPointer->next){
        playlistDeleteNext(plLoopPointer);
    }
}


//csFIFO---------------------------

void csFIFO_Add(_int16 newData, struct csFIFO *t){
  if(t->indexZero > 0){
      t->indexZero = t->indexZero - 1;
  }
  else{
      t->indexZero = CSFIFOSIZE-1;
  }
  t->data[newData];
}

_int16 csFIFO_Peek(int atIndex, struct csFIFO *t){
  return(t->data[(t->indexZero + atIndex)%CSFIFOSIZE]);
}

//Delay------------------------------------------

double delayProcess(_int16 input, struct delayLine *t){
  double retvalue = 0.0;

  if(t->index < t->length){
    t->data[t->index] = input;
    if(t->index < t->length - 1){
      retvalue = t->data[t->index + 1];
    }
    else{
      retvalue = t->data[0];
    }
  }

  t->index = t->index +1;
  if(t->index > t->length-1){
    t->index = 0;
  }
  return(retvalue);
}

void delayInit(int theLength, struct delayLine *t){
  int i = 0;
  t->index = 0;
  for(i=0; i<DELAYMAX; i++){
    t->data[i] = 0;
  }
  t->length = theLength;
}
