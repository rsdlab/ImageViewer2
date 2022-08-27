// -*- C++ -*-
// <rtc-template block="description">
/*!
 * @file  ImageViewer2.cpp
 * @brief Image Viewer Component with common camera interface 2.0 for RTM2.0
 *
 */
// </rtc-template>

#include "ImageViewer2.h"

// Module specification
// <rtc-template block="module_spec">
#if RTM_MAJOR_VERSION >= 2
static const char* const imageviewer2_spec[] =
#else
static const char* imageviewer2_spec[] =
#endif
  {
    "implementation_id", "ImageViewer2",
    "type_name",         "ImageViewer2",
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
ImageViewer2::ImageViewer2(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ImageIn("Image", m_Image),
    m_CameraCaptureServicePort("CameraCaptureService")
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ImageViewer2::~ImageViewer2()
{
}



RTC::ReturnCode_t ImageViewer2::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Image", m_ImageIn);
  
  // Set OutPort buffer

  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  m_CameraCaptureServicePort.registerConsumer("CameraCaptureService", "Img::CameraCaptureService", m_CameraCaptureService);
  
  // Set CORBA Service Ports
  addPort(m_CameraCaptureServicePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("capture_frame_num", m_capture_frame_num, "0");
  // </rtc-template>

  
  return RTC::RTC_OK;
}

RTC::ReturnCode_t ImageViewer2::onActivated(RTC::UniqueId /*ec_id*/)
{
  RTC::PortServiceList* portlist;
  
  portlist = this->get_ports();
  connection_check = new bool [portlist->length()];
  for(unsigned int i=0; i<portlist->length(); ++i)
  {
	  RTC::PortService_ptr port;
	  port = (*portlist)[i];
	  if(port->get_port_profile()->connector_profiles.length()!=0)
	    connection_check[i]=true;
	  else
      connection_check[i]=false;
  }

  //Connection check of service port
  if(connection_check[1])
  {
    //Setting the capture mode to continuous capture mode
	  if(m_capture_frame_num == 0)
	  {
		std::cout << "start continuous image streaming." << std::endl;
		RTC_INFO(("Send command of \"start continuous\" via CameraCaptureService."));
		m_CameraCaptureService->start_continuous();
	  }
	  //Setting the capture mode to 1shot capture mode
	  else if(m_capture_frame_num == 1)
	  {
		std::cout << "take one frame." << std::endl;
		RTC_INFO(("Send command of \"take one frame\" via CameraCaptureService."));
		m_CameraCaptureService->take_one_frame();
	  }
	  //Setting the capture mode to specified number of images.
	  else if(m_capture_frame_num > 1)
	  {
		std::cout << "take multi frames (" << m_capture_frame_num << " frames)." << std::endl;
		RTC_INFO(("Send command of \"take multi frames\" via CameraCaptureService."));
		m_CameraCaptureService->take_multi_frames(m_capture_frame_num);
	  }
	  else
	  {
		std::cerr << "Please set capture_frame_num to more than 0" << std::endl;
		RTC_ERROR(("Configuration Param <frames_num> should be over 0. [%d]",m_capture_frame_num));
		return RTC::RTC_ERROR;
	  }
  }

  cv::namedWindow("Image Window", cv::WINDOW_AUTOSIZE);
  std::cout << "Start image view" << std::endl;
  std::cout << "If you want to take a 1 shot image as image file, please push s on Captured Image Window!" << std::endl;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageViewer2::onDeactivated(RTC::UniqueId /*ec_id*/)
{
  if(connection_check[1]){
	//If the capture mode is continuous mode, the image capture is stopped
    if(m_capture_frame_num == 0)
	{
      std::cout << "Stop image streaming" << std::endl;
      RTC_INFO(("Send command of \"stop continuous\" via CameraCaptureService."));
      m_CameraCaptureService->stop_continuous();
    }
  }

  if(connection_check != NULL)
	delete [] connection_check;
  
  //Destroy image viewer
  cv::destroyWindow("Image Window");
  std::cout << "Stop image view" << std::endl;

  //Release image memory
  image.release();

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageViewer2::onExecute(RTC::UniqueId /*ec_id*/)
{
	//Inport data check
	if(m_ImageIn.isNew())
	{
		m_ImageIn.read();
    		
		width = m_Image.data.image.width;
		height = m_Image.data.image.height;
		channels = (m_Image.data.image.format == Img::CF_GRAY) ? 1 :
			   (m_Image.data.image.format == Img::CF_RGB || m_Image.data.image.format == Img::CF_PNG || m_Image.data.image.format == Img::CF_JPEG) ? 3 :
			   (m_Image.data.image.raw_data.length()/width/height);
		RTC_TRACE(("Capture image size %d x %d", width, height));
		RTC_TRACE(("Channels %d", channels));
		
		if(channels == 3)
			image.create(height, width, CV_8UC3);
		else
			image.create(height, width, CV_8UC1);		

		long data_length = m_Image.data.image.raw_data.length();

		if( m_Image.data.image.format == Img::CF_RGB || m_Image.data.image.format == Img::CF_GRAY)
		{
			for(int i=0; i<height; ++i)
				memcpy(&image.data[i*image.step],&m_Image.data.image.raw_data[i*width*channels],sizeof(unsigned char)*width*channels);
			if(channels == 3)
			  cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		}
		else if( m_Image.data.image.format == Img::CF_JPEG || m_Image.data.image.format == Img::CF_PNG )
		{
			std::vector<uchar> compressed_image = std::vector<uchar>(data_length);
			memcpy(&compressed_image[0], &m_Image.data.image.raw_data[0], sizeof(unsigned char) * data_length);

			//Decode received compressed image
			cv::Mat decoded_image;
			if(channels == 3)
			{
			  decoded_image = cv::imdecode(cv::Mat(compressed_image), cv::IMREAD_COLOR);
				image = decoded_image;
			}
			else
			{
			  decoded_image = cv::imdecode(cv::Mat(compressed_image), cv::IMREAD_GRAYSCALE);
				image = decoded_image;
			}
		}
  }

  //IF the captured image is available, the image is shown in viewer
  if(!image.empty())
  {
		cv::imshow("Image Window", image);
  }

  char key = cv::waitKey(3);

  //Image save process
  if ( key == 's')
  {
    char file[80];
    sprintf( file, "CapturedImage%03d.png",++saved_image_counter);
    cv::imwrite( file, image );
  }

  return RTC::RTC_OK;
}


//RTC::ReturnCode_t ImageViewer2::onAborting(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2::onError(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2::onReset(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2::onStateUpdate(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t ImageViewer2::onRateChanged(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}



extern "C"
{
 
  void ImageViewer2Init(RTC::Manager* manager)
  {
    coil::Properties profile(imageviewer2_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageViewer2>,
                             RTC::Delete<ImageViewer2>);
  }
  
}
