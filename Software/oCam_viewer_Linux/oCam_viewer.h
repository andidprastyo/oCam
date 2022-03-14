/*******************************************************************************#
#                                                                               #
# Withrobot Camera GUI for oCam                                                 #
#                                                                               #
# Copyright (C) 2015 Withrobot. Inc.                                            #
#                                                                               #
# This program is free software: you can redistribute it and/or modify          #
# it under the terms of the GNU General Public License as published by          #
# the Free Software Foundation, either version 3 of the License, or             #
# (at your option) any later version.                                           #
#                                                                               #
# This program is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of                #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 #
# GNU General Public License for more details.                                  #
# You should have received a copy of the GNU General Public License             #
# along with this program.  If not, see <http://www.gnu.org/licenses/>          #
#                                                                               #
********************************************************************************/

/**
 * @file oCam_viewer.h
 * @brief oCam viewer GUI header
 */

#ifndef OCAM_H
#define OCAM_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QDir>
#include <QTreeWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <vector>
#include <omp.h>

#include "controlform_integer.h"
#include "controlform_menu.h"
#include "controlform_boolean.h"
#include "controlform_pushbtn.h"
#include "format_tree_form.h"
#include "format_converter/format_converter.hpp"
#include "withrobot_camera.hpp"
#include "camera_thread.h"
/**
 * @defgroup Viewer GUI
 *
 * @{
 */

#include <QDebug>

#define DEFAULT_IMAGE_WIDTH         640
#define DEFAULT_IMAGE_HEIGHT        480
#define DEFAULT_PIXFORMAT           Withrobot::fourcc_to_pixformat('Y','U','V','Y')

#define MAX_WINDOW_WIDTH            1920
#define MAX_WINDOW_HEIGHT           1080

#define DEFAULT_WINDOW_GEO_WIDTH    1024
#define DEFAULT_WINDOW_GEO_HEIGHT   768

//#define STATIC_QTIMER_RATE          1  /**< undefine 되면, 카메라 설정에 따라 이미지 그리는 스레드가 동작한다. */

//#define NO_DRAW	/**< DEBUG: define 되면, 영상 출력은 안하고, fps 만 계산한다. */

namespace Ui {
class oCam;
}

class oCam : public QMainWindow
{
    Q_OBJECT

public:
    explicit oCam(QWidget *parent = 0);
    ~oCam();

public slots:
    void update_gui();

private slots:
    void on_btnStart_clicked();
    void on_tbtnDevRefresh_clicked();
    void calculate_color_correction();
    void reset_color_correction();
    void set_default_color_correction();
    void control_command(uint32_t value);

    void on_ckbToggleConvert_toggled(bool checked);

private:
    bool start();
    bool stop();
    void restart();

    void enum_dev_list();
    void generate_IR_image(unsigned char* frame_buffer, unsigned char* ir_buffer);
    void generate_RGB_image(unsigned char* frame_buffer, unsigned char* rgb_buffer);
    void Split_Stereo_image(unsigned char* Src, unsigned char* Dst, int Width, int Height);

private:
    Ui::oCam *ui;

    QTimer* qtmr_timer;
    QMargins ui_default_contents_margin;

    ControlFormPushBtn* defaultPushBtn;
    ControlFormPushBtn* resetPushBtn;
    ControlFormPushBtn* correctionPushBtn;

    int defaultWbCompBlue;
    int defaultWbCompRed;

    bool no_device;
    bool camColorCorrectionFlag;

    int DEFAULT_WB_COMP_BLUE;
    int DEFAULT_WB_COMP_RED;

public:
    /** 장치 이름 */
    std::string dev_node;
    /** 포멧 이름 */
    std::string fmt_name;
    /** 장치 이름 + 포멧 이름 */
    std::string title;



    /**
     * 화면 출력 이미지 buffer (rgb)
     */
    unsigned char* rgb_buffer;

    /**
     * 화면 출력 이미지 buffer (IR)
     */
    unsigned char* ir_buffer;
        /**
     * 화면 출력 이미지 buffer (stereo)
     */
    unsigned char* stereo_buffer;
    /**
     * Format converter
     */
    GuvcviewFormatConverter* format_converter;

    /**
     * myCam API
     *
     * v4l2 API로 myCam의 UVC 연결 제어
     */
    Withrobot::Camera* ocam;

    /**
     * myCam 이미지 포멧 구조체
     */
    Withrobot::camera_format format;

    /**
     * myCam 컨트롤 구조체
     */
    Withrobot::camera_control control;

    /**
     * DEFAULT_DEVICE_FOLDER의 video 장치 목록을 가지고 있는다.
     */
    std::vector<Withrobot::usb_device_info> dev_list;

    /**
     * 정수형 컨트롤러들을 담고 있다 (슬라이드바 형식)
     */
    std::vector<ControlFormInteger*> integer_control_form_list;

    /**
     * 불리언 컨트롤러들을 담고 있다 (라디오 버튼 선택 형식)
     */
    std::vector<ControlFormBoolean*> boolean_control_form_list;

    /**
     * 메뉴 컨트롤러들을 담고 있다 (콤보박스 형식)
     */
    std::vector<ControlFormMenu*> menu_control_form_list;

    /**
     * Miscllaneous의 누름버튼 형태의 컨트롤러들을 담고 있다
     */
    std::vector<ControlFormPushBtn*> misc_control_form_pushBtn;

    /**
     * myCam 지원 포멧 목록을 담고 있다. (트리구조)
     */
    FormatTreeForm* format_tree;

    /**
     * 초당 프레임수를 계산하기 위한 타이머 모듈
     */
    Withrobot::Timer frame_interval;

    /**
     * bayerRGB 이미지를 RGB 이미지로 보여줄지 여부, IR 이미지로 보여줄지 여부
     */
    bool showConvertImage;

    /**
     * bayerRGB 이미지를 RGB 이미지로 보여줄지 여부
     */
    bool IRImage;

    /**
     * Stereo 이미지를 Split 이미지로 보여줄지 여부
     */
    bool StereoImage;
    
    /**
     * @todo Withrobot thread 사용하는 방법 모색
     */
    Withrobot::Mutex mutex;

public:
    /**
     * 카메라 출력 이미지 buffer (yuy2)
     */
    unsigned char* frame_buffer;
    int format_size;
};

/** @} */ // end of myCam viewer main window class

#endif // OCAM_H
