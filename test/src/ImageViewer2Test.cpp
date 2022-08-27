// -*- C++ -*-
// <rtc-template block="description">
/*!
 * @file  ImageViewer2Test.cpp
 * @brief Image Viewer Component with common camera interface 2.0 for RTM2.0 (test code)
 *
 */
// </rtc-template>

#include "ImageViewer2Test.h"

// Module specification
// <rtc-template block="module_spec">
#if RTM_MAJOR_VERSION >= 2
static const char* const imageviewer2_spec[] =
#else
static const char* imageviewer2_spec[] =
#endif
  {
    "implementation_id", "ImageViewer2Test",
    "type_name",         "ImageViewer2Test",
    "description",       "Image Viewer Component with common camera interface 2.0 for RTM2.0",
    "version",           "1.0.0",
    "vendor",            "Kenichi Ohara, Meijo University",
    "category",          "ImageProcessing",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.capture_frame_num", "0",

    // Widget
    "conf.__widget__.capture_frame_num", "text",
    // Constraints

    "conf.__type__.capture_frame_num", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ImageViewer2Test::ImageViewer2Test(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ImageOut("Image", m_Image),
    m_CameraCaptureServicePort("CameraCaptureService")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageViewer2Test::~ImageViewer2Test()
{
}



RTC::ReturnCode_t ImageViewer2Test::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("Image", m_ImageOut);
  
  // Set service provider to Ports
  m_CameraCaptureServicePort.registerProvider("CameraCaptureService", "Img::CameraCaptureService", m_CameraCaptureService);
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  addPort(m_CameraCaptureServicePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("capture_frame_num", m_capture_frame_num, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageViewer2Test::onFinalize()
{
  return RTC::RTC_OK;
}
*/


//RTC::ReturnCode_t ImageViewer2Test::onStartup(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2Test::onShutdown(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


RTC::ReturnCode_t ImageViewer2Test::onActivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageViewer2Test::onDeactivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageViewer2Test::onExecute(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


//RTC::ReturnCode_t ImageViewer2Test::onAborting(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2Test::onError(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2Test::onReset(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2Test::onStateUpdate(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2Test::onRateChanged(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


bool ImageViewer2Test::runTest()
{
    return true;
}


extern "C"
{
 
  void ImageViewer2TestInit(RTC::Manager* manager)
  {
    coil::Properties profile(imageviewer2_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageViewer2Test>,
                             RTC::Delete<ImageViewer2Test>);
  }
  
}
